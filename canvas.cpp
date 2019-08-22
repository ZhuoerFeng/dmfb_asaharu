#include "canvas.h"
#include "chip.h"
#include "waterdrop.h"
#include "command.h"
#include <QPen>
#include <QDebug>
#include <QLabel>

Canvas::Canvas(QWidget* parent) : QWidget(parent)
{
    row = 0;
    column = 0;
    grid_height = 50;
    grid_width = 50;
    start_point.setX(200);
    start_point.setY(200);
//    qDebug() << "Riichi";
//    qDebug() << "Rooooon!";
    //还需要判断是否有非法输入
}

//column, row, input_x, input_y, output_x, output_y
void Canvas::init(int _column, int _row, QVector<int> input_x, QVector<int> input_y, int output_x, int output_y) {
    qDebug() << "canvas initializing...";
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
    qDebug() << "Init Riichi";
    my_chip.init(column, row, Input, Output);
    qDebug() << "Init Ron!";
}

void Canvas::drawGrid(QPainter& p) {
    QPoint line_end_point;
    line_end_point.setX(start_point.x() + column * grid_width);
    line_end_point.setY(start_point.y() + row * grid_height);
    QPoint now_start_point = start_point;
    for (int i = 0; i <= row; ++i) {
        now_start_point.setY(start_point.y() + i * grid_height);
        QPoint now_end_point = line_end_point;
        now_end_point.setY(now_end_point.y() + (i - row) * grid_height);

        p.drawLine(now_start_point, now_end_point);
    }
    now_start_point = start_point;
    for (int j = 0; j <= column; ++j) {
        now_start_point.setX(start_point.x() + j * grid_width);
        QPoint now_end_point = line_end_point;
        now_end_point.setX(line_end_point.x() + (j - column) * grid_width);
        qDebug() << "draw";

        p.drawLine(now_start_point, now_end_point);
    }
}

void Canvas::drawTerminal(QPainter& p, QPoint point, QColor c, QString s) {
    qDebug() << "fuck";
    qDebug() << point;
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

//前半部分没问题，setChip有问题
void Canvas::paintEvent(QPaintEvent *ev) {
    if (row == 0 || column == 0) {
        return;
    }
    qDebug() << row << " " << column;
//    if (!should_paint) return;
    QPainter p(this);
    p.setPen(QPen(QColor(0, 0, 0), 4));
    p.setBrush(QColor(255, 0, 0, 120));
      //draw grids
    drawGrid(p);
    //输入端口 输出端口
    for (auto& input : Input) {
        this->drawTerminal(p, input, QColor(0, 255, 255), "INPUT");
    }
    this->drawTerminal(p, Output, QColor(116, 52, 129), "OUTPUT");

//    画水滴
//    ellipse yigedian
//    p.setPen(QPen(QColor(255, 255, 255), 10));
//    p.drawPoint(125, 75);

    // 画点
    /*
    QPoint upper_left_point = line_start_point + QPoint(12.5, 12.5);
    for (int i = 0; i < column; ++i) {
        for (int j = 0; j < row; ++j) {
            p.drawEllipse(upper_left_point.x() + i * 50, upper_left_point.y() + j * 50, 25, 25);
        }
    }
    */



    should_paint = false;
    setChip();
}

void Canvas::setChip() {
    QPainter p(this);
    QFont f;
    f.setPointSize(24);
    p.setFont(f);
    p.setPen(QPen(QColor(0, 0, 255, 100), 2));
//    p.drawLine(200, 200, 500, 500);
//    p.drawLine(200, 600, 600, 200);
    int start_x = 225;
    int start_y = 225;
    for (int i = 1; i <= column; ++i) {
        for (int j = 1; j <= row; ++j) {
            int number = my_chip.get_cell(i, j)->get_waste_time();
            //为什么下方两个qdebug之间不能划线？
            QPainter temp(this);
//            temp.setPen(QPen(QColor(0, 0, 255, 100), 2));
            qDebug() << number << "(" << i << "," << j << ")";
//            temp.drawLine(200, 200, 500, 500);
//            p.drawLine(200, 200, 500, 500);
            p.drawText(QPoint(start_x + (i-1) * grid_width, start_y + (j-1) * grid_height), QString::number(number));
//            qDebug() << "OK1";
            drawWaste(my_chip.get_cell(i, j));
//            qDebug() << "OK2";
            drawWaterDrop(my_chip.get_cell(i, j)->get_occupied());
//            qDebug() << "OK3";
        }
    }
}

//没问题
void Canvas::drawWaterDrop(Waterdrop *w) {
//    qDebug() << "drop Riichi";
    if (w == nullptr) return;
    qDebug() << "drop Chii";
    QPainter p(this);
    p.setPen(QPen(QColor(0, 0, 0), 1));
    p.setBrush(w->get_color());
    QPoint* current = w->get_position();
    int radius = 20;
    if (w->get_status() == NORMAL) {
        p.drawEllipse(current->x() - radius, current->y() - radius, radius, radius);
    } else if (w->get_status() == BIG_H) {
        p.drawEllipse(current->x() - 3 * radius, current->y() - radius, 3 * radius, radius);
    } else if (w->get_status() == BIG_V) {
        p.drawEllipse(current->x() - radius, current->y() - radius * 3, radius, 3 * radius);
    }
//    qDebug() << "drop Ron!";
}

//没问题
void Canvas::drawWaste(Cell *c) {
//    qDebug() << "waste Riichi";
    if (c == nullptr) return;
    QPainter p(this);
    QFont f;
    f.setPointSize(24);
    p.setFont(f);
    p.setPen(QPen(QColor(0, 0, 255, 100), 2));
    int num = c->get_waste_time();
    p.setBrush(QColor(121, 0, 255, (255 - 25 * num)));
    p.drawEllipse(c->get_position().x(), c->get_position().y(), 15, 15);
    p.drawText(c->get_position().x(), c->get_position().y(), QString::number(num));
//    qDebug() << "waste Ron!";
}

void Canvas::perform_command() {
//    for (auto i : command) {
//        if (current_timestamp == i.getTime()) {
//            qDebug() << "Command run";
//            my_chip.doCommand(i);
//            qDebug() << "Command end";
//        }
//    }
}

void Canvas::keyPressEvent(QKeyEvent *ev) {

}

void Canvas::mouseMoveEvent(QMouseEvent *ev) {

}

void Canvas::mousePressEvent(QMouseEvent *ev) {

}

void Canvas::mouseReleaseEvent(QMouseEvent *ev) {

}

void Canvas::pause() {

}

void Canvas::refresh() {

}

void Canvas::next() {

}

void Canvas::prev() {

}

