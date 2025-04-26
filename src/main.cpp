// Qt
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QJSValue>
#include <QQmlContext>
#include <QQmlEngine>

// Project
#include "PModController.h"
#include "PModItem.h"
#include "PModMgr.h"
#include "PConfigMgr.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Register the PAppController singleton
    PAppController *p_state = new PAppController(&app);
    qmlRegisterSingletonInstance("PandaLdr", 1, 0, "PAppController", p_state);

    // Register the PModController singleton
    PModController *controller = new PModController(&app, p_state);
    PAppController *state = new PAppController(&app);
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
    qRegisterMetaType<PModMgr*>("PModMgr*");
    qmlRegisterType<PModMgr>("PandaLdr", 1, 0, "PModMgr");
    qmlRegisterType<PModController>("PandaLdr", 1, 0, "PModController");

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
