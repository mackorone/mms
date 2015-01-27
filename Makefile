# Program name
PROGRAM = MMSim

# Any external libraries
LIBS = -lGL -lglut -lGLU

# Flags
# -std=c++11: Use the C++11 standard
# -MMD: Automatically generate dependency files
# -g: Add debugging symbols
# -Os: Optimize code for size
# -Wall: Show all warning messages
# TODO: -pedantic-errors
FLAGS = -MMD -std=c++11#-Os TODO: We can optimize later... #-Wall TODO: Fix all errors

# Directories
SRC = ./src/
OBJ = ./obj/
BIN = ./bin/

# Recursively find all source files
PRELIMSOURCES = $(shell find $(SRC) -name '*.cpp')
SOURCES = $(PRELIMSOURCES:$(SRC)%=%) 

# Specify the object files that should mirror the source files
OBJECTS = $(SOURCES:.cpp=.o)

# Add prefixes to all of the source files
PREFIXED_SOURCES = $(addprefix $(SRC),$(SOURCES))

# Add prefixes to all of the object files
PREFIXED_OBJECTS = $(addprefix $(OBJ),$(OBJECTS))

# Add prefix to the program name
PREFIXED_PROGRAM = $(addprefix $(BIN),$(PROGRAM))

# Make 'all' instructions - this serves as the default target
all: $(PREFIXED_SOURCES) $(PREFIXED_PROGRAM)

# Make '$(PROGRAM)' instructions
$(PREFIXED_PROGRAM): $(PREFIXED_OBJECTS)
	@mkdir -p $(@D) # Makes the 'bin' directory if it doesn't exist
	$(CXX) $(PREFIXED_OBJECTS) -o $@ $(LIBS)

# Make '$(OBJECTS)' instructions
$(OBJ)%.o: $(SRC)%.cpp
	@mkdir -p $(@D) # Makes the directories if they don't exist
	$(CXX) -c $(FLAGS) $< -o $@

# Make 'clean' instructions
clean:
	rm -rf $(OBJ)
	rm -rf $(BIN)

# Include all of our dependency files, if they exist
-include $(addprefix $(OBJ),$(SOURCES:.cpp=.d))
