all: ./bin/decoder

./bin/decoder: ./obj/quadtree.o ./obj/image.o ./obj/bits.o ./obj/decoder.o 
	gcc ./obj/*.o -o ./bin/decoder
./obj/image.o: ./src/image.c
	gcc -o ./obj/image.o -c ./src/image.c
./obj/quadtree.o: ./src/quadtree.c
	gcc -o ./obj/quadtree.o -c ./src/quadtree.c
./obj/bits.o: ./src/bits.c
	gcc -o ./obj/bits.o -c ./src/bits.c
./obj/decoder.o: ./src/decoder.c
	gcc -o ./obj/decoder.o -c ./src/decoder.c

clean:
	rm ./obj/*.o ./bin/decoder