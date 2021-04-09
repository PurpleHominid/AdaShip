#include "../headers/adaship.h"


gameplay::gameplay(int humanplayers, bool salvo, int mines) { //construtor with initialisation for tiles
	config = configuration::getInstance();
	humanplayers_ = humanplayers; //store the number of players
	salvo_ = salvo; //
	mines_ = mines; //store the number of mines, 0 = no mines, or n mines
}

std::string gameplay::gametype() {
	std::string type = "Standard";
	if(salvo_) {
		type = "Salvo";
		if(mines_ > 0) {
			type = "Hidden Mines Salvo";
		}
	} else if(mines_ > 0) {
		type = "Hidden Mines";
	}
	switch (humanplayers_) {
		case 0: type += " Computer V Computer"; break;
		case 1: type += " Player V Computer"; break;
		case 2: type += " Player V Player"; break;
	}
	return type;
}

void gameplay::damagestatus(int playerid) {
	int available = player_[playerid].boatsavailable();
	int registered = config->boatsregistered();
	int destroyed = registered - available;
	com_output("\nEnemy Fleet Status: " + std::to_string(destroyed) + "/" + std::to_string(registered) + " Boats Destroyed", FG_BRIGHT_CYAN);
}

int gameplay::winner() { //returns the winning player id 0 or 1; -1 no winner
	int attacker, defender;
	roles(attacker, defender);

	if(player_[attacker].boatsavailable() <= 0) { //if the attacker has no more boats, the defender is the winner
		return defender;
	} else if(player_[defender].boatsavailable() <= 0) { //if the defender has no more boats, the attacker is the winnder
		return attacker;
	} 
	return COM_NOVALUE;
}

void gameplay::roles(int &attacker, int &defender) { //updates information about who's attacking, who's defending
	if(currentplayer_ == 0) {
		attacker = 0;
		defender = 1;
	} else {
		attacker = 1;
		defender = 0;
	}
}

int gameplay::defenderid() {
	int attacker, defender;
	roles(attacker, defender);
	return defender;
}

udt_targethitbox gameplay::autotarget() {
	//auto select target from attackers target board
	int attacker, defender;
	std::string coordinate;
	udt_targethitbox hitbox;

	roles(attacker, defender); //confirm roles
	coordinate = player_[attacker].autotarget(); //autoselect target
	hitbox = player_[defender].targethitbox(coordinate); //use target to acquire defender's hitbox
	return hitbox;
}

