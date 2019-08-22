#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "canvas.h"
#include "dialognew.h"
#include "command.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent* event);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionLoad_triggered();

private:
    Ui::MainWindow *ui;
    Canvas* canvas;
    int time;
};

#endif // MAINWINDOW_H
