CC = g++
CFLAGS = -Wall
DEPS = headers.h
OBJ = calcSHA.o client.o utils.o helpers.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

client: $(OBJ)
	$(CC) $(CFLAGS) -pthread -o $@ $^ -lssl -lcrypto -I/usr/include/openssl

clean:
	rm -rf *o tracker