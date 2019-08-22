#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_dialognew.h"
#include <QMessageBox>
#include <QFont>
#include <QDebug>
#include <QObject>
#include <QDir>
#include <QFile>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    canvas(new Canvas(this)),
    time(0)
{
    ui->setupUi(this);
    setFixedSize(this->width(), this->height());
    canvas->should_paint = true;


    canvas->setGeometry(20, 20, 1000, 1000);
    canvas->resize(2000, 2000);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete canvas;
}

void MainWindow::on_actionNew_triggered()
{
    auto* newDialogSession = new DialogNew;
    connect(newDialogSession, &DialogNew::parametersReady, canvas, &Canvas::init);
    qDebug() << "new triggered...";
    newDialogSession->exec();
    delete newDialogSession;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    int ret = QMessageBox::warning(0, "Quit", "Are you sure to quit?",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}


void MainWindow::on_actionLoad_triggered()
{
    auto filename = QFileDialog::getOpenFileName(nullptr, "Open a file",
                                                 QDir::homePath(), "*.txt");
    if (!filename.isEmpty()) {
        QFile f(filename);
        if (!f.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, "unable to open file", f.errorString());
            return;
        }
        QString info = f.readAll();
        auto cmdSet = Command::parse_command_set(info);
        for (auto i: cmdSet) {
            i.printCommand();
        }
        ui->textEdit->setText(info);

    }



}
