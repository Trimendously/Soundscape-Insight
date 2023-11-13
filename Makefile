# Compiler and compiler flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Linker flags
LDFLAGS = -L./audio-processing/fftw -Wl,-rpath=./audio-processing/fftw -lfftw3 -lm

# Executable name
EXECUTABLE = Audio-Insight

# Source files
SRCS = audio-processing/filter.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(EXECUTABLE)

# Build the executable
$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(EXECUTABLE)
