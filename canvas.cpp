#include "canvas.h"
#include "chip.h"
#include "waterdrop.h"
#include "command.h"
#include <QPen>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>
#include <QMap>
#include <QSound>
#include <QPainter>

Canvas::Canvas(QWidget* parent) : QWidget(parent), timer(new QTimer), my_chip(new Chip), command_runner(new CommandFactory)
{
    qDebug() << "canvas construct";
    connect(timer, &QTimer::timeout, [=](){
        if (!command_runner->reachEnd()) {
            command_runner->next();
        } else {
            emit play_over();
        }
    });
    connect(command_runner, &CommandFactory::chip_changed, [=]() {
        my_chip = command_runner->get_chip();
        this->update();
    });
    qDebug() << "canvas done";
    //还需要判断是否有非法输入
}

Canvas::~Canvas() {
    delete timer;
    delete my_chip;
    for (auto i: AllChips) {
        delete i;
    }
}

//column, row, input_x, input_y, output_x, output_y
void Canvas::init(int _column, int _row, QVector<int> input_x, QVector<int> input_y, int output_x, int output_y,
                  int _washinx, int _washiny, int _washoutx, int _washouty) {
    qDebug() << "canvas initializing...";
//    if (initialized) break;
    grid_height = 50;
    grid_width = 50;
    start_point.setX(200);
    start_point.setY(200);
    column = _column;
    row = _row;

    for (int i = 0; i < input_x.size(); ++i) {
        QPoint temp;
        temp.setX(input_x[i]);
        temp.setY(input_y[i]);
        Input.append(temp);
    }
    Output.setX(output_x);
    Output.setY(output_y);
    wash_input.setX(_washinx);
    wash_input.setY(_washiny);
    waste_output.setX(_washoutx);
    waste_output.setY(_washouty);

    my_chip->init(column, row, Input, Output, wash_input, waste_output);
    initialized = true;
    qDebug() << "canvas set ready!";
}

void Canvas::initCommand(QString& info) {
    command_runner->init(info, my_chip);
//    command = Command::parse_command_set(info);
}

void Canvas::setWashCommands(Chip *initial_chip) {

}

CommandFactory* Canvas::get_command_runner() {
    return command_runner;
}

void Canvas::drawGrid(QPainter& p) {
    qDebug() << "draw grid begin";
    QPoint line_end_point;
    line_end_point.setX(start_point.x() + column * grid_width);
    line_end_point.setY(start_point.y() + row * grid_height);

    QPoint now_start_point = start_point;
    for (int i = 0; i <= row; ++i) {
        now_start_point.setY(start_point.y() + i * grid_height);
        QPoint now_end_point = line_end_point;
        now_end_point.setY(now_end_point.y() + (i - row) * grid_height);

        p.drawLine(now_start_point, now_end_point);
        qDebug() << now_start_point;
    }
    now_start_point = start_point;
    for (int j = 0; j <= column; ++j) {
        now_start_point.setX(start_point.x() + j * grid_width);
        QPoint now_end_point = line_end_point;
        now_end_point.setX(line_end_point.x() + (j - column) * grid_width);
//        qDebug() << "draw";
        p.drawLine(now_start_point, now_end_point);
    }
    qDebug() << "grid drawn ready";
}

void Canvas::drawTerminal(QPainter& p, QPoint point, QColor c, QString s) {
//    qDebug() << "fuck";
//    qDebug() << point << s;
    p.setBrush(c);
    QPoint current;
    current.setX(start_point.x() + point.x() * grid_width);
    current.setY(start_point.y() + point.y() * grid_height);

//    p.setBrush(QColor(0, 0, 0));
    if (point.x() == 0) {
        p.drawRect((current.x() - grid_width * 2), (current.y() - grid_height),
                    grid_width * 2, grid_height);
        p.drawText((current.x() - grid_width * 2), (current.y() - grid_height), s);
    } else if (point.x() == column + 1) {
        p.drawRect((current.x() - grid_width), (current.y() - grid_height),
                    grid_width * 2, grid_height);
        p.drawText((current.x() - grid_width), (current.y() - grid_height), s);
    } else if (point.y() == 0) {
        p.drawRect((current.x() - grid_width), (current.y() - grid_height * 2),
                    grid_width, grid_height * 2);
        p.drawText((current.x() - grid_width), (current.y() - grid_height * 2), s);
    } else if (point.y() == row + 1) {
        p.drawRect((current.x() - grid_width), (current.y() - grid_height),
                    grid_width, grid_height * 2);
        p.drawText((current.x() - grid_width), (current.y() - grid_height), s);
    }
}

