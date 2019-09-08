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
#include <QSound>
#include <QObject>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    canvas(new Canvas(this)),
    time(0), runner(canvas->get_command_runner())
{
    ui->setupUi(this);
    //??
//    canvas->installEventFilter(this);
    this->installEventFilter(this);
    this->background = new QSound(":/sound/src/sound/zhongxialvdong.wav");
    setFixedSize(this->width(), this->height());
    canvas->should_paint = true;
    connect(canvas, &Canvas::play_over, [=]() {
        on_actionPause_triggered();
    });
    connect(canvas, &Canvas::music_begin, [=]() {
        background->play();
    });

    connect(canvas->get_command_runner(), &CommandFactory::error_found, [=]() {
        canvas->timer->stop();
        ui->actionWash->setEnabled(false);
        ui->actionNew->setEnabled(false);
        ui->actionLoad->setEnabled(true);
        ui->actionRun->setEnabled(false);
        ui->actionNext->setEnabled(false);
        ui->actionPrev->setEnabled(false);
        ui->actionPause->setEnabled(false);
        ui->actionRefresh->setEnabled(true);
    });
    background = new QSound(":/sound/src/sound/zhongxialvdong.wav");

    canvas->setGeometry(20, 20, 1000, 1000);
    canvas->resize(2000, 2000);

    ui->actionNew->setEnabled(true);
    ui->actionLoad->setEnabled(false);
    ui->actionRun->setEnabled(false);
    ui->actionNext->setEnabled(false);
    ui->actionPrev->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->actionRefresh->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete canvas;
}

void MainWindow::mousePressEvent ( QMouseEvent * e ) {
    this->canvas->mousePressEvent(e);
}

void MainWindow::on_actionNew_triggered()
{
    auto* newDialogSession = new DialogNew;
    connect(newDialogSession, &DialogNew::parametersReady, canvas, &Canvas::init);
    qDebug() << "new triggered...";
    newDialogSession->exec();
    delete newDialogSession;
    ui->actionRun->setEnabled(false);
    ui->actionLoad->setEnabled(true);
    ui->actionNext->setEnabled(false);
    ui->actionPrev->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->actionRefresh->setEnabled(false);

    ui->actionNew->setEnabled(false);
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

CommandFactory* MainWindow::get_runner() {
    return canvas->get_command_runner();
}

void MainWindow::on_actionLoad_triggered()
{
    auto filename = QFileDialog::getOpenFileName(nullptr, "Open a file",
                                                 QDir::homePath(), "*.txt");
    runner->filen = filename;
    if (!filename.isEmpty()) {
        QFile f(filename);
        if (!f.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, "unable to open file", f.errorString());
            return;
        }
        QString info = f.readAll();
//        auto cmdSet = Command::parse_command_set(info);
//        for (auto i: cmdSet) {
//            i.printCommand();
//        }
        ui->textEdit->setText(info);
        this->canvas->initCommand(info); // parsing
    }

    ui->actionLoad->setEnabled(false);

    ui->actionRun->setEnabled(true);
    ui->actionNext->setEnabled(false);
    ui->actionPrev->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->actionRefresh->setEnabled(false);
    ui->actionWash->setEnabled(true);

}

void MainWindow::on_actionRun_triggered()
{
    qDebug() << "Run ";
    this->canvas->play();
    ui->actionRun->setEnabled(false);
    ui->actionNext->setEnabled(false);
    ui->actionPrev->setEnabled(false);
    ui->actionPause->setEnabled(true);
    ui->actionRefresh->setEnabled(false);
    ui->actionWash->setEnabled(false);
}

void MainWindow::on_actionNext_triggered()
{
    this->canvas->pause();
    this->canvas->get_command_runner()->next();
    ui->actionWash->setEnabled(true);
}

void MainWindow::on_actionPrev_triggered()
{
    this->canvas->pause();
    this->canvas->get_command_runner()->prev();
    ui->actionWash->setEnabled(true);
}

void MainWindow::on_actionPause_triggered()
{
    this->canvas->pause();
    ui->actionRun->setEnabled(true);
    ui->actionNext->setEnabled(true);
    ui->actionPrev->setEnabled(true);
    ui->actionPause->setEnabled(false);
    ui->actionRefresh->setEnabled(true);
    ui->actionWash->setEnabled(true);

}

void MainWindow::on_actionRefresh_triggered()
{
//    on_actionPause_triggered();
    this->canvas->get_command_runner()->reset();
    ui->actionLoad->setEnabled(false);

    ui->actionRun->setEnabled(true);
    ui->actionNext->setEnabled(false);
    ui->actionPrev->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->actionRefresh->setEnabled(false);
    ui->actionWash->setEnabled(true);
}

void MainWindow::on_actionMusic_triggered(bool checked)
{
    if (checked == true) {
        background->play();
    } else {
        background->stop();
    }
}

void MainWindow::on_actionWash_triggered(bool checked)
{
    this->runner->washMode = (checked);
    qDebug() << checked;
}
