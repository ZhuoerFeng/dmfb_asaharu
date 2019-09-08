#ifndef DIALOGNEW_H
#define DIALOGNEW_H

#include <QDialog>
#include <QVector>

namespace Ui {
class DialogNew;
}

class DialogNew : public QDialog
{
    Q_OBJECT

public:
    DialogNew(QWidget* parent = nullptr);
    ~DialogNew();

signals:
    void parametersReady(int, int, QVector<int>, QVector<int>, int, int, int, int, int, int);

public slots:
    void update_info();

private slots:
    void on_toolButton_clicked();
    void on_pushButton_clicked();
    void on_pushButtonCheck_clicked();
    void on_buttonBox_accepted();

private:
    Ui::DialogNew* ui;
};

#endif // DIALOGNEW_H
