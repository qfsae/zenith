# Interrupts

This folder is for interrupt only functions. For interrupts that pass data into tasks, see `src/interrupts/README.md`

## Structure
Interrupts should be separated into their own `.c` files. All interrupt share a single `interrupts.h` file that allows the linker to find them.

## Initialization
All interrupts that do not interact with tasks are initialized directly in the main function.