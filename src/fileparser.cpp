#include "../headers/adaship.h"


void fileparser_readfile(std::string filename) {
	//parse the config file
	std::ifstream file;
	std::string data = "";
	udt_split split;

	configuration* config = configuration::getInstance();

	file.open(filename, std::ios::in); //open config file
	if(file) { //chech if file opened
		while(!file.eof()) { //loop through each line
			getline(file, data); //get each current line
			split = com_split(data, ":");
			if(com_tolower(split.left) == "board") {
				split = com_split(split.right, "x");
				config->setboard(
					com_stoi(split.left, DEFAULT_BOARDWIDTH),
					com_stoi(split.right, DEFAULT_BOARDHEIGHT)
				);
			}

			if(com_tolower(split.left) == "boat") {
				split = com_split(split.right, ",");
				config->registerboat(split.left, com_stoi(split.right, COM_NOVALUE));
			}
		}
		file.close();
		
	} else {
		//file failed to open, is it there?
		com_exit("\nFailed to open " + filename + ") file", false);
	}
}