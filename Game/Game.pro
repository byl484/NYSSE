TEMPLATE = app
TARGET = NYSSE

QT += core gui widgets network multimedia

CONFIG += c++14

SOURCES += \
    bettermainwindow.cpp \
    city.cpp \
    enemylogic.cpp \
    engine.cpp \
    graphicactor.cc \
    graphicbus.cc \
    graphichost.cc \
    graphicstop.cc \
    main.cc \
    passengerinfo.cc \
    playerlogic.cpp \
    startwindow.cc \
    statistics.cpp \

RESOURCES += \
    images/bomb_images/exp1.png \
    images/bomb_images/exp2.png \
    images/bomb_images/exp3.png \
    images/bomb_images/exp4.png \
    images/helicopter1.png \
    images/helicopter2.png \
    images/enemy_helicopter1.png \
    images/enemy_helicopter2.png \
    images/bus.png \
    images/better_map_images/tampere_22862x15770.png \
    images/stop.png \
    images/start_banner.png \
    images/win_banner.png \
    images/game_over_banner.png


win32:CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../Course/CourseLib/release/ -lCourseLib
else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../Course/CourseLib/debug/ -lCourseLib
else:unix: LIBS += \
    -L$$OUT_PWD/../Course/CourseLib/ -lCourseLib

INCLUDEPATH += \
    $$PWD/../Course/CourseLib

DEPENDPATH += \
    $$PWD/../Course/CourseLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/release/libCourseLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/debug/libCourseLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/release/CourseLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/debug/CourseLib.lib
else:unix: PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/libCourseLib.a

HEADERS += \
    bettermainwindow.hh \
    city.hh \
    enemylogic.hh \
    engine.hh \
    graphicactor.hh \
    graphicbus.hh \
    graphichost.hh \
    graphicstop.hh \
    passengerinfo.hh \
    playerlogic.hh \
    startwindow.hh \
    statistics.hh \

FORMS += \
    bettermainwindow.ui \
    startwindow.ui
