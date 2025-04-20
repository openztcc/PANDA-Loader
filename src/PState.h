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
public:
    explicit PState(QObject *parent = nullptr);
    Q_INVOKABLE int launchZT();
    QString getGamePath();
    void setGamePath(QString);
    bool loadPandaCfg();
    bool loadZooIni();
    QStringList getZtdList();
    Q_INVOKABLE PConfigMgr* settings() const { return m_pandacfg; }
    Q_INVOKABLE PConfigMgr* zooConfig() const { return m_zooini; }

signals:
    void pathChanged();
private:
    PConfigMgr *m_pandacfg;
    PConfigMgr *m_zooini;
    QString m_path;// = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.exe";
    QString m_resource_path;// = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\dlupdate\\";
    QVector<PModItem> m_mods;
    QString m_configPath = QDir::homePath() + "/.panda";

};

#endif // PSTATE_H
