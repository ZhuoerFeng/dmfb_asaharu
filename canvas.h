#ifndef CANVAS_H
#define CANVAS_H

#include <QPaintEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QDialog>
#include <QtCore/qmath.h>
#include <QPainter>
#include <QVector>

#include "chip.h"
#include "command.h"


class Chip;
class Command;
class Waterdrop;

class Canvas : public QWidget
{
    Q_OBJECT
public:
    Canvas(QWidget* parent = nullptr);
    bool should_paint = true;

    void paintEvent(QPaintEvent *ev);
    void keyPressEvent(QKeyEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

    void setChip();
    void drawGrid(QPainter&);
    void drawTerminal(QPainter&, QPoint, QColor, QString);
    void drawCleanWaterTerminal(QPainter&);
    void drawWaterDrop(Waterdrop*);
    void drawWaste(Cell*);


    void init(int, int, QVector<int>, QVector<int>, int, int);


    void pause();
    void refresh();
    void next();
    void prev();
    void perform_command();

    Chip my_chip;
private:
    int column = 0;
    int row = 0;
    int grid_height;
    int grid_width;
    bool clean_mode = false;
    QPoint start_point;

    QVector<QPoint> Input;
    QVector<Command> command;
    QTimer *timer = nullptr;
    int current_timestamp = 0;
    QPoint Output;
};

#endif // CANVAS_H
