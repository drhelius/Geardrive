/********************************************************************************
** Form generated from reading UI file 'About.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QLabel *label;
    QPushButton *okButton;

    void setupUi(QDialog *About)
    {
        if (About->objectName().isEmpty())
            About->setObjectName(QStringLiteral("About"));
        About->resize(752, 284);
        label = new QLabel(About);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 731, 261));
        label->setTextFormat(Qt::RichText);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label->setOpenExternalLinks(true);
        okButton = new QPushButton(About);
        okButton->setObjectName(QStringLiteral("okButton"));
        okButton->setGeometry(QRect(500, 210, 161, 41));

        retranslateUi(About);
        QObject::connect(okButton, SIGNAL(clicked()), About, SLOT(accept()));

        QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QDialog *About)
    {
        About->setWindowTitle(QApplication::translate("About", "Dialog", 0));
        label->setText(QApplication::translate("About", "<html><head/><body><p align=\"center\"><span style=\" font-size:16pt; font-weight:600;\">Gearsystem</span></p><p align=\"center\"><span style=\" text-decoration: underline;\">Copyright \302\251 2013 by Ignacio Sanchez</span><br/><br/>Gearsystem is a Sega Master System / Game Gear emulator written in C++.</p><p align=\"center\">The emulator is focused on readability of source code, but nevertheless it has good compatibility.</p><p align=\"center\">A lot of effort has gone into this in order to follow OOP and keep it as simple as possible.</p><p><br/></p><p><span style=\" font-weight:600;\">Official web site: </span><a href=\"https://github.com/drhelius/Gearsystem\"><span style=\" text-decoration: underline; color:#0000ff;\">github.com/drhelius/Gearsystem</span></a></p><p><span style=\" font-weight:600;\">Author website</span>: <a href=\"http://www.ignaciosanchezgines.com\"><span style=\" text-decoration: underline; color:#0000ff;\">www.ignaciosanchezgines.com</span></a></p></body></html>", 0));
        okButton->setText(QApplication::translate("About", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
