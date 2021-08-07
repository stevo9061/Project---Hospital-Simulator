# General Information

This project was built in collaboration, with the team members being:
 - Stefan Bittgen
 - Eva Grabler
 - Nikolaus Rieder

 # Development process

 For our project we focused on a V-Model Development-process.

 We began by defining our Requirements and worked towards defining how we will structurize the data used throughout the program.
 Then we handed out tasks to each team-member to implement basic functions, which will create/manipulate/free those data structures.

 After the foundation in the codebase was set, we continued adding necessary functions & features to suffice our requirements.

 Before we had a user-interface we wrote functional tests in C (basically main files executing our functions) to find problems early on.

 One of the biggest hurdles was the implementation of the graphical User-Interface.

 The main focus for the team was stability, once we had a working graphical application. So we started working on the unit-tests, which test our main project sources. (`persons.c` & `seat_rows.c`)

 We struggled to write unit-tests for the GUI and in the end gave up on doing so, because it didn't seam feasable with the GTest Framework.

 Once we came to this point, we started documenting our functions using doxygen and prepared for finalization of the project.

# Project Structure

Inside the Project, there's a folder structure, which contains various files as described below:
 - `src/` ... contains all project-source files, necessary to build the main application.
 - `inc/` ... contains all project-header files, used inside the source-files.
 - `tests/` ... contains all unit-test-source files, which test code-coverage of the project using the Google Framework `gtest`
 - `modules/` ... contains the cmake-files which are required by the Google Test Framework.
 - `cmake-build/` ... is the output-directory for building the project. Also contains a README on how to build the project.
 
 Further below is an explanation on how to build the project.
 The project includes Doxygen-Documentation and Unit-Tests, which need to be built according to the build-targets.

 # Build pre-requisites / Necessary Libraries

 This project can only be built if the following library requirements are met:

 | Library | Install-Command | Explanation |
 | --- | --- | --- |
 | PKG-CONFIG | `sudo apt install pkg-config` | Installs `pkg-config`, which is used for library includes and Cflags in Build-Commands |
 | MAKE | `sudo apt install make` | Installs `make` |
 | CMAKE | `sudo apt install cmake` | Installs `cmake` |
 | DOXYGEN | `sudo apt install doxygen` | Installs `doxygen` |
 | GTK3+ | `sudo apt install libgtk-3-dev` | Installs the GTK3-Library |
 | GTest | `sudo apt install libpthread-stubs0-dev gcovr` | Installs the Gcovr library and THREADS |


 # Info on Requirements-Linking to Code

 In our collaboration we tried to use many of the provided Features in GitLab to track our Requirements.
 We might not have done a perfect job, since it was a learning experience from the start, but we've tried to make it traceable.

 Either commits/mergeRequests are directly linked in the requirements or there is at least a comment in the requirement, which does so.

 Late in the development process we've also created some implementation-issues, which describe certain features that were left over to implement, after the initial codebase was written.

 To view all completed project requirements, use this link:
 https://es.technikum-wien.at/bic/bic2_hwse2_2021/6/-/issues?scope=all&utf8=%E2%9C%93&state=closed

 Our requirements are all labeled with the corresponding Requirement-Category (Yellow labels) and also with the corresponding Feature-Category (Green labels). By selecting those labels in the search-bar, the issues can be filtered.
Some features, which were implemented later in the project, were created based on Implementation-issues (Orange labels).

# How to build the project

Included in this project is a Makefile and a CMakeLists. 
Since the makefile includes targes for cmake, it is optional to use cmake directly.
It is recommended to use the provided Makefile, as it correctly uses the provided output-folder `cmake-build`.

| target | purpose |
| --- | --- |
| `make` | Executes the default-target, which is `build` |
| `make build` | Builds the project using CMAKE - generates the main & unit-tests executable inside `cmake-build/bin/` |
| `make cmake-clean` | Clean the build-directory `cmake-build` |
| `make test` | Builds the project & executes the unit-tests |
| `make coverage` | Builds the project & executes the unit-tests including coverage-report generation |
| `make testrun` | Builds the project & executes the unit-tests-executable directly (different view of tests) |
| `make current` | Builds the current state of the project with MAKE, generating the executable `build/current` |
| `make doc` | Builds the doxygen-documentation in HTML format, which is generated into `html/` |

# Alternative build methods

In case something goes wrong using the Makefile, the whole project can be built with CMake manually:
_(Copy&Paste in terminal from Project-Root-Directory)_

`cd cmake-build && cmake .. && make`

In case something goes wrong using the CMakeFile, the project main executable can be built with Make manually: 
_(Copy&Paste in terminal from Project-Root-Directory)_

`make current`

In case both the Makefile and CMakeFile cause errors, the project can be built with `gcc` manually:
_(Copy&Paste in terminal from Project-Root-Directory)_

```sh
gcc -Iinc -Wall -Wextra -g -Wno-unused-parameter -c src/persons.c -o build/persons.o
gcc -Iinc -Wall -Wextra -g -Wno-unused-parameter -c src/seat_rows.c -o build/seat_rows.o
gcc `pkg-config --cflags gtk+-3.0` -Iinc -Wall -Wextra -g -c src/gtk3.c -o build/gtk3.o `pkg-config --libs gtk+-3.0`
gcc `pkg-config --cflags gtk+-3.0` -Iinc -Wall -Wextra -g src/main.c -o build/hospital-simulator `pkg-config --libs gtk+-3.0`
```
