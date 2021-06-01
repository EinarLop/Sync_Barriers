finally: program
	./program

program: barrier.o mm.o
	gcc -o program barrier.o mm.o -pthread

barrier.o:
	gcc -c -pthread barrier.c

mm.o:
	gcc -c -pthread mm.c

clean:
	rm barrier.o mm.o program