CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
TARGET = lab6
SRC = Lab6.cpp

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
