#include "../headers/adaship.h"


player::player() {
	config = configuration::getInstance(); //access the config controller	
}

void player::fleetstatus() {
	fleet_.fleetstatus();
}

int player::boatsavailable() {
	return fleet_.boatsavailable();
}

void player::sendcomms(std::string message) {
	comms_.push_back({
		false,
		message
	});
}

int player::reviewcomms(bool includeall) {
	int i = 0;
	std::string communication = "";
	for (auto& it : comms_) {
		if(!it.archive || includeall) {
			i++;
			if(i == 1) {
				com_output(">>> Urgent Priority - Incoming Comms", FG_CYAN);
			}
			communication = "\n" + std::to_string(i) + ". " + it.message + "";
			com_output(communication, FG_WHITE);
			it.archive = true;
		}
	}
	return i;
}

udt_fleet player::boat(int boatidx) {
	return fleet_.boat(boatidx);
}

void player::deploymines(int mines, bool clearexisting) {
	board_[board_type::SHIPBOARD].deploymines(mines, clearexisting);
}

void player::updateboard_state(board_type type, std::string coordinate, status state) {
	board_[type].settile_state(coordinate, state); 
}

bool player::registerdamaage(int boatidx, std::string coordinate) {
	if(boatidx >= 0 && boatidx < fleet_.boatsregistered()) {
		return fleet_.boatdamage(boatidx, coordinate); //if true, ship destroyed
	}
	return false;
} 

std::string player::autotarget() {
	//returns a random coordinate
	int minlength = 1;
	udt_targetcoordinate target = randomcoordinate(minlength, TARGETBOARD, AVAILABLE); 
	return target.coordinate;
}


udt_targethitbox player::targethitbox(std::string coordinate) {	
	udt_targethitbox hitbox = { "", status::ERROR, false, false, COM_NOVALUE} ; //set defaults

	if( coords_validatecoordinate(coordinate, config->boardwidth(), config->boardheight()) ) { //check is valid coordinate
		hitbox.coordinate = coordinate; //record the coordinate
		hitbox.originalstate = board_[board_type::SHIPBOARD].tile(coordinate).state; //record state 

		if(board_[board_type::SHIPBOARD].tile(coordinate).boatidx >= 0 ) { //boat at location?
			hitbox.hit = true; //record hit
			hitbox.boatidx = board_[board_type::SHIPBOARD].tile(coordinate).boatidx; //record boatidx
		}

		if(board_[board_type::SHIPBOARD].tile(coordinate).mined ) { //mined locations?
			hitbox.mined = board_[board_type::SHIPBOARD].tile(coordinate).mined;
		}
	}
	return hitbox;
}

bool player::fleetdeployed() { //returns true if all boats in fleet are deployed
	bool deployed = true; //set default return state
	for (int i = 0; i < fleet_.boatsregistered(); i++) {
		if(fleet_.boat(i).state != boatstate::DEPLOYED) {
			deployed = false;
		}
	}
	return deployed;
}

void player::recallboat(int boatidx) {
	//recalls a boat allowing it to be redeployed- updates board and boat tiles
	std::vector < int > idx = board_[board_type::SHIPBOARD].collecttiles(boatidx); //collect all board tiles associated with boat
	std::vector < udt_boardref > recallcoords; //setup a vector; reduce passed values
	for (int i = 0; i < idx.size(); i++) { //update each board location 
		board_[board_type::SHIPBOARD].settile_boatidx(idx[i], COM_NOVALUE);
		recallcoords.push_back({ //setup default recalled coordinates 
			-1,
			-1
		});
	}
	//update the boat's internal coordinates and reset status
	fleet_.boatupdatecoordinates(boatidx, recallcoords, boatstate::WAITING);
}

udt_targetcoordinate player::randomcoordinate(int minlength, board_type type, status state) {
	//return a suitable coordinate and orientation based on the min length value
	std::vector < int > available = board_[type].collecttiles(state, minlength); //collect suitable tiles
	int idx = COM_NOVALUE; //set default index
	udt_targetcoordinate target = { "", orientation::HORIZONTAL }; //set default 
	if(available.size() > 0) { //at least 1 coordinate available
		idx = available[com_randrange(0, (available.size() - 1), false)]; //pick a random index from available
		target.coordinate = coords_getcoordinate(idx, config->boardwidth());
		udt_lengths lengths = board_[type].availablelength(idx);
		if(lengths.maxcols >= minlength && lengths.maxrows >= minlength) {
			//pick random orientation
			rand() % 2 == 0 ? target.direction = orientation::HORIZONTAL : target.direction = orientation::VERTICAL;
		} else if (lengths.maxcols >= minlength) {
			target.direction = orientation::HORIZONTAL;
		} else {
			target.direction = orientation::VERTICAL;
		}
	}
	return target;
} 

bool player::deployboat(int boatidx, bool redeployable) { //deploy boat; wrapper random deployment
	udt_fleet deploying = fleet_.boat(boatidx); //get the details of the specified boat
	udt_targetcoordinate target = randomcoordinate(deploying.boat.length, board_type::SHIPBOARD, status::AVAILABLE);
	return deployboat(boatidx, target.coordinate, target.direction, redeployable);
}
bool player::deployboat(int boatidx, std::string coordinate, orientation direction, bool redeployable) {
	std::vector <udt_boardref> deplyedcoordinates;
	bool deployed = false; //set default return status
	
	if(fleet_.boat(boatidx).state == boatstate::WAITING || redeployable) { //ensure boat is ready for deployment
		recallboat(boatidx); //recall boat; if not already deployed should have no effect

		udt_fleet deploying = fleet_.boat(boatidx); //get the details of the specified boat
		udt_lengths lenghts = board_[board_type::SHIPBOARD].availablelength(coordinate); //get coordinate's max lenghts 

		if( (direction == orientation::HORIZONTAL && lenghts.maxcols >= deploying.boat.length) || //check coordinate is suitable
			(direction == orientation::VERTICAL && lenghts.maxrows >= deploying.boat.length)) {
			
			udt_boardref boardref = coords_getboardref(coordinate); //convert coordiante to col and row
			int col = 0, row = 0; //initialise counters
			for (int i = 0; i < deploying.boat.length; i++) { //iterate the boat length
				board_[board_type::SHIPBOARD].settile_boatidx( (boardref.col + col), (boardref.row + row), boatidx, true); //update board			
				deplyedcoordinates.push_back({ //record deployed coordinated
					boardref.col + col,
					boardref.row + row
				});

				if(direction == orientation::HORIZONTAL) { //update board reference depending on direction
					col ++;
				} else {
					row ++;
				}
			}

			//update the specific boat with the recorded coordinates
			fleet_.boatupdatecoordinates(boatidx, deplyedcoordinates, boatstate::DEPLOYED);
			deployed = true;
		}
	} 
	return deployed;
}

void player::renderboard(board_type type, bool rendermines) {
	std::string title = (type == board_type::SHIPBOARD) ? "\nYOUR FLEET DEPLOYMENT" : "\nYOUR TARGETING HISTORY"; 
	com_output(title, FG_BLUE);
	board_[type].render(rendermines);
}