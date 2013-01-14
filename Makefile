APPNAME=rts
SRC=launcher.c
CFLAGS=-O2 -Wall
SHARED_LIBRARIES=-lpthread

all: $(APPNAME)
	
$(APPNAME):$(SRC)
	gcc $(SRC) -o $(APPNAME) $(CFLAGS) $(SHARED_LIBRARIES)

clean:
	rm -f $(APPNAME)
