#pragma once //ensure header only included once during compilation


const std::string MENU_COLOUR = FG_YELLOW;
const std::string SUBMENU_COLOUR = FG_CYAN;


void menu_launchgame();
int menu_launchgame_options(std::string regex);

int menu_boardsetup(int playerid, bool allowcontinue = false);
int menu_playerturn(int playerid, int maxtorpedoes, int availabletorpedoes);

udt_command menu_placeboatcommand(int boatsregistered);
int menu_autoplaceboatcommand(int boatsregistered);

std::vector <utd_torpedotarget> menu_torpedotargetcoordinates(int torpedoes = 1);