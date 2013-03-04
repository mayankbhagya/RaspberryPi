CC=gcc
INCLUDES=-Iincludes/ -Iincludes/interface/vcos/pthreads/ -Iincludes/interface/vmcs_host/linux/
LDPATH=-L/opt/vc/lib/
LIBS=-lbcm_host -lEGL -lGLESv2

all: a.out

helloGL.o: helloGL.c
	$(CC) $(INCLUDES) -c  helloGL.c -o helloGL.o

a.out: helloGL.o
	$(CC) $(LIBS) $(LDPATH) helloGL.o -o a.out

clean:
	rm -rf *.o a.out

