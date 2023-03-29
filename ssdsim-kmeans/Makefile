# ssdsim linux support
all:ssd 
	
clean:
	rm -f ssd *.o *~
.PHONY: clean

ssd: ssd.o avlTree.o flash.o initialize.o pagemap.o kmeans.o   
	cc -g -o ssd ssd.o avlTree.o flash.o initialize.o pagemap.o kmeans.o -lm
ssd.o: flash.h initialize.h pagemap.h
	gcc -c -g ssd.c
flash.o: pagemap.h
	gcc -c -g flash.c
initialize.o: avlTree.h pagemap.h
	gcc -c -g initialize.c
pagemap.o: initialize.h
	gcc -c -g pagemap.c
avlTree.o: 
	gcc -c -g avlTree.c
kmeans.o: kmeans.h
	gcc -c -g kmeans.c -lm

