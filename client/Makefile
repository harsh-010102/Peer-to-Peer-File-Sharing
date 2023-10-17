CC = g++
CFLAGS = -Wall
DEPS = headers.h
OBJ = file.o calcSHA.o  utils.o helpers.o client.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

client: $(OBJ)
	$(CC) $(CFLAGS) -pthread -o $@ $^ -lssl -lcrypto -I/usr/include/openssl

clean:
	rm -rf *o tracker