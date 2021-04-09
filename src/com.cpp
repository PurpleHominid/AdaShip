#include "../headers/adaship.h"


void com_exit(std::string prompt, bool okay) {
	std::cout << prompt;
	if(okay) { exit(EXIT_SUCCESS); } else { exit(EXIT_FAILURE); }
}

void com_output(std::string output, std::string colour) {
	std::cout << colour << output << COLOUR_RESET;
	std::cout.flush();
}

void com_randseed() {
  //initialise a random seed based on time
  srand (time(NULL));
}

int com_randrange(int min, int max, bool reseed) {
	//returns a selected value (at random) based on (and including) the 'min' and 'max' values
	if(reseed) { //reseed if required
		com_randseed();
	}
	return rand() % ((max+1) - min) + min;
}

bool com_isnumber(std::string str) {
	double number; //setup a container to catch the value
	std::istringstream value(str); //transfer the string variable to a stream variable value
	value >> number; //output value to the number variable
	return !value.fail() && value.eof(); //if the output didnt fail and is eof return true
}

character_type com_chartype(char c) {
	if(isalpha(c)) { return character_type::ISALPHA; } 
	else if (isdigit(c)) { return character_type::ISDIGIT; } 
	else if (ispunct(c)) { return character_type::ISPUNCT; } 
	else if (isspace(c) || isblank(c)) { return character_type::ISSPACE; } else if (iscntrl(c)) { return character_type::ISCNTRL; } 
	else { return character_type::ISUNKNOWN;}
}

int com_stoi(std::string str, int initvalue) {
	//return an int version of the string (or if not available the initialise value)
	return com_isnumber(str) ? stoi(str) : initvalue;
}

std::string com_ltrim(std::string str) { //left trim string
	return std::regex_replace(str, std::regex(REGEXP_LEADINGSPACES), std::string(""));
}

std::string com_rtrim(std::string str) { //right trim
	return std::regex_replace(str, std::regex(REGEXP_TRAILINGSPACES), std::string(""));
}

std::string com_trim(std::string str) { //full trim
	return com_ltrim(com_rtrim(str));
}

std::string com_erasecharacter(std::string str, char character) { //removes the specific character
	str.erase(std::remove(str.begin(), str.end(), character), str.end());
	return str;
}

std::string com_extractdigits(std::string str) { //filter all non-digits
	str.erase(std::remove_if(str.begin(), str.end(), [](char character) { 
		return !std::isdigit(character); 
	}), str.end());
	return str;
}

std::string com_extractalpha(std::string str) { //filter all non-alpha
	str.erase(std::remove_if(str.begin(), str.end(), [](char character) { 
		return !std::isalpha(character) && !std::isspace(character); 
	}), str.end());
	return str;
}

std::string com_toupper(std::string str) {
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);
	return str;
}

std::string com_tolower(std::string str) {
	std::transform(str.begin(), str.end(),str.begin(), ::tolower);
	return str;
}

void com_sleep(int ms) {
	//when using threads its worth noting that writing to streams is likely not thread-safe,
	//cout is buffered-stream, to ensure that any information in the stream is 'send' the cout buffer needs to be flushed
	//std::cout.flush(); the "\n" does not include a 'flush' whereare std:endl does - use endl as a default to ensure
	//the stream is output
	std::this_thread::sleep_for( std::chrono::milliseconds { ms } );
}

std::string com_getch() {
	//wait for a single key to be pressed - does not require 'return/enter' key
	char buf = 0;
	struct termios old = {0};
	std::string str;

	fflush(stdout);
	if(tcgetattr(0, &old) < 0) { perror("tcsetattr()"); }

	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	
	if(tcsetattr(0, TCSANOW, &old) < 0) { perror("tcsetattr ICANON"); }

	if(read(0, &buf, 1) < 0) { perror("read()"); }
	str.push_back( buf );

	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	
	if(tcsetattr(0, TCSADRAIN, &old) < 0) { perror("tcsetattr ~ICANON");}
	return str;
}

std::vector < std::string > com_breakapart(std::string str) {
	std::istringstream s(str);
	std::vector < std::string > words;
	str = com_trim(str);
	if(str.length() > 0) {
		std::string word; //hold each word
		while (s >> word) { //traverse through all words
			if(word.length()>0) {
				words.push_back({
					word
				});
			}
		}
	}
	return words;
}

udt_split com_split(std::string str, std::string delim, bool trim, bool ignorecase) {
	//split string using the delim character with options
	std::string search = str; //create a local copy of the source string
	udt_split split;
	split.left = "";
	split.right = "";

	if(ignorecase) {
		search = com_tolower(str);
		delim = com_tolower(delim);
	}

	std::size_t pos = search.find(delim); //compare the delimiter based on lowercase if ignoring case
	if(pos > 0 && pos < search.length()) { //check to make sure positions are appropriate
		split.left = str.substr(0, pos);
		split.right = str.substr(pos + delim.length(), std::string::npos);
		if(trim) {
			split.left = com_trim(split.left);
			split.right = com_trim(split.right);
		}
	}
	return split;
}

