# Bibleajax program
# Bob Kasper, MVNU Computer Science
# updated January 2020 to use
# c++11 compiler option, current paths on cs.mvnu.edu

# This is a Makefile for the Bible web app demo program.
# Copy this directory to a location within your home directory. 
# Change the USER name value below to your own user name.
# Then use "make" to build the server program,
# and deploy it to the live web server directory.
# To test the program, go to http://cs.mvnu.edu/class/csc3004/USER/
# and open the bibleajax.html link.

# TO DO: Must replace "username" by your username
USER= ginlawhon

# Use GNU C++ compiler with C++11 standard
CC= g++
CFLAGS= -g -std=c++11

all: testreader sslookupserver sslookupclient.cgi PutCGI PutHTML

#add these bad boys in later: bibleajax.cgi 

# TO DO: For bibleajax.cgi, add dependencies to include
# compiled classes from Project 1 to be linked into the executable program

# Ref Object
Ref.o : Ref.h Ref.cpp
	$(CC) $(CFLAGS) -c Ref.cpp

# Verse Object
Verse.o : Ref.h Verse.h Verse.cpp
	$(CC) $(CFLAGS) -c Verse.cpp

# Bible Object
Bible.o : Ref.h Verse.h Bible.h Bible.cpp
	$(CC) $(CFLAGS) -c Bible.cpp

#fifo object?
fifo.o : fifo.h
	$(CC) $(CFLAGS) -c fifo.cpp

testreader: testreader.o Ref.o Verse.o Bible.o
		$(CC) $(CFLAGS) -o testreader testreader.o Ref.o Verse.o Bible.o -lcgicc

testreader.o: testreader.cpp Ref.h Verse.h Bible.h
		$(CC) $(CFLAGS) -c testreader.cpp

sslookupserver: sslookupserver.o Ref.o Verse.o Bible.o fifo.o
		$(CC) $(CFLAGS) -o sslookupserver sslookupserver.o Ref.o Verse.o Bible.o fifo.o -lcgicc

sslookupserver.o: sslookupserver.cpp Ref.h Verse.h Bible.h fifo.h
		$(CC) $(CFLAGS) -c sslookupserver.cpp 

sslookupclient.cgi:	sslookupclient.o Ref.o Verse.o Bible.o fifo.o
		$(CC) $(CFLAGS) -o sslookupclient.cgi sslookupclient.o Ref.o Verse.o Bible.o fifo.o -L/usr/local/lib -lcgicc
		# -l option is necessary to link with cgicc library


#bibleajax.cgi:	bibleajax.o Ref.o Verse.o Bible.o
		#$(CC) $(CFLAGS) -o bibleajax.cgi bibleajax.o Ref.o Verse.o Bible.o -lcgicc
		# -l option is necessary to link with cgicc library


# main program to handle AJAX/CGI requests for Bible references
bibleajax.o:	bibleajax.cpp Ref.h Verse.h Bible.h
		$(CC) $(CFLAGS) -c bibleajax.cpp
			
PutCGI:	sslookupclient.cgi
		chmod 755 sslookupclient.cgi
		rm -f /var/www/html/class/csc3004/$(USER)/cgi-bin/sslookupclient.cgi
		cp sslookupclient.cgi /var/www/html/class/csc3004/$(USER)/cgi-bin

		echo "Current contents of your cgi-bin directory: "
		ls -l /var/www/html/class/csc3004/$(USER)/cgi-bin/

PutHTML:
		cp bibleindex.html /var/www/html/class/csc3004/$(USER)

		echo "Current contents of your HTML directory: "
		ls -l /var/www/html/class/csc3004/$(USER)

clean:		
		rm *.o core bibleajax.cgi
