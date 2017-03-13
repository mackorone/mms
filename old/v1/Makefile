# Program name
PROGRAM = MMSim

# Directories
SRC = ./src/
OBJ = ./obj/
BIN = ./bin/

# Any external libraries
LIBS = -lGL -lglut -lGLU -lpthread

# Any extra include paths
INC =

# Flags
# -std=c++11: Use the C++11 standard
# -MMD: Automatically generate dependency files
FLAGS = -MMD -std=c++11

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
	$(CXX) $(PREFIXED_OBJECTS) -o $@ $(LIBS) $(INC)

# Make '$(OBJECTS)' instructions
$(OBJ)%.o: $(SRC)%.cpp
	@mkdir -p $(@D) # Makes the directories if they don't exist
	$(CXX) -c $(FLAGS) $< -o $@ $(INC)

# Make 'clean' instructions
clean:
	rm -rf $(OBJ)
	rm -rf $(BIN)

# Include all of our dependency files, if they exist
-include $(addprefix $(OBJ),$(SOURCES:.cpp=.d))
