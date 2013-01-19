APPNAME=rts
SRC=launcher.c worker.c
CFLAGS=-O2 -Wall -pedantic
SHARED_LIBRARIES=-lpthread -lrt

all: $(APPNAME)
	
$(APPNAME):$(SRC)
	gcc $(SRC) -o $(APPNAME) $(CFLAGS) $(SHARED_LIBRARIES)

clean:
	rm -f $(APPNAME)
