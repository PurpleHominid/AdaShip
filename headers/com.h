#pragma once //ensure header only included once during compilation


void com_exit(std::string prompt, bool okay = true);

void com_output(std::string output, std::string colour = FG_WHITE); //outputs the string to cout and flushes

void com_randseed();
int com_randrange(int min, int max, bool reseed = false);

bool com_isnumber(std::string str);
character_type com_chartype(char c);

int com_stoi(std::string str, int initvalue = COM_NOVALUE);

std::string com_ltrim(std::string str);
std::string com_rtrim(std::string str);
std::string com_trim(std::string str);
std::string com_erasecharacter(std::string str, char character = COM_SPACE);
std::string com_extractdigits(std::string str);
std::string com_extractalpha(std::string str);

std::string com_toupper(std::string str);
std::string com_tolower(std::string str);

void com_sleep(int ms = 1000);
std::string com_getch();

std::vector < std::string > com_breakapart(std::string str);
udt_split com_split(std::string str, std::string delim, bool trim = true, bool ignorecase = true);

std::string com_ui_inputstring(std::string prompt, std::string tryagain, std::regex pattern = REGEXP_ALPHANUMERIC, std::string colour = COLOUR_RESET);
std::string com_ui_inputchar(std::string prompt, std::string tryagain, std::regex pattern = REGEXP_ANY, std::string colour = COLOUR_RESET);
void com_ui_continuepause(std::string prompt = "\npress a key to continue...", std::string colour = COLOUR_RESET);

void com_clearconsole(console_commands command = console_commands::CLEAR);
void com_movecursor(cursor_control direction = cursor_control::BACKWARD, int count = 1);

void com_hidecursor(bool hide = true);
void com_positioncursor(int col = 0, int row = 0);
void com_storecursorposition();
void com_restorecursorposition();


std::string com_colours(ansi_colours fg, ansi_colours bg = ansi_colours::DEFAULT);
std::string com_bold(bool enabled = true);

udt_command com_extractinstruction(std::string raw, int boatsregistered);