void gameplay::explode(udt_targethitbox hitbox) {
	//register torpedo explosion status for defender's hitbox location
	std::vector < std::string > surround;
	int attacker, defender;
	roles(attacker, defender);

	if( coords_validatecoordinate(hitbox.coordinate, config->boardwidth(), config->boardheight()) )  {
		//valid board coordinate
		switch(int(hitbox.originalstate)) {
			case status::AVAILABLE:
				if(hitbox.mined) { //no boat but is mined

					surround = coords_getsurrounding( hitbox.coordinate, config->boardwidth() );
					for (auto& it : surround) {
						std::cout << "\n mined update - surrounding: " << it;
					}

					player_[attacker].sendcomms("Contact: Target " + hitbox.coordinate + " - Mine Exploded, No Damage Detected");
					player_[attacker].updateboard_state(TARGETBOARD, hitbox.coordinate, TORPEDO_HITMINE); //update attacker's targetboard
					
					player_[defender].sendcomms("Report In: Attack at " + hitbox.coordinate + " - Mine Exploded, No Damage Registered");
					player_[defender].updateboard_state(SHIPBOARD, hitbox.coordinate, DAMAGED_BYMINE); //update defender's shipbord
				} else { //no boat, no mine
					player_[attacker].sendcomms("No Contact: Target " + hitbox.coordinate + " - No Damage Detected");
					player_[attacker].updateboard_state(TARGETBOARD, hitbox.coordinate, TORPEDO_MISS); //update attacker's targetboard

					player_[defender].sendcomms("Report In: Attack at " + hitbox.coordinate + " - No Damage Registered");
				}
				break;
			case status::OCCUPIED:
				if(hitbox.mined) { //boat present and is mined

					surround = coords_getsurrounding( hitbox.coordinate, config->boardwidth() );
					for (auto& it : surround) {
						std::cout << "\n mined surrounding: " << it;
					}

					player_[attacker].sendcomms("Contact: Target " + hitbox.coordinate + " - Multiple Explosions (Mine & Enemy Boat), Damage Detected");
					player_[attacker].updateboard_state(TARGETBOARD, hitbox.coordinate, TORPEDO_HITMINEBOAT); //update attacker's targetboard
					
					player_[defender].sendcomms("Report In: Attack at " + hitbox.coordinate + " - Multiple Explosions (Mine & Boat), Damage Registered");
					player_[defender].updateboard_state(SHIPBOARD, hitbox.coordinate, DAMAGED_BYMINETORPEDO); //update defender's shipboard
				
				} else { //boat present, no mine
					player_[attacker].sendcomms("Contact: Target " + hitbox.coordinate + " - Boat Explosion Detected");
					player_[attacker].updateboard_state(TARGETBOARD, hitbox.coordinate, TORPEDO_HIT); //update attacker's targetboard

					player_[defender].sendcomms("Report In: Attack at " + hitbox.coordinate + " - Boat Explosion Registered");
					player_[defender].updateboard_state(SHIPBOARD, hitbox.coordinate, DAMAGED_BYTORPEDO); //update defender's shipboard
				}
				
				//register damage takem
				if(registerdamage(defender, hitbox.boatidx, hitbox.coordinate)) {
					player_[attacker].sendcomms("Important Update: Target " + hitbox.coordinate + ", Enemy " + player_[defender].boat(hitbox.boatidx).boat.name + " Sunk");
					player_[defender].sendcomms("Important Report: Attack at " + hitbox.coordinate + " Sinks " + player_[defender].boat(hitbox.boatidx).boat.name);
				} 

				break;
			case status::DAMAGED_BYMINE:
				player_[attacker].sendcomms("No Contact: Target " + hitbox.coordinate + " - Previous Target, Already Mine Damaged");
				player_[defender].sendcomms("Report In: Attack at " + hitbox.coordinate + " - Previous Target, Already Mine Damaged");
				break;
			case status::DAMAGED_BYTORPEDO: 
				player_[attacker].sendcomms("No Contact: Target " + hitbox.coordinate + " - Previous Target, Already Torpedo Damaged");
				player_[defender].sendcomms("Report In: Attack at " + hitbox.coordinate + " - Previous Target, Already Torpedo Damaged");
				break;
			case status::DAMAGED_BYMINETORPEDO: 
				player_[attacker].sendcomms("No Contact: Target " + hitbox.coordinate + " - Previous Target, Already Mine & Torpedo Damaged");
				player_[defender].sendcomms("Report In: Attack at " + hitbox.coordinate + " - Previous Target, Already Mine & Torpedo Damaged");
				break;
			default:
				std::cout << "\nUnexpected state at target: " << hitbox.coordinate << " - State: " << hitbox.originalstate;
				break;
		} 
	} else {
		std::cout << "\n Invalid coordinate" << hitbox.coordinate;
	}
}

bool gameplay::aiplayer() {
	//returns true if the current player is an ai 
	if(humanplayers_ == 0) {
		return true;
	} else if(currentplayer_+1 <= humanplayers_) {
		return false;
	}
	return true;
}

bool gameplay::validboatidx(int boatidx) {
	if(boatidx >=0 && boatidx < config->boatsregistered()) {
		return true;
	}
	return false;
}

bool gameplay::registerdamage(int playerid, int boatidx, std::string coordinate) {
	//register damage to boat
	return player_[playerid].registerdamaage(boatidx, coordinate); //register damage to boat
}

