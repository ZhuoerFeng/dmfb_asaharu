#include "command.h"
#include "chip.h"
#include <QStringList>
#include <QDebug>
#include "chip.h"
#include "waterdrop.h"
#include <QSound>
#include <QMessageBox>

enum ErrorType {AC, INPUTERROR, OUTPUTERROR, HASNEAR};

Command::Command() : direction(NO) {
}

void Command::printCommand() {
    qDebug() << type << " " << timestamp << " (" << position.x() << "," << position.y() << ") ";
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

int Command::doCommand(Chip* cur_chip) {


    if (this->getType() == INPUT) {
        qDebug() << "input exec";
        QPoint current = this->getPosition();
        qDebug() << "position ready";
        auto isNear=[](QPoint& p, QPoint& tg) {
            if (((p.x() - tg.x() == 1) && (p.y() - tg.y() == 0)) ||
                ((p.x() - tg.x() == -1) && (p.y() - tg.y() == 0)) ||
                ((p.x() - tg.x() == 0) && (p.y() - tg.y() == 1)) ||
                ((p.x() - tg.x() == 0) && (p.y() - tg.y() == -1))) return true;
            return false;
        };
        qDebug() << "lambda ready";
        bool findinput = false;
        for (auto& i : cur_chip->inputTerminal) {
            qDebug() << "in terminal";

            if (isNear(current, i)) {
                //good
                Waterdrop* water = new Waterdrop;
                water->set_position(current.x(), current.y());
                water->set_status(NORMAL);

                cur_chip->drops.append(water);
                cur_chip->get_cell(current.x(), current.y())->drop_reach(water);
                findinput = true;
                break;
            }
        }
        if (!findinput) return INPUTERROR; // 1 = INPUTERROR
        QSound::play(":/sound/src/sound/act_rich.wav");

    } else if (this->getType() == MOVE) {
        QPoint current = this->getPosition();
        CommandDirection dir = this->getDirection();

        auto hasNear =[](int x, int y, Chip* temp, Waterdrop* w) {
            for (int i = x - 1; i <= x + 1; ++i) {
                for (int j = y - 1; j <= y + 1; ++j) {
                    if (!((i == x) && (j == y))) {
                        if (temp->get_cell(i, j)->get_occupied()->id != w->id) return true;
                    }
                }
            }
            return false;
        };


//        if (hasNear) return HASNEAR;
        Waterdrop* temp = cur_chip->get_cell(current.x(), current.y())->get_occupied();

        if (dir == UP) {
//            if (hasNear(current.x(), current.y() - 1, cur_chip, temp)) return HASNEAR;

            QSound::play(":/sound/src/sound/fan_bei.wav");


            cur_chip->get_cell(current.x(), current.y())->drop_leave();
            cur_chip->get_cell(current.x(), current.y() - 1)->drop_reach(temp);
            cur_chip->get_cell(current.x(), current.y() - 1)->get_occupied()->set_position(current.x(), current.y() - 1);
        } else if (dir == DOWN) {
//            if (hasNear(current.x(), current.y() + 1, cur_chip, temp)) return HASNEAR;

            QSound::play(":/sound/src/sound/fan_nan.wav");
            cur_chip->get_cell(current.x(), current.y())->drop_leave();
            cur_chip->get_cell(current.x(), current.y() + 1)->drop_reach(temp);
            cur_chip->get_cell(current.x(), current.y() + 1)->get_occupied()->set_position(current.x(), current.y() + 1);
        } else if (dir == LEFT) {
//            if (hasNear(current.x() - 1, current.y(), cur_chip, temp)) return HASNEAR;

            QSound::play(":/sound/src/sound/fan_xi.wav");
            cur_chip->get_cell(current.x(), current.y())->drop_leave();
            cur_chip->get_cell(current.x() - 1, current.y())->drop_reach(temp);
            cur_chip->get_cell(current.x() - 1, current.y())->get_occupied()->set_position(current.x() - 1, current.y());
        } else if (dir == RIGHT) {
//            if (hasNear(current.x() + 1, current.y(), cur_chip, temp)) return HASNEAR;

            QSound::play(":/sound/src/sound/fan_dong.wav");
            cur_chip->get_cell(current.x(), current.y())->drop_leave();
            cur_chip->get_cell(current.x() + 1, current.y())->drop_reach(temp);
            cur_chip->get_cell(current.x() + 1, current.y())->get_occupied()->set_position(current.x() + 1, current.y());
        }

    } else if (this->getType() == MERGE_0) {
        QSound::play(":/sound/src/sound/act_pon.wav");
        QPoint current = this->getPosition();
        CommandDirection dir = this->getDirection();
        Waterdrop* water = new Waterdrop;
        if (dir == UP) {
            water->set_position(current.x(), current.y() - 1);
            water->set_status(BIG_V);
            cur_chip->drops.append(water);
            cur_chip->get_cell(current.x(), current.y())->get_occupied()->set_position(-1, -1);
            cur_chip->get_cell(current.x(), current.y())->drop_leave();
            cur_chip->get_cell(current.x(), current.y())->get_occupied()->set_position(-1, -1);
            cur_chip->get_cell(current.x(), current.y() - 2)->drop_leave();
            cur_chip->get_cell(current.x(), current.y() - 1)->drop_reach(water);
        } else if (dir == LEFT) {
            water->set_position(current.x() - 1, current.y());
            water->set_status(BIG_H);
            cur_chip->drops.append(water);

            cur_chip->get_cell(current.x(), current.y())->get_occupied()->set_position(-1, -1);
            cur_chip->get_cell(current.x(), current.y())->drop_leave();
            cur_chip->get_cell(current.x() - 2, current.y())->get_occupied()->set_position(-1, -1);
            cur_chip->get_cell(current.x() - 2, current.y())->drop_leave();
            cur_chip->get_cell(current.x() - 1, current.y())->drop_reach(water);
        }

    } else if (this->getType() == MERGE_1) {
        QPoint current = this->getPosition();
//        CommandDirection dir = cmd.getDirection();
        //??????
        cur_chip->get_cell(current.x(), current.y())->get_occupied()->set_status(NORMAL);

    } else if (this->getType() == SPLIT_0) {
        QSound::play(":/sound/src/sound/act_kan.wav");
        QPoint current = this->getPosition();
        CommandDirection dir = this->getDirection();
        if (dir == UP || dir == DOWN) {
            cur_chip->get_cell(current.x(), current.y())->get_occupied()->set_status(BIG_V);
        } else if (dir == LEFT || dir == RIGHT) {
            cur_chip->get_cell(current.x(), current.y())->get_occupied()->set_status(BIG_H);
        }

    } else if (this->getType() == SPLIT_1) {
        QPoint current = this->getPosition();
        Waterdrop* temp1 = new Waterdrop;
        Waterdrop* temp2 = new Waterdrop;
        CommandDirection dir = this->getDirection();
        if (dir == UP || dir == DOWN) {
            temp1->set_position(current.x(), current.y() - 1);
            temp2->set_position(current.x(), current.y() + 1);
            cur_chip->drops.append(temp1);
            cur_chip->drops.append(temp2);
//            temp1->set_position(current.x(), current.y() - 1);
//            temp2->set_position(current.x(), current.y() + 1);
            //
            cur_chip->get_cell(current.x(), current.y())->get_occupied()->set_position(-1, -1);
            cur_chip->get_cell(current.x(), current.y())->drop_leave();
            cur_chip->get_cell(current.x(), current.y() - 1)->drop_reach(temp1);
            cur_chip->get_cell(current.x(), current.y() + 1)->drop_reach(temp2);
        } else if (dir == LEFT || dir == RIGHT) {
            temp1->set_position(current.x() - 1, current.y());
            temp2->set_position(current.x() + 1, current.y());
            cur_chip->drops.append(temp1);
            cur_chip->drops.append(temp2);
            //
            cur_chip->get_cell(current.x(), current.y())->get_occupied()->set_position(-1, -1);
            cur_chip->get_cell(current.x(), current.y())->drop_leave();
            cur_chip->get_cell(current.x() - 1, current.y())->drop_reach(temp1);
            cur_chip->get_cell(current.x() + 1, current.y())->drop_reach(temp2);
        }

    } else if (this->getType() == OUTPUT) {
        QSound::play(":/sound/src/sound/act_ron.wav");
        auto isNear=[](QPoint& p, QPoint& tg) {
            if (((p.x() - tg.x() == 1) && (p.y() - tg.y() == 0)) ||
                ((p.x() - tg.x() == -1) && (p.y() - tg.y() == 0)) ||
                ((p.x() - tg.x() == 0) && (p.y() - tg.y() == 1)) ||
                ((p.x() - tg.x() == 0) && (p.y() - tg.y() == -1))) return true;
            return false;
        };
        QPoint current = this->getPosition();
        if (isNear(current, cur_chip->outputTerminal)) {
            cur_chip->get_cell(current.x(), current.y())->get_occupied()->set_position(-1, -1);
            cur_chip->get_cell(current.x(), current.y())->drop_leave();
        } else {
            return OUTPUTERROR;
        }
    }
    qDebug() << "success";
    return AC;
}

CommandType Command::getType() {
    return type;
}

int Command::getTime() {
    return timestamp;
}

QPoint Command::getPosition() {
    return position;
}

CommandDirection Command::getDirection() {
    return direction;
}

/*
QVector<Command> Command::parse_command_set(QString& info) {
    qDebug() << "Fuck0";
    QVector<Command> command;
    // parse the input file
    for (auto line: info.split("\n")) {
        qDebug() << "newline";
        qDebug() << line;
        auto temp = line.split(" ")[0];
        qDebug() << temp;
        auto proto_numbers = line.split(" ")[1];

        auto numbers = proto_numbers.split(";")[0].split(",");

        if (temp == "Input") {
//            qDebug() << "in input";
            auto _t = numbers[0].toInt();
            qDebug() << _t;
            auto _x = numbers[1].toInt();
            qDebug() << _x;
            auto _y = numbers[2].toInt();
            qDebug() << _y;
            Command input;
            QString number;

            input.setType(INPUT);
            input.setTime(_t);
            input.setPosition(_x, _y);
            input.setDirection(NO);

            qDebug() << temp << input.getTime() << input.getPosition();

            command.append(input);

        } else if (temp == "Mix") {
//            qDebug() << "in mix";
            auto setMove =[](int t, int _x0, int _y0, int _x1, int _y1) {
                Command move;
                move.setType(MOVE);
                move.setTime(t);
                if (_x0 - _x1 == 1 && _y0 - _y1 == 0) { move.setDirection(LEFT);}
                else if (_x0 - _x1 == -1 && _y0 - _y1 == 0) { move.setDirection(RIGHT);}
                else if (_x0 - _x1 == 0 && _y0 - _y1 == 1) { move.setDirection(UP);}
                else if (_x0 - _x1 == 0 && _y0 - _y1 == -1) { move.setDirection(DOWN);}
                //tepan

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
                ++cnt;
            }

        } else if (temp == "Move") {
//            qDebug() << "in move";
            auto t = numbers[0].toInt();
            auto x0 = numbers[1].toInt();
            auto y0 = numbers[2].toInt();
            auto x1 = numbers[3].toInt();
            auto y1 = numbers[4].toInt();

            auto setMove =[](int _t, int _x0, int _y0, int _x1, int _y1) {
                Command move;
                move.setType(MOVE);
                move.setTime(_t);
                if (_x0 - _x1 == 1 && _y0 - _y1 == 0) { move.setDirection(LEFT);}
                else if (_x0 - _x1 == -1 && _y0 - _y1 == 0) { move.setDirection(RIGHT);}
                else if (_x0 - _x1 == 0 && _y0 - _y1 == 1) { move.setDirection(UP);}
                else if (_x0 - _x1 == 0 && _y0 - _y1 == -1) { move.setDirection(DOWN);}
                else move.setDirection(NO);

                move.setPosition(_x0, _y0);
                return move;
            };
            qDebug() << temp << t << QPoint(x0, y0) << QPoint(x1, y1);
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
                merge1.setPosition(x1, y1 - 1);
                merge1.setDirection(NO);
            }
            command.append(merge0);
            command.append(merge1);

        } else if (temp == "Split") {
            Command split1, split2;
            split1.setType(SPLIT_0);
            split2.setType(SPLIT_1);

            auto t = numbers[0].toInt();
            auto x0 = numbers[1].toInt();
            auto y0 = numbers[2].toInt();
            auto x1 = numbers[3].toInt();
            auto y1 = numbers[4].toInt();
//            auto x2 = numbers[5].toInt();
//            auto y2 = numbers[6].toInt();

            split1.setPosition(x0, y0);
            split2.setPosition(x0, y0);
            split1.setTime(t);
            split2.setTime(t + 1);

            if (x0 - x1 == 1 && y0 - y1 == 0) { split1.setDirection(LEFT); split2.setDirection(RIGHT);}
            else if (x0 - x1 == -1 && y0 - y1 == 0) { split1.setDirection(RIGHT); split2.setDirection(LEFT);}
            else if (x0 - x1 == 0 && y0 - y1 == 1) { split1.setDirection(UP); split2.setDirection(DOWN);}
            else if (x0 - x1 == 0 && y0 - y1 == -1) { split1.setDirection(DOWN); split2.setDirection(UP);}
            command.append(split1);
            command.append(split2);

        } else if (temp == "Output") {
            auto _t = numbers[0].toInt();
            auto _x = numbers[1].toInt();
            auto _y = numbers[2].toInt();
            Command output;
            QString number;

            output.setType(OUTPUT);
            output.setTime(_t);
            output.setPosition(_x, _y);
            output.setDirection(NO);
            qDebug() << "Fuck6";
//            qDebug()
            qDebug() << temp << _t << QPoint(_x, _y);

            command.append(output);
        }
    }
    qDebug() << "parsing down";
    return command;
}
*/

bool operator< (const Command& a, const Command& b) {
    return a.timestamp < b.timestamp;
}

// actually the same as parsing
void CommandFactory::init(QString info, Chip *initialChip) {
    qDebug() << "pasring begin";
    command.clear();
    // parse the input file


    for (auto line: info.split("\n")) {
        qDebug() << "newline";
        qDebug() << line;
        auto temp = line.split(" ")[0];
        qDebug() << temp;
        auto proto_numbers = line.split(" ")[1];

        auto numbers = proto_numbers.split(";")[0].split(",");

        if (temp == "Input") {
//            qDebug() << "in input";
            auto _t = numbers[0].toInt();
            qDebug() << _t;
            auto _x = numbers[1].toInt();
            qDebug() << _x;
            auto _y = numbers[2].toInt();
            qDebug() << _y;
            Command input;
            QString number;

            input.setType(INPUT);
            input.setTime(_t);
            input.setPosition(_x, _y);
            input.setDirection(NO);

            qDebug() << temp << input.getTime() << input.getPosition();

            command.append(input);

        } else if (temp == "Mix") {
//            qDebug() << "in mix";
            auto setMove =[](int t, int _x0, int _y0, int _x1, int _y1) {
                Command move;
                move.setType(MOVE);
                move.setTime(t);
                if (_x0 - _x1 == 1 && _y0 - _y1 == 0) { move.setDirection(LEFT);}
                else if (_x0 - _x1 == -1 && _y0 - _y1 == 0) { move.setDirection(RIGHT);}
                else if (_x0 - _x1 == 0 && _y0 - _y1 == 1) { move.setDirection(UP);}
                else if (_x0 - _x1 == 0 && _y0 - _y1 == -1) { move.setDirection(DOWN);}
                //tepan

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
                ++cnt;
            }

        } else if (temp == "Move") {
//            qDebug() << "in move";
            auto t = numbers[0].toInt();
            auto x0 = numbers[1].toInt();
            auto y0 = numbers[2].toInt();
            auto x1 = numbers[3].toInt();
            auto y1 = numbers[4].toInt();

            auto setMove =[](int _t, int _x0, int _y0, int _x1, int _y1) {
                Command move;
                move.setType(MOVE);
                move.setTime(_t);
                if (_x0 - _x1 == 1 && _y0 - _y1 == 0) { move.setDirection(LEFT);}
                else if (_x0 - _x1 == -1 && _y0 - _y1 == 0) { move.setDirection(RIGHT);}
                else if (_x0 - _x1 == 0 && _y0 - _y1 == 1) { move.setDirection(UP);}
                else if (_x0 - _x1 == 0 && _y0 - _y1 == -1) { move.setDirection(DOWN);}
                else move.setDirection(NO);

                move.setPosition(_x0, _y0);
                return move;
            };
            qDebug() << temp << t << QPoint(x0, y0) << QPoint(x1, y1);
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
                merge1.setPosition(x1, y1 - 1);
                merge1.setDirection(NO);
            }
            command.append(merge0);
            command.append(merge1);

        } else if (temp == "Split") {
            Command split1, split2;
            split1.setType(SPLIT_0);
            split2.setType(SPLIT_1);

            auto t = numbers[0].toInt();
            auto x0 = numbers[1].toInt();
            auto y0 = numbers[2].toInt();
            auto x1 = numbers[3].toInt();
            auto y1 = numbers[4].toInt();
//            auto x2 = numbers[5].toInt();
//            auto y2 = numbers[6].toInt();

            split1.setPosition(x0, y0);
            split2.setPosition(x0, y0);
            split1.setTime(t);
            split2.setTime(t + 1);

            if (x0 - x1 == 1 && y0 - y1 == 0) { split1.setDirection(LEFT); split2.setDirection(RIGHT);}
            else if (x0 - x1 == -1 && y0 - y1 == 0) { split1.setDirection(RIGHT); split2.setDirection(LEFT);}
            else if (x0 - x1 == 0 && y0 - y1 == 1) { split1.setDirection(UP); split2.setDirection(DOWN);}
            else if (x0 - x1 == 0 && y0 - y1 == -1) { split1.setDirection(DOWN); split2.setDirection(UP);}
            command.append(split1);
            command.append(split2);

        } else if (temp == "Output") {
            auto _t = numbers[0].toInt();
            auto _x = numbers[1].toInt();
            auto _y = numbers[2].toInt();
            Command output;
            QString number;

            output.setType(OUTPUT);
            output.setTime(_t);
            output.setPosition(_x, _y);
            output.setDirection(NO);
            qDebug() << "Fuck6";
//            qDebug()
            qDebug() << temp << _t << QPoint(_x, _y);

            command.append(output);
        }
    }
    std::sort(command.begin(), command.end());
    for (auto& cmd: command) {
        maxt = std::max(maxt, cmd.getTime());
        mint = std::min(mint, cmd.getTime());
    }
    curt = mint - 1;
    mint = mint - 1;
    AllChips.clear();
    AllChips.append(initialChip);
    commandIter = command.begin();

    qDebug() << "parsing done";
}

