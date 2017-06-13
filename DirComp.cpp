


/******************************************************************************************
* Author:					Seth H. Urbach						
* Date Created:				08/26/04
* Last Modification Date:	08/26/04
* Project:					DirComp
* Filename:					DirComp.cpp
*
* Overview:
*	This program will get the 
*	"C:\\CdriveDirTree.txt" .
*
* 
********************************************************************************************/


#include <iostream>
#include <fstream>
#include <string>
#include <io.h>
#include <direct.h>
using namespace std;

//This function gets the string of the directories to be compared from the user
void getDirectory ( char * &dirptr );
void DirectoryCompare ( const char * dirptr, const char * compptr );

const char DEFAULT_ROOT[] = "C:\\"  ;	// The default directory for the tree to build from
// Error message to be throw by exceptions
const char * no_memory = "\n\nThere is no more memory available to allocate!!!\n\n";
// Error message to be throw by exceptions
const char * bad_path = "\n\nUnable to locate the directory ";
const char DIRECTORY_FILE[] = "DirectoryComparison.txt"; // "C:\\DirectoryComparison.txt";


void main ( void )
{
	try
	{
		char buffer[256] ;				// The directory string
		char buffer2[256] ;				// The comparison directory string 
		char * dirptr  = buffer ;		// The pointer to the directory string
		char * compptr  = buffer2 ;		// The pointer to the directory string

		cout << "\n\nDirectory Content Comparison Utility... \n" << endl;
		getDirectory ( dirptr );	
		getDirectory ( compptr );
		DirectoryCompare ( dirptr, compptr ) ;
	
		
		cout << "\n\n ---END COMPARISON--- \n\n";
	}

	catch ( char * msg )
	{
		cout << msg;
	}
     
}//End of main



/***********************************************************************************************
*		Function:	void getDirectory (  char * &dirptr )	
*
*		Purpose:  This method gets the string of the root directory from the user
*
*       Entry:		dirptr is the pointer to buffer, which is used to store the
*				string of the root directory entered in by the user.
*				
*		Exit:	This function fills dirptr with the string inputted by the user.
*				It gets the string from the standard input, converts it to all uppercase,
*				and then it checks to make sure the first part of the string syntax is 
*				correct.
*
***********************************************************************************************/


void getDirectory (  char * &dirptr )
{
	bool loop (true);	// continue to loop or not
	int c (0) ;			// counting integer

	while ( loop )
	{
		cout << "Enter directory to compare : ";
		//cin.ignore( cin.rdbuf()->in_avail() );
		cin.getline( dirptr, strlen(dirptr) );
		cin.ignore( cin.rdbuf()->in_avail() );
		

		while ( dirptr[c] )
		{
			if ( dirptr[c] >= 'a' && dirptr[c] <= 'z' )
				dirptr[c] = dirptr[c] - 32;
			++c;
		}

		if ( dirptr[0] >= 'A' && dirptr[0] <= 'Z' && dirptr[1] == ':' && dirptr[2] == '\\' )
			loop = false;
		else
		{
			cout << "\n\nINVALID beginning for a directory! Please Re-enter directory.\n"; 
			loop = true;
		}
	}
}





void DirectoryCompare ( const char * dirptr, const char * compptr )  
{
	char no_path[50];										// Array to store an exception message
	struct _finddata_t dir_file_info, comp_file_info ;		// Object that stores found files properties
	long d_file_num(0), c_file_num(0);							// Number for operating _findnext
	bool first_time(true), loop (true), match(false);						// Continue to loop boolean variables
	int count(0), error(0), c(0);
	
	string directory_path = dirptr;							// The path of the current directory
	string comp_path = compptr;
	string dir_file, comp_file ;							// current file name

	ofstream fout ( DIRECTORY_FILE );
	
	strcpy ( no_path, bad_path );							// make the exception message
	//strcat ( no_path, dirptr );
	if( _chdir( dirptr ) )
		throw no_path;

	strcpy ( no_path, bad_path );							// make the exception message2
	//strcat ( no_path, compptr );
	if( _chdir( compptr ) )
		throw no_path;

	else
	{
		//system( "dir *.*");

		// find the first file
		_chdir( dirptr ) ;
		if ( (d_file_num =_findfirst ( "*.*", &dir_file_info )) == -1L )
		{
			cout << "\n\nThere are NO FILES OR DIRECTORIES in the 1st directory!\n\n";
			loop = false;
		}

		dir_file = dir_file_info.name ;

		_chdir( compptr );
		if ( (c_file_num = _findfirst ( "*.*", &comp_file_info )) == -1L )
		{
			cout << "\n\nThere are NO FILES OR DIRECTORIES in the 2nd directory!\n\n";
			loop = false;
		}

		comp_file = comp_file_info.name ;
		c = 0;
				
		while( loop )
		{
			if ( comp_file.length() > 40 )
			{
				do 
				{
					if ( dir_file[c] == comp_file[c] )
					{
						c++;
						match = true;
					}
					else
						match = false;

				} while ( c <= 40 && match );
			}
			else if ( dir_file == comp_file )
			{
				match = true;
			}


			if (match)
			{
				// find the next file
				chdir( dirptr );
				if( _findnext( d_file_num, &dir_file_info ) != 0 )
					loop = false;
				else
				{
					dir_file = dir_file_info.name ;
					first_time = true;
				}
			}

			else
			{
                chdir( compptr );
				if ( first_time )
				{
					c_file_num = _findfirst ( "*.*", &comp_file_info ) ;
					first_time = false ;
				}
				else
				{
					error = _findnext( c_file_num, &comp_file_info );
					comp_file = comp_file_info.name ;

					if ( error != 0 )
					{
						cout << ++count << " Missing File: \t" << dir_file << endl;
						fout << count << " Missing File: \t" << dir_file << endl;
						
						chdir( dirptr );
						if( _findnext( d_file_num, &dir_file_info ) != 0 )
							loop = false;
						else
							dir_file = dir_file_info.name ;

						first_time = true;
					}
				}
			}

			c = 0;
			match = false;
		}


		if ( count == 0 )
			cout << "\nDIRECTORIES MATCH" ;

		_chdir( dirptr );			// Set directory to the root directory
		_findclose ( d_file_num );	// Close the structure _finddata_t object
		_findclose ( c_file_num );	// Close the structure _finddata_t object

	}
}

