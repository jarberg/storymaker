#pragma once


#include <Utils.h>


#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>


enum class Location { NONE, CAFE, PARK, HOME, LIBRARY };
enum class Mood { HAPPY, SAD, ANGRY, NEUTRAL };

class TimeFormat {};

class Character {

public:
    virtual ~Character() = default;
	virtual void setMood(Mood mood) = 0;  // Pure virtual function to set mood
	virtual void setLocation(Location location) = 0;  // Pure virtual function to set location
};

struct Character {
    std::string name;
    Location currentLocation;
    Mood currentMood;
    std::unordered_map<std::string, int> feelingsTowardsOthers;  // Key: character name, Value: relationship level (e.g., 0-100)
    int feelingsTowardsPlayer;  // 0-100, where 100 is a strong positive relationship

    struct ScheduleEntry {
        TimeFormat time;  // "08:00", "12:00", etc.
        Location location;
    };

    std::vector<ScheduleEntry> schedule;

    // Constructor to initialize a character
    Character(std::string characterName) : name(characterName), currentLocation(Location::NONE), currentMood(Mood::NEUTRAL), feelingsTowardsPlayer(50) {}

    virtual void setMood(Mood mood)  {
        currentMood = mood;
    }

    void changeFeelingsTowardsPlayer(int change) {
        feelingsTowardsPlayer = clamp<int>(feelingsTowardsPlayer + change, 0, 100);  // Keep between 0 and 100
    } 

    void changeFeelingsTowardsOther(std::string otherCharacter, int change) {
        feelingsTowardsOthers[otherCharacter] = clamp<int>(feelingsTowardsOthers[otherCharacter] + change, 0, 100);  // Keep between 0 and 100
    }
};
