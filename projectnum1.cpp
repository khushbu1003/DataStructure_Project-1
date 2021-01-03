//************************************* PROGRAM IDENTIFICATION ***************************************
//*                                                                                                  *
//*     PROGRAM FILE NAME: Source1.cpp                ASSIGNMENT #1:               GRADE: _____      *
//*                                                                                                  *
//*     PROGRAM AUTHOR:     ______________________________                                           *
//*                                Khushbu Shah                                                      *
//*                                                                                                  *
//*     COURSE #: CSC 36000 11                                          DUE DATE: February 05,2020   *
//*                                                                                                  *
//****************************************************************************************************

//*************************************** PROGRAM DESCRIPTION ****************************************
//*                                                                                                  *
//*  PROCESS:         This program is designed to read in passenger information and assign           *
//*                   passengers to their seats according to their requests and the seating rules.   *
//*  USER DEFINED                                                                                    *
//*    MODULES     :   Header.........................Prints the output preamble                     *
//*                    Footer.........................Prints the output footer                       *
//*                    getColNum......................Converts column request to the col number      *
//*                    processRequests................Processes data file, updating flight elements  *
//*                                                   accordingly                                    *
//*                                                                                                  *
//****************************************************************************************************


//preprocessor directives
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#define PAGEWIDTH 95;       // Set constant page width
#define PAGEHEIGHT 76;      // Set constant page height
int LINESUSED = 0;          // Number of lines used in output
int MAXLINES = PAGEHEIGHT;  // Maximum number of lines for output

using namespace std;

//***************************************** FLIGHT CLASS *******************************************

class Flight {
private:
	static const int rows = 10;                             // Define constant row capacity
	static const int cols = 3;                              // Define constant column capacity
	static const int waitCapacity = 50;                     // Define constant waiting list capacity
	int id;                                                 // Flight ID
	string departure;                                       // Flight departure city
	string destination;                                     // Flight destination
	int seating[rows][cols];                                // Seating chart
	int waiting[waitCapacity];                              // Waiting list
public:
	Flight(int, string, string);                            // Constructor
	int getId() { return id; };                             // Accesses ID
	string getDepart() { return departure; };              // Accesses departure city
	string getDest() { return destination; };              // Accesses destination city
	int getSeatingChart() { return seating[rows][cols]; }; // Accesses seating chart
	int getWaitingList() { return waiting[waitCapacity]; };// Accesses waiting list
	bool seatAvailable(int, int);                           // Checks seat for availability
	void assignSeat(int, int, int);                         // Adds passenger to seating chart
	void addToWait(int);                                    // Adds passenger to waiting list
	int assignFirstAvailable(int, int, int);                // Adds passenger first available seat
	int assignFirstAvailable(int, int, int, int);           // Adds passenger first available seat
																// within a specified column
	void printSeatingChart(ofstream&);                     // Prints the current seating chart
	void printWaitingList(ofstream&);                      // Prints the current seating chart
	//void printSeatingToConsole();
};

// --------------------------------------------------------------------------------------------------
	// Flight constructor
Flight::Flight(int newId, string newDep, string newDest) {
	id = newId;             // Set flight ID
	departure = newDep;     // Set departure city
	destination = newDest;  // Set destination city

		// Initialize seating chart
	for (int i = 0; i <= rows; i++) {
		for (int j = 0; j <= cols; j++) {
			seating[i][j] = -999;
		}
	}
	// Initialize waiting list
	for (int i = 0; i <= waitCapacity; i++) {
		waiting[i] = -999;
	}
}

// --------------------------------------------------------------------------------------------------
	// Check Seat Availability
bool Flight::seatAvailable(int row, int col) {

	// Receives - a row number and column number
	// Task - check if the seat specified by this row and column is available
	// Returns - true if the seat is available; false if not

		// If seat is unoccupied, return true
	if (seating[row][col] == -999) {
		return true;
	}
	// Return false otherwise
	else {
		return false;
	}
}

