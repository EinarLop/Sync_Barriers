finally: program
	./program

program: barrier.o mat.o
	gcc -o program barrier.o mat.o -pthread

barrier.o:
	gcc -c -pthread barrier.c

mat.o:
	gcc -c -pthread mat.c

clean:
	rm barrier.o mat.o program