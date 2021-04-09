#pragma once //ensure header only included once during compilation

//define constant colour string sequences
const std::string CONSOLE_CLEAR("\x1b[2J\x1b[1;1H"); //2J clear console (top to bottom), 1; 1H position cursor at row 1 column 1
const std::string CONSOLE_CLEARTOENDOFLINE("\x1b[K"); //clears from the current cursor to the end of the line
const std::string CONSOLE_CLEARFROMSTARTOFLINE("\x1b[1K"); //clears the start of the line up to the current cursor
const std::string CONSOLE_CLEARLINE("\x1b[2K\r"); //clear line and return cursor (\r) to start of line
const std::string CONSOLE_CLEARTOBOTTOM("\x1b[0J\r"); //clear to bottom and return cursor (\r) to start of line


const std::string COLOUR_RESET("\x1b[0m");
const std::string FG_BLACK("\x1b[30m");
const std::string FG_RED("\x1b[31m");
const std::string FG_GREEN("\x1b[32m");
const std::string FG_YELLOW("\x1b[33m");
const std::string FG_BLUE("\x1b[34m");
const std::string FG_MAGENTA("\x1b[35m");
const std::string FG_CYAN("\x1b[36m");
const std::string FG_WHITE("\x1b[37m");
const std::string FG_BRIGHT_BLACK("\x1b[90m");
const std::string FG_BRIGHT_RED("\x1b[91m");
const std::string FG_BRIGHT_GREEN("\x1b[92m");
const std::string FG_BRIGHT_YELLOW("\x1b[93m");
const std::string FG_BRIGHT_BLUE("\x1b[94m");
const std::string FG_BRIGHT_MAGENTA("\x1b[95m");
const std::string FG_BRIGHT_CYAN("\x1b[96m");
const std::string FG_BRIGHT_WHITE("\x1b[97m");

const std::string BG_BLACK("\x1b[40m");
const std::string BG_RED("\x1b[41m");
const std::string BG_GREEN("\x1b[42m");
const std::string BG_YELLOW("\x1b[43m");
const std::string BG_BLUE("\x1b[44m");
const std::string BG_MAGENTA("\x1b[45m");
const std::string BG_CYAN("\x1b[46m");
const std::string BG_WHITE("\x1b[47m");
const std::string BG_BRIGHT_BLACK("\x1b[100m");
const std::string BG_BRIGHT_RED("\x1b[101m");
const std::string BG_BRIGHT_GREEN("\x1b[102m");
const std::string BG_BRIGHT_YELLOW("\x1b[103m");
const std::string BG_BRIGHT_BLUE("\x1b[104m");
const std::string BG_BRIGHT_MAGENTA("\x1b[105m");
const std::string BG_BRIGHT_CYAN("\x1b[106m");
const std::string BG_BRIGHT_WHITE("\x1b[107m");