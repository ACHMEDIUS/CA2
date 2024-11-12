# "Which parts of your emulator do and do not work."

The emulator is able to read R- I- S- U- B- and J-Type instructions, with several commonly used instructions, such as most add, load, and store instructions. Aswell as Branch, and Jump instructions. From an InstructionWord of 32 bits, the RD, RS1, RS2, Func3, Func7, Opcode, and Immediate are extracted and decoded. We are able to run basic.bin, msg.bin, triangle.bin, hellof.bin, comp.bin and brainfck.bin with -p and with forwarding enabled

# "The contents of the pipeline registers, and why it was necessary to include each field."

# "Any particular choices you have made for your implementation."

# "Your approach to testing: how did you test the implemented instructions? Did you write micro-programs, why/why not?"

Due to restrictions in time, we were unable to write specific micro-programs, since we did this assignment last year, we used our code from the previous year, applied actual knowledge of RISC-V instruction decoding from the manual. Then, testing individual instructions using -x, we were able to test our formatter, pipeline, stages and CSC. However, unit tests were not prepared.

# "(for level 6) A comparison of the non-pipelined and pipelined CPI for the test programs. How does it compare to the ideal CPI?"

Again, due to time constraints, we were unable to calculate and analyze the CPI for pipelined vs non-pipelined for all 6 .bin files.

# "If you worked in a team: each individual student should write a paragraph describing their contributions (what parts did you work on?) and how you experienced the collaboration (was the collaboration efficient and equally divided?)"

As mentioned previously, we had this course and assignment last year aswell. We worked 4 full nights, fixing our previous implementation, and doing all parts together, using the Live Share feature on VSCode, and did not split certain parts of the emulator to a specific individual.
