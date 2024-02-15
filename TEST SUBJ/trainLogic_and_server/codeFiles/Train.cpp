#include "Station.h"
#include <string>
#include <iostream>

#include "Train.h"

Train::Train( int length, Section* startSection, list<Section*> preferredPath, int trainID ) :
    length(length),
    step(0),
    preferredPath(preferredPath),
    passed(false),
    id(trainID)
{

}

void Train::showOC() {
    for (Section* section : this->occupiedSections) {
        cout << section->name << endl;
    }
}

int Train::getId() {
    return this->id;
}

void Train::moveForward(Station* graph)
{    
    if (passed)
    {
        std::cout << "I'm memory leak. You should to delete passed trains!\n";
        return;
    }

    Section* nextSec = graph->findPath(head(), this->preferredPath);
    if (nextSec == nullptr)
    {
        std::cout << "no way at now\n";
        return;
    }

    if (preferredPath.size() != 1) {
        if (this->head() == preferredPath.front()) {
            preferredPath.pop_front(); // дошли до желаемой точки, теперь стремимся к следующей
        }
    }
    else {
        nextSec = graph->findPath(head(), this->preferredPath);
    }
    //else {
        //OccupySection(nextSec);
    //}

    occupySection(nextSec);

};

Section* Train::head()
{
    return occupiedSections.front();
}

std::list<StationEvent *> &Train::getEvents()
{
    return events;
}

void Train::clearEvents()
{
    events.clear();
}

void Train::setToStart(Station* graph) {
    if (graph->sections["LINE_A_START"]->occupied()) {
        Train::occupiedSections.push_back(graph->sections["LINE_A_START"]);
        graph->sections["LINE_A_START"]->train = this;
        OnSectionEnter(graph->sections["LINE_A_START"]);
    }
    else {
        cout << "sorry, start is occupy(" << endl;
    }
}

bool Train::isLeave() {
    return this->passed;
}

void Train::occupySection(Section* s)
{
    s->train = this;
    step++;
    if (s->isEndPoint) // голова поезда находится или заезжает на выезд
    {
        cout << s->train << " is leaving!!" << endl;

        // если только что заехали на выезд, ставим голову на него. потом она остается на выезде а хвост будет сокращаться
        if (head() != s) occupiedSections.push_front(s);

        // освобождаем заднюю секцию, но никакой следующей не занимаем. голова стоит на секции выезде.
        Section* releasedSection = occupiedSections.back();
        releasedSection->train = nullptr;
        OnSectionRelease(releasedSection);
        occupiedSections.pop_back();

        // занятых секций под поездом нет. он уехал.
        if (occupiedSections.size() == 0)
        {
            passed = true;
            std::cout << "i'm passed. delete me someone?\n";
        }
        return;

    }

    occupiedSections.push_front(s);
    OnSectionEnter(s);

    if (occupiedSections.size() > length) // поезд полностью находится на станции, освобождаем секцию под бывшим хвостом
    {
        Section* releasedSection = occupiedSections.back();
        releasedSection->train = nullptr;
        OnSectionRelease(releasedSection);
        occupiedSections.pop_back();
    }
}

void Train::OnSectionEnter(Section *s)
{
    events.push_back(new StationEvent(this, s, StationEvent::occupySection));
}

void Train::OnSectionRelease(Section *s)
{
    events.push_back(new StationEvent(this, s, StationEvent::releaseSection));
}
