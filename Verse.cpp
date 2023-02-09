// Verse class function definitions
// Computer Science, MVNU

#include "Verse.h"
#include <iostream>
using namespace std;    
// Define delimiter characters used in Bible files
#define DELIM  ".,;:'\" ?"  // characters that separate tokens

Verse::Verse() {  	// Default constructor
    verseText = "Uninitialized Verse!";
    verseRef = Ref();
    verseNum = 0;
} 

// REQUIRED: Parse constructor - pass verse string from file
Verse::Verse(const string s) {
	// use Ref constructor to create verseRef
	// store the remainder of the string in verseText
    string buffer = s;
    int book = atoi(GetNextToken(buffer, DELIM).c_str());
    int chap = atoi(GetNextToken(buffer, DELIM).c_str());   
    int verse = atoi(GetNextToken(buffer, DELIM).c_str());
    verseRef = Ref(book, chap, verse);
    verseText = buffer;
    verseNum = verse;
}

// REQUIRED: Accessors
string Verse::getVerse() {
    return verseText;
}

Ref Verse::getRef() {
    return verseRef;
}

// display reference and verse
void Verse::display() {
    cout << verseNum << " " << verseText;
 }
