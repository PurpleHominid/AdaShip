#pragma once //ensure header only included once during compilation

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string> //needed for std::stoi
#include <algorithm> //needed for transform
#include <iomanip> //add support for - setw
#include <stdlib.h> //general purpose functions e.g. srand, rand
#include <time.h> //time
#include <regex> //input validaition
#include <ios> //for <streamsize>  
#include <limits> //for numeric_limits

#include <chrono> //used for wait function
#include <thread> //used for wait function

#include <unistd.h> //for blocked, single char input without 'enter'
#include <termios.h> //support for terminal/console control - busy, getch

#include "constants.h" //include support for constant and user data types
#include "colours.h" //include support for colours
#include "regexp_patterns.h" //include support for reg expression patterns
#include "com.h" //include support for common services/functions
#include "configuration.h" //include support for the configuration sington object
#include "fileparser.h" //include support for the file reader
#include "busy.h" //include support for busy timer
#include "coords.h" //include support for the coordinate system
#include "board.h" //include support for the board system
#include "fleet.h" //include support for fleets (boats)
#include "player.h" //include support for players
#include "launcher.h" //include support for game launching
#include "menu.h" //include support for menu
#include "gameplay.h" //include support for gameplay