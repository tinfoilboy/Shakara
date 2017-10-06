
CC=g++

FILES:=$(shell find ./Shakara -type f -name "*.cpp")
OBJECTS=$(FILES:.cpp=.o)

all: $(OBJECTS)
	$(CC) -g $^ ./ShakaraStandalone/main.cpp -o ./sa

%.o: %.cpp
	$(CC) -g -c $< -o $@

PHONY: clean
clean:
	rm -fv $(OBJECTS)
	rm -v ./sa
