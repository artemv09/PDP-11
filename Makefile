CC = g++
PROGRAM = rezults


allp:
	$(CC) main.c compiler.c emulator.c function.c memory.c -o $(PROGRAM)

run: allp
	./$(PROGRAM)

clean:
	rm -f *.dot *.png