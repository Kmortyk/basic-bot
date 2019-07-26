#ifndef MAPLOADER_H
#define MAPLOADER_H

#include "robotmap.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QFile>

#define LABEL_MAP   "[map]"
#define LABEL_ROBOT "[robot]"

#define STATE_NONE  0
#define STATE_MAP   2
#define STATE_ROBOT 4

class MapLoader{

public:

    static RobotMap* load(QString fileName) {

        QFile file(fileName);
        unsigned state = STATE_NONE;

        RobotMap* rm = new RobotMap();
        Robot* robot = nullptr;
        std::vector<std::vector<int>> mapVector;

        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "File " << fileName << " cannot be open for loading map.";
            return rm;
        }

        QTextStream in(&file);
        while(!in.atEnd()) {

            QString line = in.readLine();

            if(line == LABEL_MAP)   { state = STATE_MAP; continue; }
            if(line == LABEL_ROBOT) { state = STATE_ROBOT; continue; }
            if(line.isEmpty() || line.isNull()) { continue; }

            switch(state) {
            case STATE_NONE:
                qWarning() << "Non label line at none statement: \"" << line << "\"" << "\n\r";
                break;
            case STATE_MAP: {
                std::vector<int> tiles = splitMapLine(line);
                mapVector.push_back(tiles);
                break;
            }
            case STATE_ROBOT:
                robot = getRobotFromParams(line);
                break;
            }

        }

        rm->setMap(mapVector);
        rm->setRobot(robot);

        file.close();
        return rm;
    }

    static std::vector<int> splitMapLine(QString line) {

        std::vector<int> ints;
        QStringList list = line.split(" ");

        for(QString s: list) {
            if(s.isEmpty()) continue;
            ints.push_back(s.toInt());
        }

        return ints;

    }

    static Robot* getRobotFromParams(QString line) {
        QStringList list = line.split(" ");
        size_t i = list[0].toUInt();
        size_t j = list[1].toUInt();
        return new Robot(i, j);
    }


};

#endif
