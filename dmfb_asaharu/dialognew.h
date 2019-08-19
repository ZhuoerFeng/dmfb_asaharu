#ifndef DIALOGNEW_H
#define DIALOGNEW_H
#include <QDialog>

namespace Ui {
class DialogNew;
}

class DialogNew : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNew(QWidget *parent = nullptr);
    ~DialogNew();

//private slots:
//    void on_actionNew_triggered();

private:
    Ui::DialogNew *ui;
};
#endif // DIALOGNEW_H
