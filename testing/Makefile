# A sample Makefile for building Google Test and using it in user
# tests.  Please tweak it to suit your environment and project.  You
# may want to move it to your project's root directory.
#
# SYNOPSIS:
#
#   make [all]  - makes everything.
#   make TARGET - makes the given target.
#   make clean  - removes all files generated by make.

# Please tweak the following variable definitions as needed by your
# project, except GTEST_HEADERS, which you can use in your own targets
# but shouldn't modify.

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = ../gtest-1.7.0

# Where to find user code.
USER_DIR = ../src
GTEST_SAMP_DIR = $(GTEST_DIR)/samples

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include -I$(USER_DIR) -std=c++11

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = sample1_unittest state_ut actVal_ut agent_ut

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.

all : test

test : $(TESTS)
	perl ./run_tests.pl $(TESTS)

clean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o

.PHONY: all test clean

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Builds a sample test.  A test should link with either gtest.a or
# gtest_main.a, depending on whether it defines its own main()
# function.

sample1.o : $(GTEST_SAMP_DIR)/sample1.cc $(GTEST_SAMP_DIR)/sample1.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(GTEST_SAMP_DIR)/sample1.cc

sample1_unittest.o : $(GTEST_SAMP_DIR)/sample1_unittest.cc \
                     $(GTEST_SAMP_DIR)/sample1.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(GTEST_SAMP_DIR)/sample1_unittest.cc

sample1_unittest : sample1.o sample1_unittest.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

state_ut.o: state_ut.cpp $(USER_DIR)/state.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

state_ut: state_ut.o gtest_main.a $(USER_DIR)/libttt.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

actVal_ut.o: actVal_ut.cpp $(USER_DIR)/actVal.hpp $(USER_DIR)/state.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

actVal_ut: actVal_ut.o $(USER_DIR)/actVal.o gtest_main.a $(USER_DIR)/libttt.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

agent_ut.o: agent_ut.cpp $(USER_DIR)/agent.hpp $(USER_DIR)/state.hpp $(USER_DIR)/actVal.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

agent_ut: agent_ut.o $(USER_DIR)/agent.o gtest_main.a $(USER_DIR)/libttt.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
