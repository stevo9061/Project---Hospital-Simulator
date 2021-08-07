# Variables for make targets
PROJECT=hospital_simulator
CC=gcc
CFLAGS=$(INC) -Wall -Wextra -pedantic -g -Wno-unused-parameter
INC=-Iinc
BUILD=./build/
CMAKE=cmake
CHANGE_DIR_CMAKE=cd cmake-build
DOXYGEN=doxygen
# For GTK
GTKFLAGS=`pkg-config --cflags gtk+-3.0`
GTKLIB=`pkg-config --libs gtk+-3.0`
# These define where make should look for certain project files, if necessary
vpath %.o ./build
vpath %.c ./src
vpath %.h ./inc

# Found this function here: https://stackoverflow.com/questions/1814270/gcc-g-option-to-place-all-object-files-into-separate-directory
# about patsubst: https://www.gnu.org/software/make/manual/html_node/Text-Functions.html
_OBJECTS=gtk3.o seat_rows.o persons.o
OBJECTS=$(patsubst %,$(BUILD)%,$(_OBJECTS))

# To create a target for an object file, write the following:
# Headers should never be written here, as 'gcc' will look for them in the source-files anyways
#$(BUILD)object.o: source.c
#	$(CC) $(CFLAGS) -c $< -o $@

# For singular targets with just source files, which don't require any objects, write the following:
#$(BUILD)target: source.c
# 	$(CC) $(CFLAGS) $< -o $@

# For targets that user GTK, please use the following: (either with object files or direct targets)
#$(BUILD)target.o: source.c
#	$(CC) $(CFLAGS) $(GTKFLAGS) $(GTKLIB) -c $< -o $@
#$(BUILD)target: source.c
#	$(CC) $(CFLAGS) $(GTKFLAGS) $(GTKLIB) -c $< -o $@

# Special target for compiling our GUI object file, since it requires GTK flags & libraries
$(BUILD)gtk3.o: gtk3.c
	$(CC) $(CFLAGS) $(GTKFLAGS) $(GTKLIB) -c $< -o $@

# Automatic variable for compiling object files
$(BUILD)%.o: %.c
	$(CC) $(CFLAGS) $(GTKFLAGS) $(GTKLIB) -c $< -o $@

# This target is executed when just using 'make'
all: build

# Executed with: make current
current: main.c $(OBJECTS)
	$(CC) $(CFLAGS) $(GTKFLAGS) $^ -o $(BUILD)$@ $(GTKLIB)

# Executed with: make clean
clean:
	rm $(BUILD)*.o $(BUILD)current

doc: Doxyfile
	${DOXYGEN} $<

# The following targets are shortcuts to use the underlying CMakeFiles.txt in 'cmake-build'

# Cleans the created builds
cmake-clean:
	${CHANGE_DIR_CMAKE} && ${MAKE} clean
# Configures and builds the CMake-Project
build:
	${CHANGE_DIR_CMAKE} && ${CMAKE} .. && ${MAKE}
# Builds the CMake-Project and runs the testcases via make
test: build
	${CHANGE_DIR_CMAKE} && ${MAKE} test
# Builds the CMake-Project and runs the coverage report target
coverage: build
	${CHANGE_DIR_CMAKE} && ${MAKE} ${PROJECT}_coverage
# Builds the CMake-Project and runs the unit tests executable directly
testrun: build
	./cmake-build/bin/${PROJECT}_unit_tests

# For targets which don't have any input-files and just execute something
# Read this for clarification: https://web.mit.edu/gnu/doc/html/make_4.html#SEC31
.PHONY: clean cmake-clean build test coverage testrun build-test
