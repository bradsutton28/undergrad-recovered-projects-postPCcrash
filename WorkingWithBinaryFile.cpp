#include <iostream>		// Used for input and output.
#include <fstream>		// Used for file input and output.
#include <cstring>
#include <conio.h>		// Used for getch().
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include "Student.h"	

using namespace std;

// Hash table and hash file functions.
int initializeHashTable();
void writeBinary(int position, Student theStudent);
void readBinary(int position, Student& theStudent);	// Must pass by reference to keep read value.
void displayStudents();
void createStudents();
void addStudent(Student theStudent);
int hashFunction(int key);
bool addable(int position);
int linearProbe(int position);
void searchAndRemove(int search, int SNumber);
void searchByHash(int search, int SNumber);


// Miscellaneous functions.
void pressAnyKey();

// Some project tasks:
//	menu: add, search, remove, display table.
//	use quadratic probing
//	use a class other than book
//	test add 5 more books - up table size to 31 - trace at least one object through 2 collisions
//	remove objects
//	search for objects

// Hash table data.
const static int FILE_LENGTH = 37;	// Several collision/probing techniques work best if the length
									// of possible records is a prime number.
const static int REMOVED = -100;	// If this is to be placed in the key value, then it must be outside of valid range.
const static int EMPTY = -200;
const Student placeHolderStudent("------------------------", "------------------------", 0);    	
const Student removedStudent("XXXXXXXXXXXXXXXXXXXXXXXX", "XXXXXXXXXXXXXXXXXXXXXXXX", 0);    	

int search = 0;



fstream hashFile; 

// Create students for the table to fill at beginning
void createStudents()
{
	addStudent(Student("Greg", "Overstreet", 123453));
	addStudent(Student("Lisa", "Overstreet", 235676));
	addStudent(Student("Krista", "England", 342344));
	addStudent(Student("Bo", "Jackson", 452345));
	addStudent(Student("Angel", "Jackson", 566577));
	addStudent(Student("Clay", "Jenson", 223423));
	addStudent(Student("Your", "Mom", 674565));
	addStudent(Student("Dad", "Jokes", 784564));
	addStudent(Student("Spongebob", "Squarepants", 894568));
	addStudent(Student("Squidward", "Claronet", 990954));
	addStudent(Student("Patrick", "Star", 112324));
	addStudent(Student("Jeremy", "Randall", 195468));
}
 // function to display all students
void displayStudents()
{
	Student tempStudent;	// Used to read object data in binary format from file.		
	cout << "Displaying hash table contents: \n";
	for (int position = 0; position < FILE_LENGTH; position++)
	{
		tempStudent = Student("", "", 000000);
		readBinary(position, tempStudent);
		cout << "Display: " << position << ": " << tempStudent << endl;
	}
	cout << "End of hash table.\n";
}

//adds a student
void addStudent(Student theStudent)
{
	// Get hash value. It's best if the key is unique and if the keys' hash values are spread
	// evenly by the hash function.		
	int position = hashFunction(theStudent.getSNumber());
	if (!addable(position))
	{
		// Probe linearly as needed to get the correct final position.
		position = linearProbe(position);
	}
	// Add the student.
	writeBinary(position, theStudent);			// Insert into hash table.
	cout << theStudent << " and they have been added! \n";
}
//the linear probing function, unfortunatelyc could not figure out the quadratic one
int linearProbe(int position)
{
	do	// Increment position until an addable position is reached.
	{
		if (position < FILE_LENGTH - 1)
		{
			position++;
		}
		else	// If the end of the file is reached
		{		// restart in the first position.
			position = 0;
		}
	} while (!addable(position));
	
		return position;
		// Return the appropriat position to add the student.
}



//fast and efficient hash function
int hashFunction(int key)
{
	int position = key % FILE_LENGTH;
	// cout for demo purposes only. 
	cout << "Hash value: " << position << endl;
	return position;
}

//lets the program know if a spot is either open or open because a student had been deleted
bool addable(int position)
{
	Student Student;
	readBinary(position, Student);
	return (Student.getSNumber() <= 0);
}

// Press any key to continue.
void pressAnyKey()
{
	cout << "Press any key to continue" << endl << endl;
	_getch();					// Waits and gets next character entered.		
}

