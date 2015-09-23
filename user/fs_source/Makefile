CXXFLAGS := -std=c++11 -fpermissive
FS := ../fs

CXX := i686-elf-g++

all: $(FS)

$(FS): main.cpp fs.hpp
	$(CXX) $(CXXFLAGS) main.cpp -o $@

clean:
	@find . -wholename '../fs' -delete