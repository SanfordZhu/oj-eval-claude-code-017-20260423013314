CXX=g++
CXXFLAGS=-O2 -std=gnu++17 -pipe -static -s
LDFLAGS=

all: code

code: code.cpp
	$(CXX) $(CXXFLAGS) -o code code.cpp $(LDFLAGS)

clean:
	rm -f code
