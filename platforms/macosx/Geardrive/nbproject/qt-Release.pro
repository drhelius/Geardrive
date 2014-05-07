# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/CLang-MacOSX
TARGET = Geardrive
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
PKGCONFIG +=
QT = core gui widgets opengl
SOURCES += ../../../src/Audio.cpp ../../../src/Cartridge.cpp ../../../src/GeardriveCore.cpp ../../../src/Input.cpp ../../qt-shared/About.cpp ../../qt-shared/Emulator.cpp ../../qt-shared/GLFrame.cpp ../../qt-shared/InputSettings.cpp ../../qt-shared/MainWindow.cpp ../../qt-shared/RenderThread.cpp ../../qt-shared/SoundSettings.cpp ../../qt-shared/VideoSettings.cpp ../../qt-shared/main.cpp
HEADERS += ../../../src/Audio.h ../../../src/Cartridge.h ../../../src/Geardrive.h ../../../src/GeardriveCore.h ../../../src/Input.h ../../../src/definitions.h ../../qt-shared/About.h ../../qt-shared/Emulator.h ../../qt-shared/GLFrame.h ../../qt-shared/InputSettings.h ../../qt-shared/MainWindow.h ../../qt-shared/RenderThread.h ../../qt-shared/SoundSettings.h ../../qt-shared/VideoSettings.h
FORMS += ../../qt-shared/About.ui ../../qt-shared/InputSettings.ui ../../qt-shared/MainWindow.ui ../../qt-shared/SoundSettings.ui ../../qt-shared/VideoSettings.ui
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Release/CLang-MacOSX
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = clang
QMAKE_CXX = clang++
DEFINES += 
INCLUDEPATH += /usr/local/include 
LIBS += -lSDL2main -lSDL2  
QMAKE_LIBDIR += /usr/local/lib
