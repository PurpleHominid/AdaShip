#pragma once //ensure header only included once during compilation

// coordinate = A1 .. ZZ80
// idx = 1D array index
// boardref = column and row
// coordref = label and row

bool coords_validatecoordinate(std::string coordinate, int boardwidth, int boardheight, bool zerobaserows = true);

int coords_getidx(int col, int row, int width); //convert col, row to idx
int coords_getidx(std::string coordinate, int width, bool zerobaserows = true); //convert coordinate to idx

std::string coords_getcoordinate(int col, int row, int width, bool zerobaserows = true); //convert col, row to coordinate
std::string coords_getcoordinate(int idx, int width, bool zerobaserows = true); //convert idx to coordinate

std::vector < std::string > coords_getsurrounding(std::string coordinate, int width, bool zerobaserows = true);

udt_boardref coords_getboardref(int idx, int width); //convert idx to col, row
udt_boardref coords_getboardref(std::string coordinate, bool zerobaserows = true);

std::string coords_collabel(int col); //convert col value to label

int coords_col(std::string label); //convert label to col
udt_coordref coords_extract(std::string coordinate); //convert coordinate to collabel and row