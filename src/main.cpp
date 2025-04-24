// Qt
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QJSValue>
#include <QQmlContext>
#include <QQmlEngine>

// Project
#include "mgrs/PController.h"
#include "models/PModItem.h"
#include "models/PModModel.h"
#include "ldrs/PConfigMgr.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Register the PState singleton
    PState *p_state = new PState(&app);
    qmlRegisterSingletonInstance("PandaLdr", 1, 0, "PState", p_state);

    // Register the PController singleton
    PController *controller = new PController(&app, p_state);
    PState *state = new PState(&app);
    PConfigMgr *zoo = state->m_zooini.get();
    PConfigMgr *settings = state->m_pandacfg.get();
    engine.rootContext()->setContextProperty("modController", controller);
    engine.rootContext()->setContextProperty("modModel", QVariant::fromValue(controller->model()));
    engine.rootContext()->setContextProperty("state", state);
    engine.rootContext()->setContextProperty("psettings", settings);
    engine.rootContext()->setContextProperty("zoo", zoo);

    // models
    qmlRegisterAnonymousType<QAbstractListModel>("PandaLdr", 1);
    qmlRegisterType<PModItem>("PandaLdr", 1, 0, "PModItem");
    qRegisterMetaType<PModItem*>("PModItem*");
    qmlRegisterUncreatableType<PModItem>("PandaLdr", 1, 0, "PModItem", "PModItem can only be created in C++");

    // meta objects
    qRegisterMetaType<PModModel*>("PModModel*");
    qmlRegisterType<PModModel>("PandaLdr", 1, 0, "PModModel");
    qmlRegisterType<PController>("PandaLdr", 1, 0, "PController");

    // Load the main QML file
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
        engine.addImportPath(":/");
        engine.addImportPath(QCoreApplication::applicationDirPath());
        const QUrl url(QStringLiteral("file:///") + QCoreApplication::applicationDirPath() + "/PandaUI/ui/Main.qml");
        engine.load(url);



    return app.exec();
}
