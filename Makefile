###############################################################################
#                              Program Specifics                              #
###############################################################################

# Program name
PROGRAM = MMSim

LIBS = -lGL -lglut -lGLU

###############################################################################
#                     Don't edit code any below this line                     #
###############################################################################

# Flags
# -g: Add debugging symbols
# -Os: Optimize code for size
# -std=c++11: Use the C++11 standard
# -Wall: Show all warning messages
FLAGS = -Os -std=c++0x -Wall

# Directories
SRC = ./src/
OBJ = ./obj/
BIN = ./bin/

# Source files
PRELIMSOURCES = $(shell find $(SRC) -name '*.cpp')
SOURCES = $(PRELIMSOURCES:$(SRC)%=%) 

# Object files that mirror the source files
OBJECTS = $(SOURCES:.cpp=.o)

# Prefixed Source files
PREFIXED_SOURCES = $(addprefix $(SRC),$(SOURCES))

# Prefixed Object files
PREFIXED_OBJECTS = $(addprefix $(OBJ),$(OBJECTS))

# Prefixed Program name
PREFIXED_PROGRAM = $(addprefix $(BIN),$(PROGRAM))

# Make 'all' instructions - default target
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
# 'dir' gets the directories of the object files
# 'sort' sorts the directories and removes duplicates
# 'add' is pretty self-explanatory 
clean:
	rm -f $(addprefix $(OBJ), $(addsuffix *.o, $(sort $(dir $(OBJECTS)))))
