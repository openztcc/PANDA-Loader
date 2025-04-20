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
    Core
    Gui
)

qt_standard_project_setup(REQUIRES 6.5)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/libs/quazip/cmake")

find_package(QuaZip REQUIRED)

qt_add_executable(appPandaLdr
    ${SRC_RC}
    src/main.cpp
    src/PState.cpp src/PState.h
    src/PController.cpp src/PController.h
    src/PZtdMgr.cpp src/PZtdMgr.h
    src/PConfigMgr.cpp src/PConfigMgr.h
    src/PGraphicsMgr.h src/PGraphicsMgr.cpp
    src/PDatabaseMgr.cpp src/PDatabaseMgr.h
    src/PSystemMgr.h src/PSystemMgr.cpp
    models/PModItem.cpp models/PModItem.h
    models/PSettings.cpp models/PSettings.h
    models/PEntityType.cpp models/PEntityType.h
    models/PIconData.h
    models/PModModel.cpp models/PModModel.h
    models/PZooConfig.h models/PZooConfig.cpp
    interfaces/IConfigLoader.h interfaces/IConfigLoader.cpp
    interfaces/IIni.h interfaces/IIni.cpp
    interfaces/IToml.h interfaces/IToml.cpp
    )

add_subdirectory(libs/apecore)

include_directories(
    ${CMAKE_SOURCE_DIR}/libs/quazip/include
)
link_directories(
    ${CMAKE_SOURCE_DIR}/libs/quazip/lib
)
add_library(tomlplusplus INTERFACE)
add_library(simpleini INTERFACE)

target_include_directories(tomlplusplus INTERFACE ${CMAKE_SOURCE_DIR}/libs/tomlplusplus/include)
target_include_directories(simpleini INTERFACE ${CMAKE_SOURCE_DIR}/libs/simpleini/include)

target_include_directories(
    appPandaLdr PRIVATE ${CMAKE_SOURCE_DIR}/src
    appPandaLdr PRIVATE ${CMAKE_SOURCE_DIR}/models
    appPandaLdr PRIVATE ${CMAKE_SOURCE_DIR}/libs/apecore/include
)

qt_policy(SET QTP0004 NEW)
qt_add_qml_module(appPandaLdr
    URI PandaUI
    VERSION 1.0
    QML_FILES
        # main
        ui/Main.qml
        # infopane
        ui/infopane/InfoPane.qml
        ui/infopane/InfoPaneItem.qml
        # modlist
        ui/modlist/ModItem.qml
        # navbar
        ui/navbar/RailButton.qml
        ui/navbar/Navigation.qml
        # misc
        ui/misc/SvgIcon.qml
        ui/misc/ActionButton.qml
        ui/misc/SimpleModal.qml
        ui/misc/SettingsButton.qml
        ui/misc/PTextField.qml
        ui/misc/ClearButton.qml
        ui/misc/BrowseButton.qml
        ui/misc/ControlPanel.qml
        ui/misc/PCheckBox.qml
        ui/misc/AppendTable.qml
        # searchbar
        ui/searchbar/SearchBar.qml
        ui/searchbar/FilterTag.qml
        # appbar
        ui/appbar/AppBarLayout.qml
        # layouts
        ui/layouts/LayoutFrame.qml
        ui/layouts/mods/ModPage.qml
        ui/layouts/mods/components/LaunchArea.qml
        ui/layouts/mods/components/ModsList.qml
        ui/layouts/settings/SettingsPage.qml
        ui/layouts/settings/SettingsPane.qml
        # settings
        ui/layouts/settings/components/PandaSettings.qml
        ui/layouts/settings/components/VideoSettings.qml
        ui/layouts/settings/components/SoundSettings.qml
        ui/layouts/settings/components/GameplaySettings.qml
        ui/layouts/settings/components/UISettings.qml
        ui/layouts/settings/components/ScenarioSettings.qml
        ui/layouts/settings/components/ResourcePathsSettings.qml
        ui/layouts/settings/components/LanguageSettings.qml
        ui/layouts/settings/components/DebugSettings.qml
        ui/layouts/settings/SettingsConfirmationBar.qml
    RESOURCES images/launch_bg.jpg
    SOURCES
        # sources
        src/PState.h
        src/PController.h
        src/PZtdMgr.h
        src/PConfigMgr.h
        src/PGraphicsMgr.h
        src/PDatabaseMgr.h
        src/PSystemMgr.cpp
        # models
        models/PModItem.h
        models/PSettings.h
        models/PEntityType.h
        models/PSettings.h
        models/PModModel.h
        models/PZooConfig.h
        # interfaces
        interfaces/IConfigLoader.h
        interfaces/IIni.h
        interfaces/IToml.h
)

