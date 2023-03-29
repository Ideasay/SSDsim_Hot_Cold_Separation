# ssdsim linux support
all:ssd 
	
clean:
	rm -f ssd *.o *~
.PHONY: clean

ssd: ssd.o avlTree.o flash.o initialize.o pagemap.o HotCold.o    
	cc -g -o ssd ssd.o avlTree.o flash.o initialize.o pagemap.o HotCold.o
ssd.o: flash.h initialize.h pagemap.h
	gcc -c -g ssd.c
flash.o: pagemap.h
	gcc -c -g flash.c
initialize.o: avlTree.h pagemap.h
	gcc -c -g initialize.c
pagemap.o: initialize.h HotCold.h
	gcc -c -g pagemap.c
avlTree.o: 
	gcc -c -g avlTree.c
HotCold.o: HotCold.h
	gcc -c -g HotCold.c
