#pragma once //ensure header only included once during compilation


class configuration {
	public:
		static configuration *getInstance(const int &boardwidth = DEFAULT_BOARDWIDTH, const int &boardheight = DEFAULT_BOARDHEIGHT);

		//std::string reference() const;
		int boardwidth() const; //use of const after function essentially means that the method promises not to alter any members of the function 
		int boardheight() const;
		
		void setboard(int width, int height);

		void type(gametype type);
		gametype type();

		void state(gamestate state);
		gamestate state();

		void registerboat(std::string name, int length);
		int boatsregistered();
		void listboats();
		udt_boat boat(int idx);

	protected:
		//std::string reference_;
		int boardwidth_;
		int boardheight_;
		gametype type_;
		gamestate state_;
		std::vector < udt_boat > boats_;
		
	private:
		configuration(const int boardwidth, const int boardheight);
		static configuration* inst_; //a pointer to the 'one' cached (single) instance used for configuration
};