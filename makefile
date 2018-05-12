CC= gcc
CFLAGS= -Wall -lm
DEPS= bmpFunc.h cmd.h
OBJ= bmpFunc.o cmd.o main.o
PROG= main

$(PROG): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -rf $(PROG) $(OBJ)
