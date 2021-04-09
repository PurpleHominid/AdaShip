#include "./headers/adaship.h"
// https://www.rapidtables.com/code/text/unicode-characters.html
// https://en.wikipedia.org/wiki/ANSI_escape_code
// https://www.bogotobogo.com/cplusplus/C11/7_C11_Thread_Sharing_Memory.php
// https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797


int main() {
	configuration* config = configuration::getInstance(); //initialise the 'singleton' config controller
	fileparser_readfile("./adaship_config.ini"); //read the config file; config stored in controller

	launcher(); //launch game
	com_output("\nThanks for playing, goodbye...\n"); //if we are here, user has finished
	
	return 0;
}