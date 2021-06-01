# Clean makefile based on https://stackoverflow.com/questions/30573481/how-to-write-a-makefile-with-separate-source-and-header-directories
# Modified to use g++. 

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/sim-interface
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CXX=g++
CPPFLAGS := -Iinclude -MMD -MP
CFLAGS   := -Wall 
LDFLAGS  := -Llib 
LDLIBS   := -lm 

.PHONY: all clean 

all: $(EXE)

# Primary Target
$(EXE): $(OBJ) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ 

# Ensure $(BIN_DIR) and $(OBJ_DIR) exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Note: The @ disables echoing the command 
clean: 
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

# Note: The - is used to silence errors if the files don't exist yet 
-include $(OBJ:.o=.d)
