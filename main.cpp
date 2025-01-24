#include <QCoreApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    // connects both functions
    // QObject::connect(&interactor, &UserInteractor::phraseTyped, &firefox, &Firefox::browse);

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
