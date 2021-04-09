#pragma once //ensure header only included once during compilation


class fleet {

	public:
		fleet();
		udt_fleet boat(int boatidx);
		bool boatdamage(int boatidx, std::string coordinate);
		int boatsregistered();
		void boatupdatecoordinates(int boatidx, std::vector <udt_boardref> boardref, boatstate state);
		int boatsavailable();
		void fleetstatus();

	protected:


	private:
		configuration* config;
		std::vector <udt_fleet> fleetboat_;
};