bool Canvas::getCleanMode() {
    return clean_mode;
}

void Canvas::setCleanMode(bool checked) {
    clean_mode = checked;
}
//前半部分没问题，setChip有问题
void Canvas::paintEvent(QPaintEvent *ev) {
    qDebug() << "In paint event";
    if (row == 0 || column == 0) {
        return;
    }
    qDebug() << row << " " << column;
//    if (!should_paint) return;
    QPainter p(this);
    p.setPen(QPen(QColor(0, 0, 0), 4));

    p.setBrush(QColor(255, 0, 0, 120));
//    p.drawRect(QRect(QPoint(200, 200),QPoint(250, 250)));
      //draw grids
    drawGrid(p);
    //输入端口 输出端口
    for (auto& input : Input) {
        qDebug() << "OK";
        this->drawTerminal(p, input, QColor(0, 255, 255), "INPUT");
    }
    this->drawTerminal(p, Output, QColor(116, 52, 129), "OUTPUT");
    this->drawTerminal(p, wash_input,QColor(0, 255, 255, 20), "Wash input");
    this->drawTerminal(p, waste_output,QColor(160, 82, 45, 40), "Waste");
    qDebug() << "terminal drawn ready";
    should_paint = false;
    drawChip();
    qDebug() << "Interface drawn ready";
}

//没问题
void Canvas::drawWaterDrop(Waterdrop *w) {
    if (w == nullptr) {
//        qDebug() << "No water";
        return;
    }
//    qDebug() << "begin to draw drop";
    w->print_drop();
    QPainter p(this);
    p.setPen(QPen(QColor(0, 0, 255), 1));
    p.setBrush(w->get_color());
    QPoint* current = w->get_position();
    int radius = 20;
//    qDebug() << *current;
//    qDebug() << start_point.x() + current->x() * grid_width - radius - 25;
//    qDebug() <<  start_point.y()  - radius + current->y() * grid_height - radius;
    if (w->get_status() == NORMAL) {
        p.drawEllipse(start_point.x() + current->x() * grid_width - radius - 25 , start_point.y()  - radius + current->y() * grid_height - radius, 2 * radius, 2 * radius);
    } else if (w->get_status() == BIG_H) {
        p.drawEllipse(start_point.x() + current->x() * grid_width - 3 * radius - 25, start_point.y() - 25 - radius + current->y() * grid_height - radius, 6 * radius, 2 * radius);
    } else if (w->get_status() == BIG_V) {
        p.drawEllipse(start_point.x() + current->x() * grid_width - radius - 25, start_point.y() - 25 - radius + current->y() * grid_height - radius * 3, 2 * radius, 6 * radius);
    }
//    qDebug() << "drop drawn ready";
}

//没问题
void Canvas::drawWaste(Cell *c) {
    QPainter p(this);
    QFont f;
    f.setPointSize(24);
    p.setFont(f);
    int num = c->get_waste_time();
    p.drawText(QRect(QPoint(start_point.x() + c->get_position().x() * grid_width - 50,
                            start_point.y() + c->get_position().y() * grid_height - 50),
                     QPoint(start_point.x() + c->get_position().x() * grid_width,
                            start_point.y() + c->get_position().y() * grid_height)),
               Qt::AlignCenter,
               QString::number(num));

//    qDebug() << "waste Riichi";
//    qDebug() << c->get_previous()->get_position();
    if (c->get_previous() == nullptr) {
//        qDebug() << "No waste";
        return;
    }

    QColor _c = c->get_previous()->get_color();
    p.setBrush(_c);
//    p.setBrush(QColor(121, 0, 255, (125 - 25 * num)));
    QPoint cur;
    cur.setX(start_point.x() + c->get_position().x() * grid_width);
    cur.setY(start_point.y() + c->get_position().y() * grid_height);
//    qDebug() << cur;
//    p.setBrush(QColor(0, 255, 0, 100));
    //QPoint(start_x + (j-1) * grid_width, start_y + (i-1) * grid_height)
    if (c->get_waste_time() > 0) {
        p.drawEllipse(start_point.x() + (c->get_position().x() -1) * grid_width,
                      start_point.y() + (c->get_position().y() -1) * grid_width,
                      20, 20);
    }

//    qDebug() << "waste drawn ready";
}

