CC = cc
PROGRAM = unpfs

$(PROGRAM): *.c
	$(CC) $@.c -o $@

clean:
	rm -f $(PROGRAM)
