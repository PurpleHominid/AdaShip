#include "../headers/adaship.h"


board::board() { //construtor with initialisation for tiles
	config = configuration::getInstance();
	int tiles = config->boardwidth() * config->boardheight();
	udt_boardref boardref;
	std::string coordinate;

	//build board
	for(int i = 0; i < tiles; i++) {
		coordinate = coords_getcoordinate(i, config->boardwidth() );
		boardref = coords_getboardref(i, config->boardwidth() );	
		tile_.push_back({
			i, //copy of the vector array index; useful as object is accessable in multiple ways
			coordinate, //coordinate reference, e.g., A1 .. AA0 ..
			boardref.col, //base 0 col reference, board reference/location on x-axis 
			boardref.row, //base 0 row reference, board reference/location on y-axis 
			false, //mined status 
			COM_NOVALUE, //default boat idx, used to reference which boat instance occupies this tile 
			status::AVAILABLE //default tile status, available, occupied, hit, miss..
		});
	}
}

udt_symbol board::boardsymbol(udt_tile &tile, bool rendermines) {
	udt_symbol symbol = { "", "", ""};
	std::string placeholder;
	// 2620 skull, 2691 flag, 25A0 square, 00b7 middle dot
	switch (int(tile.state)) {   
		case status::ERROR:
			symbol.fgcolour = FG_CYAN;
			symbol.bgcolour = BG_BLUE;
			placeholder = "E";
			break;
		case status::AVAILABLE: 
			symbol.fgcolour = FG_BRIGHT_BLUE;
			symbol.bgcolour = BG_BLUE;
			placeholder = "\u00B7";
			break;
		case status::OCCUPIED:
			symbol.fgcolour = FG_BRIGHT_YELLOW;
			symbol.bgcolour = BG_BLUE;
			placeholder = "\u25A0";
			break;
		case status::DAMAGED_BYTORPEDO:
			symbol.fgcolour = FG_BRIGHT_BLUE;
			symbol.bgcolour = BG_BLUE;
			placeholder = "\u25A0";
			break;
		case status::DAMAGED_BYMINE: 
			symbol.fgcolour = FG_BRIGHT_BLUE;
			symbol.bgcolour = BG_MAGENTA;
			placeholder = "\u2620";
			break;
		case status::DAMAGED_BYMINETORPEDO:
			symbol.fgcolour = FG_BRIGHT_BLUE;
			symbol.bgcolour = BG_MAGENTA;
			placeholder = "\u25A0";
			break;
		case status::TORPEDO_MISS: 
			symbol.fgcolour = FG_BLACK;
			symbol.bgcolour = BG_BLUE;
			placeholder = "\u2691"; 
			break;
		case status::TORPEDO_HIT:
			symbol.fgcolour = FG_BLACK;
			symbol.bgcolour = BG_BRIGHT_YELLOW;
			placeholder = "\u2691"; 
			break;
		case status::TORPEDO_HITMINE:
			symbol.fgcolour = FG_BLACK;
			symbol.bgcolour = BG_MAGENTA;
			placeholder = "\u2620"; 
			break;
		case status::TORPEDO_HITMINEBOAT:
			symbol.fgcolour = FG_BLACK;
			symbol.bgcolour = BG_BRIGHT_YELLOW;
			placeholder = "\u2620";
			break;
	}

	if(tile.mined && rendermines) { //override the default background colour if tile is mined
		symbol.bgcolour = BG_MAGENTA;
		if(tile.state == status::AVAILABLE) { //override the placeholder if available
			symbol.fgcolour = FG_WHITE;
			placeholder = "\u2620";
		} 
	}
	symbol.symbol = " " + placeholder + " ";
	return symbol;
}

void board::render(bool rendermines) {
	using std::setw;
	using std::left;
	using std::right;

	udt_tile tile;
	std::string placeholder;

	for (int r = 0; r < 2; r++) {
		r == 1 ? placeholder = RIGHT_BEND : placeholder = "";
		std::cout << "\n" << com_colours(ansi_colours::CYAN) << setw(2) << right << "" << " " << setw(1) << right << std::setfill(' ') << placeholder << " " << com_colours(ansi_colours::RESET);
		for (int c = 0; c < config->boardwidth(); c++) {
			if(r == 0) {
				placeholder = coords_collabel(c);
				std::cout << com_colours(ansi_colours::CYAN) << setw(3) << left << (" " + placeholder) << " " << com_colours(ansi_colours::RESET);
			} else {
				std::cout << com_colours(ansi_colours::CYAN) << setw(3) << left << HORIZONTAL_LINE << " " << com_colours(ansi_colours::RESET);
			}
		}
	}

	for (int r = 0; r < config->boardheight(); r++ ) {
		std::cout << "\n" << com_colours(ansi_colours::CYAN) << setw(2) << right << (r + 1) << " " << setw(2) << right << VERTICAL_LINE << " " << com_colours(ansi_colours::RESET);
		for (int c = 0; c < config->boardwidth(); c++) {
			tile = tile_[ coords_getidx(c, r, config->boardwidth()) ];
			std::cout 
				<< boardsymbol(tile, rendermines).fgcolour 
				<< boardsymbol(tile, rendermines).bgcolour
				<< setw(3) << left << boardsymbol(tile, rendermines).symbol
				//<< setw(3) << left << tile.boatidx
				<< com_colours(ansi_colours::RESET) << " ";
		}
	}
	std::cout << "\n";
}

