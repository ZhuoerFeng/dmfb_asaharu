#include "waterdrop.h"
#include <QDebug>


int Waterdrop::MaxDrop = 0;




Waterdrop::Waterdrop() : id(MaxDrop), position(new QPoint), status(NORMAL)
{
    QMap<int, QColor> indexColor;
    indexColor[0] = Qt::red;
    indexColor[1] = Qt::blue;
    indexColor[2] = Qt::green;
    indexColor[3] = Qt::gray;
    indexColor[4] = Qt::yellow;
    indexColor[5] = Qt::cyan;
    indexColor[6] = Qt::magenta;
    ++MaxDrop;
    color = indexColor[id % 7];
}

void Waterdrop::paint(QPainter &p) {
    int radius = 20;
    if (status == NORMAL) {
        p.drawEllipse(position->x() - radius, position->y() - radius, radius, radius);
    } else if (status == BIG_V) {
        p.drawEllipse(position->x() - radius, position->y() - radius * 3, radius, radius * 3);
    } else if (status == BIG_H) {
        p.drawEllipse(position->x() - radius * 3, position->y() - radius, radius * 3, radius);
    }
}

//Cell* Waterdrop::get_current_cell() {
//    return currentCell;
//}

QColor Waterdrop::get_color() {
    return color;
}

QPoint* Waterdrop::get_position() {
    return position;
}

WaterdropStatus Waterdrop::get_status() {
    return status;
}

void Waterdrop::set_position(int x, int y) {
    position->setX(x);
    position->setY(y);
}

void Waterdrop::set_status(WaterdropStatus _status) {
    status = _status;
}

Waterdrop::~Waterdrop() {
    delete position;
}

void Waterdrop::print_drop() {
    qDebug() << color << *position << status;
}
