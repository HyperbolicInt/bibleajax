
/*sslookupserver*/

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "fifo.h"
#include "Bible.h"

using namespace std;

string receive_pipe = "SSrequest";
string send_pipe = "SSreply";

#define logging
#define LOG_FILENAME "/home/class/csc3004/tmp/ginlawhon-sslookupserver.log"
#include "logfile.h"


int main(){

 #ifdef logging 
 logFile.open(LOG_FILENAME, ios::out); 
 #endif

// Create Bible object
// which also creates the index, so we only have to create it once and can use it a bunch
Bible webBible("/home/class/csc3004/Bibles/web-complete");

/* Create the communication fifos */
Fifo recfifo(receive_pipe);
Fifo sendfifo(send_pipe);

while(true){
	//open the pipes to recieve information
	recfifo.openread();
	sendfifo.openwrite();

	//get info from client (ref)
	//find ref in the index
	//if found, send back verse
	//if not, send back uninitialized verse and error!
	cout << "Loop has started" << endl;
	//if the ref index for some reason didn't get made
	if(!webBible.getDoesRefIndexExist()){
		sendfifo.send("The Ref Index does not exist. This error should never really run.");
	}
	
	string buffer = recfifo.recv();
	int bookN = atoi(GetNextToken(buffer, ":").c_str());
	int chapN = atoi(GetNextToken(buffer, ":").c_str());   
	int verseN = atoi(GetNextToken(buffer, ":").c_str());
	int numVerse = atoi(GetNextToken(buffer, ":").c_str());
	log("Information (book, chapter, number, number of verses) has been received from the client");
	

	Ref ref(bookN, chapN, verseN);

	LookupResult result;

	Verse verse = webBible.lookup(ref, result);

	cout << "Ref and result variables has been made" << endl;
	cout << "BookN: " << bookN << " ";
	cout << "ChapN: " << chapN << " ";
	cout << "VerseN: " << verseN << " " << endl;

	ref.display();
	cout << endl;
		
	//if verse is not empty (result is success)

	if(result == SUCCESS){
		int currentVerseNumber = 0;
		int previousVerseNumber = 0;
			string verseText = to_string(result) + "$" + "<b>" + to_string(verseN) + "</b>" + verse.getVerse();
			sendfifo.send(verseText);
			log("We have parsed the first verse and have sent it to the client");
			previousVerseNumber = verse.getVerseNum();
			//cout << "Previous Verse Number: " << previousVerseNumber << endl;
			cout << verseText << endl;
				for(int i = 1; i < numVerse; i++){
					Verse nextVerse = webBible.nextVerse(result);
					currentVerseNumber = nextVerse.getVerseNum();
					if(result == SUCCESS){
						if(currentVerseNumber < previousVerseNumber){
							verseText = to_string(result) + "$" + "<br><b>" + nextVerse.getVerseBook() + " " + nextVerse.getVerseChap() + "</b><br>" + "<b>" + to_string(nextVerse.getVerseNum()) + "</b> " + nextVerse.getVerse() + " ";
							sendfifo.send(verseText);
							log("We have parsed the next verse of the request. This one has a chapter header.");
							previousVerseNumber = currentVerseNumber;
							log("chapter header should have been printed out");
							//should print out every verse we request on the terminal side
							cout << verseText << endl;
						} else {
							verseText = to_string(result) + "$" + "<b>" + to_string(nextVerse.getVerseNum()) + "</b> " + nextVerse.getVerse() + " ";
							sendfifo.send(verseText);
							log("We have parsed the next verse of the request.");
							previousVerseNumber = currentVerseNumber;
							//should print out every verse we request on the terminal side
							cout << verseText << endl;
						}

					} else
						string errorText = "ERROR$" + webBible.error(result);
						log("The next verse was an error.");
				}
			//cout << verseText << endl;
			//cout << "We have pushed the verse text to the pipe" << endl;

		} else {
			string errorText = "ERROR$" + webBible.error(result);
			sendfifo.send(errorText);

			//the "&" at the end of the file is to let the client know when to stop reading the file.
			sendfifo.send("&");

			//cout << errorText << endl;
			//cout << "Phony verse object, we have pushed the error text to the pipe" << endl;
		}

	//if verse is poodoo
	//send back the status
	//make string
	//parse the status
	cout << "End of loop" << endl;


	sendfifo.fifoclose();
	recfifo.fifoclose();
	log("Pipes are closed");
	}
}


