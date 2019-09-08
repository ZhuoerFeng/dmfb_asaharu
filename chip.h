#ifndef CHIP_H
#define CHIP_H

#include <QVector>
#include <QPoint>
//#include "waterdrop.h"
#include "command.h"
#include <QSet>

class Waterdrop;
class Command;

class Chip;

class Cell {
public:
    Cell();
    ~Cell();

    void clean_waste();
    void set_position(QPoint);
    void drop_reach(Waterdrop*);
    void drop_leave();

    QPoint get_position();
    Waterdrop* get_previous();
    Waterdrop* get_occupied();
    int get_waste_time();
    void change_blocked();


    QSet<int> waster;
    int wastedTime = 0;
    Waterdrop *occupied;
    Waterdrop *previous;
    QPoint position;
    bool isBlocked = false;

};

class Chip {
public:
    Chip();
    ~Chip();
    void init(int, int, QVector<QPoint>, QPoint, QPoint, QPoint);

    Cell* get_cell(int, int);
    void set_scale(int, int);
    void doCommand(Command&);

    Chip* get_copy();
    int get_current_time();
    void set_time(int, int);

    QVector<Cell* > cell;
    int row = 0;
    int column = 0;
    QVector<QPoint> inputTerminal;
    QPoint outputTerminal;
    QPoint washInput;
    QPoint washOutput;
    QVector<Waterdrop*> drops;

    QSet<QPair<int, int>> visited;

    int curtime = 0;
    int washtime = 0;

};

#endif // CHIP_H
