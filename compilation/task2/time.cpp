#include "time.hpp"

Time::Time(int hours, int minutes) : hours(hours), minutes(minutes) {}

void Time::reset() {
    hours = 0;
    minutes = 0;
}

Time& Time::operator+=(const Time& right) {
    int totalMinutes = 60 * (hours + right.hours) + minutes + right.minutes;
    hours = totalMinutes / 60;
    minutes = totalMinutes % 60;
    return *this;
}

Time Time::operator+(const Time& right) const {
    Time result = *this;
    result += right;
    return result;
}

std::ostream& operator<<(std::ostream& out, Time t) {
    out << std::setw(2) << std::setfill('0') << t.hours << ":"
        << std::setw(2) << std::setfill('0') << t.minutes;
    return out;
}