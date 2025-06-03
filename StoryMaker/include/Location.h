#pragma once
#include <string>
#include <vector>

#include <Events.h>

class Location {

	std::string name;
	std::vector<std::shared_ptr<Location>> connectedLocations;

	EventDispatcher locationEnteredEvent;
	EventDispatcher locationExitEvent_var;

public:
	Location() {

	};
	~Location() {};

	Location(std::string name) : name(name) {};
	Location(std::string name, std::vector<std::shared_ptr<Location>> connectedLocations) : name(name), connectedLocations(connectedLocations) {};
	std::string getName() const
	{
		return this->name;
	}
	std::vector<std::shared_ptr<Location>> getConnectedLocations() {
		return this->connectedLocations;
	}
	void exit() {
		this->locationExitEvent_var.Dispatch(std::make_shared<LocationExitEvent>(this->getName()));
		std::cout << "You have exited " << this->name << std::endl;
	}

	void enter() {
		this->locationEnteredEvent.Dispatch(std::make_shared<LocationEnteredEvent>(this->getName()));

		std::cout << "You have entered " << this->name << std::endl;
	}

	void addConnectedLocation(std::shared_ptr<Location> location) {
		this->connectedLocations.push_back(location);
	}
	void printConnectedLocations() {
		std::cout << "Connected locations to " << this->name << ": \n ";
		for (const auto loc : this->getConnectedLocations()) {
			std::cout << loc->getName() << " ";
			
		}
		std::cout << std::endl;
	}

};