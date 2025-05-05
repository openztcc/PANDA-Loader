// Qt
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QJSValue>
#include <QQmlContext>
#include <QQmlEngine>

// Project
#include "PModUIController.h"
#include "PModItem.h"
#include "PModLoader.h"
#include "PConfigMgr.h"
#include "PModQueries.h"
#include "PDatabase.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Register the PAppController singleton
    // PAppController *p_state = new PAppController(&app);
    // qmlRegisterSingletonInstance("PandaLdr", 1, 0, "PAppController", p_state);

    // Register the PModUIController singleton
    QSharedPointer<PLauncher> launcher = QSharedPointer<PLauncher>::create();
    QSharedPointer<PDatabase> pandaDb = QSharedPointer<PDatabase>::create(QDir::homePath() + "/.panda", "ModsDb",
                                                                          QStringList{PQueries::CreateModsTable, PQueries::CreateDependenciesTable});
    QSharedPointer<PModUIController> modController = QSharedPointer<PModUIController>::create(&app, pandaDb);
    modController->loadMods();
    QSharedPointer<PAppController> state = QSharedPointer<PAppController>::create(&app);
    QSharedPointer<PConfigMgr> zoo = state->m_zooini;
    QSharedPointer<PConfigMgr> settings = state->m_pandacfg;
    engine.rootContext()->setContextProperty("modController", modController.get());
    engine.rootContext()->setContextProperty("modModel", modController->model());
    engine.rootContext()->setContextProperty("state", state.get());
    engine.rootContext()->setContextProperty("psettings", settings.get());
    engine.rootContext()->setContextProperty("zoo", zoo.get());
    engine.rootContext()->setContextProperty("launcher", launcher.get());

    // models
    qmlRegisterAnonymousType<QAbstractListModel>("PandaLdr", 1);
    qmlRegisterType<PModItem>("PandaLdr", 1, 0, "PModItem");
    qRegisterMetaType<PModItem*>("PModItem*");
    qRegisterMetaType<PModItem*>("QSharedPointer<PModItem>");
    qmlRegisterUncreatableType<PModItem>("PandaLdr", 1, 0, "PModItem", "PModItem can only be created in C++");

    // meta objects
    // qRegisterMetaType<PModList*>("PModList*");
    // qmlRegisterType<PModList>("PandaLdr", 1, 0, "PModList");
    qmlRegisterType<PModUIController>("PandaLdr", 1, 0, "PModUIController");

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
