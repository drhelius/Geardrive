/********************************************************************************
** Form generated from reading UI file 'SoundSettings.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOUNDSETTINGS_H
#define UI_SOUNDSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_SoundSettings
{
public:
    QDialogButtonBox *buttonBox;
    QComboBox *comboBoxSampleRate;
    QLabel *label;
    QCheckBox *checkBoxSoundEnabled;

    void setupUi(QDialog *SoundSettings)
    {
        if (SoundSettings->objectName().isEmpty())
            SoundSettings->setObjectName(QStringLiteral("SoundSettings"));
        SoundSettings->setWindowModality(Qt::WindowModal);
        SoundSettings->resize(439, 120);
        SoundSettings->setModal(true);
        buttonBox = new QDialogButtonBox(SoundSettings);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(150, 70, 251, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        comboBoxSampleRate = new QComboBox(SoundSettings);
        comboBoxSampleRate->setObjectName(QStringLiteral("comboBoxSampleRate"));
        comboBoxSampleRate->setGeometry(QRect(310, 15, 111, 22));
        label = new QLabel(SoundSettings);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(210, 17, 121, 21));
        checkBoxSoundEnabled = new QCheckBox(SoundSettings);
        checkBoxSoundEnabled->setObjectName(QStringLiteral("checkBoxSoundEnabled"));
        checkBoxSoundEnabled->setGeometry(QRect(30, 20, 141, 17));

        retranslateUi(SoundSettings);
        QObject::connect(buttonBox, SIGNAL(accepted()), SoundSettings, SLOT(PressedOK()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SoundSettings, SLOT(PressedCancel()));

        QMetaObject::connectSlotsByName(SoundSettings);
    } // setupUi

    void retranslateUi(QDialog *SoundSettings)
    {
        SoundSettings->setWindowTitle(QApplication::translate("SoundSettings", "Sound Settings", 0));
        label->setText(QApplication::translate("SoundSettings", "Sample Rate:", 0));
        checkBoxSoundEnabled->setText(QApplication::translate("SoundSettings", "Sound Enabled", 0));
    } // retranslateUi

};

namespace Ui {
    class SoundSettings: public Ui_SoundSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOUNDSETTINGS_H
