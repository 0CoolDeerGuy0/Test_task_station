#pragma warning(push, 0)
//#include <SimpleAmqpClient/SimpleAmqpClient.h>
#pragma warning(pop)

#include <iostream>
#include "Train.h"
#include "Station.h"
#include <random>
#include <thread>
#include <string>
#include <list>
#include <nlohmann/json.hpp>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
using json = nlohmann::json;

// Блок отправки сообщений 
constexpr auto QUEUE_NAME = "hellothere";
void sendData(json data) {
    try
    {
        auto channel = AmqpClient::Channel::Create("localhost", 5672, "guest", "guest");
        channel->DeclareQueue(QUEUE_NAME, false, true, false, false);
        auto message = AmqpClient::BasicMessage::Create(data.dump());
        channel->BasicPublish("", QUEUE_NAME, message);
        clog << "[x] Message Sent" << endl;
    }
    catch (const exception& error)
    {
        cerr << error.what() << endl;
    }
}

void trainSpawn(Station* station, int id) {
    if (!station->sections["LINE_A_START"]->occupied()) {
        cout << "cant enter, enter is occupied" << endl;
    }
    else {

        list<Section*>possiblePaths = { station->sections["SECTION_A1"], station->sections["SECTION_AA_START"], station->sections["SECTION_A6"], station->sections["SECTION_AB_START"], };
        list<Section*>path;

        // Определение маршрута поезда
        int pathNum = rand() % 2;
        path.push_back(*std::next(possiblePaths.begin(), pathNum));
        pathNum = (rand() % 2) + 2;
        path.push_back(*std::next(possiblePaths.begin(), pathNum));
        path.push_back(station->sections["LINE_A_END"]);

        // Определение длины поезда, а так же создание объекта
        int len = 1 + rand() % 4;
        Train* newTrain = new Train(len, station->sections["LINE_A_START"], path, id);

        station->addTrain(newTrain);
        newTrain->setToStart(station);
        cout << "New train with ID: " << newTrain->getId() << " and len of: " << len << " is ready" << endl;
    }
}

void trainMove(Train* train, Station* station) {
    cout << "Train with ID: " << train->getId() << " trying to move" << endl;
    train->moveForward(station);
}

//void trainSpawn(Station* station, int id, int& globid) {
//    ariveTrain(station, id);
//}

int main()
{       
        int globalTrainId = 1;
        Station station;
        list<Train*> toDelete;
        json dataToSend;
        bool sendFlag = 0;

        // Добавляем вершины
        station.addSection("LINE_A_START", false);
        station.addSection("SECTION_A5", false);
        station.addSection("SECTION_A1", false);
        station.addSection("SECTION_A2", false);
        station.addSection("SECTION_A3", false);
        station.addSection("SECTION_A4", false);
        station.addSection("SECTION_A6", false);
        station.addSection("SECTION_A7", false);
        station.addSection("SECTION_A8", false);
        station.addSection("SECTION_A9", false);
        station.addSection("LINE_A_END", true);
        station.addSection("SECTION_AA_START", false);
        station.addSection("SECTION_AA1", false);
        station.addSection("SECTION_AA2", false);
        station.addSection("SECTION_AA_END", false);
        station.addSection("SECTION_AB_START", false);
        station.addSection("SECTION_AB1", false);
        station.addSection("SECTION_AB2",false);
        station.addSection("SECTION_AB_END", false);

        // Добавляем ребра
        station.addPath("LINE_A_START", "SECTION_A1");
        station.addPath("SECTION_A1", "SECTION_A2");
        station.addPath("SECTION_A2", "SECTION_A3");
        station.addPath("SECTION_A3", "SECTION_A4");
        station.addPath("SECTION_A4", "SECTION_A5");
        station.addPath("SECTION_A5", "SECTION_A6");
        station.addPath("SECTION_A6", "SECTION_A7");
        station.addPath("SECTION_A7", "SECTION_A8");
        station.addPath("SECTION_A8", "SECTION_A9");
        station.addPath("SECTION_A9", "LINE_A_END");
        station.addPath("SECTION_AA_START", "SECTION_AA1");
        station.addPath("SECTION_AA1", "SECTION_AA2");
        station.addPath("SECTION_AA2", "SECTION_AA_END");
        station.addPath("SECTION_AA_END", "SECTION_A5");
        station.addPath("LINE_A_START", "SECTION_AA_START");
        station.addPath("SECTION_A5", "SECTION_AB_START");
        station.addPath("SECTION_AB_START", "SECTION_AB1");
        station.addPath("SECTION_AB1", "SECTION_AB2");
        station.addPath("SECTION_AB2", "SECTION_AB_END");
        station.addPath("SECTION_AB_END", "LINE_A_END");


        // Отладочный блок, создающий 1 поезд и пускающий его до удаления
        /*spawnTrain(&station, globalTrainId);

        station.trains[0]->setToStart(&station);
        cout << "neadposition: " << station.trains[0]->Head()->name << endl;

        int s = 0;
        for (Train* train : station.trains) {
            while (!train->isLeave()) {
                cout << train << " train what try to move" << endl;
                train->MoveForward(&station);
                s++;
            }

            if (train->isLeave()) {
                toDelete.push_back(train);
                station.trains.erase(station.trains.begin());
            }
        }

        for (Train* train : toDelete) {
            cout << "This train is going to be deleted: " << train << endl;
            delete train;
        }

        cout << station.trains.size() << " trains on station" << endl;*/
        
        volatile bool stop = false;
        bool stopSign = false;
        auto spawnThread = new std::thread(
            [&]()
            {
                std::cout << "spawnThread started!\n";
                while (!stop)
                {
                    station.mut.lock();

                    trainSpawn(&station, globalTrainId);
                    globalTrainId++;
                    if (globalTrainId >= 256) {
                        globalTrainId = 1;
                    }
                    station.mut.unlock();

                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

                }
                std::cout << "spawnThread stoped!\n";
            }
        );

        auto moveThread = new std::thread(
            [&]()
            {
                std::cout << "moveThread started!\n";
                while (!stop)
                {
                    station.mut.lock();
                    for (Train* train : station.trains) {
                        trainMove(train, &station);

                        if (train->getEvents().size() != 0) {
                            sendFlag = 1;
                            for (auto& eve : train->getEvents()) {
                                nlohmann::json ev = {
                                    {"type", eve->type},
                                    {"section", eve->section->name},
                                    {"trainId", eve->train->getId()}
                                };
                                dataToSend["events"].push_back(ev);
                                
                            }
                            train->clearEvents();
                        }

                        if (train->isLeave()) {
                            toDelete.push_back(train);
                        }
                    }

                    if (sendFlag) {
                        sendData(dataToSend);
                        dataToSend.clear();
                        sendFlag = 0;
                    }

                    for (auto& train : toDelete) {
                        cout << " This train is going to be deleted: " << train << endl;
                        station.trains.remove(train);
                        delete train;
                    }
                    toDelete.clear();
                    station.mut.unlock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
                std::cout << "moveThread stoped!\n";
            }
        ); 

        spawnThread->join();
        moveThread->join();

    return 0;
}
