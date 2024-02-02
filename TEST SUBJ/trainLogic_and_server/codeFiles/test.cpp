#pragma warning(push, 0)
//#include <SimpleAmqpClient/SimpleAmqpClient.h>
#pragma warning(pop)

#include <iostream>
#include "Train.h"
#include "Station.h"
#include <random>
#include <thread>
#include <string>

/*

constexpr auto QUEUE_NAME = "hellothere";

*/

void spawnTrain(Station* station, int id) {
    if (!station->sections["LINE_A_START"]->occupied()) {
        cout << "cant enter, enter is occupied" << endl;
        cout << "if its 1 its realy occupied: " << station->sections["LINE_A_START"]->occupied() << endl;
    }
    else {

        list<Section*>possiblePaths = { station->sections["SECTION_A1"], station->sections["SECTION_AA_START"], station->sections["SECTION_A6"], station->sections["SECTION_AB_END"], };
        list<Section*>path;

        // генерация первого пути
        int pathNum = rand() % 2;
        path.push_front(*std::next(possiblePaths.begin(), pathNum));

        // генерация второго пути
        pathNum = 2 + rand() % 4;
        path.push_front(*std::next(possiblePaths.begin(), pathNum));

        // добавление пути выхода
        path.push_front(station->sections["LINE_A_END"]);

        // создание поезда
        int len = 1 + rand() % 4;
        Train* newTrain = new Train(len, station->sections["LINE_A_START"], path, id);

        cout << newTrain;

        // добавление поезда к станции
        station->addTrain(newTrain);

        cout << "train is ready!" << endl;
    }
}

int main()
{
    int globalTrainId = 1; 
        Station station;

        // Добавляем вершины
        station.addSection("LINE_A_START", true);
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
        station.addSection("SECTION_AB_END", true);

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

        cout << station.sections["LINE_A_START"]->train << endl;
        spawnTrain(&station, globalTrainId);
        cout << station.trains[0] << endl;
        station.trains[0]->setToStart(&station);
        cout << station.sections["LINE_A_START"]->train << endl;

        for (Train* train : station.trains) {
            train->MoveForward(&station);
            train->GetEvents();
            train->ClearEvents();
        }


/*
        volatile bool stop = false;
        bool stopSign = false;
        auto spawnThread = new std::thread(
            [&]()
            {
                std::cout << "spawnThread started!\n";
                while (!stop)
                {
                    spawnTrain(&station, globalTrainId);
                    globalTrainId += 1;
                    std::cout << "trains in station: " << station.trains.size() << endl;
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
                    for (Train* train : station.trains) {
                        train->MoveForward(&station);
                        train->GetEvents();
                        train->ClearEvents();
                        cout << "im trying!" << endl;
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
                std::cout << "moveThread stoped!\n";
            }
        );

        spawnThread->detach();
        moveThread->detach();

        std::cin >> stopSign;

*/


// TODO передача сообщений + формирование json 
/*

    try
    {
        auto channel = AmqpClient::Channel::Create("localhost", 5672, "guest", "guest");
        channel->DeclareQueue(QUEUE_NAME, false, true, false, false);
        auto message = AmqpClient::BasicMessage::Create("Hello World!");
        channel->BasicPublish("", QUEUE_NAME, message);
        clog << " [x] Sent 'Hello World!'" << endl;
    }
    catch (const exception& error)
    {   
        cout << "blyad" << endl;
        cerr << error.what() << endl;
    }
*/
    return 0;
}