bool CommandFactory::reachEnd() {
    return !(curt < maxt);
}

bool CommandFactory::reachHead() {
    return !(curt > mint);
}

void CommandFactory::next() {
    qDebug() << "next begin";
    if (!reachEnd()) {

        if (!check_status()) {
            QMessageBox::warning(nullptr, "ERROR", "Static Restraint Disobey!!",
                                 QMessageBox::Ok);
            emit this->error_found();
        }


        qDebug() << "not reach end";
        curt++;
        if (curt - mint >= AllChips.length()) {
            qDebug() << "permitted";
            auto temp_board = AllChips.back()->get_copy();
            for (auto& cmd: command) {
                qDebug() << "get cmd";
                if (cmd.getTime() == curt) {
                    auto input_error =[&]() {
                        QMessageBox::warning(0, "ERROR", "Input Drop Must Have Ajacent Terminal!!",
                                             QMessageBox::Ok);
                        emit this->error_found();
                    };
                    auto output_error =[&]() {
                        QMessageBox::warning(0, "ERROR", "Output Drop Must Have Ajacent Terminal!!",
                                             QMessageBox::Ok);
                        emit this->error_found();
                    };
                    auto has_near =[&]() {
                        QMessageBox::warning(0, "ERROR", "Static/Dynamic Restraint Disobey!!",
                                             QMessageBox::Ok);
                        emit this->error_found();
                    };
                    int noError = cmd.doCommand(temp_board);
                    if (noError == INPUTERROR) {
                        input_error();
                    } else  if (noError == OUTPUTERROR) {
                        output_error();
                    }

                }
            }
            AllChips.append(temp_board);
        }
        emit chip_changed();
    }
    qDebug() << "next end";
}

