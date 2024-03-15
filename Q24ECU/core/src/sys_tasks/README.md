# SYSTEM TASKS

1. System tasks should be added into the enum located in `sys_tasks.h`. Global definitions of the system tasks should also be placed within this file.
2. Task creation should happen within `sys_tasks/controller.c`. They are initialized to a suspended state.
3. Resume the tasks within the correct system state.