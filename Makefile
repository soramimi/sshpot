
CXXFLAGS = -O2

all: sshpot

sshpot: main.o auth.o
	g++ $^ -lssh -o $@
	strip $@

clean:
	rm -f *.o
	rm -f sshpot

install:
	install sshpot /var/sshpot
	install sshpot.rsa.key /var/sshpot/
	install app/index.cgi /var/www/html/
	install app/style.css /var/www/html/
