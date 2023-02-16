// Class Verse
// Computer Science, MVNU
//
// A Verse object represents a complete Bible verse, including its reference
//    * Ref ref          - the reference (e.g., 1:1:1)
//    * string verseText - the actual verse text (without reference)
//

#ifndef Verse_H
#define Verse_H
#include <string> 
#include <stdlib.h>
#include "Ref.h"
using namespace std;

class Verse {
 private:
   Ref verseRef;   // The reference for this verse.
   string verseText;  	// actual verse text (without reference)
   int verseNum; //number of the verse in the chapter

 public:
   Verse();   	// Default constructor
   
   // REQUIRED: implement parse constructor
   Verse(const string s);
		// pass entire line from file including reference and text

   // REQUIRED: implement accessor functions
   string getVerse();
   Ref getRef();

   // Display Verse on cout
   void display(); // Display ref & verse [with line breaks in needed].
   void displayVerseHtml(); //Display verse number in bold when displayed on the Wepage
};

#endif //Verse_H
