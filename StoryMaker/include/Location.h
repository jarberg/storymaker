#pragma once
#include <string>
#include <vector>
class Location {

	std::string name;
	std::vector<std::shared_ptr<Location>> connectedLocations;

public:
	Location() {};
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