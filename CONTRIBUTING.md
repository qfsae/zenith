# Contributing to Zenith

The team has decided to make use of the GitHub Project feature to track issues
with a KanBan style chart. Issues will be created during weekly meetings. Once a
branch has been created for an issue, it will automatically be moved to the 'In
Progress' column in the Kanban chart. When the branch has been merged to master,
the issue will be automatically moved to 'Done'. 

## Development Environment

The Zenith repository is used for software only. Instructions pertaining to
setting up a hardware design environment can be found on the
[PCB Repository](https://github.com/qfsae/pcb) readme file.

### Installation

Software Development on the team will be carried out using
[PlatformIO](https://docs.platformio.org/en/latest/what-is-platformio.html).
PlatformIO is a tool delivered as an extension to the Visual Studio Code text
editor which allows for the uploading and debugging of software on the various
micro-controllers used on the car. Given below is a step by step breakdown to
getting started with PlatformIO. If you run into any problems with the steps below, please contact Ethan or Logan.

1. First, you will need Visual Studio Code installed. VSCode is available on
   Windows, Mac and Linux and can be downloaded
   [here](https://code.visualstudio.com/download).

1. Next, install PlatformIO IDE, which is built on top of VSCode. The
   instructions are given in the PlatformIO documentation
   [here](https://platformio.org/install/ide?install=vscode).

### Running Code

With the installation of PlatformIO complete, you are ready to start work on
software projects with the team. In this example, will be running code on an STM32
Nucleo F446RE development board using a project template available in the Zenith
repository.



## Submitting Changes on GitHub & Code Review
Starting in the Q21 season, the electrical team has decided to implement a code
review process to ensure that the master branch of this repository has the
latest working code that can run on the car. 

When starting a new contribution to the repository, please follow the steps
given below:

1. BEFORE writing any code or making commits, checkout a working branch for
   yourself from master branch. In command line git, this is done by doing `git
   checkout -b yourBranchName`. Each branch should be related to an issue in the
   GitHub backlog.

    Please follow this naming convention: `[Zenith-<##>]-branch-name`

    For example, a branch name could be: `[Zenith-43]-updating-CAN-library`

2. When working on an issue / feature and actively writing code, ensure that you
   are writing unit tests for your code. Not only do unit tests help ensure that
   the code is working, it also provides the team with accessible tools to
   troubleshoot software problems on the car. 

3. Once you have finished your work and would like to submit your code for
   review, commit your latest changes using `git add .` and `git commit`. Once
   everything is committed you can push to the main repository with the command
   `git push -u origin yourBranchName`. You will be prompted for your
   credentials to complete the operation.

4. Follow the instructions on the GitHub documentation to [Open a Pull
   Request](https://docs.github.com/en/github/collaborating-with-issues-and-pull-requests/creating-a-pull-request).
   

5. Assign 2 team members to review the work. Projects will typically be assigned
   to 2 team members. Please ensure that either Brent, Logan or Ethan are
   assigned as one of the reviewers, and then your teammate as the other
   reviewer. 

6. Upon receiving feedback from the Pull Request, address and resolve any
   comments that are given. If there are comments that you don't understand,
   please use the GitHub conversation feature to ask questions. 

7. Once a senior team member has approved the Pull Request, they will merge the
   branch into master and delete it. 

8. Upon merging to master, GitHub Actions will attempt to build the project and
   run it against our test-suite. If the Continuous Integration pipeline fails,
   check the build logs and fix any issues. All projects on master should ALWAYS
   be passing our test suite. 
