#include "../headers/adaship.h"


void menu_launchgame() {
	configuration* config = configuration::getInstance(); //access the config controller
	com_clearconsole(console_commands::CLEAR); //clear the console
	switch(menu_launchgame_options("^[0-7]+$")) {
		case 1: config->type(gametype::ONEPLAYER); break;
		case 2: config->type(gametype::TWOPLAYER); break;
		case 3: config->type(gametype::ONEPLAYERSALVO); break;
		case 4: config->type(gametype::TWOPLAYERSALVO); break;
		case 5: config->type(gametype::ONEPLAYERHIDDENMINE); break;
		case 6: config->type(gametype::TWOPLAYERHIDDENMINE); break;
		case 7: config->type(gametype::COMPUTERVCOMPUTERHIDDENMINE); break;
		case 0: config->type(gametype::QUIT); break;
	}
}
int menu_launchgame_options(std::string regex) {
	com_clearconsole(console_commands::CLEAR); //clear the console
	std::regex validexpression(regex);
	std::cout << MENU_COLOUR << "Welcome to Adaship\n" << COLOUR_RESET;
	std::cout << FG_CYAN << 
	"Essentially, AdaShip is a clone of the classic ‘Battleship’ game – as a default, \n" << 
	"AdaShip is a two-player, turn based game of sea warfare. You and an opponent each \n" << 
	"place a number of ships on your own board, and you then alternate turns 'firing' \n" << 
	"torpedoes at each other’s ships. The game is won when one player has destroyed/sunk \n" << 
	"all of the other player’s ships";

	std::cout << "\n\n" << 
	"The salvo verison updates the standard game by allowing the current ‘player’ (player \n" <<
	"or computer) to ‘fire’ one torpedo per their remaining ships.";  

	std::cout << "\n\n" << 
	"The hidden mines verison updates the standard game by including randomly dropped mines. \n" <<
	"If a torpedo ‘hits’ a hidden mine that location and the eight immediately surrounding \n" <<
	"it ‘explode’, any ships that directly intersect with the ‘explosion’ are ‘hit’ as per normal";	
	std::cout << COLOUR_RESET;

	std::cout << MENU_COLOUR << "\n\n\nAdaShip - Main Menu" << COLOUR_RESET;
	std::cout << "\n1. One Player v Computer";
	std::cout << "\n2. Two Player (Player v Player)";
	std::cout << "\n3. One Player v Computer (Salvo Game)";
	std::cout << "\n4. Two Player (Salvo Game)";
	std::cout << "\n5. One Player v Computer (Hidden Mines Game)";
	std::cout << "\n6. Two Player (Hidden Mines Game)";
	std::cout << "\n7. Computer v Computer (Hidden Mines Game)";
	std::cout << "\n\n0. Quit";
	std::cout << "\n\n\n";
	return stoi(com_ui_inputchar("Enter option: ", "Invalid option - resetting.. ", validexpression, MENU_COLOUR));
}

int menu_boardsetup(int playerid, bool allowcontinue) {
	std::string colour = FG_BRIGHT_BLACK;
	std::string validation = "^[0-5]+$";	
	if(allowcontinue) {
		colour = FG_GREEN;
		validation = "^[0-6]+$";
	} 
	std::cout << MENU_COLOUR << "Player " << playerid+1 << ": Board Setup Menu" << COLOUR_RESET;
	std::cout << "\n1. Select and Place a Ship";
	std::cout << "\n2. Select and Auto-Place a Ship";
	std::cout << "\n3. Auto-Place All Available Ships";
	std::cout << "\n4. Auto-place All Ships";
	std::cout << "\n5. Reset Board";
	std::cout << colour << "\n6. Ship Deloyments Confirmed - Select to Continue..." << COLOUR_RESET;
	std::cout << "\n\n0. Exit Setup";
	std::cout << "\n\n\n";
	return stoi(com_ui_inputchar("Enter option: ", "Invalid option - resetting.. ", std::regex(validation), MENU_COLOUR));
}

int menu_playerturn(int playerid, int maxtorpedoes, int availabletorpedoes) {
	std::string options = FG_WHITE;
	std::string endround = FG_BRIGHT_BLACK;

	std::string validation = "^[0-2]+$";	
	if(availabletorpedoes == 0) {
		options = FG_BRIGHT_BLACK;
		endround = FG_GREEN;
		validation = "^[03]+$";
	} 
	std::cout << MENU_COLOUR << "Player " << playerid+1 << ": Turn Menu (" << availabletorpedoes << "/" << maxtorpedoes << " torpedoes available)" << COLOUR_RESET;
	std::cout << options << "\n1. Torpedo (Manual Launch)" << COLOUR_RESET;
	std::cout << options << "\n2. Torpedo (Auto Fire)" << COLOUR_RESET;
	std::cout << endround << "\n3. End Turn - Select to Continue..." << COLOUR_RESET;
	std::cout << "\n\n0. Quit Setup";
	std::cout << "\n\n\n";
	return stoi(com_ui_inputchar("Enter option: ", "Invalid option - resetting.. ", std::regex(validation), MENU_COLOUR));
}

udt_command menu_placeboatcommand(int boatsregistered) {
	//used to extract and place a boat - returns valid; boatidx; coordinate; direction;
	std::string raw = "";
	com_restorecursorposition();
	com_clearconsole(console_commands::CLEARTOBOTTOM);
	com_output("Select and Place a Ship\n", MENU_COLOUR);
	raw = com_ui_inputstring("Enter Boat 'Id Coordinate Orientation' (e.g. 0 A1 V): ", "Invalid instruction.. ", REGEXP_BOATPLACEMENT);
	raw = com_toupper(com_erasecharacter(raw, ' ')); //remove all spaces and convert to uppercase
	return com_extractinstruction(raw, boatsregistered);
}

int menu_autoplaceboatcommand(int boatsregistered) {
	//used to extract and place a boat - returns valid; boatidx; coordinate; direction;
	int boatidx = COM_NOVALUE;
	com_restorecursorposition();
	com_clearconsole(console_commands::CLEARTOBOTTOM);
	com_output("Select a Ship to Auto-Place\n", MENU_COLOUR);
	boatidx = com_stoi(com_ui_inputstring("Enter Boat Id (e.g. 0): ", "Invalid reference.. ", REGEXP_NUMERICONLY), -1);
	boatidx >= boatsregistered ? boatidx = -1 : boatidx = boatidx;
	return boatidx;
}

std::vector <utd_torpedotarget> menu_torpedotargetcoordinates(int torpedoes) {
	configuration* config = configuration::getInstance(); //access the config controller
	std::vector <utd_torpedotarget> targets;
	std::string raw = "";
	com_restorecursorposition();
	com_clearconsole(console_commands::CLEARTOBOTTOM);
	com_output("Specify targeting coordinates\n", MENU_COLOUR);
	raw = com_ui_inputstring("Enter Torpedo Coordinate (e.g. A3): ", "Invalid coordinate.. ", REGEXP_COORDINATES);
	std::vector < std::string > rawtargets = com_breakapart(raw); //break any multiple targets in to individual coordinates
	for (int i = 0; i < rawtargets.size(); i++) {
		targets.push_back({ com_toupper(rawtargets[i]) }); //convert to uppercase
	}
	return targets;
}