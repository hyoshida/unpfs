CC = cc
PROGRAM = unpack

$(PROGRAM): *.c
	$(CC) $@.c -o $@

clean:
	rm -f $(PROGRAM)
