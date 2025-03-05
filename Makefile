CXX = g++

CXXFLAGS = -std=c++17 -Iinclude -I/opt/homebrew/include -DGL_SILENCE_DEPRECATION

LDFLAGS = -L/opt/homebrew/lib -lglfw -framework OpenGL

TARGET = app

SRC = $(wildcard src/*.cpp)

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
