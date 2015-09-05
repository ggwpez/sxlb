CXXFLAGS := -fpermissive
FS := ../fs

all: $(FS)

$(FS): main.cpp fs.cpp fs.hpp
	$(CXX) $(CXXFLAGS) main.cpp -o $@

clean:
	@find . -wholename '../fs' -delete