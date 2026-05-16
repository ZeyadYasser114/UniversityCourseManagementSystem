cmake_minimum_required(VERSION 3.10)
project(UCMS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)

add_executable(UCMS
    main.cpp
    mainwindow.cpp
    mainwindow.h
    DataStore.cpp
    DataStore.h
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
    AuthenticationManager.cpp
    AuthenticationManager.h
    Studentdashboard.h
    Studentdashboard.cpp
    Professordashboard.h
    Professordashboard.cpp
    Admindashboard.h
    Admindashboard.cpp
    Datastore.h
    datastore.h datastore.cpp

)

target_link_libraries(UCMS Qt6::Widgets Qt6::Core)