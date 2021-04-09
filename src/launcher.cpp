#include "../headers/adaship.h"


void launcher() {
	configuration* config = configuration::getInstance(); //access the config controller
	
	while(config->type() != gametype::QUIT) { //create outer game loop
		switch(int(config->type())) {
			case gametype::MENU: menu_launchgame(); break;
			case gametype::ONEPLAYER: launch_game(); break;
			case gametype::TWOPLAYER: launch_game(2); break;
			case gametype::ONEPLAYERSALVO: launch_game(1, true); break;
			case gametype::TWOPLAYERSALVO: launch_game(2, true); break;
			case gametype::ONEPLAYERHIDDENMINE: launch_game(1, false, 5); break;
			case gametype::TWOPLAYERHIDDENMINE: launch_game(2, false, 5); break;
			case gametype::COMPUTERVCOMPUTERHIDDENMINE: launch_game(0, false, 5); break;
			case gametype::QUIT: break;
		}
	}
}

void launch_game(int humanplayers, bool salvo, int mines) {
	gameplay adaship(humanplayers, salvo, mines);
	adaship.start();
}