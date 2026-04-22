CXX=g++
CXXFLAGS=-O2 -std=gnu++17 -pipe
LDFLAGS=

all: code

code: code.cpp
	$(CXX) $(CXXFLAGS) -o code code.cpp $(LDFLAGS)

clean:
	rm -f code
