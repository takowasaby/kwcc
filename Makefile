CFLAGS=-std=c11 -g -static -Wall -pedantic
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

kwcc: $(OBJS)
	cc -o kwcc $(OBJS) $(LDFLAGS)  

$(OBJS): kwcc.h

test: kwcc
		./test.sh

clean:
		rm -f kwcc *.o *~ tmp*

.PHONY: test clean
