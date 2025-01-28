#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "PUtilties.h"
#include <QObject>
#include <QJSValue>

// utilities singleton
static QObject *putilities_provider(QQmlEngine *, QJSEngine *) {
    class PUtilitiesWrapper : public QObject {
        Q_OBJECT
    public:
        explicit PUtilitiesWrapper(QObject *parent = nullptr) : QObject(parent) {}

        Q_INVOKABLE int launchZT(const QString &path) {
            return PUtilities::launchZT(path);
        }
    };

    return new PUtilitiesWrapper();
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);



    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("PandaLdr", "Main");

    return app.exec();
}
