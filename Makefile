finally: program
	./program

test: testprogram
	./testprogram

program: barrier.o mat.o
	gcc -o program barrier.o mat.o -pthread

testprogram: barrier.o mm.o
	gcc -o testprogram barrier.o mm.o -pthread

barrier.o:
	gcc -c -pthread barrier.c

mat.o:
	gcc -c -pthread mat.c

mm.o:
	gcc -c -pthread mm.c

clean:
	rm barrier.o mat.o program