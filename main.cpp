#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "PState.h"
#include <QObject>
#include <QJSValue>
#include "PController.h"
#include <QQmlContext>
#include <QQmlEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Register the PState singleton
    PState *p_state = new PState(&app);
    qmlRegisterSingletonInstance("ozt.panda.PState", 1, 0, "PState", p_state);

    // Register the PController singleton
    PController controller;
    controller.addState(p_state);
    controller.loadMods();
    
    engine.rootContext()->setContextProperty("modController", &controller);
    engine.addImportPath("F:/QT/6.8.1/mingw_64/qml");

    // Register QML types
    qmlRegisterType<PModItem>("ozt.panda.PModItem", 1, 0, "PModItem");
    qmlRegisterType<PController>("ozt.panda.PController", 1, 0, "PController");

    // Load the main QML file
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("PandaLdr", "Main");

    return app.exec();
}
