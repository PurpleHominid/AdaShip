#pragma once //ensure header only included once during compilation


const int DEFAULT_BOARDWIDTH = 10;
const int DEFAULT_BOARDHEIGHT = 10;

const int COM_NOVALUE = -1;
const char COM_SPACE = ' ';

const std::string BLANK = "\u0020";
const std::string RIGHT_BEND = "\u250C";
const std::string HORIZONTAL_LINE = "\u2500\u252C\u2500";
const std::string VERTICAL_LINE = "\u251C"; 

enum orientation { VERTICAL, HORIZONTAL };

enum board_type { SHIPBOARD = 0, TARGETBOARD = 1 };

enum status { ERROR, IGNORESTATE, AVAILABLE, OCCUPIED, DAMAGED_BYTORPEDO, DAMAGED_BYMINE, DAMAGED_BYMINETORPEDO, TORPEDO_MISS, TORPEDO_HIT, TORPEDO_HITMINE, TORPEDO_HITMINEBOAT };
enum cursor_control { UP, DOWN, FORWARD, BACKWARD };
enum boatstate { INVALID, OVERRIDE, WAITING, DEPLOYED, DESTROYED }; //invalid used to initialise defaults, override used in recalling boats, etc

enum gametype { 
	MENU, 
	ONEPLAYER, TWOPLAYER, 
	ONEPLAYERSALVO, TWOPLAYERSALVO, 
	ONEPLAYERHIDDENMINE, TWOPLAYERHIDDENMINE, 
	COMPUTERVCOMPUTERHIDDENMINE, 
	QUIT 
};
enum gamestate { PENDING, SETUP_P1, SETUP_P2, TURN_P1, TURN_P2, WIN_P1, WIN_P2, DRAW };

enum console_commands { CLEAR, CLEAR_TOEND, CLEAR_FROMSTART, CLEARLINE, CLEARTOBOTTOM };

enum character_type { ISUNKNOWN, ISALPHA, ISDIGIT, ISPUNCT, ISSPACE, ISCNTRL };

enum ansi_colours {
	RESET = 0, DEFAULT = 0,
	HIDDEN = 8, //hide the output value, values can still be copied to clipboard
	BLACK = 30, //foreground colours start: 30+, background: 40+
	RED = 31,
	GREEN = 32,
	YELLOW = 33,
	BLUE = 34,
	MAGENTA = 35,
	CYAN = 36,
	WHITE = 37,
	BRIGHT_BLACK = 90, //bright foreground colours start: 90+, bright background: 100+
	BRIGHT_RED = 91,
	BRIGHT_GREEN = 92,
	BRIGHT_YELLOW = 93,
	BRIGHT_BLUE = 94,
	BRIGHT_MAGENTA = 95,
	BRIGHT_CYAN = 96,
	BRIGHT_WHITE = 97
};

struct udt_split {
	std::string left;
	std::string right;
};

struct udt_boat {
	std::string name;
	int length;
};

struct udt_boardref {
	int col;
	int row;
};

struct udt_lengths {
	int maxcols;
	int maxrows;
};

struct udt_coordref {
	std::string collabel;
	int row;
};

struct udt_tile {
	int idx;
	std::string coordinate;
	int col;
	int row;
	bool mined;
	int boatidx;
	status state;
};

struct udt_symbol {
	std::string fgcolour;
	std::string bgcolour;
	std::string symbol;
};


struct udt_tiles {
	int col;
	int row;
	std::string coordinate;
};

struct udt_fleet {
	int boatidx;
	int health;
	boatstate state;
	udt_boat boat;
	std::vector < udt_tiles > tiles;
};

struct udt_targetcoordinate {
	std::string coordinate;
	orientation direction;
};

struct udt_command {
	bool valid;
	int boatidx;
	std::string coordinate;
	orientation direction;
};

struct utd_torpedotarget {
	std::string coordinate;
};

struct udt_targethitbox {
	std::string coordinate;
	status originalstate;
	bool hit;
	bool mined;
	int boatidx;
};

struct udt_comms {
	bool archive;
	std::string message;
};

struct udt_torpedostatus {
	int maxavailable;
	int currentavailability;
};