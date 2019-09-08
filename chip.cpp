#include "chip.h"
#include "command.h"
#include "waterdrop.h"
#include <QDebug>
#include <QSound>

Cell::Cell() : occupied(nullptr), previous(nullptr) {
    qDebug() << "cell construct";
    wastedTime = waster.size();
    qDebug() << "cell done";
}

Cell::~Cell() {
//    delete currentChip;
}

//void Cell::set_current_chip(Chip * c) {
//    currentChip = c;
//}

void Cell::clean_waste() {
    previous = nullptr;
    occupied = nullptr;
    waster.clear();
    wastedTime = waster.size();
}

void Cell::set_position(QPoint p) {
//    qDebug() << "position Riichi";
    position.setX(p.x());
    position.setY(p.y());
//    qDebug() << "position Ron";
}

void Cell::change_blocked() {
    isBlocked = !isBlocked;
}

void Cell::drop_reach(Waterdrop * w) {
    if (wastedTime == 0) {
        qDebug() << "Polluted!";
    }
    waster.insert(w->id);
    wastedTime = waster.size();
    occupied = w;
}

void Cell::drop_leave() {
    previous = occupied;
    occupied = nullptr;
    wastedTime = waster.size();
}

QPoint Cell::get_position() {
    return position;
}

Waterdrop* Cell::get_previous() {
    return previous;
}

Waterdrop* Cell::get_occupied() {
    return occupied;
}

int Cell::get_waste_time() {
    return waster.size();
}




Chip::Chip() : row(0), column(0)
{
    qDebug() << "chip construct";
}

Chip::~Chip() {
    for (auto i : cell) {
        delete i;
    }
    for (auto i: drops) {
        delete i;
    }
}

void Chip::init(int _col, int _row, QVector<QPoint> _input, QPoint _output,
                QPoint _washinput, QPoint _washoutput) {
    row = _row;
    column = _col;
    inputTerminal = _input;
    outputTerminal = _output;
    washInput = _washinput;
    washOutput = _washoutput;
    //假设坐标1 1开始
    for (int i = 1; i <= row * column; ++i) {
        Cell* temp = new Cell;
        int x = (i - 1) % column + 1;
        int y = (i - 1) / column + 1;
        temp->set_position(QPoint(x, y));
//        temp->set_current_chip(this);
        cell.append(temp);
    }
    qDebug() << "chip set ready";
}

Chip* Chip::get_copy() {
    Chip* temp = new Chip(*this);
    QMap<Waterdrop*, Waterdrop*> findWater;
    QMap<Cell*, Cell*> findCell;
    QMap<QPoint*, QPoint*> findPosition;

//    temp->row = this->row;
//    temp->column = this->column;
//    temp->inputTerminal = this->inputTerminal;
//    temp->outputTerminal = this->outputTerminal;
    //复制水滴
    for (auto& old_water: temp->drops) {
        Waterdrop* new_water = new Waterdrop(*old_water);
        findWater[old_water] = new_water;
        old_water = new_water;
    }
    //复制cell
    for (auto& old_cell: temp->cell) {
        Cell* new_cell = new Cell(*old_cell);
        findCell[old_cell] = new_cell;
        old_cell = new_cell;
    }
    //复制位置
    for (auto& new_water: temp->drops) {
        if (new_water->get_position() != nullptr) {
            new_water->position = new QPoint(*new_water->position);
        }
    }
    for (auto& old_cell : temp->cell) {
        if (old_cell->occupied != nullptr) {
            old_cell->occupied = findWater[old_cell->occupied];
            old_cell->previous = findWater[old_cell->previous];

        }
    }
    return temp;
}


Cell* Chip::get_cell(int x, int y) {
    //约定坐标1 1开始
    if ( ((column > 0) && (row > 0))) {
        int rank = (y-1) * column + x-1; // 5, 4 => 19 =   8,1
        return cell[rank];
    } else {
        return nullptr;
    }
}

void Chip::set_scale(int _row, int _column) {
     row = _row;
     column = _column;
}

