#pragma once


template <typename T>
class Singleton {
	friend class Singleton<T>;
protected:
    Singleton() { std::cout << "Singleton Created!\n"; }
    ~Singleton() = default;

public:

    static T& getInstance() {
        static T instance;  // Static local variable ensures single instance per derived class
        return instance;
    }

};