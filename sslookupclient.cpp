/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * and even more updated by Gina Lawhon, April 2023
 * Mount Vernon Nazarene University
 * 
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching 
 * field name, and returing a "form_iterator" oject, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 * refers to the actual string entered in the form's "verse" field.
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Bible.h"
#include "Ref.h"
#include "Verse.h"
#include "fifo.h"
using namespace std;

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"
using namespace cgicc;

#define logging
#define LOG_FILENAME "/home/class/csc3004/tmp/ginlawhon-sslookupclient.log"
#include "logfile.h"


int main() {

 #ifdef logging 
 logFile.open(LOG_FILENAME, ios::out); 
 #endif


  /* A CGI program must send a response header with content type
   * back to the web client before any other output.
   * For an AJAX request, our response is not a complete HTML document,
   * so the response type is just plain text to insert into the web page.
   */
  //Create the webBible to use in the rest of the code
  Bible webBible("/home/class/csc3004/Bibles/web-complete");

  cout << "Content-Type: text/plain\n\n";
  
  Cgicc cgi;  // create object used to access CGI request data

  // GET THE INPUT DATA
  // browser sends us a string of field name/value pairs from HTML form
  // retrieve the value for each appropriate field name
  form_iterator st = cgi.getElement("search_type");
  form_iterator book = cgi.getElement("book");
  form_iterator chapter = cgi.getElement("chapter");
  form_iterator verse = cgi.getElement("verse");
  form_iterator nv = cgi.getElement("num_verse");

  // Convert and check input data
  bool validChapterInput = false;
  if (chapter != cgi.getElements().end()) {
	 int chapterNum = chapter->getIntegerValue();
	 if (chapterNum > 150) {
		 cout << "<p>The chapter number (" << chapterNum << ") is too high.</p>" << endl;
	 } else if (chapterNum < 0) {
		 cout << "<p>The chapter must be a positive number.</p>" << endl;
	 } else if (chapterNum == 0){
		 cout << "<p>Your chapter input is either 0 or empty! Please retry.</p>" << endl;
	 }else
		 validChapterInput = true;
  }

  bool validVerseInput = false;
  if (verse != cgi.getElements().end()) {
	 int verseNum = verse->getIntegerValue();
	 if (verseNum > 176) {
		 cout << "<p>The verse number (" << verseNum << ") is too high.</p>" << endl;
	 }
	 else if (verseNum < 0) {
		 cout << "<p>The verse must be a positive number.</p>" << endl;
	 } else if (verseNum == 0){
		 cout << "<p>Your verse input is either 0 or empty! Please retry.</p>" << endl;
	 }else
		 validVerseInput = true;
  }


  /*
  //This section of code is just in case the program changes to where the user inputs a number for the book instead of the dropdown boxes
  bool validBookInput = false;
  if(book != cgi.getElements().end()){
	int bookNum = book->getIntegerValue();
	if(bookNum > 65) {
		cout << "<p>The book you have selected is too high!</p>" << endl;
	} else if (bookNum <= 0){
		cout << "<p>The book musrt be a positive number.</p>" << endl;
	}
	else
		validBookInput = true;
  }
  */
  
  /* This starts my main part of the code! The general premise is this:
	-Put the user input into variables through use of getIntegerValue();
	-With that input, make a reference to correspond to a place in the Bible
	-Send the ref through a pipe to the server, which will process the lookup over there
	-The server will send it back, and the we will process the text here
	-If the result of that lookup is success, then output the verse!
	-If not, run the error code for the line of code
  */

  //inititalizing a result variable to keep track of the verse status
  LookupResult result;

	string send_pipe = "SSrequest";
	string receive_pipe = "SSreply";

	//book
	int b = book->getIntegerValue();
	//chapter
	int c = chapter->getIntegerValue();
	//starting verse
	int v = verse->getIntegerValue();
	//number of verses
	int numv = nv->getIntegerValue();
	log("Book, Chapter, Verse, and NumV variables have been made");

	//make the ref from the inputs
        Ref ref(b, c, v);
	log("Ref has been made");
        
	//assign a value to the bookmark verse, which we will call parsedVerse
	//parsedVerse = webBible.lookup(ref, result);

  	//create the pipes to send stuff to the server and get information back
  	Fifo sendfifo(send_pipe);
  	Fifo recfifo(receive_pipe);
	log("Pipes have been made");

	//open the sending pipe
	sendfifo.openwrite();
	log("Pipes have been opened");

	//send the ref over
	string refString = to_string(b) + ":" + to_string(c) + ":" + to_string(v) + ":" + to_string(numv);
	sendfifo.send(refString);
	log("Ref (string) has been sent");
	
	
	//if SUCCESS, then print the verse
	//if ERROR, print the error message

if(validChapterInput && validVerseInput){
	//quick endl; to make things formatted nicely
	cout << endl;
	
	//print out book and chapter num
	cout << "<br><b>" <<ref.gBook() << ":" << ref.getChap() << " </b><br><br>";	
	//open up the receiving pipe so we can get the stuff back from the server
        recfifo.openread();
	for(int i = 0; i < numv; i++){
		//receive each status+verse text each time the loop runs - for each verse

		string resultText = recfifo.recv();
		string buffer = resultText;
		log("You are in the loop, we've received the result text from the server");

		//parse the string into separate strings
		string statusText = GetNextToken(buffer, "$").c_str();
		string verseText = GetNextToken(buffer, "$").c_str();
		log("We have parsed the text received from the pipe into statusText (the status enumeration) and the actual verseText");


		//if the status is SUCCESS, we output the line, if not, we stop the loop and output the error message
		if(statusText == "0"){
			cout << verseText;
			log("The verse text has been printed out!");
		} else{
			//this "verseText" is the error we have received from the server
			cout << verseText;
			break;
		}
	}


}
	recfifo.fifoclose();

  return 0;
}
