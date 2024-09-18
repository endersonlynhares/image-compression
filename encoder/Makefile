all: ./bin/encoder

./bin/encoder: ./obj/quadtree.o ./obj/image.o ./obj/main.o 
	gcc ./obj/*.o -o ./bin/encoder -lm
./obj/image.o: ./src/image.c
	gcc -o ./obj/image.o -c ./src/image.c -lm
./obj/quadtree.o: ./src/quadtree.c
	gcc -o ./obj/quadtree.o -c ./src/quadtree.c -lm
./obj/main.o: ./src/main.c
	gcc -o ./obj/main.o -c ./src/main.c -lm

clean:
	rm ./obj/*.o ./bin/encoder