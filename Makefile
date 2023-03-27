CC := gcc -std=c99 -O0

build: write_img.o bmp.o
	$(CC) write_img.o bmp.o -lm -o write_img

write_img.o: write_img.c bmp.h
	$(CC) -c write_img.c -o write_img.o

bmp.o: bmp.c bmp.h
	$(CC) -c bmp.c -o bmp.o

run: write_img
	./write_img

clean:
	rm write_img.o bmp.o write_img