void gameplay::deploy(udt_command command) { //deploy a specific boat to a specific coordinate and orientation
	if(command.valid) { 
		player_[currentplayer_].deployboat(command.boatidx, command.coordinate, command.direction);
	} 
}
void gameplay::deploy(int boatidx) { //deploy a specific boat to an auto-placed coordinate and orientation
	if(validboatidx(boatidx)) {
		player_[currentplayer_].deployboat(boatidx);
	} 
}
void gameplay::deploy(boatstate state) { //deploy all boat with state to an auto-placed coordinate and orientation
	for (int boatidx = 0; boatidx < config->boatsregistered(); boatidx++) {
		if(player_[currentplayer_].boat(boatidx).state == state || state == boatstate::OVERRIDE) {
			player_[currentplayer_].deployboat(boatidx);
		}
	}
}

void gameplay::recall(int boatidx) { //recalls boats
	if(validboatidx(boatidx)) {
		player_[currentplayer_].recallboat(boatidx); //recall a specific boat
	} else if(boatidx == COM_NOVALUE) {
		for (int boatidx = 0; boatidx < config->boatsregistered(); boatidx++) {
			player_[currentplayer_].recallboat(boatidx); //recall all boats
		}
	}
}

std::string gameplay::player_description() {
	if(aiplayer()) {
		return "AI Player (Player #" + std::to_string(currentplayer_+1) + ")";
	} else {
		return "Human Player (Player #" + std::to_string(currentplayer_+1) + ")";
	}
}

void gameplay::rendersetupboard(bool showmines) {
	player_[currentplayer_].renderboard(board_type::SHIPBOARD, showmines);
	player_[currentplayer_].fleetstatus();
	com_output("\n"); com_storecursorposition();	
}

void gameplay::renderturnboards(bool showmines) {
	int messages = 0;
	player_[currentplayer_].renderboard(board_type::SHIPBOARD, showmines); //show the player's boards
	player_[currentplayer_].fleetstatus(); //show the player's fleet information
	player_[currentplayer_].renderboard(board_type::TARGETBOARD);
	damagestatus(defenderid()); //show a summary of the damage taken by the defender

	com_output("\n\n\n"); com_storecursorposition(); //save the current location
	if(!aiplayer()) { //no point rendering comm's to the ai player
		messages = player_[currentplayer_].reviewcomms(); //show the player's communications
		if(messages > 0) { //only if messages were reviewed
			com_ui_continuepause("\n\n" + player_description() + " Press any key to clear comm's and continue.. ", FG_YELLOW);
			com_restorecursorposition(); com_clearconsole(CLEARTOBOTTOM); //remove comms and continue
		}
	}
}

void gameplay::nextplayer() {
	currentplayer_++; //setup next player
	if(currentplayer_ >= 2) { //dont forget, player 1 = 0, player 2 = 1
		currentplayer_ = 0;
	}
	com_clearconsole(console_commands::CLEAR); //clear screen
	com_output("Switching to next player, please get ready.... ", FG_YELLOW);
	busy pause;
	pause.show(2500);	
}


udt_torpedostatus gameplay::torpedoesavailable() {
	udt_torpedostatus state = { 1, 1 };
	if(salvo_) {
		state.maxavailable = player_[currentplayer_].boatsavailable();
	}	
	state.currentavailability = state.maxavailable;
	return state;
}

