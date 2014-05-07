/********************************************************************************
** Form generated from reading UI file 'VideoSettings.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOSETTINGS_H
#define UI_VIDEOSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_VideoSettings
{
public:
    QDialogButtonBox *buttonBox;
    QCheckBox *checkBoxFilter;

    void setupUi(QDialog *VideoSettings)
    {
        if (VideoSettings->objectName().isEmpty())
            VideoSettings->setObjectName(QStringLiteral("VideoSettings"));
        VideoSettings->setWindowModality(Qt::NonModal);
        VideoSettings->resize(324, 128);
        VideoSettings->setModal(false);
        buttonBox = new QDialogButtonBox(VideoSettings);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(20, 70, 291, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        checkBoxFilter = new QCheckBox(VideoSettings);
        checkBoxFilter->setObjectName(QStringLiteral("checkBoxFilter"));
        checkBoxFilter->setGeometry(QRect(30, 30, 161, 20));

        retranslateUi(VideoSettings);
        QObject::connect(buttonBox, SIGNAL(accepted()), VideoSettings, SLOT(PressedOK()));
        QObject::connect(buttonBox, SIGNAL(rejected()), VideoSettings, SLOT(PressedCancel()));

        QMetaObject::connectSlotsByName(VideoSettings);
    } // setupUi

    void retranslateUi(QDialog *VideoSettings)
    {
        VideoSettings->setWindowTitle(QApplication::translate("VideoSettings", "Video Settings", 0));
        checkBoxFilter->setText(QApplication::translate("VideoSettings", "Bilinear Filtering", 0));
    } // retranslateUi

};

namespace Ui {
    class VideoSettings: public Ui_VideoSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOSETTINGS_H
