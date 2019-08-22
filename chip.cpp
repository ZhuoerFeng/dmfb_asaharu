#include "chip.h"
#include "command.h"
#include "waterdrop.h"
#include <QDebug>

Cell::Cell() : wastedTime(0), occupied(nullptr), previous(nullptr) {}

Cell::~Cell() {
}

void Cell::clean_waste() {
    previous = nullptr;
    occupied = nullptr;
    wastedTime = 0;
}

void Cell::set_position(QPoint p) {
//    qDebug() << "position Riichi";
    position.setX(p.x());
    position.setY(p.y());
//    qDebug() << "position Ron";
}


int Cell::get_waste_time() {
    return wastedTime;
}

QPoint Cell::get_position() {
    return position;
}

Waterdrop* Cell::get_occupied() {
    return occupied;
}

Waterdrop* Cell::get_previous() {
    return previous;
}

void Cell::drop_reach(Waterdrop * w) {
    previous = occupied;
    occupied = w;
}

void Cell::drop_leave() {
    occupied = nullptr;
    ++wastedTime;
}

Chip::Chip() : row(0), column(0)
{

}

void Chip::init(int _col, int _row, QVector<QPoint> _input, QPoint _output) {
//    qDebug() << "init Pon!";
    row = _row;
    column = _col;
    inputTerminal = _input;
    outputTerminal = _output;
    //假设坐标1 1开始
    for (int i = 0; i < row * column; ++i) {
//        qDebug() << "inti Chii!";
        Cell* temp = new Cell;
        int x = (i + 1) % column;
        int y = (i + 1) / column + 1;
//        qDebug() << "Pei!";
        temp->set_position(QPoint(x, y));
//        qDebug() << "init Gann!";
        cell.append(temp);
    }
}

Chip::~Chip() {
    for (auto i : cell) {
        delete i;
    }
    for (auto i: drops) {
        delete i;
    }
}

void Chip::set_scale(int _row, int _column) {
     row = _row;
     column = _column;
}

Cell* Chip::get_cell(int x, int y) {
    //约定坐标1 1开始
//    qDebug() << "gtcell Riichi";
    if ( ((column > 0) && (row > 0))) {
        int rank = (x-1) * column + y-1;
//        qDebug() << "gtcell Ron";
        return cell[rank];
    } else {
//        qDebug() << "gtcell Ron";
        return nullptr;
    }
}

void Chip::doCommand(Command& cmd) {
    if (cmd.getType() == INPUT) {
        QPoint current = cmd.getPosition();

        auto isNear=[](QPoint& p, QPoint& tg) {
            if (((p.x() - tg.x() == 1) && (p.y() - tg.y() == 0)) ||
                ((p.x() - tg.x() == -1) && (p.y() - tg.y() == 0)) ||
                ((p.x() - tg.x() == 0) && (p.y() - tg.y() == 1)) ||
                ((p.x() - tg.x() == 0) && (p.y() - tg.y() == -1))) return true;
            return false;
        };

        for (auto i : inputTerminal) {
            if (isNear(current, i)) {
                //good
                Waterdrop* water = new Waterdrop;
                water->set_position(current.x(), current.y());
                water->set_status(NORMAL);
                drops.append(water);
                get_cell(current.x(), current.y())->drop_reach(water);
            } else {
                //baocuo
            }
        }

    } else if (cmd.getType() == MOVE) {
        QPoint current = cmd.getPosition();
        CommandDirection dir = cmd.getDirection();
        if (dir == UP) {
            Waterdrop* temp = get_cell(current.x(), current.y())->get_occupied();
            get_cell(current.x(), current.y())->drop_leave();
            get_cell(current.x(), current.y() - 1)->drop_reach(temp);
        } else if (dir == DOWN) {
            Waterdrop* temp = get_cell(current.x(), current.y())->get_occupied();
            get_cell(current.x(), current.y())->drop_leave();
            get_cell(current.x(), current.y() + 1)->drop_reach(temp);
        } else if (dir == LEFT) {
            Waterdrop* temp = get_cell(current.x(), current.y())->get_occupied();
            get_cell(current.x(), current.y())->drop_leave();
            get_cell(current.x() - 1, current.y())->drop_reach(temp);
        } else if (dir == RIGHT) {
            Waterdrop* temp = get_cell(current.x(), current.y())->get_occupied();
            get_cell(current.x(), current.y())->drop_leave();
            get_cell(current.x() + 1, current.y())->drop_reach(temp);
        }

    } else if (cmd.getType() == MERGE_0) {
        QPoint current = cmd.getPosition();
        CommandDirection dir = cmd.getDirection();
        if (dir == UP) {
            Waterdrop* water = new Waterdrop;
            water->set_position(current.x(), current.y() - 1);
            water->set_status(BIG_V);
            drops.append(water);
            get_cell(current.x(), current.y())->drop_leave();
            get_cell(current.x(), current.y() - 2)->drop_leave();
            get_cell(current.x(), current.y() - 1)->drop_reach(water);
        } else if (dir == LEFT) {
            Waterdrop* water = new Waterdrop;
            water->set_position(current.x() - 1, current.y());
            water->set_status(BIG_H);
            drops.append(water);
            get_cell(current.x(), current.y())->drop_leave();
            get_cell(current.x() - 2, current.y())->drop_leave();
            get_cell(current.x() - 1, current.y())->drop_reach(water);
        }

    } else if (cmd.getType() == MERGE_1) {
        QPoint current = cmd.getPosition();
        CommandDirection dir = cmd.getDirection();
        //??????
        get_cell(current.x(), current.y())->get_occupied()->set_status(NORMAL);

    } else if (cmd.getType() == SPLIT_0) {
        QPoint current = cmd.getPosition();
        CommandDirection dir = cmd.getDirection();
        if (dir == UP || dir == DOWN) {
            get_cell(current.x(), current.y())->get_occupied()->set_status(BIG_V);
        } else if (dir == LEFT || dir == RIGHT) {
            get_cell(current.x(), current.y())->get_occupied()->set_status(BIG_H);
        }

    } else if (cmd.getType() == SPLIT_1) {
        QPoint current = cmd.getPosition();
        CommandDirection dir = cmd.getDirection();
        if (dir == UP || dir == DOWN) {
            Waterdrop* temp1 = new Waterdrop;
            Waterdrop* temp2 = new Waterdrop;
            temp1->set_position(current.x(), current.y() - 1);
            temp2->set_position(current.x(), current.y() + 1);
            drops.append(temp1);
            drops.append(temp2);

            get_cell(current.x(), current.y())->drop_leave();
            get_cell(current.x(), current.y() - 1)->drop_reach(temp1);
            get_cell(current.x(), current.y() + 1)->drop_reach(temp2);

        } else if (dir == LEFT || dir == RIGHT) {
            Waterdrop* temp1 = new Waterdrop;
            Waterdrop* temp2 = new Waterdrop;
            temp1->set_position(current.x() - 1, current.y());
            temp2->set_position(current.x() + 1, current.y());
            drops.append(temp1);
            drops.append(temp2);

            get_cell(current.x(), current.y())->drop_leave();
            get_cell(current.x() - 1, current.y())->drop_reach(temp1);
            get_cell(current.x() + 1, current.y())->drop_reach(temp2);
        }

    } else if (cmd.getType() == OUTPUT) {
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

}





