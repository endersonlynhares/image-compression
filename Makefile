# Definir o compilador a ser utilizado
CC = gcc-12

# Definir as flags de linkedição
LDFLAGS = -lm

# Regra padrão para compilar o programa
all: ./bin/imageCompressor

# Regra para criar o executável a partir dos arquivos objeto
./bin/imageCompressor: ./obj/quadtree.o ./obj/image.o ./obj/main.o 
	# Linkar os arquivos objeto para criar o executável
	$(CC) ./obj/*.o -o ./bin/imageCompressor $(LDFLAGS)

# Regra para compilar o arquivo de fonte image.c em um arquivo objeto
./obj/image.o: ./src/image.c
	# Compilar o arquivo fonte image.c em um arquivo objeto image.o
	$(CC) -o ./obj/image.o -c ./src/image.c $(LDFLAGS)

# Regra para compilar o arquivo de fonte quadtree.c em um arquivo objeto
./obj/quadtree.o: ./src/quadtree.c
	# Compilar o arquivo fonte quadtree.c em um arquivo objeto quadtree.o
	$(CC) -o ./obj/quadtree.o -c ./src/quadtree.c $(LDFLAGS)

# Regra para compilar o arquivo de fonte main.c em um arquivo objeto
./obj/main.o: ./src/main.c
	# Compilar o arquivo fonte main.c em um arquivo objeto main.o
	$(CC) -o ./obj/main.o -c ./src/main.c $(LDFLAGS)

# Regra para limpar arquivos gerados (arquivos objeto e o executável)
clean:
	# Remover arquivos objetos e o executável
	rm ./obj/*.o ./bin/imageCompressor
