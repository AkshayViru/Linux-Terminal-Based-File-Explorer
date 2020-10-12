CC = g++
CFLAGS = -Wall 
DEPS = header.h 
OBJ = main.o cmdModeOperations.o commandMode.o longListing.o modes.o normalMode.o parseCmd.o utilities.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *o main