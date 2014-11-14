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
FLAGS = -Os -std=c++11 #-Wall TODO: Turn this back on and get rid of all warnings

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
# 'dir' gets the directories of the object files
# 'sort' sorts the directories and removes duplicates
# 'add' is pretty self-explanatory 
clean:
	rm -f $(addprefix $(OBJ), $(addsuffix *.o, $(sort $(dir $(OBJECTS)))))
	rm -f $(BIN)*
