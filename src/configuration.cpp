#include "../headers/adaship.h"


configuration* configuration::inst_= nullptr; //initialise the default state of the configuration object (singleton)
//static methods should generally be defined outside the class - this is because they are are not part of the individual class objects, 
//they are treated similarly to global variables and get initialized when the program starts meaning they should be explicitly defined 
//outside of the class (in the global scope)
configuration *configuration::getInstance(const int &boardwidth, const int &boardheight) {
	//this is a safer way to create an instance rather than instance = new configuration to help avoid 
	//issue related to potential multithreaded calls wanting access at the same time
	if(inst_== nullptr){
		inst_ = new configuration(boardwidth, boardheight); //no previous instance stored, create and store the original
	}
	return inst_; //return the stored instance
}
configuration configuration::*getInstance(const int &boardwidth, const int &boardheight);



int configuration::boardwidth() const {
	return boardwidth_;
}

int configuration::boardheight() const {
	return boardheight_;
}

void configuration::setboard(int width, int height) {
	boardwidth_ = width;
	boardheight_ = height;	
}

void configuration::type(gametype type) {
	type_ = type;
}

gametype configuration::type() {
	return type_;
}

void configuration::state(gamestate state) {
	state_ = state;
}

gamestate configuration::state() {
	return state_;
}

void configuration::registerboat(std::string name, int length) {
	boats_.push_back({
		name,
		length
	});
}

void configuration::listboats() {
	for (auto& it : boats_) {
		std::cout << "\nBoat: " << it.name << ", " << it.length;
	}
}

int configuration::boatsregistered() {
	return boats_.size();
}

udt_boat configuration::boat(int idx) {
	udt_boat boat = { "", COM_NOVALUE }; //set default return
	if(idx >= 0 && idx < boats_.size()) { //check range
		boat.name = boats_[idx].name; //copy to return variable
		boat.length = boats_[idx].length;
	}
	return boat;
}

configuration::configuration(const int boardwidth, const int boardheight): 
	// directly initialise const member variable
	boardwidth_ (boardwidth), //initialise the variable 'configuration::boardwidth_' with the value passed as 'boardwidth 
	boardheight_ (boardheight) //initialise the variable 'configuration::boardheight_' with the value passes as 'boardheight'
	//the above code lines use an initialisation list, because 'const' must be initialised (once)
	//and is not owned by a specific instance we can use the method above to initialise the values
	//search 'initialisation list c++' for more info
	{
		//possible constructor code
		type_ = gametype::MENU; //set initial game type
		state_ = gamestate::PENDING; //set the initial game state
		com_randseed(); //reseed the random generator
}