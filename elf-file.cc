/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    elf-file.cc - ELF file parsing class.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 *
 * This file loosely based on:
 *
 * S.M.A.C.K - An operating system kernel
 * Copyright (C) 2010,2011 Mattias Holm and Kristian Rietveld
 * For licensing and a full list of authors of the kernel, see the files
 * COPYING and AUTHORS.
 */

#include "elf-file.h"
#include "memory.h"

#include "elf.h"

#include <stdexcept>
#include <functional>

#ifndef _MSC_VER
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#else
#define __builtin_bswap32 _byteswap_ulong
#define __builtin_bswap16 _byteswap_ushort
#endif

#include <filesystem>
#include <algorithm>
namespace fs = std::filesystem;

ELFFile::ELFFile(std::string_view filename)
{
  load(filename);
}

ELFFile::~ELFFile()
{
  if (! isBad)
    unload();
}

void
ELFFile::load(std::string_view filename)
{
#ifdef _MSC_VER
  if (fs::is_directory(filename)) {
    throw std::system_error(std::make_error_code(std::errc::is_a_directory));
  }

  fd = CreateFileA(filename.data(), GENERIC_READ,
    0, nullptr, OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL, nullptr);

  if (fd == INVALID_HANDLE_VALUE) {
    throw std::runtime_error("Could not open file.");
  }

  mapping = CreateFileMappingA(fd, nullptr, 
    PAGE_READONLY, 0, 0, nullptr);

  if (mapping == nullptr) {
    CloseHandle(fd);
    throw std::runtime_error("Failed to create memory map.");
  }

  mapAddr = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0,0);
  if (mapAddr == nullptr) {
    CloseHandle(mapping);
    CloseHandle(fd);
    throw std::runtime_error("Failed to setup memory map.");
  }

#else
  fd = open(filename.data(), O_RDONLY);
  if (fd < 0)
    throw std::system_error(std::error_code(static_cast<int>(errno),
                            std::generic_category()));

  struct stat statbuf;
  if (fstat(fd, &statbuf) < 0)
    {
      close(fd);
      throw std::runtime_error("Could not retrieve file attributes.");
    }

  if (statbuf.st_mode & S_IFDIR)
    {
      close(fd);
      throw std::system_error(std::make_error_code(std::errc::is_a_directory));
    }

  programSize = statbuf.st_size;
  mapAddr = mmap(NULL, programSize, PROT_READ, MAP_PRIVATE, fd, 0);
  if (mapAddr == MAP_FAILED)
    {
      close(fd);
      throw std::runtime_error("Failed to setup memory map.");
    }
#endif

  /* For now, we hardcode the OpenRISC target */
  if (! isELF() || ! isTarget(ELFCLASS32, ELFDATA2MSB, EM_OPENRISC))
    {
      unload();
      throw std::invalid_argument("File is not an OpenRISC ELF file.");
    }

  isBad = false;
}

void
ELFFile::unload()
{
#ifdef _MSC_VER
  UnmapViewOfFile(mapAddr);
  CloseHandle(mapping);
  CloseHandle(fd);

  mapping = nullptr;
#else
  munmap(mapAddr, programSize);
  close(fd);
#endif

  mapAddr = nullptr;

  /* Select correct default on all platforms */
  fd = decltype(fd){};
}

bool
ELFFile::isELF() const
{
  const auto *elf = static_cast<const Elf64_Ehdr *>(mapAddr);

  if (!(elf->e_ident[EI_MAG0] == 0x7f
        && elf->e_ident[EI_MAG1] == 'E'
        && elf->e_ident[EI_MAG2] == 'L'
        && elf->e_ident[EI_MAG3] == 'F'))
    return false;

  if (elf->e_ident[EI_VERSION] != EV_CURRENT)
    return false;

  return true;
}

bool
ELFFile::isTarget(const uint8_t elf_class,
                  const uint8_t endianness,
                  const uint8_t machine) const
{
  const auto *elf = static_cast<Elf32_Ehdr *>(mapAddr);

  if (elf->e_ident[EI_CLASS] != elf_class)
    return false;

  if (elf->e_ident[EI_DATA] != endianness)
    return false;

  if ((elf->e_type == ET_EXEC || elf->e_type == ET_DYN) &&
      (elf->e_machine != machine || elf->e_version != EV_CURRENT))
    return false;

  if (elf->e_phoff == 0)
    // Not a proper elf executable (must have program header)
    return false;

  return true;
}