// might be useless
void Chip::doCommand(Command& cmd) {

    if (cmd.getType() == INPUT) {
        qDebug() << "input exec";
        QPoint current = cmd.getPosition();
        qDebug() << "position ready";
        auto isNear=[](QPoint& p, QPoint& tg) {
            if (((p.x() - tg.x() == 1) && (p.y() - tg.y() == 0)) ||
                ((p.x() - tg.x() == -1) && (p.y() - tg.y() == 0)) ||
                ((p.x() - tg.x() == 0) && (p.y() - tg.y() == 1)) ||
                ((p.x() - tg.x() == 0) && (p.y() - tg.y() == -1))) return true;
            return false;
        };
        qDebug() << "lambda ready";
        for (auto& i : inputTerminal) {
            qDebug() << "in terminal";

            if (isNear(current, i)) {
                //good
                Waterdrop* water = new Waterdrop;
                water->set_position(current.x(), current.y());
                water->set_status(NORMAL);

                drops.append(water);
                get_cell(current.x(), current.y())->drop_reach(water);
                break;
            } else {
                //baocuo
            }
        }
        QSound::play(":/sound/src/sound/act_rich.wav");

    } else if (cmd.getType() == MOVE) {
        QPoint current = cmd.getPosition();
        CommandDirection dir = cmd.getDirection();
        Waterdrop* temp = get_cell(current.x(), current.y())->get_occupied();

        if (dir == UP) {
            QSound::play(":/sound/src/sound/fan_bei.wav");
            get_cell(current.x(), current.y())->drop_leave();
            get_cell(current.x(), current.y() - 1)->drop_reach(temp);
            get_cell(current.x(), current.y() - 1)->get_occupied()->set_position(current.x(), current.y() - 1);
        } else if (dir == DOWN) {
            QSound::play(":/sound/src/sound/fan_nan.wav");
            get_cell(current.x(), current.y())->drop_leave();
            get_cell(current.x(), current.y() + 1)->drop_reach(temp);
            get_cell(current.x(), current.y() + 1)->get_occupied()->set_position(current.x(), current.y() + 1);
        } else if (dir == LEFT) {
            QSound::play(":/sound/src/sound/fan_xi.wav");
            get_cell(current.x(), current.y())->drop_leave();
            get_cell(current.x() - 1, current.y())->drop_reach(temp);
            get_cell(current.x() - 1, current.y())->get_occupied()->set_position(current.x() - 1, current.y());
        } else if (dir == RIGHT) {
            QSound::play(":/sound/src/sound/fan_dong.wav");
            get_cell(current.x(), current.y())->drop_leave();
            get_cell(current.x() + 1, current.y())->drop_reach(temp);
            get_cell(current.x() + 1, current.y())->get_occupied()->set_position(current.x() + 1, current.y());
        }

    } else if (cmd.getType() == MERGE_0) {
        QSound::play(":/sound/src/sound/act_pon.wav");
        QPoint current = cmd.getPosition();
        CommandDirection dir = cmd.getDirection();
        Waterdrop* water = new Waterdrop;
        if (dir == UP) {
            water->set_position(current.x(), current.y() - 1);
            water->set_status(BIG_V);
            drops.append(water);
            get_cell(current.x(), current.y())->drop_leave();
            get_cell(current.x(), current.y() - 2)->drop_leave();
            get_cell(current.x(), current.y() - 1)->drop_reach(water);
        } else if (dir == LEFT) {
            water->set_position(current.x() - 1, current.y());
            water->set_status(BIG_H);
            drops.append(water);
            get_cell(current.x(), current.y())->drop_leave();
            get_cell(current.x() - 2, current.y())->drop_leave();
            get_cell(current.x() - 1, current.y())->drop_reach(water);
        }

    } else if (cmd.getType() == MERGE_1) {
        QPoint current = cmd.getPosition();
//        CommandDirection dir = cmd.getDirection();
        //??????
        get_cell(current.x(), current.y())->get_occupied()->set_status(NORMAL);

    } else if (cmd.getType() == SPLIT_0) {
        QSound::play(":/sound/src/sound/act_kan.wav");
        QPoint current = cmd.getPosition();
        CommandDirection dir = cmd.getDirection();
        if (dir == UP || dir == DOWN) {
            get_cell(current.x(), current.y())->get_occupied()->set_status(BIG_V);
        } else if (dir == LEFT || dir == RIGHT) {
            get_cell(current.x(), current.y())->get_occupied()->set_status(BIG_H);
        }

    } else if (cmd.getType() == SPLIT_1) {
        QPoint current = cmd.getPosition();
        Waterdrop* temp1 = new Waterdrop;
        Waterdrop* temp2 = new Waterdrop;
        CommandDirection dir = cmd.getDirection();
        if (dir == UP || dir == DOWN) {
            temp1->set_position(current.x(), current.y() - 1);
            temp2->set_position(current.x(), current.y() + 1);
            drops.append(temp1);
            drops.append(temp2);
//            temp1->set_position(current.x(), current.y() - 1);
//            temp2->set_position(current.x(), current.y() + 1);

            get_cell(current.x(), current.y())->drop_leave();
            get_cell(current.x(), current.y() - 1)->drop_reach(temp1);
            get_cell(current.x(), current.y() + 1)->drop_reach(temp2);
        } else if (dir == LEFT || dir == RIGHT) {
            temp1->set_position(current.x() - 1, current.y());
            temp2->set_position(current.x() + 1, current.y());
            drops.append(temp1);
            drops.append(temp2);

            get_cell(current.x(), current.y())->drop_leave();
            get_cell(current.x() - 1, current.y())->drop_reach(temp1);
            get_cell(current.x() + 1, current.y())->drop_reach(temp2);
        }

    } else if (cmd.getType() == OUTPUT) {
        QSound::play(":/sound/src/sound/act_ron.wav");
        auto isNear=[](QPoint& p, QPoint& tg) {
            if (((p.x() - tg.x() == 1) && (p.y() - tg.y() == 0)) ||
                ((p.x() - tg.x() == -1) && (p.y() - tg.y() == 0)) ||
                ((p.x() - tg.x() == 0) && (p.y() - tg.y() == 1)) ||
                ((p.x() - tg.x() == 0) && (p.y() - tg.y() == -1))) return true;
            return false;
        };
        QPoint current = cmd.getPosition();
        if (isNear(current, outputTerminal)) {
            get_cell(current.x(), current.y())->drop_leave();
        } else {
            // baocuo
        }
    }
    qDebug() << "success";
}

int Chip::get_current_time() {
    return curtime;
}

void Chip::set_time(int cur, int wash) {
    curtime = cur;
    washtime = wash;
}





