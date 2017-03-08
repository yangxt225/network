#CC=gcc
CC=/usr/local/arm/4.3.2/bin/arm-linux-gcc
CFLAGS=-Wall -O2
ALIB=libnetwork.a
TARGET=maintest
MYSERVER=openserver
AR=ar
ARFLAGS=-rc

staticLib: $(ALIB)
main: $(TARGET)
server: $(MYSERVER)

$(TARGET): net_test.o
	$(CC) -o $(TARGET) net_test.o -L. -lnetwork

$(ALIB): libnetwork.o
	$(AR) $(ARFLAGS) $(ALIB) libnetwork.o

$(MYSERVER): server_demo.o
	$(CC) -o $(MYSERVER) server_demo.o
	
%.o: %.c
	$(CC) -c $< $(CLAGS)

clean:
	rm -f *.o
	rm -f $(ALIB)
	rm -f $(TARGET)
	rm -f $(MYSERVER)

