#pragma once

#include <iostream>
#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>


// Base Event class
class Event {
public:
    virtual ~Event() = default;
};  

class LocationEnteredEvent : public Event {
public:
    std::string locationName;
    LocationEnteredEvent(std::string name) : locationName(name){};
    
};

class LocationExitEvent : public Event {
public:
    std::string locationName;
    LocationExitEvent(std::string name) : locationName(name) {};
};

class advanceTimeEvent : public Event {
public:
    int time;
    int day;
    int week;
    advanceTimeEvent(int _time, int _day, int _week) : time(_time), day(_day), week(_week) {}

};
// Type alias for Event Callbacks
using EventCallback = std::function<void(std::shared_ptr<Event>)>;

// Event Dispatcher Class
class EventDispatcher {
private:
    std::unordered_map<std::size_t, std::vector<EventCallback>> listeners;

    // Helper function to get a unique event type ID
    template<typename T>
    static std::size_t GetEventTypeID() {
        return typeid(T).hash_code();
    }

public:
    // Subscribe to an event type
    template<typename T>
    void Subscribe(std::function<void(std::shared_ptr<T>)> callback) {
        listeners[GetEventTypeID<T>()].push_back(
            [callback](std::shared_ptr<Event> e) {
                callback(std::static_pointer_cast<T>(e));
            }
        );
    }

    // Dispatch an event
    template<typename T>
    void Dispatch(std::shared_ptr<T> event) {
        auto it = listeners.find(GetEventTypeID<T>());
        if (it != listeners.end()) {
            for (const auto& listener : it->second) {
                listener(event);
            }
        }
    }
};