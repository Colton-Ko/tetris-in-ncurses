/*

        AUTHOR
        TAM CHUN KIT	(3035686554)
        CHOW NAM FUNG	(3035712767)

        DATE
        2020-05-08

        FILENAME
        
                bookkeeper.cpp

        REFERENCES

                This piece of code has direct references to these sources
                - https://www.tutorialspoint.com/cplusplus/cpp_date_time.htm
                - https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
                - https://www.geeksforgeeks.org/csv-file-management-using-c/



        VIEW
                Tabsize	:	8
                Indentation:	Space
*/

#include "tetris/blocks.h"
#include "tetris/controller.h"
#include "tetris/game.h"
#include "tetris/debug.h"
#include "tetris/options.h"
#include "tetris/constants.h"
#include "tetris/types.h"
#include "tetris/bookkeeper.h"

#include <sstream>
#include <iomanip>
#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <fstream>

scoreEntry * head;
scoreEntry * tail;

using namespace std;

// Returns a formatted std::string given a formatting string and arguments string
// This code is referred directly from https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template<typename ... Args>
std::string formatString( const std::string& format, Args ... args )
{
        size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1;     // Extra space for '\0'
        if( size <= 0 )
        { 
                throw std::runtime_error( "Error during formatting." ); 
        }

        std::unique_ptr<char[]> buf( new char[ size ] ); 

        snprintf( buf.get(), size, format.c_str(), args ... );
        
        return std::string( buf.get(), buf.get() + size - 1 );                  // We don't want the '\0' inside
}

// Returns a string of local date time
// Format: [ MM/DD HH:MM:SS ]
string getCurrentDateTime()
{       
        time_t timeObject = time(0);
        tm * currentLocalTime = localtime(&timeObject);
        
        string output = "[ ";
        output += formatString("%02d/", currentLocalTime->tm_mon+1)     ;       // Fill Month
        output += formatString("%02d ", currentLocalTime->tm_mday)      ;       // Fill Day in Month
        output += formatString("%02d:", currentLocalTime->tm_hour)      ;       // Fill hour
        output += formatString("%02d:", currentLocalTime->tm_min)       ;       // Fill minute
        output += formatString("%02d ]", currentLocalTime->tm_sec)      ;       // Fill second
        return output;
}

// Appends a scoreEntry record link list. Receieves a scoreEntry pointer name head and tail
// and the integer of the score
void appendRecordToTable(string currentLocalTimeString, int score)
{
        // Steps to add a new record to tableOfScores
        // 1. Create new scoreEntry pointer
        // 2. Fill that pointer with dateTime, and score
        // 3. Put that to the end of of the linked list
        
        // New scoreEntry pointer
        scoreEntry * currentEntry = new scoreEntry;

        // Fill information to the currentEntryPointer
        currentEntry->playerScore       = score                         ;       // The score that player won
        currentEntry->dateTimeString    = currentLocalTimeString        ;

        currentEntry->next              = NULL                          ;

        // Append link List
        // Check whether HEAD is NULL. If it is then append that add that to HEAD.
        // Also the tail is the same entry

        if (head == NULL)               // If the head does not even exist
        {
                head = currentEntry;    // Set the head and tail to be the currenEntry
                tail = currentEntry;
        }
        else                            // If head exists
        {
                tail->next = currentEntry;      // Set the next item in the existing tail to currentEntry
                tail = currentEntry;            // Update the tail entry to be the current
        }
}

// Prints the player history, given a scoreEntry pointer head.
void showHistory(scoreEntry * head)
{
        scoreEntry * current = head;
        while (current != NULL)
        {
                cout << current->dateTimeString << " " << current->playerScore << endl;
                current = current->next;
        }
}

// Referred from
// https://www.geeksforgeeks.org/csv-file-management-using-c/
//
// Reads a history in csv format and load that to link list
// Receives head as pointer to first item in record link list, 
// tail as the last item in record link list
// Returns the highest score found in the file if read successfully
// Returns -1 if unable to read the file
int readHistoryFile()
{
        ifstream fin;
        fin.open(HISTORY_FILENAME);

        if ( !fin )                             // Error while opening the file
        {
                return OPERATION_FAILED;
        }

        string buffer = "";                     // String buffer during the read progress
        string row = "";                        // Current line to be processed
        string column = "";
        bool isThisScore = false;
        int playerScore = 0;
        int highScore = 0;
        string dateTime = "";

        head = NULL;
        tail = NULL;

        while (fin >> buffer)
        {
                getline(fin, row);              // Get a new line from filestream
                stringstream columnStream(row); // Put line into stringstream, breaking down the columns

                while
                (
                        getline                 // For every column in columStream
                        (                       // Where
                                columnStream,   // Streams of columns is input
                                column,         // Content of column is output
                                SEPARATION_CHAR // Separation Character is deliminator
                        )
                )
                {       
                        if (isThisScore)        // If this is player's score
                        {                       // Put that in playerScore

                                playerScore = atoi(column.c_str());

                        }
                        else                    // Since either it is score or dateTime
                        {                       // If it is not score, it must be dateTime
                                dateTime = column;
                        }

                        // Finished parsing CSV, append it to record link list
                        appendRecordToTable(dateTime, playerScore);
                        
                        // Meanwhile also find highScore
                        if (playerScore > highScore)    // If score of current record Player
                        {                               // is higher than previous record
                                highScore = playerScore;// Replace highScore with the new one
                        }

                        isThisScore = !isThisScore;     // Toggle isScore flag
                }
                
        }
        return highScore;                               // Returns the high score
}

// Writes a text file named HISTORY_FILENAME. Returns true if successful. 
// false if writing is not successful.
bool writeToHistoryTextFile(string dateTimeString, int playerScore)
{
        ofstream fout;                          // ofstream object for writing file
        fout.open(HISTORY_FILENAME,ios::app);   // Open the file as append mode
        if ( !fout )                            // If the file could not be opened
        {
                return false;                   // Return false for error handling
        }
        fout << dateTimeString + "," + to_string(playerScore) << endl;
        return true;
}
