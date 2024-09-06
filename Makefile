all: ./bin/imageCompressor

./bin/imageCompressor: ./obj/quadtree.o ./obj/image.o ./obj/main.o 
	gcc ./obj/*.o -o ./bin/imageCompressor
./obj/image.o: ./src/image.c
	gcc -o ./obj/image.o -c ./src/image.c
./obj/quadtree.o: ./src/quadtree.c
	gcc -o ./obj/quadtree.o -c ./src/quadtree.c
./obj/main.o: ./src/main.c
	gcc -o ./obj/main.o -c ./src/main.c

clean:
	rm ./obj/*.o ./bin/imageCompressor