#ifndef STATION_H
#define STATION_H

#include <string>
#include <map>
#include <vector>
#include <chrono>
#include <list>
#include <mutex>

using namespace std;

class Train;

class Section {
public:
    string name;
    vector<Section*> neighbors;
    Train* train;
    bool occupied(){return train == nullptr;}

    // является ли секция въездом или выездом со станции
    bool isEndPoint;
    Section(string _name, bool isEnd) : name(_name), train(nullptr), isEndPoint(isEnd) {}
};

class StationEvent
{
public:

    enum EventType
    {
        occupySection,
        releaseSection,
        leaveStation,
    };
    EventType type;
    Train* train;
    Section* section;
    chrono::time_point<std::chrono::system_clock> eventTime;

    StationEvent(Train* t, Section* s, EventType et) :
        type(et),
        train(t),
        section(s),
        eventTime(std::chrono::system_clock::now())
    {

    }

};


class Station {
public:
    mutex mut;
    map<string, Section*> sections;
    list<Train*> trains;    
    Section* FindPath(Section* headPosition, list<Section*> target);
    Section* NewPath(Section* headPosition, list<Section*> target);
    void addSection(string name, bool isEndPoint = false);
    void addPath(string from, string to);
    void addTrain(Train* train);

};

#endif 
