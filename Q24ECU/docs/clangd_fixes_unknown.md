## Clangd Fixes
This is a temp file to remember how this works. Should be deleted before merge into master.

#### What Happened
For the most part what happened or why is unknown. Something to do with the GCC header files not being found by clang. I think this may have something to do with the clang installation being under vim. Note the project still does not compile under clang llvm.

The main problem is that Clangd is unable to find many of the stdlib files required by this project, so it errors out. This is unrelated to the multi-error problem where every include is not found. That is a different problem that can be fixed by generating the `compile_commands.json` file. (made by running CMake build)

#### Temporary Solution
1. Locate the GCC header files on the host system. On the laptop these were in `C:\Program Files (x86)\GNU Arm Embedded Toolchain\10 2021.10\arm-none-eabi`. If on linux or WSL they will be under `/usr/x86_64-w64-mingw32/include` or similar.

2. Locate the vim plugins folder and Clangd. For me this was under `\\wsl.localhost\Ubuntu\home\jacob\.local\share\nvim\mason\packages\clangd\clangd_17.0.3\lib\clang\17\include\include` on the windows side and `~/.local/share/nvim/mason/packages/clangd/` on the Linux side.

3. Locate where Clang stores its includes and copy paste the files from GCC into Clang ignoring duplicates
