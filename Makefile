CFLAGS=-std=c11 -g -static

kwcc: kwcc.c
	cc $(CFLAGS) -o kwcc kwcc.c

test: kwcc
		./test.sh

clean:
		rm -f kwcc *.o *~ tmp*

.PHONY: test clean
