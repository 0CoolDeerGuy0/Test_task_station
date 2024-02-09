#include "Station.h"
#include <string>
#include <iostream>

#include "Train.h"

Train::Train( int length, Section* startSection, list<Section*> preferredPath, int trainID ) :
    m_length(length),
    m_step(0),
    m_preferredPath(preferredPath),
    m_passed(false),
    m_id(0)
{

}

void Train::showOC() {
    for (Section* section : this->occupiedSections) {
        cout << section->name << endl;
    }
}

void Train::MoveForward(Station* graph)
{    
    if (m_passed)
    {
        std::cout << "I'm memory leak. You should to delete passed trains!\n";
        return;
    }

    Section* nextSec = graph->FindPath(Head(), this->m_preferredPath);
    if (nextSec == nullptr)
    {
        std::cout << "no way at now\n";
        return;
    }
    if (m_preferredPath.size() > 1) {
        if (nextSec == m_preferredPath.front()) {
            m_preferredPath.pop_front(); // дошли до желаемой точки, теперь стремимся к следующей
        }
    }
    else {
        OccupySection(nextSec);
    }

    OccupySection(nextSec);

};

Section* Train::Head()
{
    return occupiedSections.front();
}

std::list<StationEvent *> &Train::GetEvents()
{
    return m_Events;
}

void Train::ClearEvents()
{
    m_Events.clear();
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
    return this->m_passed;
}

/*void Train::getOccupied() {
    list<Section>::iterator it = this.occupiedSections.begin();
    cout << this->occupiedSections << endl;
}*/

void Train::OccupySection(Section* s)
{
    s->train = this;
    m_step++;
    if (s->isEndPoint) // голова поезда находится или заезжает на выезд
    {
        cout << s->train << " is leaving!!" << endl;

        // если только что заехали на выезд, ставим голову на него. потом она остается на выезде а хвост будет сокращаться
        if (Head() != s) occupiedSections.push_front(s);

        // освобождаем заднюю секцию, но никакой следующей не занимаем. голова стоит на секции выезде.
        Section* releasedSection = occupiedSections.back();
        releasedSection->train = nullptr;
        OnSectionRelease(releasedSection);
        occupiedSections.pop_back();

        // занятых секций под поездом нет. он уехал.
        if (occupiedSections.size() == 0)
        {
            m_passed = true;
            std::cout << "i'm passed. delete me someone?\n";
        }
        return;

    }

    occupiedSections.push_front(s);
    OnSectionEnter(s);

    if (occupiedSections.size() > m_length) // поезд полностью находится на станции, освобождаем секцию под бывшим хвостом
    {
        Section* releasedSection = occupiedSections.back();
        releasedSection->train = nullptr;
        OnSectionRelease(releasedSection);
        occupiedSections.pop_back();
    }
}

void Train::OnSectionEnter(Section *s)
{
    m_Events.push_back(new StationEvent(this, s, StationEvent::occupySection));
}

void Train::OnSectionRelease(Section *s)
{
    m_Events.push_back(new StationEvent(this, s, StationEvent::releaseSection));
}