std::string com_ui_inputstring(std::string prompt, std::string tryagain, std::regex pattern, std::string colour) {
	//wait and validate user input	
	busy spinner;
	std::string input = ""; //create a local variable to hold the user's input
	while(true) { //repeat till break
		com_output(com_bold() + prompt + com_bold(false), colour); //show the prompt
		std::cin.clear(); //clear any existing steam data
		getline(std::cin, input); //get the user's input
		if (std::regex_match(input, pattern)) { //verify 
			break; //exit the loop
		} else { //invalid input
			com_output(tryagain); //issue the failed input message and flush
			spinner.show(1000);
			
			com_clearconsole(CLEARLINE); //clear the try again
			com_movecursor(UP, 1); //move cursor up
			com_clearconsole(CLEARLINE); //clear the prompt message
		}
	}
	std::cin.clear();
	return input;
}

std::string com_ui_inputchar(std::string prompt, std::string tryagain, std::regex pattern, std::string colour) {
	//wait for a single key to be pressed - does not require 'return/enter' key
	//wait and validate user input	
	busy spinner;
	std::string input = ""; //create a local variable to hold the user's input
	while(true) { //repeat till break
		com_output(com_bold() + prompt + com_bold(false), colour); //show the prompt
		input = com_getch(); //get the user's input
		if (std::regex_match(input, pattern)) { //verify 
			break; //exit the loop
		} else { //invalid input
			com_output(tryagain); //issue the failed input message and flush
			spinner.show(1000);
			com_clearconsole(CLEARLINE); //clear the try again
		}
	}
	std::cin.clear();
	return input;
}

void com_ui_continuepause(std::string prompt, std::string colour) {
	com_hidecursor(true);
	com_output(prompt, colour); //show the prompt
	com_getch(); //wait for a key press
	com_hidecursor(false);
}

void com_clearconsole(console_commands command) {
	std::string instr = "";
	switch (int(command)) {
		case console_commands::CLEAR:
			instr = CONSOLE_CLEAR;
			break;
		case console_commands::CLEAR_TOEND: 
			instr = CONSOLE_CLEARTOENDOFLINE;
			break;
		case console_commands::CLEAR_FROMSTART: 
			instr = CONSOLE_CLEARFROMSTARTOFLINE;
			break;
		case console_commands::CLEARLINE: 
			instr = CONSOLE_CLEARLINE;
			break;
		case console_commands::CLEARTOBOTTOM: 
			instr = CONSOLE_CLEARTOBOTTOM;
			break;
	}
	com_output(instr); //output and flush;
}

void com_movecursor(cursor_control direction, int count) {
	std::string instr = "\x1b[" + std::to_string(count);
	switch(direction) {
		case UP: instr += "A"; break;
		case DOWN: instr += "B"; break;
		case FORWARD: instr += "C"; break;
		case BACKWARD: instr += "D"; break;
	}
	com_output(instr); //output and flush;
}

void com_hidecursor(bool hide) {
	if(hide) {
		std::cout << "\033[?25l"; //hide the cursor
	} else {
		std::cout << "\033[?25h"; //show the cursor
	}
	std::cout.flush();
}

void com_positioncursor(int col, int row) {
	std::string instr = "\x1b[" + std::to_string(row) + ";" + std::to_string(col) + "f";
	com_output(instr); //output and flush;
}

void com_storecursorposition() {
	std::cout << "\033[s"; //store the cursor
}

void com_restorecursorposition() {
	std::cout << "\033[u"; //restore the cursor
}

std::string com_colours(ansi_colours fg, ansi_colours bg) {
	// \x1b is used to represent hex values, \033 is decimal
	if(bg == ansi_colours::DEFAULT) { //no specified background colour specified
		return "\x1b[" + std::to_string(fg) + "m";
	} else { //specified background colour specified
		return "\x1b[" + std::to_string(fg) + ";" + std::to_string((bg + 10)) + "m";
	}
}

std::string com_bold(bool enabled) {
	return enabled ? "\x1b[1m" : "\x1b[0m";
}

udt_command com_extractinstruction(std::string raw, int boatsregistered) {
	udt_command instruction = { false, COM_NOVALUE, "", orientation::VERTICAL }; //set default

	character_type last = character_type::ISDIGIT, current = character_type::ISUNKNOWN;
	std::vector < std::string > segments;
	std::string temp = "";
	raw = com_trim(com_toupper(com_erasecharacter(raw, ' '))) += "!"; //remove all spaces, convert to uppercase and final marker
	int MAX_BOATS = 5;

	for(int i = 0; i < raw.length(); i++) {
		current = com_chartype(raw[i]);
		if(current == last) {
			temp.push_back(raw[i]); //append to temp
		} else {
			last = current;
			segments.push_back({ temp }); //append to segments
			temp = ""; //clear
			temp.push_back(raw[i]); //append character to temp string
		}
	}

	instruction.boatidx = com_stoi(segments[0], -1);
	if(instruction.boatidx >=0 && instruction.boatidx < boatsregistered) {
		instruction.valid = true;
		instruction.coordinate = segments[1] + segments[2];
		if(segments[3] == "VERTICAL" || segments[3] == "V") {
			instruction.direction = orientation::VERTICAL;
		} else if (segments[3] == "HORIZONTAL" || segments[3] == "H") {
			instruction.direction = orientation::HORIZONTAL;
		} else {
			instruction.valid = false;
		}
	}
	return instruction;
}