void Canvas::drawChip() {
    QPainter p(this);
    QFont f;
    f.setPointSize(24);
    p.setFont(f);
    p.setPen(QPen(QColor(0, 0, 255, 100), 2));

    int start_x = 225;
    int start_y = 225;

    for (int i = 1; i <= row; ++i) {
        for (int j = 1; j <= column; ++j) {
            if (my_chip->visited.contains(QPair<int, int>(j, i))) {
                QPainter temp(this);
                temp.setPen(Qt::transparent);
                temp.setBrush(QColor(3, 169, 244, 32));
                temp.drawRect(QRect(QPoint(150 + j * grid_width, 150 + i * grid_height),
                QPoint(200 + j * grid_width, 200 + i * grid_height)));
            }
            if (my_chip->get_cell(j, i)->isBlocked) {
                QPainter temp(this);
                temp.setPen(Qt::transparent);
                temp.setBrush(QColor(250, 255, 240));
                temp.drawRect(QRect(QPoint(150 + j * grid_width, 150 + i * grid_height),
                           QPoint(200 + j * grid_width, 200 + i * grid_height)));
            }
            int number = my_chip->get_cell(j, i)->get_waste_time();
            QPainter temp(this);
//            qDebug() << number << "(" << i << "," << j << ")";

//            p.drawText(QPoint(start_x + (j-1) * grid_width, start_y + (i-1) * grid_height), QString::number(number));
//            qDebug() << "Lable drawn ready";
            drawWaterDrop(my_chip->get_cell(j, i)->get_occupied());
            drawWaste(my_chip->get_cell(j, i));
        }
    }
}

bool Canvas::isEnd() {
    int max_time = 0;
    for (auto cmd: command) {
        if (cmd.getTime() > max_time) {
            max_time = cmd.getTime();
        }
    }
    if (current_timestamp > max_time) {
        return true;
    }
    return false;
}

/*
void Canvas::perform_all_command() {
    if (isEnd()) return;
    qDebug() << "In performance";
    my_chip = my_chip->get_copy();

    for (auto& i : command) {
        qDebug() << current_timestamp << i.getTime();
        if (current_timestamp == i.getTime()) {
            qDebug() << "Command run";
            my_chip->doCommand(i);
            qDebug() << "Command end";
            this->update();
        }
    }
    AllChips.append(my_chip);

    ++current_timestamp;
}

void Canvas::perform_current_command() {
    if (isEnd()) return;
    qDebug() << "In performance";
    my_chip = my_chip->get_copy();
    for (auto& i : command) {
        qDebug() << current_timestamp << i.getTime();
        if (current_timestamp == i.getTime()) {
            qDebug() << "Command run";
            my_chip->doCommand(i);
            qDebug() << "Command end";
            this->update();
        }
    }
    AllChips.append(my_chip);
}
*/

void Canvas::keyPressEvent(QKeyEvent *ev) {

}

void Canvas::mouseMoveEvent(QMouseEvent *ev) {

}

void Canvas::mousePressEvent(QMouseEvent *ev) {
    qDebug() << "captured *********************************";
    qDebug() << ev->pos();
    int clickx = ev->x();
    int clicky = ev->y();
    if ((clickx >= 200 && clickx <= 200 + column * grid_width) && (clicky >= 200 &&  clicky <= 200 + row * grid_height)) {
        int x = (clickx - 200) / grid_width + 1;
        int y = (clicky - 200) / grid_height + 1;
        qDebug() << x << y;
        if (this->my_chip->get_cell(x, y)!= nullptr) {
            this->my_chip->get_cell(x, y)->change_blocked();
        }
        qDebug() << ev->x() << ev->y() << "]]]]]]]]]]]]";
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *ev) {

}

void Canvas::play() {
    emit music_begin();
    timer->start(500);
}

void Canvas::pause() {
    timer->stop();
}
