CXX=clang-cl $(CXXFLAGS)
CXXFLAGS=/std:c++latest /EHsc /O2
TARGETS=implements.exe

all: $(TARGETS)

implements.exe: *.cpp *.h
	$(CXX) /Feimplements.exe main.cpp
