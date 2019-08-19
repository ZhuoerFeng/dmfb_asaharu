#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_dialognew.h"
#include <QMessageBox>
#include <QFont>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    auto* dialog = new QDialog;
    auto* newDialogSession = new Ui::DialogNew;
    newDialogSession->setupUi(dialog);

    QFont f;
    f.setPointSize(16);
    f.setFamily("Menlo");

    newDialogSession->ColumnLabel->setFont(f);
    newDialogSession->RowLabel->setFont(f);
    newDialogSession->InputXLabel ->setFont(f);
    newDialogSession->InputYLabel->setFont(f);
    newDialogSession->OutputXLabel->setFont(f);
    newDialogSession->OutputYLabel->setFont(f);

    f.setPointSize(20);
    newDialogSession->groupBox->setFont(f);

    dialog->show();
}

void MainWindow::on_buttonBox_accepted()
{

}
