# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pierciro/Scrivania/seal_demo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pierciro/Scrivania/seal_demo

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/pierciro/Scrivania/seal_demo/CMakeFiles /home/pierciro/Scrivania/seal_demo//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/pierciro/Scrivania/seal_demo/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named app

# Build rule for target.
app: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 app
.PHONY : app

# fast build rule for target.
app/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/build
.PHONY : app/fast

src/main/main.o: src/main/main.cpp.o
.PHONY : src/main/main.o

# target to build an object file
src/main/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/src/main/main.cpp.o
.PHONY : src/main/main.cpp.o

src/main/main.i: src/main/main.cpp.i
.PHONY : src/main/main.i

# target to preprocess a source file
src/main/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/src/main/main.cpp.i
.PHONY : src/main/main.cpp.i

src/main/main.s: src/main/main.cpp.s
.PHONY : src/main/main.s

# target to generate assembly for a file
src/main/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/src/main/main.cpp.s
.PHONY : src/main/main.cpp.s

src/receiver/receiver.o: src/receiver/receiver.cpp.o
.PHONY : src/receiver/receiver.o

# target to build an object file
src/receiver/receiver.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/src/receiver/receiver.cpp.o
.PHONY : src/receiver/receiver.cpp.o

src/receiver/receiver.i: src/receiver/receiver.cpp.i
.PHONY : src/receiver/receiver.i

# target to preprocess a source file
src/receiver/receiver.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/src/receiver/receiver.cpp.i
.PHONY : src/receiver/receiver.cpp.i

src/receiver/receiver.s: src/receiver/receiver.cpp.s
.PHONY : src/receiver/receiver.s

# target to generate assembly for a file
src/receiver/receiver.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/src/receiver/receiver.cpp.s
.PHONY : src/receiver/receiver.cpp.s

src/sender/sender.o: src/sender/sender.cpp.o
.PHONY : src/sender/sender.o

# target to build an object file
src/sender/sender.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/src/sender/sender.cpp.o
.PHONY : src/sender/sender.cpp.o

src/sender/sender.i: src/sender/sender.cpp.i
.PHONY : src/sender/sender.i

# target to preprocess a source file
src/sender/sender.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/src/sender/sender.cpp.i
.PHONY : src/sender/sender.cpp.i

src/sender/sender.s: src/sender/sender.cpp.s
.PHONY : src/sender/sender.s

# target to generate assembly for a file
src/sender/sender.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/src/sender/sender.cpp.s
.PHONY : src/sender/sender.cpp.s

src/utils/utils.o: src/utils/utils.cpp.o
.PHONY : src/utils/utils.o

# target to build an object file
src/utils/utils.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/src/utils/utils.cpp.o
.PHONY : src/utils/utils.cpp.o

src/utils/utils.i: src/utils/utils.cpp.i
.PHONY : src/utils/utils.i

# target to preprocess a source file
src/utils/utils.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/src/utils/utils.cpp.i
.PHONY : src/utils/utils.cpp.i

src/utils/utils.s: src/utils/utils.cpp.s
.PHONY : src/utils/utils.s

# target to generate assembly for a file
src/utils/utils.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/src/utils/utils.cpp.s
.PHONY : src/utils/utils.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... app"
	@echo "... src/main/main.o"
	@echo "... src/main/main.i"
	@echo "... src/main/main.s"
	@echo "... src/receiver/receiver.o"
	@echo "... src/receiver/receiver.i"
	@echo "... src/receiver/receiver.s"
	@echo "... src/sender/sender.o"
	@echo "... src/sender/sender.i"
	@echo "... src/sender/sender.s"
	@echo "... src/utils/utils.o"
	@echo "... src/utils/utils.i"
	@echo "... src/utils/utils.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

