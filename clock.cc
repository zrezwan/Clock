#include <iostream>
#include <string>
#include <ctime>
#include <vector>

using namespace std;

struct timeObj {
    int hours_;
    int minutes_;
    timeObj(int hours, int minutes) {hours_ = hours; minutes_ = minutes;}
    bool operator==(const timeObj & rhs) {
        return (hours_ == rhs.hours_) && (minutes_ == rhs.minutes_); 
    }
};
class Clock {
    protected:
        vector<timeObj*> alarms;
    public :
        virtual ~Clock();
        Clock() {}
        Clock(const Clock & rhs);
        Clock& operator=(const Clock & rhs);
        bool operator==(const Clock & rhs) const;

        virtual int get_hours();
        int get_minutes();
        string get_time();
        void set_alarm(int hours, int minutes);
};
class WorldClock : public  Clock {  
    public :
        int offset_;
        WorldClock(int offset);
        bool operator==(const WorldClock & rhs) const;
        int get_hours();
};

Clock::Clock(const Clock & rhs) {
    for (unsigned i = 0; i < rhs.alarms.size(); i++) {
        timeObj * temp = new timeObj(rhs.alarms[i]->hours_, rhs.alarms[i]->minutes_);
        alarms.push_back(temp);
    }
}

Clock& Clock::operator=(const Clock & rhs) {
    for (int i = 0; i < this->alarms.size(); i++) {
        delete this->alarms[i];
    }
    alarms.clear();
    for (unsigned i = 0; i < rhs.alarms.size(); i++) {
        timeObj * temp = new timeObj(rhs.alarms[i]->hours_, rhs.alarms[i]->minutes_);
        alarms.push_back(temp);
    }
    return *this;
}
bool Clock::operator==(const Clock & rhs) const {
    if (alarms.size() != rhs.alarms.size())
        return false;
    for (unsigned i = 0; i < alarms.size(); i++) {
        if (!(alarms[i]->hours_ == rhs.alarms[i]->hours_ && alarms[i]->minutes_ == rhs.alarms[i]->minutes_))
            return false;
    }
    return true;
}

int Clock::get_hours() {
    time_t current_time = time(0);
    tm* local_time = localtime(&current_time);
    int hours = local_time->tm_hour;
    delete local_time;
    return hours;
}

int Clock::get_minutes() {
    time_t current_time = time(0);
    tm* local_time = localtime(&current_time);
    int minutes = local_time->tm_min;
    delete local_time;
    return minutes;
}

string Clock::get_time() {
    string ret = "";
    int hours = get_hours();
    int minutes = get_minutes();
    ret += to_string(hours) + ":" + to_string(minutes);
    for (int i = 0; i < alarms.size(); i++) {
        if (alarms[i]->hours_ == hours && alarms[i]->minutes_ == minutes) {
            ret += " \u23F0";
            break;
        }
    }
    return ret;
}

void Clock::set_alarm(int hours, int minutes) {
    timeObj * temp = new timeObj(hours, minutes);
    alarms.push_back(temp);
}

Clock::~Clock() {
    for (int i = 0; i < this->alarms.size(); i++) {
        delete this->alarms[i];
    }
}

WorldClock::WorldClock(int offset) :
    offset_(offset)
{}

bool WorldClock::operator==(const WorldClock & rhs) const {
    return Clock::operator==(rhs) && offset_ == rhs.offset_;
}


int WorldClock::get_hours() {
    int ret = Clock::get_hours() + offset_;
    if (ret < 0) {
        ret = 24 + ret;
    } else if (ret > 23) {
        ret -= 23;
    }
    return ret;
}

int main(int argc, char ** argv) {
    Clock c1;

    c1.set_alarm(4, 5);
    Clock c2;
    cout << (c2 == c1) << endl;

    c2 = c1;
    cout << (c2 == c1) << endl;

    WorldClock w1(3);
    w1.set_alarm(7, 6);
}