// reads the file
void readBinary(int position, Student& theStudent)
{
	hashFile.seekg(position * sizeof(Student));
	hashFile.read((char *)&theStudent, sizeof(Student));
}

// writes the file
void writeBinary(int position, Student theStudent)
{
	hashFile.seekp(position * sizeof(Student));				// seek position
	hashFile.write((char *)&theStudent, sizeof(Student));	// writes to that position
	hashFile.flush();
	hashFile.seekp(0 * sizeof(Student));
}

//Creates the hash table with default values
int initializeHashTable()
{
	// Open hash file.
	hashFile.open("StudentTable.dat", ios::in | ios::out | ios::binary | ios::trunc);
	if (!hashFile)
	{
		cout << "Hash file error - file did not open successfully\n"
			<< "Program will end.\n";
		pressAnyKey();
		return 1;
	}
	// Initialize hash file (hash table) with empty values.	
	for (int position = 0; position < FILE_LENGTH; position++)
	{
		writeBinary(position, placeHolderStudent);
	}
	return 0;	// Successful completion of hash file intialization.
}

// The search and remove function
void searchByHash(int search, int SNumber)
{
	int tempSearchPosition = search;
	Student tempStudent;
	hashFile.seekg(search * sizeof(Student));
	hashFile.read((char *) &tempStudent, sizeof(Student));
	if (tempStudent.getSNumber() == EMPTY)					//if the temp students number isnt found then it outputs below
	{
		cout << "No student was found with that student number!\n";
	}
	else
	{
		while (tempStudent.getSNumber() != SNumber)    //while the temp stdents number is not the current student number enter this loop
		{
			tempSearchPosition++;
			if (tempSearchPosition > (FILE_LENGTH - 1))  
			{
				tempSearchPosition = 0;
			}
			hashFile.seekg(tempSearchPosition * sizeof(Student));    //search
			hashFile.read((char *)&tempStudent, sizeof(Student));		//read
			if (tempStudent.getSNumber() == EMPTY)				//if the temps number is empty then it return blow
			{
				cout << "No student was found with that student number!\n";
				return;
			}
		}
		tempStudent.print();											//prints the temp studen
		
	}
}

//search and remove function
void searchAndRemove(int search, int SNumber)
{
	int tempSearchPosition = search;
	Student tempStudent;
	hashFile.seekg(search * sizeof(Student));
	hashFile.read((char *)&tempStudent, sizeof(Student));
	if (tempStudent.getSNumber() == EMPTY)					//if the temp students number isnt found then it outputs below
	{
		cout << "No student was found with that student number!\n";
	}
	else
	{
		while (tempStudent.getSNumber() != SNumber)    //while the temp stdents number is not the current student number enter this loop
		{
			tempSearchPosition++;
			if (tempSearchPosition > (FILE_LENGTH - 1))
			{
				tempSearchPosition = 0;
			}
			hashFile.seekg(tempSearchPosition * sizeof(Student));    //search
			hashFile.read((char *)&tempStudent, sizeof(Student));		//read
			if (tempStudent.getSNumber() == EMPTY)				//if the temps number is empty then it return blow
			{
				cout << "No student was found with that student number!\n";
				return;
			}
		}
		tempStudent.print();											//prints the temp studen
		hashFile.seekp((tempSearchPosition * sizeof(Student)));			//seek
		hashFile.write((char *)&removedStudent, sizeof(Student));		//writes the removes student
		hashFile.seekp(0 * sizeof(Student));
	}
}



