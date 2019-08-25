#ifndef WATERDROP_H
#define WATERDROP_H

#include <QPoint>
#include <QPainter>
#include <QColor>
#include <QMap>
//#include "chip.h"
//class Cell;

enum WaterdropStatus {NORMAL, BIG_V, BIG_H};

class Waterdrop
{
public:
    Waterdrop();
    void paint(QPainter&);
    void set_position(int, int);
    void set_status(WaterdropStatus);

    void print_drop();
//    Cell* get_current_cell();

    WaterdropStatus get_status();
    QColor get_color();
    QPoint* get_position();
    ~Waterdrop();
    static int MaxDrop;

//private:
    int id;
    QPoint* position;
    WaterdropStatus status;
    QColor color;

//    Cell* currentCell;

};

#endif // WATERDROP_H
