#pragma once //ensure header only included once during compilation

class gameplay {

	public:
		gameplay(int humanplayers, bool salvo, int mines);
		std::string gametype();
		void damagestatus(int playerid);
		void start();
		int winner();

		void roles(int &attacker, int &defender);
		int defenderid();
		udt_targethitbox autotarget();
		void explode(udt_targethitbox hitbox);

		bool aiplayer();
		bool validboatidx(int boatidx);
		bool registerdamage(int playerid, int boatidx, std::string coordinate);
		
		void deploy(udt_command command);
		void deploy(int boatidx);
		void deploy(boatstate state = boatstate::WAITING);

		void recall(int boatidx = COM_NOVALUE);
		std::string player_description();

		void rendersetupboard(bool showmines = false);
		void renderturnboards(bool showmines = false);

		udt_torpedostatus torpedoesavailable();
		void nextplayer();

	protected:

	private:
		configuration* config;
		player player_[2]; //register players
		int humanplayers_;
		bool salvo_;
		int mines_;
		int currentplayer_ = 0;
};