// --------------------------------------------------------------------------------------------------
	// Add Passenger to Seating Chart
void Flight::assignSeat(int boardingNum, int row, int col) {

	// Receives - a passenger's boarding number, a row number, and a column number
	// Task - inserts the boarding number into specified place on seating chart
	// Returns - the seating chart is updated accordingly

		// Insert new passenger number into seating chart
	seating[row][col] = boardingNum;
}

// --------------------------------------------------------------------------------------------------
	// Add Passenger to Waiting List
void Flight::addToWait(int boardingNum) {

	// Receives - a passenger's boarding number
	// Task - append this passenger to the waiting list
	// Returns - the waiting list is updated accordingly

		// Find next available spot in waiting list
	for (int i = 0; i < waitCapacity; i++) {
		// Check if waiting spot is available
		if (waiting[i] == -999) {
			// Assign passenger to available spot
			waiting[i] = boardingNum;
			return;
		}
	}
}

// --------------------------------------------------------------------------------------------------
	// Assign Passenger to First Available Seat
int Flight::assignFirstAvailable(int boardingNum, int startingRow, int endingRow) {

	// Receives - a passenger's boarding number, and the lower and upper bounds to search
	// Task - assigns the furthest forward available seat to the passenger
	// Returns - The passenger is inserted into the seating chart if successful;
	//          If successful, this function returns a positive integer to indicate so.
	//          If unsuccessful, this function returns a negative integer.

		// Search through seat selection
	for (int row = startingRow; row <= endingRow; row++) {
		// Search through each column in a row
		for (int col = 0; col < cols; col++) {
			// If a vacant seat is found
			if (seatAvailable(row, col)) {
				// Assign passenger to this seat
				assignSeat(boardingNum, row, col);
				// Return indication of success
				return 1;
			}
		}
	}
	// If no seat is found, return indication of failure
	return -1;
}

// --------------------------------------------------------------------------------------------------
	// Assign Passenger to First Available Seat Within a Specified Column
int Flight::assignFirstAvailable(int boardingNum, int startingRow, int endingRow, int column) {

	// Receives - a passenger's boarding number, the lower and upper bounds to search,
	//              and a specified column to search within
	// Task - assigns the furthest forward available seat to the passenger
	// Returns - The passenger is inserted into the seating chart if successful;
	//          If successful, this function returns a positive integer to indicate so.
	//          If unsuccessful, this function returns a negative integer.

		// Search through seat selection
	for (int row = startingRow; row <= endingRow; row++) {
		// If a vacant seat is found
		if (seatAvailable(row, column)) {
			// Assign passenger to this seat
			assignSeat(boardingNum, row, column);
			// Return indication of success
			return 1;
		}
	}
	// If no seat is found, return indication of failure
	return -1;
}

// --------------------------------------------------------------------------------------------------
// Print seating chart
void Flight::printSeatingChart(ofstream& outputFile) {

	// Receives - a reference to an output file
	// Task - prints the seating chart to the output file
	// Returns - the output file is altered accordingly

	// Process rows
	for (int row = 0; row < rows; row++) {
		// Process columns
		for (int col = 0; col < cols; col++) {
			outputFile << seating[row][col];
			outputFile << "          ";
		}
		outputFile << endl;
		LINESUSED++;    // Increment line count
	}
}

// --------------------------------------------------------------------------------------------------
	// Print Waiting List
void Flight::printWaitingList(ofstream& outputFile) {

	// Receives - reference to an output file
	// Task - prints the waiting list to the output file
	// Returns - the output file is altered accordingly

	int prntCount = 0;  // Number of numbers printed so far
		// If the waiting list is empty
	if (waiting[0] == -999) {
		// State so
		outputFile << "There is no waiting list for this flight.";
		outputFile << endl;
		LINESUSED += 2;    // Increment line count
	}
	// Otherwise, print the waiting list
	else {
		for (int i = 0; i < waitCapacity; i++) {
			if (waiting[i] != -999) {
				if (prntCount >= 10) {
					outputFile << endl;
					prntCount = 0;
					LINESUSED++;    // Increment line count
				}
				outputFile << waiting[i] << "   ";
				prntCount++;    // Increment print count
			}
		}
		outputFile << endl;
		LINESUSED += 2;    // Increment line count
	}
}


