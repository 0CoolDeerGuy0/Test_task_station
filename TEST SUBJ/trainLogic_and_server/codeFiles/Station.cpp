#include "Station.h"
#include <list>



Section* Station::FindPath(Section* headPosition, list<Section*> target)
{
    if (target.size() == 1) return target.back(); // едем "сквозь" выход

    for (auto& section : headPosition->neighbors)
    {
        if (!section->occupied()) // если секция занята, не важно, ведет ли она к цели, все равно придется ждать ее освобождения
        {
            if (section == target.back()) return section;
            auto secToTarget = FindPath(section, target);
            if (secToTarget != nullptr) return section;
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

