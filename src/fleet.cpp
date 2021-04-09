#include "../headers/adaship.h"


fleet::fleet() { //fleet constructor
	config = configuration::getInstance();
	int health = 0;
	for(int i = 0; i < config->boatsregistered(); i++ ) { //add registered boats to the fleet
		health = config->boat(i).length; //set the health as the lenght of the boat
		fleetboat_.push_back({
			i, //initialise boatidx (boatidx)
			health,
			boatstate::WAITING,
			{  //initialise nested udt struct; e.g., fleetboat[i].boat.name
				config->boat(i).name, 
				config->boat(i).length 
			},
		});
		for(int t = 0; t < config->boat(i).length; t++) { //initialise boat tiles
			fleetboat_[i].tiles.push_back({
				-1,
				-1,
				"-"
			});
		}
	}
}

int fleet::boatsregistered() {
	return fleetboat_.size();
}

udt_fleet fleet::boat(int boatidx) { 
	//return a specific boat instance
	udt_fleet boat_ = { -1, -1, boatstate::INVALID, { "", -1 } };
	if(boatidx >= 0 && boatidx < fleetboat_.size()) {
		boat_ = fleetboat_[boatidx];	
	}
	return boat_;
}

bool fleet::boatdamage(int boatidx, std::string coordinate) {
	for(int i = 0; i < fleetboat_[boatidx].tiles.size(); i++) {
		if(fleetboat_[boatidx].tiles[i].coordinate == coordinate ) {
			fleetboat_[boatidx].tiles[i].coordinate = "--";
			fleetboat_[boatidx].health --;
			if(fleetboat_[boatidx].health <= 0) {
				fleetboat_[boatidx].health = 0;
				fleetboat_[boatidx].state = boatstate::DESTROYED;
				return true; //boat destroyed
			}
		}
	}
	return false;
}

void fleet::boatupdatecoordinates(int boatidx, std::vector <udt_boardref> boardref, boatstate state) {
	//update boat coordinates using the provided vector
	if(boatidx >= 0 && boatidx < fleetboat_.size()) { //check boat idx is valid
		if( boardref.size() == fleetboat_[boatidx].boat.length) {
			fleetboat_[boatidx].state = state;
			for (int i = 0; i < boardref.size(); i++ ){
				fleetboat_[boatidx].tiles[i].col = boardref[i].col;
				fleetboat_[boatidx].tiles[i].row = boardref[i].row;
				fleetboat_[boatidx].tiles[i].coordinate = coords_getcoordinate(boardref[i].col, boardref[i].row, config->boardwidth());
			}
		}
	}
}

int fleet::boatsavailable() {
	int available = 0;
	for (auto& it : fleetboat_) {
		if(it.health > 0) {
			available++;
		}
	}
	return available;
}

void fleet::fleetstatus() {
	std::string colour;
	std::string location;
	int damage = -1;
	std::cout << "\n" << FG_BLUE << 
		std::setw(2) << std::right << "Id" << "  " <<
		std::setw(15) << std::left << "Type" << 
		std::setw(6) << std::left << "Len." <<
		std::setw(6) << std::left << "Dmg." <<
		"Status" << COLOUR_RESET;
	for (auto& it : fleetboat_) {
		colour = FG_WHITE;
		damage = it.boat.length - it.health;
		if(it.state == boatstate::DESTROYED) {
			colour = FG_BRIGHT_BLACK;
		}
		std::cout << "\n" << colour <<
			std::setw(2) << std::right << it.boatidx << "  " <<
			std::setw(15) << std::left << it.boat.name << 
			std::setw(6) << std::left << it.boat.length <<
			std::setw(6) << std::left << damage;
		switch(int(it.state)) {
			case boatstate::WAITING: 
				std::cout << "Awaiting orders"; 
				break;
			case boatstate::DEPLOYED: 
				location = "";
				for (int i = 0; i < it.tiles.size(); i++ ) {
					location += it.tiles[i].coordinate + " ";
				}
				std::cout << "Deployed (" << com_trim(location) << ")";
				break;
			case boatstate::DESTROYED: 
				std::cout << "Destroyed"; 
				break;
		}
	}
	std::cout << "\n\n" << COLOUR_RESET;	
}