void CommandFactory::prev() {
    if (!reachHead()) {
        --curt;
        emit chip_changed();
    }
}

void CommandFactory::reset() {
    curt = mint;
    emit chip_changed();
}

Chip* CommandFactory::get_chip() {
    return AllChips[curt - mint];
}

bool CommandFactory::check_status() {
    int t0 = curt;
    int t1 = curt+1;
    qDebug() << t0 << t1 << mint << maxt;
    auto abs =[=](int t) {
        if (t >= 0) return t;
        return -t;
    };

    auto isNear =[=](Waterdrop* w0, Waterdrop* w1) {
        if (w0->get_status() == BIG_H || w0->get_status() == BIG_V || w1->get_status() == BIG_H || w1->get_status() == BIG_V) {
            return false;
        }
        if ((abs(w0->get_position()->x() - w1->get_position()->x()) > 1) || (abs(w0->get_position()->y() - w1->get_position()->y()) > 1)) return false;
        return true;
    };

    if (t1 < maxt && t0 > mint) {
        Chip* chip0 = AllChips[t0 - mint - 1];
        Chip* chip1 = AllChips[t1 - mint - 1];
        for (auto& w : chip0->drops) {
            for (auto& temp: chip0->drops) {
                if ((w->id != temp->id) && (w->get_position()->x()!= -1) && (temp->get_position()->x()!= -1)) {
                    if (isNear(w, temp)) {
                        qDebug() << "===================================================================================================================================================================================================================================================================";
                        qDebug() << *(w->get_position()) << *temp->get_position();
                        curt --;
//                        AllChips.pop_back();
                        return false;
                    }
                }
            }
            for (auto& temp: chip1->drops) {
                if ((w->id != temp->id) && (w->get_status() == NORMAL) && (temp->get_status() == NORMAL) &&
                        (w->get_position()->x()!= -1) && (temp->get_position()->x()!= -1)) {
                    if (isNear(w, temp)) {
                        qDebug() << "****************************************************************************************************************************";
                        qDebug() << *(w->get_position())  << t0 << QString::number(w->get_status());
                        qDebug() << *(temp->get_position())  << t1 << QString::number(temp->get_status());
                        curt--;
//                        AllChips.pop_back();
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
