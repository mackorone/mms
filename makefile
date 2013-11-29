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
FLAGS = -g -Wall

# Directories
SRC = ./src/
OBJ = ./obj/
BIN = ./bin/

# Source files
PRELIMSOURCES = $(wildcard $(SRC)/**.cpp)
SOURCES =$(PRELIMSOURCES:$(SRC)/%=%)

# Object files
OBJECTS=$(SOURCES:.cpp=.o)

# Prefixed Source files
PREFIXED_SOURCES=$(addprefix $(SRC),$(SOURCES))

# Prefixed Object files
PREFIXED_OBJECTS=$(addprefix $(OBJ),$(OBJECTS))

# Prefixed Program name
PREFIXED_PROGRAM=$(addprefix $(BIN),$(PROGRAM))

# Make 'all' instructions
all: $(PREFIXED_SOURCES) $(PREFIXED_PROGRAM)

# Make '$(PROGRAM)' instructions
$(PREFIXED_PROGRAM): $(PREFIXED_OBJECTS)
	$(CXX) $(PREFIXED_OBJECTS) -o $@ $(LIBS)

# Make '$(OBJECTS)' instructions
$(OBJ)%.o: $(SRC)%.cpp
	$(CXX) -c $(CFlags) $< -o $@ -std=c++0x

# Make 'clean' instructions
clean:
	rm -f $(OBJ)*
