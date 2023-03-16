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
    buildRefIndex();
}

// REQUIRED: lookup finds a given verse in this Bible
Verse Bible::lookup(Ref ref, LookupResult& status) { 
	//pass ref!
	//find the ref in the map!
	auto refIter = refIndex.find(ref);
	//if the ref doesn't exist, run the error
	if(refIter == refIndex.end()){
		//check to see if ref with verse as 1 works (if so, verse is the problem)
		//then do this with chap
		Ref verseCheck = Ref(ref.getBook(), ref.getChap(), 1);
		auto refIter2 = refIndex.find(verseCheck);
		if(refIter2 == refIndex.end()){
			Ref chapCheck = Ref(ref.getBook(), 1, 1);
			auto refIter3 = refIndex.find(chapCheck);
			if(refIter3 == refIndex.end()){
				status = NO_BOOK;
			} else
			status = NO_CHAPTER;
		} else
		status = NO_VERSE;
	return Verse();
	}
	//if we get le number back
	//seek it in the original file
	//we must also clear the instream because we've already gone through the bible once
	//and C++ is cranky with that
	instream.clear();
	instream.seekg(refIndex[ref]);

	//read that line
	string l;
	getline(instream, l);
	
	//make a verse out of the line
	Verse verse = Verse(l);

	//return that verse
	return verse;
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

void Bible::buildRefIndex(){
	string l;
	do{
	streampos position = instream.tellg();
	getline(instream, l);
        	if (!l.empty()) {
			Ref curRef = Ref(l);
			refIndex[curRef] = position;
			lastValuePosition = instream.tellg();
		}
	} while (!instream.fail());
}

int Bible::getIndexSize(){
	return refIndex.size();
}

int Bible::getlastIndexByteOffset(){
	return lastValuePosition;
}

int Bible::getRefPosition(Ref r){
	if(refIndex.find(r) == refIndex.end()){
		return 10;
	}else
		return refIndex[r];
}

	
// OPTIONAL access functions
// OPTIONAL: Return the reference after the given ref
Ref Bible::next(const Ref ref, LookupResult& status) {
	auto refIter = refIndex.find(ref);
	refIter++;
	return refIter->first;
}
	

// OPTIONAL: Return the reference before the given ref
//Ref Bible::prev(const Ref ref, LookupResult& status) {}