void gameplay::start()  {
	std::string playerdescription;
	bool setup_loop = true;
	bool turns_loop = false;
	bool gameover = false;
	bool showminesinsetup = false; //allow mines to be seen during setup
	bool showminesinturn = true; //allow mines to be seen during turns

	//setup boards
	udt_command command; //struct udt_command { bool valid; int boatidx; std::string coordinate; orientation direction; };
	int option = -1;
	int boatidx = -1;
	
	while(setup_loop) {
		com_clearconsole(console_commands::CLEAR); //clear the console
		com_output("AdaShip " + gametype() + ": " + player_description() + "\n", MENU_COLOUR);

		player_[currentplayer_].deploymines(mines_, true); //deploy mines; remove and reset if already exists

		if(aiplayer()) { //ai type player board setup
			recall(); //reset the board; could be commented out to reset board each iteration
			deploy(boatstate::WAITING); //auto place all "WAITING" boats; could use "OVERRIDE" overall all
			rendersetupboard(showminesinsetup);
			option = 6; //press a key to continue
		} else {
			//human type player board set
			rendersetupboard(showminesinsetup);
			option = menu_boardsetup(currentplayer_, player_[currentplayer_].fleetdeployed());
		}

		switch(option) {
			case 1: //manually place a boat
				command = menu_placeboatcommand(config->boatsregistered());
				deploy(command);
				break;
			case 2: //auto-place a boat
				 boatidx = menu_autoplaceboatcommand(config->boatsregistered());
				 deploy(boatidx);
				 break;
			case 3: //auto-place all boats waiting for orders
				deploy(boatstate::WAITING);
				break;
			case 4: //auto-place all boats
				deploy(boatstate::OVERRIDE);
				break;
			case 5: //reset board
				recall();
				break;
			case 6: //contiue to next player
				if(player_[currentplayer_].fleetdeployed()) { //check whether fleet is deployed
					com_restorecursorposition(); //restore to last cursor position 
					com_clearconsole(console_commands::CLEARTOBOTTOM); //clear off menu (essentially where last stored position was)
					com_ui_continuepause("\n" + player_description() + " setup complete, press any key to switch to next player.. ", FG_BRIGHT_YELLOW);

					nextplayer();
					if(player_[0].fleetdeployed() && player_[1].fleetdeployed()) {
						setup_loop = false; //disable setup 
						turns_loop = true; //enable turns
					}
				}
				break;
			case 0:
				setup_loop = false; //terminate this loop
				config->type(gametype::MENU); //update state to return to game menu
				break;
		}
	}

	//player turns
	std::vector <utd_torpedotarget> targets;
	udt_torpedostatus torpedostatusp; //used to control the number of launches per round
	torpedostatusp = torpedoesavailable();

	while(turns_loop) {
		udt_targethitbox hitbox;
		
		com_clearconsole(console_commands::CLEAR); //clear the console
		com_output("AdaShip " + gametype() + ": " + player_description() + "\n", MENU_COLOUR);
		
		renderturnboards(showminesinturn);

		if(winner() >= 0) {
			turns_loop = false;
			gameover = true;
		};

		if(aiplayer()) { //ai type player board setup
			if(torpedostatusp.currentavailability > 0) {
				option = 2;
			} else {
				option = 3;
			}
		} else {
			option = menu_playerturn(currentplayer_, torpedostatusp.maxavailable, torpedostatusp.currentavailability);
		}

		switch(option) {
			case 1: //manual fire torpedo 
				targets = menu_torpedotargetcoordinates(); //get array vector of coordinates
				for(int i = 0; i < targets.size() && torpedostatusp.currentavailability > 0; i++) { //loop specified targets 
					hitbox = player_[defenderid()].targethitbox(targets[i].coordinate); //get the defenders details
					explode(hitbox);
					torpedostatusp.currentavailability--;
				}
			 	break;

			case 2: //auto fire torpedo
				hitbox = autotarget();
				explode(hitbox);
				torpedostatusp.currentavailability--;
				break;

			case 3: //continue to next player - end turn
				if(torpedostatusp.currentavailability == 0) { //check whether all torpedoes are launched
					if(currentplayer_ == winner()) {
						com_ui_continuepause("\nGreat job, " + player_description() + " you've won, press any key continue.. ", FG_BRIGHT_YELLOW);
					} 
					com_restorecursorposition(); //restore to last cursor position
					com_clearconsole(console_commands::CLEARTOBOTTOM); //clear off menu (essentially where last stored position was)
				
					if(aiplayer()) {
						com_ui_continuepause("\n" + player_description() + " turn completed, press any key switch to next player.. ", FG_BRIGHT_YELLOW);
					}
					nextplayer();
					torpedostatusp = torpedoesavailable(); //reset torpedo status
				}
				break;

			case 0: //terminate this game
				turns_loop = false; //terminate this loop
				config->type(gametype::MENU); //update state to return to game menu
				break;
		}
	}

	while(gameover) {
		//winning condition -
		com_ui_continuepause("\n\n" + player_description() + " Game over, press any key to contiue.. ", FG_BRIGHT_GREEN);
		config->type(gametype::MENU); //update state to return to game menu
		gameover = false;
	}
}
