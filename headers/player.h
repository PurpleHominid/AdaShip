#pragma once //ensure header only included once during compilation


class player {

	public:
		player();

		udt_fleet boat(int boatidx);
		void fleetstatus();
		int boatsavailable();

		void sendcomms(std::string message);
		int reviewcomms(bool includeall = false);

		void recallboat(int boatidx); //recalls a deployed boat
		udt_targetcoordinate randomcoordinate(int minlength = 1, board_type type = board_type::SHIPBOARD, status state = status::AVAILABLE);
		
		void deploymines(int mines, bool clearexisting = true);
		void updateboard_state(board_type type, std::string coordinate, status state);
		bool registerdamaage(int boatidx, std::string coordinate);
		
		std::string autotarget();
		udt_targethitbox targethitbox(std::string coordinate);

		bool fleetdeployed();
		bool deployboat(int boatidx, bool redeployable = true); //random deployment
		bool deployboat(int bidx, std::string coordinate, orientation direction, bool redeployable = true); //deploys boat to a specific coodinate
		
		void renderboard(board_type type, bool rendermines = false);


	protected:


	private:
		configuration* config;
		board board_[2];
		fleet fleet_;
		std::vector < udt_comms > comms_;
};