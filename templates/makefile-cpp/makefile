# Clean makefile based on https://stackoverflow.com/questions/30573481/how-to-write-a-makefile-with-separate-source-and-header-directories
# Modified to use g++. 

##############################
#	USER DEFINED VARIABLES	 #
##############################
# Set the name of the target executable
PROJECT_NAME := main
# Add any arguments to the `make run` routine, if applicable 
RUN_ARGS := 

# Add any include directories required in addition to the project include directory.
# For example, if you are linking with another project in the repository add it's include directory on a new line here,
# prefixed by -I and suffixed with \. 
INCLUDE_DIRS = \
-Iinclude \


LIB_DIRS = -Llib 

##############################
#      STATIC VARIABLES      #
##############################
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/$(PROJECT_NAME)
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

##############################
#       COMPILER SETUP       #
##############################
CXX=g++
CPPFLAGS := $(INCLUDE_DIRS) -MMD -MP
CFLAGS   := -Wall -g
LDFLAGS  := $(LIB_DIRS) 
LDLIBS   := -lm 

##############################
#        MAKE TARGETS        #
##############################
.PHONY: all clean run
all: $(EXE)

# Primary Target
$(EXE): $(OBJ) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ 

# Ensure $(BIN_DIR) and $(OBJ_DIR) exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# compile and run the program directly after with supplied arguments in $(RUN_ARGS) above
run: $(EXE)
	@echo -e ''
	@./$(EXE) $(RUN_ARGS)

# Clean up project outputs
# Note: The @ disables echoing the command 
clean: 
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

# Note: The - is used to silence errors if the files don't exist yet 
-include $(OBJ:.o=.d)
