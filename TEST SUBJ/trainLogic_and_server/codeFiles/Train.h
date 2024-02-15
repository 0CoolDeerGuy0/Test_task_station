#ifndef TRAIN_H
#define TRAIN_H

#include <string>
#include <iostream>
#include <list>

class Section;
class Station;
class StationEvent;

class Train {

public:   

    Train( int length, Section* startSection, std::list<Section*> preferredPath, int trainID );

    void moveForward(Station* graph);
    void setToStart(Station* graph);
    bool isLeave();
    void showOC(); //для отладки, показывает занятые поездом секции
    int getId();

    // получение секции, на котиорой находится голова поезда
    Section* head();

    // Полученпе списка событий. После получения список должен быть очищен через ClearEvents.
    // Владение содержимым списка переходит к вызвавшему, он должен удалить события после обработки.
    std::list<StationEvent*>& getEvents();

    // Очистка списка событий. НЕ УДАЛЕНИЕ СОБЫТИЙ!
    void clearEvents();

private:

    // занятие секции новой секции. секция под хвостом освобождается если надо
    void occupySection(Section*);

    // длина поезда
    int length;

    // количество движений. для отладки
    int step;

    // признак выезда со станции. если стал true надо удалить поезд
    bool passed;

    // номер поезда что бы красить его на фронте
    int id;

    // желаемый путь проезда
    std::list<Section*> preferredPath;

    // список занятых секций. front = голова поезда, back = хвост
    std::list<Section*> occupiedSections;

    // список событий, произведенных этим поездом.
    std::list<StationEvent*> events;

    void OnSectionEnter(Section* s);
    void OnSectionRelease(Section* s);
};

#endif