// --------------------------------------------------------------------------------------------------


/* ************************************************************************************************ */

/* ******************************************** MAIN ********************************************** */

void Header(ofstream&);
void Footer(ofstream&);

// ifstream &inputFile, int numFlights, Flight flights[]
void processRequests(ifstream&, int, Flight[]);
int getColNum(char);

int main()
{
	// Number of flights
	int numFlights = 0;
	// Set flight information, create 8 objects
	Flight flight1(1010, "Jackson, Mississippi", "Memphis, Tennessee");
	Flight flight2(1015, "Memphis, Tennessee", "Jackson, Mississippi");
	Flight flight3(1020, "Jackson, Mississippi", "Little Rock, Arkansas");
	Flight flight4(1025, "Little Rock, Arkansas", "Jackson, Mississippi");
	Flight flight5(1030, "Jackson, Mississippi", "Shreveport, Louisiana");
	Flight flight6(1035, "Shreveport, Louisiana", "Jackson, Mississippi");
	Flight flight7(1040, "Jackson, Mississippi", "Orlando, Florida");
	Flight flight8(1045, "Orlando, Florida", "Jackson, Mississippi");
	// Create list of flights
	Flight flights[] = { flight1, flight2, flight3, flight4, flight5, flight6, flight7, flight8 };
	numFlights = 8; // Set number of flights to 8

		// Get input file
	ifstream inputFile("data1.txt", ios::in);
	// Get output file
	ofstream outputFile("output.txt", ios::out);

	// Process input file containing passenger requests
	processRequests(inputFile, numFlights, flights);

	// ----- Print results -----
	Header(outputFile);     // Print output header
	LINESUSED += 4;           // Increment line counter
		// Print all flight records
	for (int i = 0; i < numFlights; i++) {

		// Print flight information header
		outputFile << "SOUTHERN COMFORT AIRLINES" << endl << endl;
		outputFile << "Flight " << flights[i].getId();
		outputFile << endl;
		outputFile << "FROM: " << flights[i].getDepart();
		outputFile << endl;
		outputFile << "TO: " << flights[i].getDest();
		outputFile << endl << endl;
		LINESUSED += 7;       // Increment line counter

			// Print the flight seating chart
		flights[i].printSeatingChart(outputFile);
		outputFile << endl;
		LINESUSED += 2;       // Increment line counter

			// Print the flight waiting list
		outputFile << "Waiting list: " << endl;
		flights[i].printWaitingList(outputFile);
		outputFile << endl << endl;

		// Print flight record closing
		outputFile << "END OF SEATING CHART.";
		outputFile << endl << endl;
		LINESUSED += 2;       // Increment line counter

			// Insert a page break
		for (int j = 0; j < (MAXLINES - LINESUSED); j++) {
			outputFile << endl;
		}

		LINESUSED = 0;      // Reset line counter
	}
	// Print output footer
	Footer(outputFile);

	return 0;
}
                                        
//********************************** END OF MAIN**************************************************