qt_add_resources(appPandaLdr "resources"
    PREFIX "/"
    FILES
        # ----------- icons
        # navrail
        icons/mods.svg
        icons/about.svg
        icons/exit_app.svg

        # appbar
        icons/notifications.svg

        # misc controls
        icons/add.svg
        icons/delete.svg
        icons/refresh.svg
        icons/author.svg
        icons/folder.svg
        icons/close.svg
        icons/check.svg
        icons/language.svg
        icons/ui.svg
        icons/search.svg

        # settings
        icons/settings.svg
        icons/tv.svg 
        icons/paw.svg 
        icons/sound.svg 
        icons/keyboard.svg 
        icons/map.svg 
        icons/article.svg 
        icons/plumbing.svg

        # ----------- images
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
    ApeCore
    Qt6::Core
    Qt6::Quick 
    Qt6::QuickControls2
    Qt6::QuickLayouts
    Qt6::Svg
    Qt6::Gui
    ${CMAKE_SOURCE_DIR}/libs/quazip/lib/quazip.a 
    Qt6::Sql 
    tomlplusplus
    simpleini
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
#     src/PZtdMgr.h src/PZtdMgr.cpp
#     src/PConfigMgr.h src/PConfigMgr.cpp
#     src/PDatabaseMgr.h src/PDatabaseMgr.cpp
# )

# # link the required libraries
# target_link_libraries(testZtdMgr
#     PRIVATE
#         Qt6::Test
#         Qt6::Core
#         Qt6::Sql
#         ${CMAKE_SOURCE_DIR}/libs/quazip/lib/quazip.a
#         tomlplusplus
#         ApeCore
# )

# # add test data as resource
# add_custom_command(TARGET testZtdMgr POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/tests/test_data
#     ${CMAKE_BINARY_DIR}/test_data
# )

# ------------- PTestConfigMgr -------------
qt_add_executable(testConfigMgr
    tests/PTestConfigMgr.cpp
    src/PConfigMgr.h src/PConfigMgr.cpp
    src/PZtdMgr.h src/PZtdMgr.cpp
    interfaces/IConfigLoader.h interfaces/IConfigLoader.cpp
    interfaces/IIni.h interfaces/IIni.cpp
    interfaces/IToml.h interfaces/IToml.cpp

)

# link the required libraries
target_link_libraries(testConfigMgr
    PRIVATE
        Qt6::Test
        Qt6::Core
        Qt6::Sql
        ${CMAKE_SOURCE_DIR}/libs/quazip/lib/quazip.a
        tomlplusplus
        simpleini
)

# add test data as resource
add_custom_command(TARGET testConfigMgr POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/tests/test_data
    ${CMAKE_BINARY_DIR}/test_data
)

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

# -------------- PTestGraphicsMgr --------------
# qt_add_executable(testGraphicsMgr
#     tests/PTestGraphicsMgr.cpp
#     src/PConfigMgr.h src/PConfigMgr.cpp
#     src/PGraphicsMgr.h src/PGraphicsMgr.cpp
#     src/PZtdMgr.h src/PZtdMgr.cpp
# )

# # link the required libraries
# target_link_libraries(testGraphicsMgr
#     PRIVATE
#         Qt6::Test
#         Qt6::Core
#         Qt6::Sql
#         ${CMAKE_SOURCE_DIR}/libs/quazip/lib/quazip.a
#         ApeCore
#         tomlplusplus
# )

# # add test data as resource
# add_custom_command(TARGET testGraphicsMgr POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/tests/test_data
#     ${CMAKE_BINARY_DIR}/test_data
# )

# add_test(NAME testZtdMgr COMMAND testZtdMgr)
add_test(NAME testConfigMgr COMMAND testConfigMgr)
# # add_test(NAME testDatabaseMgr COMMAND testDatabaseMgr)
# add_test(NAME testGraphicsMgr COMMAND testGraphicsMgr)
