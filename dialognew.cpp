#include "dialognew.h"
#include "mainwindow.h"
#include "ui_dialognew.h"
#include <QObject>
#include <QDebug>
#include <QMessageBox>
#include <QVector>
#include <QPoint>
#include <QRegExp>
#include <QLineEdit>
#include <QPalette>

DialogNew::DialogNew(QWidget* parent) :
    QDialog (parent),
    ui(new Ui::DialogNew)
{
    ui->setupUi(this);

    QFont f;
    f.setPointSize(12);
//    f.setFamily("Menlo");

    ui->ColumnLabel->setFont(f);
    ui->RowLabel->setFont(f);
    ui->InputXLabel->setFont(f);
    ui->InputYLabel->setFont(f);
    ui->OutputXLabel->setFont(f);
    ui->OutputYLabel->setFont(f);

    f.setPointSize(20);
    ui->groupBox->setFont(f);

    connect(ui->RowLine, &QLineEdit::textChanged, this, &DialogNew::update_info);
    connect(ui->ColumnLine, &QLineEdit::textChanged, this, &DialogNew::update_info);
    connect(ui->InputXLine, &QLineEdit::textChanged, this, &DialogNew::update_info);
    connect(ui->InputYLine, &QLineEdit::textChanged, this, &DialogNew::update_info);
    connect(ui->OutputXLine, &QLineEdit::textChanged, this, &DialogNew::update_info);
    connect(ui->OutputYLine, &QLineEdit::textChanged, this, &DialogNew::update_info);
}

DialogNew::~DialogNew() {

}

void DialogNew::on_toolButton_clicked()
{
    qDebug() << "tool button clicked" << endl;
    QString info = "You are trying to type in the terminal coordinates.\nIf there are more than one terminals, please use one space to seperate them.\nHere is an example:\n";
    QString example = "There are two terminials, (0, 1), (0, 3) respectively. You can type in \"0 0\" in input_X, \"1 3\" in input_Y";
    int ret = QMessageBox::information(nullptr, "Input Terminal", info + example, QMessageBox::Ok);
}

void DialogNew::on_pushButton_clicked()
{
    qDebug() << "load file button clicked." << endl;
    ui->InputXLine->setText("nima fuck!");
}

void DialogNew::on_buttonBox_accepted()
{

}

void DialogNew::update_info() {
    int column;
    int row;
    QFont f;
    QPalette p = ui->pushButtonCheck->palette();
    p.setColor(QPalette::ButtonText, QColor(255, 0, 0));
    ui->pushButtonCheck->setPalette(p);
    // 检查row column
    if (!((ui->ColumnLine->text().toInt() > 3) && (ui->RowLine->text().toInt() > 3))) {
        qDebug() << "Row & column error" << endl;
        ui->pushButtonCheck->setText("Row & column error");
        return;
    }
    column = ui->ColumnLine->text().toInt();
    row = ui->RowLine->text().toInt();

    QVector<int> input_x;
    QVector<int> input_y;
    int output_x;
    int output_y;
    QString inputX = ui->InputXLine->text();

    //检查input X Y
    for (auto i: inputX.split(" ")) {
        if (i.length()) {
            input_x.append(i.toInt());
        }
    }

    QString inputY = ui->InputYLine->text();
    for (auto i: inputY.split(" ")) {
        if (i.length()) {
            input_y.append(i.toInt());
        }
    }

    if (input_x.size() != input_y.size()) {
        qDebug() << "input Xs not equal to output Ys" << endl;
        ui->pushButtonCheck->setText("input Xs not equal to output Ys");
        return;
    }

    for (int i = 0; i < input_x.size(); ++i) {
        if (!( ((input_x[i] == 0) && (input_y[i] >= 1 && input_y[i] <= row)) ||
               ((input_x[i] == column + 1) && (input_y[i] >= 1 && input_y[i] <= row)) ||
               ((input_y[i] == 0) && (input_x[i] >= 1 && input_x[i] <= column)) ||
               ((input_y[i] == row + 1) && (input_x[i] >= 1 && input_x[i] <= column))))
        {
            qDebug() << "input terminal not on the margin";
            ui->pushButtonCheck->setText("input terminal not on the margin");
            return;
        }
    }

    // 检查output
    output_x = ui->OutputXLine->text().toInt();
    output_y = ui->OutputYLine->text().toInt();

    if (!( ((output_x == 0) && (output_y >= 1 && output_y <= row)) ||
           ((output_x == column + 1) && (output_y >= 1 && output_y <= row)) ||
           ((output_y == 0) && (output_x >= 1 && output_x <= column)) ||
           ((output_y == row + 1) && (output_x >= 1 && output_x <= column))))
    {
        qDebug() << "output terminal not on the margin";
        ui->pushButtonCheck->setText("output terminal not on the margin");
        return;
    }

    qDebug() << "accepted!";
    ui->pushButtonCheck->setText("accepted!");
    emit parametersReady(column, row, input_x, input_y, output_x, output_y);
//    qDebug() << "signals emitted .....hahaha";
    return;
}

void DialogNew::on_pushButtonCheck_clicked() {
    qDebug() << "tool button clicked" << endl;
    QString info = "You are trying to type in the terminal coordinates.\nIf there are more than one terminals, please use one space to seperate them.\nHere is an example:\n";
    QString example = "There are two terminials, (0, 1), (0, 3) respectively. You can type in \"0 0\" in input_X, \"1 3\" in input_Y";
    QMessageBox::information(nullptr, "Input Terminal", info + example, QMessageBox::Ok);
}

