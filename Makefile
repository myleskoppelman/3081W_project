CXX = clang++

# Add OpenGL, GLFW, and GLEW flags to CXXFLAGS and LDFLAGS
CXXFLAGS = -std=c++17 -I/opt/homebrew/include -I/opt/homebrew/include/SDL2 -Ilibs -DGL_SILENCE_DEPRECATION

LDFLAGS = -L/opt/homebrew/lib -lglew -lglfw -framework OpenGL

TARGET = newtons_simulator

SRC = $(wildcard src/*.cpp)

OBJ = $(SRC:.cpp=.o)

# Default target to build the program
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
