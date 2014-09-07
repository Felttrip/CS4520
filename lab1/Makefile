
CC= gcc

CLEAN= rm -rf *.o copy

all: copy

clean:
	$(CLEAN)

copy: as1.o
	$(CC) $^ -o $@

as1.o: as1.c 
	$(CC) -c $^
