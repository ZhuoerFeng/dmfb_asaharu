#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "canvas.h"
#include "dialognew.h"
#include "command.h"
#include <QSound>

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

    QSound* background;

    CommandFactory* get_runner();

private slots:
    void on_actionNew_triggered();

    void on_actionLoad_triggered();

    void on_actionRun_triggered();

    void on_actionNext_triggered();

    void on_actionPrev_triggered();

    void on_actionPause_triggered();

    void on_actionRefresh_triggered();

    void on_actionMusic_triggered(bool checked);

    void on_actionWash_triggered(bool checked);

private:
    Ui::MainWindow *ui;
    Canvas* canvas;
    int time;
};

#endif // MAINWINDOW_H
