/********************************************************************************
** Form generated from reading UI file 'InputSettings.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INPUTSETTINGS_H
#define UI_INPUTSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_InputSettings
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QLineEdit *lineEdit1;
    QLabel *label;
    QLineEdit *lineEditUp;
    QLineEdit *lineEditDown;
    QLineEdit *lineEditLeft;
    QLabel *label_2;
    QLineEdit *lineEditRight;
    QLineEdit *lineEdit2;
    QLabel *label_6;
    QLineEdit *lineEditStart;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_3;
    QLabel *label_7;
    QGroupBox *groupBox_2;
    QLineEdit *lineEdit1_2;
    QLabel *label_8;
    QLineEdit *lineEditUp_2;
    QLineEdit *lineEditDown_2;
    QLineEdit *lineEditLeft_2;
    QLabel *label_9;
    QLineEdit *lineEditRight_2;
    QLineEdit *lineEdit2_2;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;

    void setupUi(QDialog *InputSettings)
    {
        if (InputSettings->objectName().isEmpty())
            InputSettings->setObjectName(QStringLiteral("InputSettings"));
        InputSettings->setWindowModality(Qt::WindowModal);
        InputSettings->resize(412, 374);
        InputSettings->setModal(true);
        buttonBox = new QDialogButtonBox(InputSettings);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(50, 330, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        groupBox = new QGroupBox(InputSettings);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 391, 141));
        lineEdit1 = new QLineEdit(groupBox);
        lineEdit1->setObjectName(QStringLiteral("lineEdit1"));
        lineEdit1->setGeometry(QRect(270, 20, 111, 22));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 20, 62, 16));
        lineEditUp = new QLineEdit(groupBox);
        lineEditUp->setObjectName(QStringLiteral("lineEditUp"));
        lineEditUp->setGeometry(QRect(70, 20, 111, 22));
        lineEditUp->setReadOnly(false);
        lineEditDown = new QLineEdit(groupBox);
        lineEditDown->setObjectName(QStringLiteral("lineEditDown"));
        lineEditDown->setGeometry(QRect(70, 80, 111, 22));
        lineEditLeft = new QLineEdit(groupBox);
        lineEditLeft->setObjectName(QStringLiteral("lineEditLeft"));
        lineEditLeft->setGeometry(QRect(70, 110, 111, 22));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 50, 62, 16));
        lineEditRight = new QLineEdit(groupBox);
        lineEditRight->setObjectName(QStringLiteral("lineEditRight"));
        lineEditRight->setGeometry(QRect(70, 50, 111, 22));
        lineEdit2 = new QLineEdit(groupBox);
        lineEdit2->setObjectName(QStringLiteral("lineEdit2"));
        lineEdit2->setGeometry(QRect(270, 50, 111, 22));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(210, 20, 62, 16));
        lineEditStart = new QLineEdit(groupBox);
        lineEditStart->setObjectName(QStringLiteral("lineEditStart"));
        lineEditStart->setGeometry(QRect(270, 80, 111, 22));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 110, 62, 16));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(210, 50, 62, 16));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 80, 62, 16));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(210, 80, 62, 16));
        groupBox_2 = new QGroupBox(InputSettings);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 160, 391, 141));
        lineEdit1_2 = new QLineEdit(groupBox_2);
        lineEdit1_2->setObjectName(QStringLiteral("lineEdit1_2"));
        lineEdit1_2->setGeometry(QRect(270, 20, 111, 22));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 20, 62, 16));
        lineEditUp_2 = new QLineEdit(groupBox_2);
        lineEditUp_2->setObjectName(QStringLiteral("lineEditUp_2"));
        lineEditUp_2->setGeometry(QRect(70, 20, 111, 22));
        lineEditUp_2->setReadOnly(false);
        lineEditDown_2 = new QLineEdit(groupBox_2);
        lineEditDown_2->setObjectName(QStringLiteral("lineEditDown_2"));
        lineEditDown_2->setGeometry(QRect(70, 80, 111, 22));
        lineEditLeft_2 = new QLineEdit(groupBox_2);
        lineEditLeft_2->setObjectName(QStringLiteral("lineEditLeft_2"));
        lineEditLeft_2->setGeometry(QRect(70, 110, 111, 22));
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(10, 50, 62, 16));
        lineEditRight_2 = new QLineEdit(groupBox_2);
        lineEditRight_2->setObjectName(QStringLiteral("lineEditRight_2"));
        lineEditRight_2->setGeometry(QRect(70, 50, 111, 22));
        lineEdit2_2 = new QLineEdit(groupBox_2);
        lineEdit2_2->setObjectName(QStringLiteral("lineEdit2_2"));
        lineEdit2_2->setGeometry(QRect(270, 50, 111, 22));
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(210, 20, 62, 16));
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(10, 110, 62, 16));
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(210, 50, 62, 16));
        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(10, 80, 62, 16));
        QWidget::setTabOrder(lineEditUp, lineEditRight);
        QWidget::setTabOrder(lineEditRight, lineEditDown);
        QWidget::setTabOrder(lineEditDown, lineEditLeft);
        QWidget::setTabOrder(lineEditLeft, lineEdit1);
        QWidget::setTabOrder(lineEdit1, lineEdit2);
        QWidget::setTabOrder(lineEdit2, lineEditStart);
        QWidget::setTabOrder(lineEditStart, buttonBox);

        retranslateUi(InputSettings);
        QObject::connect(buttonBox, SIGNAL(accepted()), InputSettings, SLOT(SaveKeys()));
        QObject::connect(buttonBox, SIGNAL(rejected()), InputSettings, SLOT(RestoreKeys()));

        QMetaObject::connectSlotsByName(InputSettings);
    } // setupUi

    void retranslateUi(QDialog *InputSettings)
    {
        InputSettings->setWindowTitle(QApplication::translate("InputSettings", "Input Settings", 0));
        groupBox->setTitle(QApplication::translate("InputSettings", "Player 1", 0));
        label->setText(QApplication::translate("InputSettings", "Up:", 0));
        label_2->setText(QApplication::translate("InputSettings", "Right:", 0));
        label_6->setText(QApplication::translate("InputSettings", "1:", 0));
        label_4->setText(QApplication::translate("InputSettings", "Left:", 0));
        label_5->setText(QApplication::translate("InputSettings", "2:", 0));
        label_3->setText(QApplication::translate("InputSettings", "Down:", 0));
        label_7->setText(QApplication::translate("InputSettings", "Start:", 0));
        groupBox_2->setTitle(QApplication::translate("InputSettings", "Player 2", 0));
        label_8->setText(QApplication::translate("InputSettings", "Up:", 0));
        label_9->setText(QApplication::translate("InputSettings", "Right:", 0));
        label_10->setText(QApplication::translate("InputSettings", "1:", 0));
        label_11->setText(QApplication::translate("InputSettings", "Left:", 0));
        label_12->setText(QApplication::translate("InputSettings", "2:", 0));
        label_13->setText(QApplication::translate("InputSettings", "Down:", 0));
    } // retranslateUi

};

namespace Ui {
    class InputSettings: public Ui_InputSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INPUTSETTINGS_H
