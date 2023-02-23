/*  What is up gamers
*   I hope you like this swag program
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
	int b, c, v;
	
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
		cout << "This lovely index that I've made has " << webBible.getIndexSize() << " references in it." << endl;
		cout << "The byte offset for the last verse of this index is: " << webBible.getlastIndexByteOffset() << endl;
		Ref genesisTest = Ref(1,1,1);
		cout << "The byte offset for Genesis 1:1 is: " << webBible.getRefPosition(genesisTest) << endl;
		Ref exodusTest = Ref(2,1,1);
		cout << "The byte offset for Exodus 1:1 is: " << webBible.getRefPosition(exodusTest) << endl;
	}

}