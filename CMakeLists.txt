cmake_minimum_required(VERSION 3.10)
project(UCMS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(UCMS
    main.cpp
    mainwindow.cpp
    mainwindow.h
    Course.cpp
    Course.h
    CourseManager.cpp
    CourseManager.h
    Student.cpp
    Student.h
    Professor.cpp
    Professor.h
    Admin.cpp
    Admin.h
    UI.cpp
    UI.h
    AuthenticationManager.cpp
    AuthenticationManager.h
    Studentdashboard.h
    Studentdashboard.cpp
    Professordashboard.h
    Professordashboard.cpp
    Admindashboard.h
    Admindashboard.cpp
)

target_link_libraries(UCMS Qt6::Widgets)