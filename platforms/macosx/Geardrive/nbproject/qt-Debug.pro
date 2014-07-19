# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Debug/CLang-MacOSX
TARGET = Geardrive
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += debug 
PKGCONFIG +=
QT = core gui widgets opengl
SOURCES += ../../../src/audio.cpp ../../../src/cartridge.cpp ../../../src/geardrive_core.cpp ../../../src/input.cpp ../../../src/video.cpp ../../qt-shared/about.cpp ../../qt-shared/emulator.cpp ../../qt-shared/gl_frame.cpp ../../qt-shared/input_settings.cpp ../../qt-shared/main.cpp ../../qt-shared/main_window.cpp ../../qt-shared/render_thread.cpp ../../qt-shared/sound_settings.cpp ../../qt-shared/video_settings.cpp
HEADERS += ../../../src/audio.h ../../../src/cartridge.h ../../../src/definitions.h ../../../src/geardrive.h ../../../src/geardrive_core.h ../../../src/input.h ../../../src/video.h ../../qt-shared/about.h ../../qt-shared/emulator.h ../../qt-shared/gl_frame.h ../../qt-shared/input_settings.h ../../qt-shared/main_window.h ../../qt-shared/render_thread.h ../../qt-shared/sound_settings.h ../../qt-shared/video_settings.h
FORMS += ../../qt-shared/About.ui ../../qt-shared/InputSettings.ui ../../qt-shared/MainWindow.ui ../../qt-shared/SoundSettings.ui ../../qt-shared/VideoSettings.ui
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/CLang-MacOSX
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = clang
QMAKE_CXX = clang++
DEFINES += 
INCLUDEPATH += /usr/local/include 
LIBS += -lSDL2main -lSDL2  
QMAKE_LIBDIR += /usr/local/lib