// ***********************************************************************************************
void processRequests(ifstream& inputFile, int numFlights, Flight flights[]) {

	// Receives - a reference to the input data file, the number of flights being dealt with, and
	//              the list of flight elements
	// Task - process the input file and add passengers to the flight records
	// Returns - the flight elements and their corresponding seating charts and waiting lists
	//              are updated accordingly

		// Passenger info
	int passenger;      // passenger ID
	int flightNum;      // passenger's flight
	char section;       // passenger's requested section (C or F)
	char reqCol;        // passenger's requested seat column (L, M, or R)
	int seatRow = 0;      // passenger's requested row number
	int seatCol = 0;      // passenger's requested seat column number

	int sectStart = 0;  // Beginning of section (row number)
	int sectEnd = 9;    // End of section (row number)

	int stat;           // Status indicator

		// Get first passenger boarding number from the input file
	inputFile >> passenger;
	// Continue to process input file until the sentinel is reached
	while (passenger != -999) {
		// Receive passenger information from data file
		inputFile >> flightNum;     // Get passenger flight number
		inputFile >> ws;
		section = inputFile.get();  // Get passenger's requested section
		inputFile >> ws;
		inputFile >> seatRow;       // Get passenger's requested seat row
		inputFile >> ws;
		reqCol = inputFile.get();   // Get passenger's requested column
		inputFile >> ws;

		// Get seat column number
		seatCol = getColNum(reqCol);

		// Determine lower and upper bound of seating section
		switch (section) {
			// First class seating
		case 'F':
			// Set seat selection from rows 0 - 2
			sectStart = 0;
			sectEnd = 2;
			break;
			// Coach seating
		case 'C':
			// Set seat selection from rows 3 - 9
			sectStart = 3;
			sectEnd = 9;
			break;
		}

		// Find flight that the passenger requested
		for (int reqFlight = 0; reqFlight < numFlights; reqFlight++) {

			// When the specified flight is found
			if (flights[reqFlight].getId() == flightNum) {
				// Check if the requested seat is available on requested flight
				if (flights[reqFlight].seatAvailable(seatRow, seatCol)) {
					// If so, add passenger to the seating chart
					flights[reqFlight].assignSeat(passenger, seatRow, seatCol);
				}

				// In case seat is unavailable
				else {
					// Try assigning passenger the furthest forward seat in the column
					stat = flights[reqFlight].assignFirstAvailable(passenger,
						sectStart, sectEnd, seatCol);
					// If requested column is completely unavailable
					if (stat < 0) {
						// Try assigning passenger to any seat available
						stat = flights[reqFlight].assignFirstAvailable(passenger,
							sectStart, sectEnd);

						// If still no seat can be found (seating chart is full)
						if (stat < 0) {
							// Add the passenger to the waiting list
							flights[reqFlight].addToWait(passenger);
						}
					}
				}
			}
		} // Finish processing passenger's request

			// Begin processing next passenger
		inputFile >> passenger;

	} // Finish processing all passengers
}
// ***************************************************************************************************

// ***************************************************************************************************
int getColNum(char requestedColumn) {

	// Receives - a character specifying a requested column
	// Task - return the number that this column character corresponds to
	// Returns - an integer indicating a seating column

	// Determine seat column number
	switch (requestedColumn) {
		// Left column
	case 'L':
		return 0;        // Set column number to 0
		break;
		// Middle Column
	case 'M':
		return 1;        // Set column number to 1
		break;
		// Right column
	case 'R':
		return 2;        // Set column number to 2
		break;
	}
	// Return 0 by default
	return 0;
}
//***************************************************************************************************

//********************************************** HEADER *********************************************
void Header(ofstream& Outfile)
{
	// Receives - the output file
	// Task - Prints the output preamble
	// Returns - Nothing
	Outfile << setw(30) << "Khushbu Shah";
	Outfile << setw(17) << "CSC 36000";
	Outfile << setw(15) << "Section 11" << endl;
	Outfile << setw(30) << "Spring 2020";
	Outfile << setw(20) << "Assignment #1" << endl;
	Outfile << setw(35) << "-----------------------------------";
	Outfile << setw(35) << "-----------------------------------\n\n";

	return;
}
//****************************************************************************************************

//************************************************ FOOTER ********************************************
void Footer(ofstream& Outfile)
{
	// Receives - the output file
	// Task - Prints the output salutation
	// Returns - Nothing
	Outfile << endl;
	Outfile << setw(35) << " --------------------------------- " << endl;
	Outfile << setw(35) << "|      END OF PROGRAM OUTPUT      |" << endl;
	Outfile << setw(35) << " --------------------------------- " << endl;

	return;
}
//****************************************************************************************************