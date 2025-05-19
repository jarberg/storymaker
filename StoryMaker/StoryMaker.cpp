// StoryMaker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <cstdlib>
#include <windows.h>

#include <Events.h>
#include <Timer.h>
#include <Location.h>
#include <graphics.h>


std::shared_ptr<Location> currentLocation;


void parseInput(const std::string& input, std::vector<std::string>& commands) {
	std::istringstream iss(input);
	std::string command, argument;

	iss >> command;         // First word (e.g., "goto")
	std::getline(iss, argument); // The rest (e.g., "castle")

	// Trim leading spaces from argument
	argument.erase(0, argument.find_first_not_of(" "));

	if (command == "goto") {
		if (!argument.empty()) {
			commands = { command, argument };
		}
		else {
			std::cout << "Please specify a location to go to.\n";
		}
	}
	else if (command == "look") {
		if (argument.empty()) {
			commands = { command, argument };
		}
		else {
			std::cout << "Please specify what to look at\n";
		}
	}
	else {
		std::cout << "Unknown command: " << command << std::endl;
	}
}

void handle_input(std::string command, std::vector<std::string> arguments) {
	if (command == "goto") {
		std::string location = arguments[0];
		for (auto loc: currentLocation->getConnectedLocations()) {
			if (loc->getName() == location) {
				currentLocation = loc;
				break;
			}
		}
		std::cout << "Going to " << location << std::endl;
	}
	else if (command == "look") {
		currentLocation->printConnectedLocations();
	}
	else {
		std::cout << "Unknown command: " << command << std::endl;
	}
}

void printFullWidthLine(char ch, int width) {
	for (int i = 0; i < width; ++i) {
		std::cout << ch;
	}
	
}

int bla(std::vector<std::string> commands, std::vector<std::string>& arguments) {
	// Only extract arguments if there is more than one command
	if (commands.size() > 1) {
		arguments = std::vector<std::string>(commands.begin() + 1, commands.end());
	}
	else {
		arguments.clear(); // no arguments
	}

	// Make sure there's at least one command before accessing front()
	if (!commands.empty()) {
		return 0;
	}
	else {
		std::cout << "No command entered.\n";
	}
	return 1;
}


// Function to watch console resize events
void renderGUI(std::pair<int, int> size) {
	std::string title = "StoryMaker";

	int actualW = (size.first - title.length()) / 2;
	int actualH = size.second / 4 * 3;
	
	int renderwindowH = size.second - (1 + actualH);
	
	
	
	//std::cout << "\n[Resize Detected] New width: " << newWidth << std::endl;
	system("cls");

	
	printFullWidthLine('=', actualW);
	std::cout << title;
	printFullWidthLine('=', actualW);
	std::cout << std::endl;

	renderDoorInFrame(size.first, actualH, size.first/3, actualH/2, 1, 5);
	std::cout << std::endl;

	printFullWidthLine('=', size.first);
	std::cout << std::endl;
}

int main()
{
    std::cout << "Hello World! \n";
    Timer test = Timer::getInstance();
	
	// Start the watcher in a separate thread
	std::thread resizeThread(watchConsoleResize, [](std::pair<int, int> size) {
		renderGUI(size);
		});

	auto entrance = std::make_shared<Location>("entrance");
	auto room = std::make_shared<Location>("room");
	auto hallway = std::make_shared<Location>("hallway");
	auto kitchen = std::make_shared<Location>("kitchen");
	auto toilet = std::make_shared<Location>("toilet");

	entrance->addConnectedLocation(hallway);
	hallway->addConnectedLocation(entrance);

	hallway->addConnectedLocation(room);
	room->addConnectedLocation(hallway);

	hallway->addConnectedLocation(kitchen);
	kitchen->addConnectedLocation(hallway);

	hallway->addConnectedLocation(toilet);
	toilet->addConnectedLocation(hallway);

	std::string input;
	currentLocation = entrance;

	std::vector<std::string> commands;

	std::vector<std::string> arguments;
    while (true) {

		renderGUI(getConsoleSize());

		currentLocation->printConnectedLocations();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::getline(std::cin, input);
		parseInput(input, commands);
		// Only extract arguments if there is more than one command
		if (bla(commands, arguments) == 0) {
			handle_input(commands.front(), arguments);
			test.showTime();
			test.advancetime();
		}
    }
	keepRunning = false;
	resizeThread.join();
	return 0;
}	

