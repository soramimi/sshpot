
CXXFLAGS = -O2

all: sshpot

sshpot: main.o auth.o
	g++ $^ -lssh -o $@
	strip $@

clean:
	rm -f *.o
	rm -f sshpot
