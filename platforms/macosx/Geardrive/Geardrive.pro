#-------------------------------------------------
#
# Project created by QtCreator 2015-03-20T18:16:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += opengl

TARGET = Geardrive
TEMPLATE = app

INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/lib

LIBS += -L/usr/local/lib -lSDL2main -lSDL2

SOURCES += \
    ../../qt-shared/about.cpp \
    ../../qt-shared/emulator.cpp \
    ../../qt-shared/gl_frame.cpp \
    ../../qt-shared/input_settings.cpp \
    ../../qt-shared/main_window.cpp \
    ../../qt-shared/main.cpp \
    ../../qt-shared/render_thread.cpp \
    ../../qt-shared/sound_settings.cpp \
    ../../qt-shared/video_settings.cpp \
    ../../../src/GZ80/gz80_core.cpp \
    ../../../src/GZ80/gz80_opcodes.cpp \
    ../../../src/GZ80/gz80_opcodes_cb.cpp \
    ../../../src/GZ80/gz80_opcodes_ed.cpp \
    ../../../src/audio.cpp \
    ../../../src/cartridge.cpp \
    ../../../src/geardrive_core.cpp \
    ../../../src/input.cpp \
    ../../../src/video.cpp \
    ../../../src/miniz/miniz.c

HEADERS  += \
    ../../qt-shared/about.h \
    ../../qt-shared/emulator.h \
    ../../qt-shared/gl_frame.h \
    ../../qt-shared/input_settings.h \
    ../../qt-shared/main_window.h \
    ../../qt-shared/render_thread.h \
    ../../qt-shared/sound_settings.h \
    ../../qt-shared/video_settings.h \
    ../../../src/GZ80/gz80_core.h \
    ../../../src/GZ80/gz80_core_inl.h \
    ../../../src/GZ80/gz80_definitions.h \
    ../../../src/GZ80/gz80_eight_bit_register.h \
    ../../../src/GZ80/gz80_ioports_interface.h \
    ../../../src/GZ80/gz80_memory_interface.h \
    ../../../src/GZ80/gz80_opcode_cb_names.h \
    ../../../src/GZ80/gz80_opcode_daa.h \
    ../../../src/GZ80/gz80_opcode_dd_names.h \
    ../../../src/GZ80/gz80_opcode_ddcb_names.h \
    ../../../src/GZ80/gz80_opcode_ed_names.h \
    ../../../src/GZ80/gz80_opcode_fd_names.h \
    ../../../src/GZ80/gz80_opcode_fdcb_names.h \
    ../../../src/GZ80/gz80_opcode_names.h \
    ../../../src/GZ80/gz80_opcode_timing.h \
    ../../../src/GZ80/gz80_opcode_xx_names.h \
    ../../../src/GZ80/gz80_sixteen_bit_register.h \
    ../../../src/audio.h \
    ../../../src/cartridge.h \
    ../../../src/definitions.h \
    ../../../src/geardrive.h \
    ../../../src/geardrive_core.h \
    ../../../src/input.h \
    ../../../src/video.h

FORMS += \
    ../../qt-shared/About.ui \
    ../../qt-shared/InputSettings.ui \
    ../../qt-shared/MainWindow.ui \
    ../../qt-shared/SoundSettings.ui \
    ../../qt-shared/VideoSettings.ui
