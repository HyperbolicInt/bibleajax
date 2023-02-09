// Bible class function definitions
// Computer Science, MVNU

#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

Bible::Bible() { // Default constructor
	infile = "/home/class/csc3004/Bibles/web-complete";
    instream.open(infile.c_str(), ios::in);
    if (!instream) {
        cerr << "Error - can't open input file: " << infile << endl;
        exit(2);
    }
}

// Constructor – pass bible filename
Bible::Bible(const string s) { 
    infile = s;
    instream.open(infile.c_str(), ios::in);
    isOpen = true;
    if (!instream) {
        cerr << "Error - can't open input file: " << infile << endl;
        exit(2);
    }
}

// REQUIRED: lookup finds a given verse in this Bible
Verse Bible::lookup(Ref ref, LookupResult& status) { 
    // TODO: scan the file to retrieve the line that holds ref ...
    // update the status variable
	status = OTHER; // placeholder until retrieval is attempted
    string l;
    bool sentinel = true;
    bool bookFound = false;
    bool chapFound = false;
    bool verseFound = false;
    //error verse to return if verse is not found
    Verse eVerse = Verse("1:1:1 Error verse, try again!");
    
    //find the verse the user is looking for and if it doesn't exist, throw the correct error
    do {
        getline(instream, l);
        if (!l.empty()) {
            Ref mark = Ref(l);
            if (mark.getBook() == ref.getBook()) {
                bookFound = true;
                if (mark.getChap() == ref.getChap()) {
                    chapFound = true;
                    if (mark.getVerse() == ref.getVerse()) {
                        verseFound = true;
                        Verse foundVerse = Verse(l);
                        status = SUCCESS;
                        return foundVerse;
                    }
                }
            }
        }
        status = OTHER;
        sentinel = false;
    } while (!instream.fail() || sentinel == true);

    if (bookFound == false) {
        status = NO_BOOK;
    }
    else if (chapFound == false) {
        status = NO_CHAPTER;
    }
    else if (verseFound == false) {
        status = NO_VERSE;
    }

    return eVerse;
    /*
	// create and return the verse object
    do {
        // get the next verse
        getline(instream, l);
        if (!l.empty()) {
            Ref verse = Ref(l);
            if (verse.getBook() == ref.getBook() || verse.getBook() < ref.getBook() || ref.getBook() < 66){
                if (verse.getChap() == ref.getChap()|| verse.getChap() < ref.getChap()){
                    if (verse.getVerse() == ref.getVerse()|| verse.getVerse() < ref.getVerse()){
                        Verse foundVerse = Verse(l);
                        if (foundVerse.getRef() == ref) {
                            status = SUCCESS;
                            return foundVerse;
                        }
                    }
                    else if(verse.getVerse() > ref.getVerse())
                    {
                        status = NO_VERSE;
                        sentinel = false;
                    }
                }
                else if(verse.getChap() > ref.getChap())
                {
                    status = NO_CHAPTER;
                    sentinel = false;
                }
            }
            else 
            {
                status = NO_BOOK;
                sentinel = false;
            }
        }
    } while (!instream.fail() || sentinel == true);

    return(eVerse);
    */
}

// REQUIRED: Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
Verse Bible::nextVerse(LookupResult& status) {
    string l;

    if (instream.is_open()) {
        getline(instream, l);
    }
    else
    {
        instream.open(infile);
        getline(instream, l);
    }
    if (!l.empty()) {
        Verse verse(l);
        return verse;
    }
    else {
        status = NO_VERSE;
        return Verse();
    }
}

// REQUIRED: Return an error message string to describe status
string Bible::error(LookupResult status) {
    if (status == NO_BOOK) {
        return "Error: The book number you have input is not in the bible.";
    }
    else if (status == NO_CHAPTER) {
        return "Error: That is not a chapter in the book of the bible you are trying to lookup.";
    }
    else if (status == NO_VERSE) {
        return "Error: That verse number is not in the book you are trying to lookup.";
    }
    else if (status == OTHER) {
        return "Error: Unspecified";
    }
    else
        return " ";
}

void Bible::display() {
	cout << "Bible file: " << infile << endl;
}
	
// OPTIONAL access functions
// OPTIONAL: Return the reference after the given ref
//Ref Bible::next(const Ref ref, LookupResult& status) {}

// OPTIONAL: Return the reference before the given ref
//Ref Bible::prev(const Ref ref, LookupResult& status) {}
