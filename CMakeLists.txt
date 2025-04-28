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
    # app
    src/app/PAppController.cpp src/app/PAppController.h
    src/app/PLauncher.cpp src/app/PLauncher.h
    src/app/PAppSettings.h src/app/PAppSettings.cpp
    # graphics
    src/graphics/PApeFile.cpp src/graphics/PApeFile.h
    # config
    src/config/IConfigLoader.h
    src/config/PConfigMgr.cpp src/config/PConfigMgr.h
    src/config/PIniConfig.cpp src/config/PIniConfig.h
    src/config/PTomlConfig.cpp src/config/PTomlConfig.h
    # models
    src/models/PModItem.h src/models/PModItem.cpp
    src/models/PEntityType.cpp src/models/PEntityType.h
    src/models/PIconData.h
    src/models/PFileData.h
    src/models/PDependency.h
    # filesystem
    src/filesystem/IVirtualFileSystem.h
    src/filesystem/PFileSystem.h src/filesystem/PFileSystem.cpp
    src/filesystem/PFile.h src/filesystem/PFile.cpp
    src/filesystem/PZip.h src/filesystem/PZip.cpp
    # mods
    src/mods/PModController.cpp src/mods/PModController.h
    src/mods/PModDataAccess.cpp src/mods/PModDataAccess.h
    src/mods/PModLoader.cpp src/mods/PModLoader.h
    src/mods/PDepDal.cpp src/mods/PDepDal.h
    src/mods/PModQueries.h
    # database
    src/database/PDatabase.cpp src/database/PDatabase.h
    # data
    src/data/PDataList.h

    src/ldrs/PZtdMgr.h src/ldrs/PZtdMgr.cpp 
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
        # app
        src/app/PAppController.cpp
        src/app/PLauncher.cpp
        src/app/PAppSettings.cpp
        # graphics
        src/graphics/PApeFile.cpp
        # config
        src/config/PConfigMgr.cpp
        src/config/PIniConfig.cpp
        src/config/PTomlConfig.cpp
        # models
        src/models/PModItem.cpp
        src/models/PEntityType.cpp
        # filesystem
        src/filesystem/PFileSystem.cpp
        src/filesystem/PFile.cpp
        src/filesystem/PZip.cpp
        # database
        src/database/PDatabase.cpp
        # mods
        src/mods/PModController.cpp
        src/mods/PModDataAccess.cpp
        src/mods/PModLoader.cpp
        src/mods/PDepDal.cpp

        src/ldrs/PZtdMgr.cpp 
)

target_include_directories(appPandaLdr
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/models
        ${CMAKE_SOURCE_DIR}/src/app
        ${CMAKE_SOURCE_DIR}/src/graphics
        ${CMAKE_SOURCE_DIR}/src/filesystem
        ${CMAKE_SOURCE_DIR}/src/config
        ${CMAKE_SOURCE_DIR}/src/database
        ${CMAKE_SOURCE_DIR}/src/ldrs
        ${CMAKE_SOURCE_DIR}/src/mods
        ${CMAKE_SOURCE_DIR}/src/data
        ${CMAKE_SOURCE_DIR}/libs/apecore/include
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

# ------------- PTestFileSystem -------------
qt_add_executable(testFileSystem
    tests/PTestFileSystem.cpp
    # config
    src/config/IConfigLoader.h
    src/config/PConfigMgr.cpp src/config/PConfigMgr.h
    src/config/PIniConfig.cpp src/config/PIniConfig.h
    src/config/PTomlConfig.cpp src/config/PTomlConfig.h
    # models
    src/models/PFileData.h
    # filesystem
    src/filesystem/IVirtualFileSystem.h
    src/filesystem/PFileSystem.h src/filesystem/PFileSystem.cpp
    src/filesystem/PFile.h src/filesystem/PFile.cpp
    src/filesystem/PZip.h src/filesystem/PZip.cpp

    src/ldrs/PZtdMgr.h src/ldrs/PZtdMgr.cpp
)

target_include_directories(testFileSystem
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/models
        ${CMAKE_SOURCE_DIR}/src/app
        ${CMAKE_SOURCE_DIR}/src/graphics
        ${CMAKE_SOURCE_DIR}/src/filesystem
        ${CMAKE_SOURCE_DIR}/src/config
        ${CMAKE_SOURCE_DIR}/src/database
        ${CMAKE_SOURCE_DIR}/src/ldrs
        ${CMAKE_SOURCE_DIR}/src/mods
        ${CMAKE_SOURCE_DIR}/libs/apecore/include
)

# link the required libraries
target_link_libraries(testFileSystem
    PRIVATE
        Qt6::Test
        Qt6::Core
        Qt6::Sql
        ${CMAKE_SOURCE_DIR}/libs/quazip/lib/quazip.a
        tomlplusplus
        ApeCore
        quazip
        simpleini
)

# add test data as resource
add_custom_command(TARGET testFileSystem POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/tests/test_data
    ${CMAKE_BINARY_DIR}/test_data
)

# ------------- PTestConfigMgr -------------
# qt_add_executable(testConfigMgr
#     tests/PTestConfigMgr.cpp
#     src/ldrs/PConfigMgr.cpp src/ldrs/PConfigMgr.h
#     src/ldrs/PZtdMgr.cpp src/ldrs/PZtdMgr.h
#     src/interfaces/IConfigLoader.h src/interfaces/IConfigLoader.cpp
#     src/ldrs/PIniConfig.cpp src/ldrs/PIniConfig.h
#     src/ldrs/PTomlConfig.cpp src/ldrs/PTomlConfig.h

# )

# # link the required libraries
# target_link_libraries(testConfigMgr
#     PRIVATE
#         Qt6::Test
#         Qt6::Core
#         Qt6::Sql
#         ${CMAKE_SOURCE_DIR}/libs/quazip/lib/quazip.a
#         tomlplusplus
#         simpleini
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
#     PDatabase.h PDatabase.cpp
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
#     src/PApeFile.h src/PApeFile.cpp
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

add_test(NAME testFileSystem COMMAND testFileSystem)
# add_test(NAME testConfigMgr COMMAND testConfigMgr)
# # add_test(NAME testDatabaseMgr COMMAND testDatabaseMgr)
# add_test(NAME testGraphicsMgr COMMAND testGraphicsMgr)
