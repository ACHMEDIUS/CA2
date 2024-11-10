#include "gfx.h"
#include "io.h"

/* Some small programs used to test the different graphics modes and
 * graphics library */

#if 0
#define SHIFT 1
#define RES (256 << SHIFT)
int main(){
  gfx_context c;
  if (gfx_init(&c, RES, RES, GFX_RGB555)){
    puts("Error initialising graphics");
    return 1;
  }
  for (_u32 y = 0; y < RES; y++)
    for (_u32 x = 0; x < RES; x++)
      set_pixel_short(&c, x, y, (x >> SHIFT) + ((y * RES) >> SHIFT));

  return 0;
}
#endif

#if 0
#define SHIFT 2
#define RES (128 << SHIFT)

int main(){
  gfx_context c;
  if (gfx_init(&c, RES, RES, GFX_RGB332)){
    puts("Error initialising graphics");
    return 1;
  }
  for (_u32   y = 0; y < RES; y++)
    for (_u32 x = 0; x < RES; x++)
      set_pixel(&c, x, y, (x + y) >> SHIFT);

  return 0;
}
#endif

#if 1
#define SHIFT 2
#define RES (128 << SHIFT)

int main(){
  gfx_context c;
  if (gfx_init(&c, RES, RES, GFX_RGB24)){
    puts("Error initialising graphics");
    return 1;
  }
  for (_u32   y = 0; y < RES; y++)
    for (_u32 x = 0; x < RES; x++){
      _u8 v = (x + y) >> SHIFT;
      set_pixel_rgb(&c, x, y, v >> 1, v >> 2, v);
    }

  return 0;
}
#endif

/* Fill the framebuffer with a diagonal striped pattern of index values
 * and start changing the palette to random colours using fixed-seed PCG32
 *
 */

#if 0
// *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)

typedef unsigned long long _u64;

typedef struct { _u64 state; _u64 inc; } pcg32_random_t;

_u32 pcg32_random_r(pcg32_random_t* rng)
{
    _u64 oldstate = rng->state;
    // Advance internal state
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc|1);
    // Calculate output function (XSH RR), uses old state for max ILP
    _u32 xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    _u32 rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}
#define SHIFT 2
#define RES (128 << SHIFT)

int main(){
  gfx_context c;
  if (gfx_init(&c, RES, RES, GFX_INDEXED)){
    puts("Error initialising graphics");
    return 1;
  }
  for (_u32   y = 0; y < RES; y++)
    for (_u32 x = 0; x < RES; x++)
      set_pixel(&c, x, y, (x + y) >> SHIFT);

  pcg32_random_t r = { 0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL };
  _u8 i = 0;
  _u32 a = 0;
  while (c.control->enable && ++a < (1 << 22)){
    i++;
    c.palette[i] = pcg32_random_r(&r);
  }
  gfx_quit(&c);
  return 0;
}
#endif
