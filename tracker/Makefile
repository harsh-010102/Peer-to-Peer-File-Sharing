CC = g++
CFLAGS = -Wall
DEPS = headers.h
OBJ = customClass.o tracker.o utils.o helper.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

tracker: $(OBJ)
	$(CC) $(CFLAGS) -pthread -o $@ $^ 

clean:
	rm -rf *o tracker