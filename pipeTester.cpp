/*  What is up gamers
*   I hope you like this swag program 2 electric boogaloo
*   Author: Gina Lawhon
*/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Bible.h"
#include "Ref.h"
#include "Verse.h"
using namespace std;


int main(int argc, char **argv){
	// Create Bible object
	Bible webBible("/home/class/csc3004/Bibles/web-complete");
	LookupResult result;
	//user input
	int b, c, v, nv;
	
	if (argc == 4){
		//intake of arguments from user
		//book
		b = atoi(argv[1]);
		//chapter
		c = atoi(argv[2]);
		//verse
		v = atoi(argv[3]);
		//make a reference object out of that data
		Ref ref(b, c, v);
		cout << endl;
		//display bible
		cout << "Using Bible from: ";
		webBible.display();
		cout << endl;
		//lookup verse
		Verse verse = webBible.lookup(ref, result);
		if(result == SUCCESS){
		   //print out book and chapter num
		   cout << ref.gBook() << " " << ref.getChap() << endl;
		   verse.display();
		   cout << endl;
		} else
		  cout << webBible.error(result) << endl;
		
		/*
		
		cout << "This lovely index that I've made has " << webBible.getIndexSize() << " references in it." << endl;
		cout << "The byte offset for the last verse of this index is: " << webBible.getlastIndexByteOffset() << endl;
		Ref genesisTest = Ref(1,1,1);
		cout << "The byte offset for Genesis 1:1 is: " << webBible.getRefPosition(genesisTest) << endl;
		Ref exodusTest = Ref(2,1,1);
		cout << "The byte offset for Exodus 1:1 is: " << webBible.getRefPosition(exodusTest) << endl;

		*/
	}
	//If four arguments - the user wants a span of verses
	if (argc == 5) {
		//intake of arguments from user
		//book
		b = atoi(argv[1]);
		//chapter
		c = atoi(argv[2]);
		//starting verse
		v = atoi(argv[3]);
		//number of verses
		nv = atoi(argv[4]);
		//create ref with the first verse
		Ref ref(b, c, v);
		//Show what Bible we are using
		cout << endl;
		cout << "Using Bible from: ";
		webBible.display();
		cout << endl;
		Verse verse = webBible.lookup(ref, result);
		//display the first verse
		if(result == SUCCESS){
		   //print out book and chapter num
		   cout << ref.gBook() << " " << ref.getChap() << endl;
		   verse.display();
		   cout << endl;
		   for(int i = 0; i < nv; i++){
			if(result == SUCCESS){
				verse = webBible.nextVerse(result);
				verse.display();
				cout << endl;
				}
			}
		} else
		  cout << webBible.error(result) << endl;
		
	}


}