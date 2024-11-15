CXX = g++

CXXFLAGS = -std=c++11 -Iinclude -Iinclude/SDL -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -fstack-protector-all

SDL2_CFLAGS = $(shell sdl2-config --cflags)
SDL2_LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf

TARGET = RimWorld

SRCS = $(wildcard src/*.cpp)

OBJDIR = build

OBJS = $(patsubst src/%.cpp, $(OBJDIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(SDL2_LDFLAGS)

$(OBJDIR)/%.o: src/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(SDL2_CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJS) $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run