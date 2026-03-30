cmake_minimum_required(VERSION 3.16)
project(Helpdesk VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(Helpdesk
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ticketmodel.cpp
    ticketmodel.h
    ticketdialog.cpp
    ticketdialog.h
    ticketdialog.ui
)

target_link_libraries(Helpdesk PRIVATE Qt6::Widgets)
