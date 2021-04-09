#include "../headers/adaship.h"


busy::busy() {}

void busy::show(int ms) {
	std::thread td = std::thread( [this] { interval(); } );
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	terminate_ = true;
	td.join();
}

void busy::interval() {
	terminate_ = false;
	std::vector < std::string > spinner {
		"▎   ",
		"▍   ",
		"▌   ",
		"▋   ",
		"▉   ",
		"▉▎  ",
		"▉▍  ",
		"▉▌  ",
		"▉▋  ",
		"▉▉  ",
		"▉▉▎ ",
		"▉▉▍ ",
		"▉▉▌ ",
		"▉▉▋ ",
		"▉▉▉ ",
		"▉▉▉▎",
		"▉▉▉▍",
		"▉▉▉▌",
		"▉▉▉▋",
		"▉▉▉▉",
		"▉▉▉▉",
		"▉▉▉▊",
		"▉▉▉▋",
		"▉▉▉▌",
		"▉▉▉▍",
		"▉▉▉▎",
		"▉▉▉ ",
		"▉▉▊ ",
		"▉▉▋ ",
		"▉▉▌ ",
		"▉▉▍ ",
		"▉▉▎ ",
		"▉▉  ",
		"▉▊  ",
		"▉▋  ",
		"▉▌  ",
		"▉▍  ",
		"▉▎  ",
		"▉   ",
		"▊   ",
		"▋   ",
		"▌   ",
		"▍   ",
		"▎   "
	};
	int i = 0;
	std::cout << "\033[40m"; //set the colour
	com_hidecursor();
		
	termios oldt;
	tcgetattr(STDIN_FILENO, &oldt);
	termios newt = oldt;
	newt.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	
	std::cout.flush(); //reset colours and show the cursor
	while(!terminate_) { //keep looping
		std::cout << "\033[s"; //save the cursor position
		std::cout << "\033[1;33m" << spinner[i] << "\033[30m"; std::cout.flush(); //render the animation
		i >= spinner.size()-1 ? i = 0 : i++; //setup next animation
	
		std::this_thread::sleep_for(std::chrono::milliseconds(20)); //animation pause
		std::cout << "\033[" << spinner[i].length() << "D"; std::cout.flush(); //clear the animation
		std::cout << "\033[u"; //restore the cursor to the last saved position
	}

	std::cout << "\033[0m"; std::cout.flush(); //reset colours and show the cursor
	com_hidecursor(false);
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	std::cout.flush(); //reset colours and show the cursor
}