/* We don't want to expose the elf.h types in the elf-file.h header,
 * so we keep this function internal and outside of the class definition.
 */
using ForeachSegmentFunction = std::function<void(const Elf32_Ehdr *elf, const Elf32_Shdr &header)>;

static void
foreachSegment(void *mapAddr, ForeachSegmentFunction func)
{
  const auto *elf = static_cast<Elf32_Ehdr *>(mapAddr);

  const auto *sheaders = reinterpret_cast<Elf32_Shdr *>(reinterpret_cast<uintptr_t>(elf) + __builtin_bswap32(elf->e_shoff));

  for (int i = 0; i < __builtin_bswap16(elf->e_shnum); ++i)
    {
      const Elf32_Shdr &header = sheaders[i];
      Elf32_Word sh_flags = __builtin_bswap32(header.sh_flags);
      if ((sh_flags & SHF_ALLOC) == SHF_ALLOC)
        func(elf, header);
    }
}


std::vector<std::unique_ptr<MemoryInterface>>
ELFFile::createMemories() const
{
  std::vector<std::unique_ptr<MemoryInterface>> memories;

  foreachSegment(mapAddr, [&memories](const Elf32_Ehdr *elf, const Elf32_Shdr &header) -> void
    {
      Elf32_Word sh_flags = __builtin_bswap32(header.sh_flags);
      Elf32_Word sh_size = __builtin_bswap32(header.sh_size);
      Elf32_Word sh_type = __builtin_bswap32(header.sh_type);
      Elf32_Off sh_offset = __builtin_bswap32(header.sh_offset);
      Elf32_Addr sh_addr = __builtin_bswap32(header.sh_addr);

      size_t align = __builtin_bswap32(header.sh_addralign);

      auto *segment =
          new (std::align_val_t{ align }, std::nothrow) std::byte[sh_size];
      if (!segment)
        throw std::runtime_error("Could not allocate aligned memory.");

      if (reinterpret_cast<uintptr_t>(segment) & ((align - 1) != 0))
        throw std::runtime_error("Allocated pointer for segment is not aligned.");

      /* Transfer section data or clear the section. */
      if (sh_type == SHT_PROGBITS)
        {
          const auto *segdata =
              reinterpret_cast<const std::byte *>(elf) + sh_offset;
          std::copy_n(segdata, sh_size, segment);
        }
      else
        std::fill_n(segment, sh_size, std::byte{ 0 });

      /* FIXME: determine correct name for segment. */
      std::string name{ "data" };
      if ((sh_flags & SHF_EXECINSTR) == SHF_EXECINSTR)
        name = "text";

      auto memory = std::make_unique<Memory>(name, segment,
                                             sh_addr,
                                             sh_size,
                                             align);
      if ((sh_flags & SHF_WRITE) == SHF_WRITE)
        memory->setMayWrite(true);

      memories.push_back(std::move(memory));
    });

  return memories;
}

bool
ELFFile::getTextSegment(std::vector<std::byte> &segmentData,
                        MemAddress &segmentBase,
                        size_t &segmentSize) const
{
  bool found = false;
  segmentData.clear();

  foreachSegment(mapAddr, [&segmentData, &segmentBase, &segmentSize, &found](const Elf32_Ehdr *elf, const Elf32_Shdr &header) -> void
    {
      Elf32_Word sh_flags = __builtin_bswap32(header.sh_flags);
      Elf32_Word sh_size = __builtin_bswap32(header.sh_size);
      Elf32_Off sh_offset = __builtin_bswap32(header.sh_offset);
      Elf32_Addr sh_addr = __builtin_bswap32(header.sh_addr);

      if ((sh_flags & SHF_EXECINSTR) != SHF_EXECINSTR)
        return;

       segmentData.resize(sh_size);

       const auto *segdata =
           reinterpret_cast<const std::byte *>(elf) + sh_offset;
       std::copy_n(segdata, sh_size, segmentData.begin());

       segmentBase = sh_addr;
       segmentSize = sh_size;

       found = true;
    });

    return found;
}

uint64_t
ELFFile::getEntrypoint() const
{
  return __builtin_bswap32(static_cast<Elf64_Ehdr *>(mapAddr)->e_entry);
}
