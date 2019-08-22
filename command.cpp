#include "command.h"
#include "chip.h"
#include <QStringList>
#include <QDebug>

Command::Command() : direction(NO) {
//    position = new QPoint;

}

//Command::Command(QPoint _position, QString& _name, int _time, CommandType _type) :
//    name(_name),
//    position(_position),
//    type(_type),
//    timestamp(_time)
//{

//}

void Command::printCommand() {
    qDebug() << type << " " << timestamp << " (" << position.x() << "," << position.y() << ") ";
}

Command::~Command() {
//    delete position;
}

void Command::setTime(int _time) {
    timestamp = _time;
}

void Command::setType(CommandType _type) {
    type = _type;
}

void Command::setPosition(int x, int y) {
    position.setX(x);
    position.setY(y);
}

void Command::setDirection(CommandDirection dir) {
    direction = dir;
}

int Command::getTime() {
    return timestamp;
}

QPoint Command::getPosition() {
    return position;
}

CommandType Command::getType() {
    return type;
}

CommandDirection Command::getDirection() {
    return direction;
}

QVector<Command> Command::parse_command_set(QString& info) {
    qDebug() << "Fuck0";
    QVector<Command> command;
    // parse the input file
    for (auto line: info.split("\n")) {
        qDebug() << "newline";
        qDebug() << line;
        auto temp = line.split(" ")[0];
        qDebug() << temp;
        auto proto_numbers = line.split(";")[0];
        auto numbers = proto_numbers.split(",");

        if (temp == "Input") {
//            qDebug() << "in input";
            auto _t = numbers[0].toInt();
            auto _x = numbers[1].toInt();
            auto _y = numbers[2].toInt();
            Command input;
            QString number;

            input.setType(INPUT);
            input.setTime(_t);
            input.setPosition(_x, _y);
            input.setDirection(NO);

            command.append(input);
            qDebug() << "ending...";

        } else if (temp == "Mix") {
//            qDebug() << "in mix";
            auto setMove =[](int t, int _x0, int _y0, int _x1, int _y1) {
                Command move;
                move.setType(MOVE);
                move.setTime(t);
                if (_x0 - _x1 == 1 && _y0 - _y1 == 0) { move.setDirection(LEFT);}
                else if (_x0 - _x1 == -1 && _y0 - _y1 == 0) { move.setDirection(RIGHT);}
                else if (_x0 - _x1 == 0 && _y0 - _y1 == 1) { move.setDirection(UP);}
                else if (_x0 - _x1 == 1 && _y0 - _y1 == 0) { move.setDirection(DOWN);}
                else move.setDirection(NO);

                move.setPosition(_x0, _y0);
                return move;
            };

            auto t = numbers[0].toInt();
            int cnt = 0;
            for (int i = 1; i < numbers.size() - 2; i += 2) {
                auto x0 = numbers[i].toInt();
                auto y0 = numbers[i + 1].toInt();
                auto x1 = numbers[i + 2].toInt();
                auto y1 = numbers[i + 3].toInt();
                command.append(setMove(t + cnt, x0, y0, x1, y1));
            }

        } else if (temp == "Move") {
//            qDebug() << "in move";
            auto t = numbers[0].toInt();
            auto x0 = numbers[1].toInt();
            auto y0 = numbers[2].toInt();
            auto x1 = numbers[3].toInt();
            auto y1 = numbers[4].toInt();

            auto setMove =[](int t, int _x0, int _y0, int _x1, int _y1) {
                Command move;
                move.setType(MOVE);
                move.setTime(t);
                if (_x0 - _x1 == 1 && _y0 - _y1 == 0) { move.setDirection(LEFT);}
                else if (_x0 - _x1 == -1 && _y0 - _y1 == 0) { move.setDirection(RIGHT);}
                else if (_x0 - _x1 == 0 && _y0 - _y1 == 1) { move.setDirection(UP);}
                else if (_x0 - _x1 == 1 && _y0 - _y1 == 0) { move.setDirection(DOWN);}
                else move.setDirection(NO);

                move.setPosition(_x0, _y0);
                return move;
            };
            command.append(setMove(t, x0, y0, x1, y1));

        } else if (temp == "Merge") {
//            qDebug() << "merge";
            Command merge0, merge1;
            merge0.setType(MERGE_0);
            merge1.setType(MERGE_1);

            auto t = numbers[0].toInt();
            auto x0 = numbers[1].toInt();
            auto y0 = numbers[2].toInt();
            auto x1 = numbers[3].toInt();
            auto y1 = numbers[4].toInt();

            merge0.setTime(t);

            merge1.setTime(t + 1);

            if (x0 - x1 == 2 && y0 - y1 == 0) {
                merge0.setPosition(x0, y0);
                merge0.setDirection(LEFT);
                merge1.setPosition(x0 - 1, y0);
                merge1.setDirection(NO);
            } else if (x0 - x1 == -2 && y0 - y1 == 0) {
                merge0.setPosition(x1, y1);
                merge0.setDirection(LEFT);
                merge1.setPosition(x1 - 1, y1);
                merge1.setDirection(NO);
            } else if (x0 - x1 == 0 && y0 - y1 == 2) {
                merge0.setPosition(x0, y0);
                merge0.setDirection(UP);
                merge1.setPosition(x0, y0 - 1);
                merge1.setDirection(NO);
            }
            else if (x0 - x1 == 0 && y0 - y1 == -2) {
                merge0.setPosition(x1, y1);
                merge0.setDirection(UP);
                merge1.setPosition(x1, y1 + 1);
                merge1.setDirection(NO);
            }
            command.append(merge0);
            command.append(merge1);

        } else if (temp == "Split") {
            Command split0, split1;
            split0.setType(SPLIT_0);
            split1.setType(SPLIT_1);

            auto t = numbers[0].toInt();
            auto x0 = numbers[1].toInt();
            auto y0 = numbers[2].toInt();
            auto x1 = numbers[3].toInt();
            auto y1 = numbers[4].toInt();
            auto x2 = numbers[5].toInt();
            auto y2 = numbers[6].toInt();

            split0.setPosition(x0, y0);
            split1.setPosition(x0, y0);
            split0.setTime(t);
            split1.setTime(t + 1);

            if (x0 - x1 == 1 && y0 - y1 == 0) { split0.setDirection(LEFT); split1.setDirection(LEFT);}
            else if (x0 - x1 == -1 && y0 - y1 == 0) { split0.setDirection(RIGHT); split1.setDirection(LEFT);}
            else if (x0 - x1 == 0 && y0 - y1 == 1) { split0.setDirection(UP); split1.setDirection(LEFT);}
            else if (x0 - x1 == 0 && y0 - y1 == -1) { split0.setDirection(DOWN); split1.setDirection(LEFT);}
            command.append(split0);
            command.append(split1);

        } else if (temp == "Output") {
            auto _t = numbers[0].toInt();
            auto _x = numbers[1].toInt();
            auto _y = numbers[2].toInt();
            Command output;
            QString number;

            output.setType(INPUT);
            output.setTime(_t);
            output.setPosition(_x, _y);
            output.setDirection(NO);
            qDebug() << "Fuck6";

            command.append(output);
        }
    }
    qDebug() << "parsing down";
    return command;
}

bool operator< (const Command& a, const Command& b) {
    return a.timestamp < b.timestamp;
}
