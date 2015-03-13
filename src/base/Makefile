CC=$(CXX)
CXXFLAGS=-I../ \
	       -Wall -Werror -Wno-sign-compare -ansi -O3 -MD

ALL_SOURCES:=$(wildcard *.cc)
TEST_SOURCES:=$(wildcard test_*.cc)
SOURCES=$(filter-out $(TEST_SOURCES),$(ALL_SOURCES))

# All objects will auto build by "$(CXX) $(CXXFLAGS) -c" if there is no rule
# to make it
ALL_OBJS:=$(patsubst %.cc,%.o,$(ALL_SOURCES))
OBJS:=$(patsubst %.cc,%.o,$(SOURCES))
TESTS:=$(patsubst %.cc,%,$(TEST_SOURCES))

.PHONY: all clean rebuild test

all : $(OBJS)

clean :
	@rm -f $(ALL_OBJS) $(TESTS) *.d

rebuild : clean all

test : $(TESTS)

test_flags : test_flags.cc flags.o
	$(CXX) $(CXXFLAGS) -o $@ $^

