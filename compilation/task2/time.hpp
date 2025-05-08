#ifndef TIME_HPP
#define TIME_HPP

#include <iostream>
#include <iomanip>

class Time {
private:
    int hours;
    int minutes;

public:
    Time(int hours, int minutes);
    void reset();
    Time& operator+=(const Time& right);
    Time operator+(const Time& right) const;
    friend std::ostream& operator<<(std::ostream& out, Time t);
};

#endif // TIME_HPP