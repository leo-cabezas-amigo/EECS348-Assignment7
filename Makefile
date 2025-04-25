CC = g++
CFLAGS =  # No CFLAGS by default.
LFLAGS = -lmysqlcppconn

Assignment7: Assignment7.o
	$(CC) $(CFLAGS) Assignment7.o -o Assignment7 $(LFLAGS)

Assignment7.o: Assignment7.cpp
	$(CC) $(CFLAGS) -c Assignment7.cpp

clean:
	rm -f Assignment7 *.o