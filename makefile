# compiler
CC=g++

# compiler flags
CFLAGS= -Wall -g

#directories and sources
SRC_DIR = .
HEADER_DIR = lib
OBJ_DIR = bin

# all command
all: $(OBJ_DIR)/main

# compiling the main executable in the bin directory
$(OBJ_DIR)/main: $(OBJ_DIR)/.dirstamp $(SRC_DIR)/*.cpp $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) -o $@ $(SRC_DIR)/*.cpp


#create object files
$(OBJ_DIR)/main.o: main.cpp $(OBJ_DIR)/.dirstamp $(OBJ_DIR)/user.o $(OBJ_DIR)/movie.o
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/user.o: user.cpp user.h $(OBJ_DIR)/.dirstamp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/movie.o: movie.cpp movie.h $(OBJ_DIR)/.dirstamp $(HEADER_DIR)/*.h
	$(CC) $(CFLAGS) -c $< -o $@

# make sure bin exists
$(OBJ_DIR)/.dirstamp:                                                                            
	-@mkdir -p $(OBJ_DIR) && touch $@

# memcheck rule
.PHONY: memcheck
memcheck: $(OBJ_DIR)/main
	valgrind -v --tool=memcheck --leak-check=yes $(OBJ_DIR)/main mdata.txt

# make clean
.PHONY: clean
clean:
	rm -rf *~ *.o *.dSYM main
	rm -rf bin
	rm -rf data/*~

