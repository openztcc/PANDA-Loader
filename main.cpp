#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "PState.h"
#include <QObject>
#include <QJSValue>
#include "PController.h"
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    PState *p_state = new PState(&app);
    qmlRegisterSingletonInstance("ozt.panda.PState", 1, 0, "PState", p_state);

    PController controller;
    engine.rootContext()->setContextProperty("modController", &controller);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("PandaLdr", "Main");

    return app.exec();
}
