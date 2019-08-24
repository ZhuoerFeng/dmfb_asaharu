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
#include <QTimer>
#include "chip.h"
#include "command.h"


class Chip;
class Command;
class Waterdrop;
class CommandFactory;

class Canvas : public QWidget
{
    Q_OBJECT
public:
    Canvas(QWidget* parent = nullptr);
    ~Canvas();
    bool should_paint = true;

    void paintEvent(QPaintEvent *ev);
    void keyPressEvent(QKeyEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

    void drawChip();
    void drawGrid(QPainter&);
    void drawTerminal(QPainter&, QPoint, QColor, QString);
    void drawCleanWaterTerminal(QPainter&);
    void drawWaterDrop(Waterdrop*);
    void drawWaste(Cell*);

    bool getCleanMode();
    void setCleanMode(bool checked);

    void initCommand(QString& info);

    void pause();
    void play();

//    void perform_all_command();

//    void perform_current_command();
    bool isEnd();

//    void copyChip();

    Chip* my_chip;
    CommandFactory* get_command_runner();
    QTimer *timer = nullptr;

signals:
    void play_over();

public slots:
    void init(int, int, QVector<int>, QVector<int>, int, int);

private:
    int column = 0;
    int row = 0;
    int grid_height;
    int grid_width;
    bool clean_mode = false;
    bool initialized = false;

    CommandFactory* command_runner;

    QPoint start_point;

    QVector<QPoint> Input;
    QPoint Output;

    QPoint wash_input;
    QPoint waste_output;

    QVector<Command> command;


    QVector<Chip*> AllChips;


    int current_timestamp = 0;
    int wash_timestamp = 0;

};

#endif // CANVAS_H
