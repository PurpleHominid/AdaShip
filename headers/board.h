#pragma once //ensure header only included once during compilation


class board {
	public:
		board();

		void render(bool rendermines = false); //render (display) the board
		udt_symbol boardsymbol(udt_tile &tile, bool rendermines = true); //pass 'tile' by reference; essentially a pointer to the location rather than a copy of the value
		
		void deploymines(int mines, bool clearexisting = true);

		void settile_mined(std::string coordinate, bool mined); //update tile mined status
		void settile_state(std::string coordinate, status state); //update tile state (occupied, available, etc)
		
		void settile_boatidx(int col, int row, int boatidx, bool updatestate = true); //wrapper function
		void settile_boatidx(std::string coordinate, int boatidx, bool updatestate = true); //overloaded wrapper
		void settile_boatidx(int idx, int boatidx, bool updatestate = true); //update tile boat index and (if requested) status as occupied
		
		udt_tile tile(std::string coordinate); //overloaded wrapper
		udt_tile tile(int col, int row); //overloaded wrapper
		udt_tile tile(int idx); //return a specific tile instance
		
		std::vector < int > collecttiles(int boatidx); //collect all tiles matching boatidx
		std::vector < int > collecttiles(status state = status::AVAILABLE, int minlength = 1); //collects all tiles matching state
		
		udt_lengths availablelength(std::string coordinate);//overloaded wrapper
		udt_lengths availablelength(int col, int row);//overloaded wrapper
		udt_lengths availablelength(int idx); //collects max lengths at the specified tile

	protected:


	private:
		configuration* config;
		std::vector < udt_tile > tile_;
};