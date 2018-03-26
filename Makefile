CFLAGS = `pkg-config --cflags opencv` -Wall -ggdb -Wextra -pedantic -std=c++11 -Wno-unused-parameter -O2
LIBS = `pkg-config --libs opencv` -lboost_program_options

com_targets := $(addprefix bin/, $(basename $(notdir $(wildcard src/com/*.cpp))))
com_obj := $(addprefix obj/com/,$(notdir $(addsuffix .o, $(com_targets))))

CPP_FILES_COMMON := $(wildcard src/*.cpp)
OBJ_FILES_COMMON := $(addprefix obj/,$(addsuffix .o, $(basename $(notdir $(CPP_FILES_COMMON)))))

TP: $(com_targets)

obj/com/%.o : src/com/%.cpp
	g++ $(CFLAGS) -o $@ -c $<

obj/%.o : src/%.cpp src/%.h
	g++ $(CFLAGS) -o $@ -c $<

bin/% : obj/com/%.o $(OBJ_FILES_COMMON)
	g++ $(CFLAGS) -o $@ $^ $(LIBS)

.SECONDARY: $(com_obj) $(OBJ_FILES_COMMON)

.PHONY: clean

clean:
	rm -f $(com_targets) $(com_obj) $(OBJ_FILES_COMMON)