int main()
{
	initializeHashTable();		// Initialize empty hash table with dummy records and display.	
	displayStudents();
	pressAnyKey();
	createStudents();				// Creates the students used in this hash file demo.	
	displayStudents();				//Displays the students
	pressAnyKey();


	char FName[40];
	char LName[80];
	int SNumber;

	int choice = 0;
	bool keepGoing = true;
	while (keepGoing = true)
	{
		cout << endl
			<< "---------------------------------------------------------------------\n"
			<< " Select an option from the menu below\n"
			<< " by entering the number of the choice\n"
			<< "---------------------------------------------------------------------\n"
			<< "\t1\tAdd a student object to the file\n"
			<< "\t2\tSearch for a student by student number\n"
			<< "\t3\tRemoving a student\n"
			<< "\t4\tDisplay the hash table aswell as the positions\n"
			<< "\t5\tEnd the program\n"
			<< "---------------------------------------------------------------------\n\n"
			<< "Enter your choice : \n\n";

		cin >> choice;
		switch (choice)
		{
			case 1: //this case is for adding a student to the hash table
			{
				cout << "Please enter the students first name: ";
				cin >> FName;
				cout << "Please enter the students last name: ";
				cin >> LName;
				cout << "Please enter the students number: ";
				cin >> SNumber;
				addStudent(Student(FName, LName, SNumber)); //add the student with the above information
				pressAnyKey();
			}
			break;

			case 2: //this case is for searching for a student by student number
			{
				cout << "Please enter the student number you wish to search for a student with: ";
				cin >> SNumber;
				//search = hashFunction(SNumber);
				searchByHash(search, SNumber);
				pressAnyKey();
			}
			break;


			case 3: //this case is to remove a student from the table
			{
				cout << "Please enter the number of the student you wish to remove from the file: ";
				cin >> SNumber;
				//search = hashFunction(SNumber);
				searchAndRemove(search, SNumber);
				pressAnyKey();
			}
			break;
			case 4: //this is the case to show the hash table
			{
				displayStudents();
				pressAnyKey();
			}
			break;

			case 5: //case for ending the program
			{
				keepGoing = false;
				cout << "The program is now ending! \n";
				pressAnyKey();
				hashFile.close();
				return 0;
			}
			break;

			default:
			{
				cout << "This option is currently unavailable!\n";
				pressAnyKey();
			}
			break;
		}
	}
	hashFile.close();
	return 0;
}
	


//===========================
// Miscelleneous demo code.
// Create objects.
/* -----------   Student student1("Brad", "Sutton", 496600);
Student student2("Shaun", "White", 234567);
Student student3;

// Write objects to the binary file.
fstream file("test-binary.dat",ios::binary | ios::in | ios::out | ios::trunc );
if(!file.is_open())
{
cout << "error while opening the file";
}

// Display books.
cout << "Student objects: " << endl;
student1.print();
student2.print();
student3.print();
cout << endl << endl;
pressAnyKey();

// Read books from file and display.
cout << endl << "Book writes and reads." << endl;
file.write((char *)&student1, sizeof(Student));
file.write((char *)&student2, sizeof(Student));
file.write((char *)&student3, sizeof(Student));
Student tempStudent1, tempStudent2, tempStudent3;
file.seekg(0);
file.read( (char *)&tempStudent1, sizeof(Student) );
tempStudent1.print();
file.read( (char *)&tempStudent2, sizeof(Student) );
tempStudent2.print();
file.read( (char *)&tempStudent3, sizeof(Student) );
tempStudent3.print();
pressAnyKey();

// Random access reads from file (looking at file in reverse order).
cout << endl << "Book random access look ups " << endl;
file.seekg(2 * sizeof(Student) );					// Find the position in file.
file.read( (char *)&tempStudent1, sizeof(Student) );	// Read from that position.
tempStudent1.print();								// Display what was read.
file.seekg(1 * sizeof(Student) );
file.read( (char *)&tempStudent2, sizeof(Student) );
tempStudent2.print();
file.seekg(0 * sizeof(Student) );
file.read( (char *)&tempStudent3, sizeof(Student) );
tempStudent3.print();
pressAnyKey();

// Random access over-writes.
cout << endl << "Random access over-writes" << endl;
Student student4("Random overwrite", "who cares", 345678);	// Obtain data to write with.
file.seekp(0 * sizeof(Student) );								// Find position in file.
file.write( (char *)&student4, sizeof(Student));					// Write data to that position.
Student student5("second random overwrite", "Another who cares", 456789);
file.seekp(2 * sizeof(Student) );
file.write( (char *)&student5, sizeof(Student) );

// Read and display over-written and non-over-written objects in file to check previous writes.
file.seekg(0 * sizeof(Student) );
file.read( (char *)&tempStudent1, sizeof(Student) );
tempStudent1.print();
file.seekg(1 * sizeof(Student) );
file.read( (char *)&tempStudent2, sizeof(Student) );
tempStudent2.print();
file.seekg(2 * sizeof(Student) );
file.read( (char *)&tempStudent3, sizeof(Student) );
tempStudent3.print();
pressAnyKey();

file.close();	// Close file and end program.
*/












