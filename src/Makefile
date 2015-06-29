### Makefile --- 

## Author: wkretzsch@gmail.com
## Version: $Id: Makefile,v 0.0 2015/06/27 11:51:11 winni Exp $
## Keywords: 
## X-URL: 


SOURCES := $(shell find . -name '*.cpp' | grep -v main.cpp )
BASES := $(basename $(SOURCES))
OBJECTS := $(addsuffix .o, $(BASES))
HEADERS := $(addsuffix .hpp, $(BASES))

export CPPFLAGS += -std=c++11

all: main.o libttt.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o ttt -lboost_program_options

%.o : %.cpp $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

libttt.a: $(OBJECTS)
	$(AR) cr $@ $^

clean:
	rm -f *.a *.o

### Makefile ends here
