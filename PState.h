#ifndef PSTATE_H
#define PSTATE_H

#include <QObject>
#include <QProcess>
#include <QMutex>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QQmlEngine>

// static functions

class PState : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString m_path READ getPath WRITE setPath NOTIFY pathChanged)
public:
    explicit PState(QObject *parent = nullptr);
    Q_INVOKABLE int launchZT();

signals:
    void pathChanged();
    QString getPath();
    void setPath(QString);

private:
    QString m_path;// = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.exe";
};

#endif // PSTATE_H
