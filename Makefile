# Compilador e flags
CC = gcc
CFLAGS = -Iinclude -Wall -O3 -march=native
OBJ_DIR = obj
SRC_DIR = src
BIN_DIR = bin
BIN = $(BIN_DIR)/image_compressor

# Arquivos fonte e objetos
SRCS = $(SRC_DIR)/image_compression.c $(SRC_DIR)/quadtree.c $(SRC_DIR)/main.c
OBJS = $(OBJ_DIR)/image_compression.o $(OBJ_DIR)/quadtree.o $(OBJ_DIR)/main.o

# Alvo padrão
all: $(BIN)

# Verificar se o diretório bin existe e criar se necessário
$(BIN): $(OBJS)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CC) $(OBJS) -o $(BIN) $(CFLAGS)

# Regras para gerar os arquivos objeto
$(OBJ_DIR)/image_compression.o: $(SRC_DIR)/image_compression.c include/image_compression.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/image_compression.c -o $(OBJ_DIR)/image_compression.o

$(OBJ_DIR)/quadtree.o: $(SRC_DIR)/quadtree.c include/quadtree.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/quadtree.c -o $(OBJ_DIR)/quadtree.o

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c include/image_compression.h include/quadtree.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o

# Limpeza de arquivos gerados
clean:
	del /Q $(OBJ_DIR)\*.o $(BIN)

# Executar o programa (opcional)
run: all
	./$(BIN) images/input/input.pgm images/output/output.pgm
