#ifndef PUTILTIES_H
#define PUTILTIES_H

#include <QObject>
#include <QProcess>
#include <QMutex>
#include <QString>
#include <QDebug>

// static functions

class PUtilitiesWrapper : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString m_path READ m_path CONSTANT)
public:
    explicit PUtilitiesWrapper(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void launchZT();

private:
    QString m_path = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.exe";     
};

#endif // PUTILTIES_H
