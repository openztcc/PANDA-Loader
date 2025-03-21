cmake_minimum_required(VERSION 3.16)

project(PandaLdr VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS 
    Quick 
    Test 
    Sql
    QuickControls2
    QuickLayouts
    Svg
    Xml 
    Core
    Gui
)

qt_standard_project_setup(REQUIRES 6.5)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/libs/quazip/cmake")

find_package(QuaZip REQUIRED)

qt_add_executable(appPandaLdr
    src/main.cpp
    src/PState.cpp src/PState.h
    src/PController.cpp src/PController.h
    src/PModItem.cpp src/PModItem.h
    src/PZtdMgr.cpp src/PZtdMgr.h
    src/PConfigMgr.cpp src/PConfigMgr.h
    src/PDatabaseMgr.cpp src/PDatabaseMgr.h
)

include_directories(
    ${CMAKE_SOURCE_DIR}/libs/quazip/include
)
link_directories(
    ${CMAKE_SOURCE_DIR}/libs/quazip/lib
)
add_library(tomlplusplus INTERFACE)
target_include_directories(
    tomlplusplus INTERFACE ${CMAKE_SOURCE_DIR}/libs/tomlplusplus/include
)

target_include_directories(
    appPandaLdr PRIVATE ${CMAKE_SOURCE_DIR}/src
)

qt_policy(SET QTP0004 NEW)
qt_add_qml_module(appPandaLdr
    URI PandaLdr
    VERSION 1.0
    QML_FILES
        ui/Main.qml
        ui/RailButton.qml
        ui/SvgIcon.qml
        ui/ActionButton.qml
        ui/ModItem.qml
        ui/InfoPane.qml
        ui/InfoPaneItem.qml
        ui/SearchBar.qml
        ui/FilterTag.qml
    RESOURCES images/launch_bg.jpg
    SOURCES
        src/PState.cpp src/PState.h
        src/PController.cpp src/PController.h
        src/PModItem.cpp src/PModItem.h
        src/PZtdMgr.cpp src/PZtdMgr.h
        src/PConfigMgr.cpp src/PConfigMgr.h
        src/PDatabaseMgr.cpp src/PDatabaseMgr.h
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
        icons/author.svg
        icons/path.svg
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
    PRIVATE 
    Qt6::Core
    Qt6::Quick 
    Qt6::QuickControls2
    Qt6::QuickLayouts
    Qt6::Svg
    Qt6::Xml
    Qt6::Gui
    ${CMAKE_SOURCE_DIR}/libs/quazip/lib/quazip.a 
    Qt6::Sql 
    tomlplusplus
)

include(GNUInstallDirs)
install(TARGETS appPandaLdr
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Set up unit tests

# enable_testing()

# # add the main project modules
# # creates the test exe

# # ------------- PTestZtdMgr -------------
# qt_add_executable(testZtdMgr
#     tests/PTestZtdMgr.cpp
#     PZtdMgr.h PZtdMgr.cpp
#     PConfigMgr.h PConfigMgr.cpp
#     PDatabaseMgr.h PDatabaseMgr.cpp
# )

# # link the required libraries
# target_link_libraries(testZtdMgr
#     PRIVATE
#         Qt6::Test
#         Qt6::Core
#         Qt6::Sql
#         ${CMAKE_SOURCE_DIR}/libs/quazip/lib/quazip.a
#         tomlplusplus
# )

# # add test data as resource
# add_custom_command(TARGET testZtdMgr POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/tests/test_data
#     ${CMAKE_BINARY_DIR}/test_data
# )

# # ------------- PTestConfigMgr -------------
# qt_add_executable(testConfigMgr
#     tests/PTestConfigMgr.cpp
#     PConfigMgr.h PConfigMgr.cpp
#     PZtdMgr.h PZtdMgr.cpp
# )

# # link the required libraries
# target_link_libraries(testConfigMgr
#     PRIVATE
#         Qt6::Test
#         Qt6::Core
#         Qt6::Sql
#         ${CMAKE_SOURCE_DIR}/libs/quazip/lib/quazip.a
#         tomlplusplus
# )

# # add test data as resource
# add_custom_command(TARGET testConfigMgr POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/tests/test_data
#     ${CMAKE_BINARY_DIR}/test_data
# )

# # ------------- PTestDatabaseMgr -------------
# qt_add_executable(testDatabaseMgr
#     tests/PTestDatabaseMgr.cpp
#     PDatabaseMgr.h PDatabaseMgr.cpp
#     PConfigMgr.h PConfigMgr.cpp
#     PZtdMgr.h PZtdMgr.cpp
# )

# # link the required libraries
# target_link_libraries(testDatabaseMgr
#     PRIVATE
#         Qt6::Test
#         Qt6::Core
#         Qt6::Sql
#         ${CMAKE_SOURCE_DIR}/libs/quazip/lib/quazip.a
#         tomlplusplus
# )

# # add test data as resource
# add_custom_command(TARGET testDatabaseMgr POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/tests/test_data
#     ${CMAKE_BINARY_DIR}/test_data
# )

# add_test(NAME testZtdMgr COMMAND testZtdMgr)
# add_test(NAME testConfigMgr COMMAND testConfigMgr)
# add_test(NAME testDatabaseMgr COMMAND testDatabaseMgr)
