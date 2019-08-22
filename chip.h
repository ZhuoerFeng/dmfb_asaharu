#ifndef CHIP_H
#define CHIP_H

#include <QVector>
#include <QPoint>
//#include "waterdrop.h"
#include "command.h"

class Waterdrop;
class Command;

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

private:
    int wastedTime;
    Waterdrop *occupied;
    Waterdrop *previous;
    QPoint position;
};

class Chip
{
public:
    Chip();
    ~Chip();
    Cell* get_cell(int, int);
    void set_scale(int, int);
    void doCommand(Command&);

    void init(int, int, QVector<QPoint>, QPoint);

private:
    QVector<Cell* > cell;
    int row;
    int column;
    QVector<QPoint> inputTerminal;
    QPoint outputTerminal;
    QVector<Waterdrop*> drops;

};

#endif // CHIP_H
