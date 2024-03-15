# Interrupts

Interrupts used on their own or for passing notifiers into tasks.

All Interrupts should be initialized similarly to tasks and should be shut down when not in use. Using interrupts on their own should be avoided when possible.

Ensure that a global definition of the interrupt is placed in the `main.h` file so that it can be linked with the startup script.