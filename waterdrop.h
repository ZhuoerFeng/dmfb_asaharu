#ifndef WATERDROP_H
#define WATERDROP_H

#include <QPoint>
#include <QPainter>
#include <QColor>

enum WaterdropStatus {NORMAL, BIG_V, BIG_H};

class Waterdrop
{
public:
    Waterdrop();
    void paint(QPainter&);
    void set_position(int, int);
    void set_status(WaterdropStatus);

    WaterdropStatus get_status();
    QColor get_color();
    QPoint* get_position();
    ~Waterdrop();
    static int MaxDrop;

private:
    int id;
    QPoint* position;
    WaterdropStatus status;
    QColor color;

};

#endif // WATERDROP_H
