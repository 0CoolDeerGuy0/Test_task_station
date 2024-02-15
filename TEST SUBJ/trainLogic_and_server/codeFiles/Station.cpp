#include "Station.h"
#include <list>
#include <iostream>



Section* Station::findPath(Section* headPosition, list<Section*> target)
{
    if (target.back() == headPosition) { return target.back(); } // едем "сквозь" выход

    if (headPosition->neighbors.size() > 1) {

        return newPath(headPosition, target);

    } 
    else {
        if (headPosition->neighbors.front()->occupied() && headPosition->neighbors.size() != 0) {
            return headPosition->neighbors.front();
        }
    }
    return nullptr;
}

Section* Station::newPath(Section* headPosition, list<Section*> target)
{
    for (auto& section : headPosition->neighbors)
    {
        if (section->occupied()) // если секция занята, не важно, ведет ли она к цели, все равно придется ждать ее освобождения
        {
            if (section == target.front()) { return section; }

            auto secToTarget = newPath(section, target);

            if (secToTarget != nullptr) { return section; }
        }
    }
    return nullptr;
}

void Station::addTrain(Train* train) {
    trains.push_back(train);
}

void Station::addSection(string name, bool isEndPoint) {
    Section* newSection = new Section(name, isEndPoint);
    sections[name] = newSection;
}

void Station::addPath(string from, string to) {
    Section* fromSection = sections[from];
    Section* toSection = sections[to];

    if (fromSection && toSection) {
        fromSection->neighbors.push_back(toSection);
    }
}

