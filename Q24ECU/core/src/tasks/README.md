# Tasks

Tasks are purely software tasks that may interact with interfaces. An example of this would be a throttle control task that retrieves data from the CAN bus and ADC interfaces, runs a computation, and then adds data to the que of messages waiting to get sent out over CAN bus.

## Structure
All tasks should be contained within their own files. Collections of tasks may be in the same file when they relate. All tasks should be externally defined within `taskHandlers.h`. All tasks should be prefixed with `tsk_`.

## Initialization
All tasks should be initialized within the `taskHandlers.c` file.  The task handles should also be initialized within this file. External definitions to the task handlers should be put in the `taskHandlers.h` file.
