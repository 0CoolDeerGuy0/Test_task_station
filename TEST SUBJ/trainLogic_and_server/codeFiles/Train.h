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

    void Here() const;
    void MoveForward(Station* graph);
    void setToStart(Station* graph);
    void getOccupied();

    // получение секции, на котиорой находится голова поезда
    Section* Head();

    // Полученпе списка событий. После получения список должен быть очищен через ClearEvents.
    // Владение содержимым списка переходит к вызвавшему, он должен удалить события после обработки.
    std::list<StationEvent*>& GetEvents();

    // Очистка списка событий. НЕ УДАЛЕНИЕ СОБЫТИЙ!
    void ClearEvents();

private:

    // занятие секции новой секции. секция под хвостом освобождается если надо
    void OccupySection(Section*);

    // длина поезда
    int m_length;

    // количество движений. для отладки
    int m_step;

    // признак выезда со станции. если стал true надо удалить поезд
    bool m_passed;

    // номер поезда что бы красить его на фронте
    int m_id;

    // желаемый путь проезда
    std::list<Section*> m_preferredPath;

    // список занятых секций. front = голова поезда, back = хвост
    std::list<Section*> occupiedSections;

    // список событий, произведенных этим поездом.
    std::list<StationEvent*> m_Events;

    void OnSectionEnter(Section* s);
    void OnSectionRelease(Section* s);
};

#endif
