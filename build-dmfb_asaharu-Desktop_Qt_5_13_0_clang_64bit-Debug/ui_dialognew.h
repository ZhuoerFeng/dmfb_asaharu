/********************************************************************************
** Form generated from reading UI file 'dialognew.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGNEW_H
#define UI_DIALOGNEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DialogNew
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *RowLabel;
    QLabel *OutputYLabel;
    QLineEdit *OutputYLine;
    QLabel *InputYLabel;
    QLineEdit *OutputXLine;
    QLineEdit *InputXLine;
    QLabel *ColumnLabel;
    QLabel *OutputXLabel;
    QLineEdit *ColumnLine;
    QLineEdit *RowLine;
    QLabel *InputXLabel;
    QLineEdit *InputYLine;
    QPushButton *pushButton;

    void setupUi(QDialog *DialogNew)
    {
        if (DialogNew->objectName().isEmpty())
            DialogNew->setObjectName(QString::fromUtf8("DialogNew"));
        DialogNew->resize(488, 491);
        buttonBox = new QDialogButtonBox(DialogNew);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(70, 420, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        groupBox = new QGroupBox(DialogNew);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(80, 60, 341, 321));
        groupBox->setAlignment(Qt::AlignCenter);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        RowLabel = new QLabel(groupBox);
        RowLabel->setObjectName(QString::fromUtf8("RowLabel"));

        gridLayout->addWidget(RowLabel, 1, 0, 1, 1);

        OutputYLabel = new QLabel(groupBox);
        OutputYLabel->setObjectName(QString::fromUtf8("OutputYLabel"));

        gridLayout->addWidget(OutputYLabel, 5, 0, 1, 2);

        OutputYLine = new QLineEdit(groupBox);
        OutputYLine->setObjectName(QString::fromUtf8("OutputYLine"));

        gridLayout->addWidget(OutputYLine, 5, 2, 1, 1);

        InputYLabel = new QLabel(groupBox);
        InputYLabel->setObjectName(QString::fromUtf8("InputYLabel"));

        gridLayout->addWidget(InputYLabel, 3, 0, 1, 2);

        OutputXLine = new QLineEdit(groupBox);
        OutputXLine->setObjectName(QString::fromUtf8("OutputXLine"));

        gridLayout->addWidget(OutputXLine, 4, 2, 1, 1);

        InputXLine = new QLineEdit(groupBox);
        InputXLine->setObjectName(QString::fromUtf8("InputXLine"));

        gridLayout->addWidget(InputXLine, 2, 2, 1, 1);

        ColumnLabel = new QLabel(groupBox);
        ColumnLabel->setObjectName(QString::fromUtf8("ColumnLabel"));

        gridLayout->addWidget(ColumnLabel, 0, 0, 1, 1);

        OutputXLabel = new QLabel(groupBox);
        OutputXLabel->setObjectName(QString::fromUtf8("OutputXLabel"));

        gridLayout->addWidget(OutputXLabel, 4, 0, 1, 2);

        ColumnLine = new QLineEdit(groupBox);
        ColumnLine->setObjectName(QString::fromUtf8("ColumnLine"));

        gridLayout->addWidget(ColumnLine, 0, 2, 1, 1);

        RowLine = new QLineEdit(groupBox);
        RowLine->setObjectName(QString::fromUtf8("RowLine"));

        gridLayout->addWidget(RowLine, 1, 2, 1, 1);

        InputXLabel = new QLabel(groupBox);
        InputXLabel->setObjectName(QString::fromUtf8("InputXLabel"));

        gridLayout->addWidget(InputXLabel, 2, 0, 1, 2);

        InputYLine = new QLineEdit(groupBox);
        InputYLine->setObjectName(QString::fromUtf8("InputYLine"));

        gridLayout->addWidget(InputYLine, 3, 2, 1, 1);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 6, 0, 1, 1);


        retranslateUi(DialogNew);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogNew, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogNew, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogNew);
    } // setupUi

    void retranslateUi(QDialog *DialogNew)
    {
        DialogNew->setWindowTitle(QCoreApplication::translate("DialogNew", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("DialogNew", "Please input the column and rows:", nullptr));
        RowLabel->setText(QCoreApplication::translate("DialogNew", "Row", nullptr));
        OutputYLabel->setText(QCoreApplication::translate("DialogNew", "output y coordinate", nullptr));
        InputYLabel->setText(QCoreApplication::translate("DialogNew", "input y coordinate", nullptr));
        ColumnLabel->setText(QCoreApplication::translate("DialogNew", "Column", nullptr));
        OutputXLabel->setText(QCoreApplication::translate("DialogNew", "output x coordinate", nullptr));
        InputXLabel->setText(QCoreApplication::translate("DialogNew", "input x coordinate", nullptr));
        pushButton->setText(QCoreApplication::translate("DialogNew", "Read from file...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogNew: public Ui_DialogNew {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGNEW_H
