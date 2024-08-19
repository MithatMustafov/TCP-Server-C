# Define the directories
BIN_DIR = bin
SRC_DIR = src

# Define the target executables
SERVER_TARGET = $(BIN_DIR)/server.exe
CLIENT_TARGET = $(BIN_DIR)/client.exe

# Define the source files
SERVER_SRC = $(SRC_DIR)/server.c
CLIENT_SRC = $(SRC_DIR)/client.c
TCP_SRC = $(SRC_DIR)/tcp.c
VECTOR_SRC = $(SRC_DIR)/vector.c

# Define the object files
SERVER_OBJ = $(BIN_DIR)/server.o
CLIENT_OBJ = $(BIN_DIR)/client.o
TCP_OBJ = $(BIN_DIR)/tcp.o
VECTOR_OBJ = $(BIN_DIR)/vector.o

# Define compiler and flags
CC = gcc
CFLAGS = -Wall -I$(SRC_DIR)
LDFLAGS = -lws2_32

# The default target to build
all: $(SERVER_TARGET) $(CLIENT_TARGET)

# Rule to build the server executable
$(SERVER_TARGET): $(SERVER_OBJ) $(TCP_OBJ) $(VECTOR_OBJ)
	@echo "Linking $(SERVER_TARGET)"
	$(CC) $(CFLAGS) -o $(SERVER_TARGET) $(SERVER_OBJ) $(TCP_OBJ) $(VECTOR_OBJ) $(LDFLAGS)

# Rule to build the client executable
$(CLIENT_TARGET): $(CLIENT_OBJ) $(TCP_OBJ) $(VECTOR_OBJ)
	@echo "Linking $(CLIENT_TARGET)"
	$(CC) $(CFLAGS) -o $(CLIENT_TARGET) $(CLIENT_OBJ) $(TCP_OBJ) $(VECTOR_OBJ) $(LDFLAGS)

# Rule to build the server object file
$(BIN_DIR)/server.o: $(SERVER_SRC) $(SRC_DIR)/tcp.h
	@echo "Compiling $(SERVER_SRC)"
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CC) $(CFLAGS) -c $(SERVER_SRC) -o $(BIN_DIR)/server.o

# Rule to build the client object file
$(BIN_DIR)/client.o: $(CLIENT_SRC) $(SRC_DIR)/tcp.h
	@echo "Compiling $(CLIENT_SRC)"
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CC) $(CFLAGS) -c $(CLIENT_SRC) -o $(BIN_DIR)/client.o

# Rule to build the TCP object file
$(BIN_DIR)/tcp.o: $(TCP_SRC) $(SRC_DIR)/tcp.h
	@echo "Compiling $(TCP_SRC)"
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CC) $(CFLAGS) -c $(TCP_SRC) -o $(BIN_DIR)/tcp.o

# Rule to build the vector object file
$(BIN_DIR)/vector.o: $(VECTOR_SRC)
	@echo "Compiling $(VECTOR_SRC)"
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CC) $(CFLAGS) -c $(VECTOR_SRC) -o $(BIN_DIR)/vector.o

# Rule to clean up build files
clean:
	@echo "Cleaning up..."
	del /Q $(BIN_DIR)\*.exe $(BIN_DIR)\*.o

# Rule to create a new clean build
rebuild: clean all

.PHONY: all clean rebuild
