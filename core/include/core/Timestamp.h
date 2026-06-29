#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <chrono>

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

// UTC timestamp
struct Timestamp {
    Timestamp() : _timePoint() {}
    explicit Timestamp(TimePoint timePoint) : _timePoint(timePoint) {}
    static Timestamp FromSeconds(int64_t seconds) {
        return Timestamp(TimePoint(std::chrono::seconds(seconds)));
    }
    static Timestamp FromMilliseconds(int64_t milliseconds) {
        return Timestamp(TimePoint(std::chrono::milliseconds(milliseconds)));
    }
    static Timestamp Now() {
        return Timestamp(TimePoint(std::chrono::system_clock::now()));
    }
    
    int64_t Seconds() const { return std::chrono::duration_cast<std::chrono::seconds>(_timePoint.time_since_epoch()).count(); }
    int64_t Milliseconds() const { return std::chrono::duration_cast<std::chrono::milliseconds>(_timePoint.time_since_epoch()).count(); }
    bool isNull() const { return _timePoint.time_since_epoch().count() == 0; }

    bool equal(Timestamp &other){
        return _timePoint == other._timePoint;
    }
    bool less(Timestamp &other){
        return _timePoint < other._timePoint;
    }
    bool greater(Timestamp &other){
        return _timePoint > other._timePoint;
    }

    bool operator==(Timestamp &other){
        return equal(other);
    }
    bool operator!=(Timestamp &other){
        return !equal(other);
    }
    bool operator<(Timestamp &other){
        return less(other);
    }
    bool operator>(Timestamp &other){
        return greater(other);
    }
private:
    std::chrono::time_point<std::chrono::system_clock> _timePoint;
};

#endif