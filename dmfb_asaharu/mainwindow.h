#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "ui_dialognew.h"

namespace Ui {
class MainWindow;
class DialogNew;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_buttonBox_accepted();

private:
    Ui::MainWindow *ui;
    Ui::DialogNew* dialog_ui;
};

#endif // MAINWINDOW_H
