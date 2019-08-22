#include "waterdrop.h"

int Waterdrop::MaxDrop = 0;

Waterdrop::Waterdrop() : id(MaxDrop), position(nullptr), color(Qt::blue), status(NORMAL)
{
    ++MaxDrop;
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
