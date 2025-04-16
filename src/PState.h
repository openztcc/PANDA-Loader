#ifndef PSTATE_H
#define PSTATE_H

#include <QObject>
#include <QProcess>
#include <QMutex>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QQmlEngine>
#include "../models/PModItem.h"
#include <QDir>
#include <QtQml/qqmlregistration.h>
#include "../models/PSettings.h"
#include "PSystemMgr.h"
#include "../models/PZooConfig.h"

// static functions

class PState : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString m_path READ getGamePath WRITE setGamePath NOTIFY pathChanged)
    Q_PROPERTY(QString m_resource_path READ getResourcePath WRITE setResourcePath NOTIFY pathChanged)
public:
    explicit PState(QObject *parent = nullptr);
    Q_INVOKABLE int launchZT();
    QString getGamePath();
    void setGamePath(QString);
    void loadConfig();
    QStringList getZtdList();
    Q_INVOKABLE PSettings* settings() const { return m_settings; }
    Q_INVOKABLE PZooConfig* zoo() const { return m_zooConfig; }

signals:
    void pathChanged();
private:
    PSettings *m_settings;
    PZooConfig *m_zooConfig;
    QString m_path;// = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.exe";
    QString m_resource_path;// = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\dlupdate\\";
    QVector<PModItem> m_mods;
};

#endif // PSTATE_H