void board::deploymines(int mines, bool clearexisting) {
	//deply mines randomly
	std::vector < int > idx = board::collecttiles(status::IGNORESTATE, 0); //collect all tiles
	std::string coordinate;
	if(clearexisting) { //clear all existing mine
		for(int i = 0; i < idx.size(); i++) {
			settile_mined(coords_getcoordinate(idx[i], config->boardwidth()), false);
		}
	}
	if(mines <= idx.size()) { //check the number of mines is less than the number of tiles
		std::random_shuffle ( idx.begin(), idx.end() ); //shuffle the order
		for(int i = 0; i < mines; i++) {
			settile_mined(coords_getcoordinate(idx[i], config->boardwidth()), true);
		}
	}
}

void board::settile_mined(std::string coordinate, bool mined) { //update tile's mined status
	int idx = coords_getidx(coordinate, config->boardwidth());
	tile_[idx].mined = mined;
}

void board::settile_state(std::string coordinate, status state) { //update tile's state
	int idx = coords_getidx(coordinate, config->boardwidth());
	tile_[idx].state = state;
}

void board::settile_boatidx(int col, int row, int boatidx, bool updatestate) { //wrapper function
	int idx = coords_getidx(col, row, config->boardwidth());
	settile_boatidx(idx, boatidx, updatestate);
}
void board::settile_boatidx(std::string coordinate, int boatidx, bool updatestate) { //wrapper function
	int idx = coords_getidx(coordinate, config->boardwidth());
	settile_boatidx(idx, boatidx, updatestate);
}
void board::settile_boatidx(int idx, int boatidx, bool updatestate) { //update tile's boatidx
	if(idx >= 0 && idx < tile_.size()) {
		if(boatidx <= COM_NOVALUE || boatidx >= config->boatsregistered()) { //if no value update tile's boatidx and status
			tile_[idx].boatidx = COM_NOVALUE;
			if(updatestate) {
				tile_[idx].state = status::AVAILABLE;
			}
		} else {
			tile_[idx].boatidx = boatidx; //store boatind and occupied status
			if(updatestate) {
				tile_[idx].state = status::OCCUPIED;
			}
		}
	}
}

udt_tile board::tile(std::string coordinate) { //wrapper function
	int idx = coords_getidx(coordinate, config->boardwidth());
	return tile(idx);
}
udt_tile board::tile(int col, int row) { //wrapper function
	int idx = coords_getidx(col, row, config->boardwidth());
	return tile(idx);
}
udt_tile board::tile(int idx) { //returns the specific tile
	udt_tile tile = { COM_NOVALUE, "", COM_NOVALUE, COM_NOVALUE, false, COM_NOVALUE, status::ERROR }; //set default tile
	if(idx >= 0 && idx < tile_.size()) {
		tile = tile_[idx];
	} 
	return tile;
}

std::vector < int > board::collecttiles(status state, int minlength) { //returns vector array of tile indexes
	std::vector < int > idx;
	udt_lengths lengths;
	for(int i = 0; i < tile_.size(); i++) {
		if(tile_[i].state == state || state == status::IGNORESTATE) {
			lengths = availablelength(i);
			if((lengths.maxcols >= minlength) || (lengths.maxrows >= minlength)) {
				idx.push_back({
					i
				});
			}
		}
	}
	return idx;
}

std::vector < int > board::collecttiles(int boatidx) { //returns vector array of tile indexes matching boatidx
	std::vector < int > idx;
	udt_lengths lengths;
	for(int i = 0; i < tile_.size(); i++) {
		if(tile_[i].boatidx == boatidx) {
			idx.push_back({
				i
			});
		}
	}
	return idx;
}

udt_lengths board::availablelength(std::string coordinate) { //wrapper function
	int idx = coords_getidx(coordinate, config->boardwidth());
	return availablelength(idx);
}
udt_lengths board::availablelength(int col, int row) { //wrapper function
	int idx = coords_getidx(col, row, config->boardwidth());
	return availablelength(idx);
}
udt_lengths board::availablelength(int idx) {
	udt_lengths available = { 0, 0 }; //initialise return variable structure
	
	int col = tile(idx).col;
	int row = tile(idx).row;

	if(col >= 0 && row >= 0) {
		for(int c = col; c < config->boardwidth(); c++) { //find all available cols
			if(tile(c, row).state == status::AVAILABLE) {
				available.maxcols++;
			} else { //hit the first non-available tile - can't go any further
				break;
			}
		}
		for(int r = row; r < config->boardheight(); r++) { //find all available rows
			if(tile(col, r).state == status::AVAILABLE) { //check available
				available.maxrows++;
			} else { //hit the first non-available tile - can't go any further
				break;
			}
		}
		return available;
	} 
	return { COM_NOVALUE, COM_NOVALUE };
}