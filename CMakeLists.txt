cmake_minimum_required(VERSION 3.16)

project(PandaLdr VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appPandaLdr
    main.cpp
)

qt_add_qml_module(appPandaLdr
    URI PandaLdr
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES RailButton.qml
        QML_FILES SvgIcon.qml
        RESOURCES images/launch_bg.jpg
        QML_FILES ActionButton.qml
)

qt_add_resources(appPandaLdr "resources"
    PREFIX "/"
    FILES
        icons/mods.svg
        icons/about.svg
        icons/exit_app.svg
        icons/notifications.svg
        icons/add.svg
        icons/delete.svg
        icons/refresh.svg
        images/launch_bg.jpg
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appPandaLdr PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appPandaLdr
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appPandaLdr
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appPandaLdr
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
