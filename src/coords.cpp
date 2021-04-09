#include "../headers/adaship.h"

bool coords_validatecoordinate(std::string coordinate, int boardwidth, int boardheight, bool zerobaserows) {
	//return true if coordinate is valid re board size
	udt_boardref boardref = coords_getboardref(coordinate, zerobaserows);
	if( (boardref.col >= 0 && boardref.col < boardwidth) &&
		(boardref.row >= 0 && boardref.row < boardheight) ) {
			return true;
	}
	return false; 
}

int coords_getidx(std::string coordinate, int width, bool zerobaserows) {
	//return the idx based on the coordinate
	udt_boardref coords = coords_getboardref(coordinate, zerobaserows);
	int idx = coords_getidx(coords.col, coords.row, width);
	return idx;
}

int coords_getidx(int col, int row, int width) {
	//return the based 0 index based on the column, row and width
	if(col == 0 && row == 0) { return 0; } //deal with all zeros
	return (col + width * row);
}

std::string coords_getcoordinate(int col, int row, int width, bool zerobaserows) { 
	//return the coordinate based on col and row
	int idx = coords_getidx(col, row, width);
	return coords_getcoordinate(idx, width, zerobaserows);
}

std::string coords_getcoordinate(int idx, int width, bool zerobaserows) {
	//return coordinate based on the idx
	udt_boardref boardref = coords_getboardref(idx, width);
	std::string label = coords_collabel(boardref.col);
	return label + std::to_string( zerobaserows ? boardref.row + 1 : boardref.row );
}

std::vector < std::string > coords_getsurrounding(std::string coordinate, int width, bool zerobaserows) {
	udt_boardref boardref = coords_getboardref(coordinate);
	std::vector < std::string > surrounding;
	int c = boardref.col - 1; //get starting position
	int r = boardref.row - 1;

	for(int i = 0; i < 3; i++) {
		for(int n = 0; n < 3; n++) {
			surrounding.push_back({
				coords_getcoordinate(c + i, r + n, width, zerobaserows)
			});
		}
	}
	return surrounding;
}

udt_boardref coords_getboardref(int idx, int width) {
	//return the column and row based on the base 0 index
	if(idx == 0 ) { return { 0, 0 }; }
	int col = idx % width; //use modulo to get the remainder of idx / width;
	int row = idx / width; //integer division
	return { col, row };
}

udt_boardref coords_getboardref(std::string coordinate, bool zerobaserows) {
	//rturn the column and row based on the coordinate (e.g., A1 to col,row coordinate)
	udt_coordref coordref = coords_extract(coordinate);
	udt_boardref boardref;

	boardref.col = coords_col(coordref.collabel); //convert colref (A..Z) to column index
	boardref.row = zerobaserows ? coordref.row - 1: coordref.row; //update row if base 0

	return boardref;
}

std::string coords_collabel(int col) {
	//convert col to label; valid range 0 to 702 (ZZ), col's only relates to A..ZZ 
	std::string reference = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string label = ""; //set default return

	if(col >= 0 && col <= ((26*26)+26)) { //check valid range
		int x = col / 26; //check range; 0 if A..Z
		if(x == 0) {
			label = reference[col]; //return label
		} else {
			col = col - (x * 26); //update col value
			label = reference[(x-1)]; //add initial label; A..Z
			label += reference[col]; //add secondary label; AA..ZZ
		}
	}
	return label;
}

int coords_col(std::string label) {
	//convert column label (A[a] = 0, B[b] = 2, .. Z[a] = 25, AA[aa], AB[bb]..ZZ[zz] = 701) to an col (0..701)
	if(label.length() == 1) {//deal with single letter value (A = 0, B = 2 .. Z = 25)
		return toupper(int(label[0])) - 65; // 65 represents ASCII 'A' 'A' - 65 = 0
	} else if(label.length() == 2) {//deal with pair values (AA = 26, BA = 52, CA = 78 .. ZZ = 701)
		return ((toupper(int(label[1])) - 65) + (toupper(int(label[0])) - 64) * 26); // -64 to get base 1 thus avoiding 0 * n = 0 issue
	}
	return -1; //no match 
}

udt_coordref coords_extract(std::string coordinate) {
	//extract column and row reference from a coordinate (e.g., A1..ZZ80)
	std::string col = "", row = "";
	udt_coordref coordref;
	coordinate = com_erasecharacter(coordinate); //remove any whitespace
	for (std::string::iterator it = coordinate.begin(); it != coordinate.end(); ++it) {
		if (std::isalpha(*it)) {
			col += toupper(*it); //extract and convert label value to upper
		} else if (isdigit(*it)) {
			row += *it; //extract and store 'numeric' row value
		}
	}
	
	coordref.collabel = col;
	coordref.row = com_stoi(row); //convert to int
	return coordref;
}