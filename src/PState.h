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
#include "PSystemMgr.h"
#include "PConfigMgr.h"

// static functions

class PState : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString m_path READ getGamePath WRITE setGamePath NOTIFY pathChanged)
    Q_PROPERTY(PConfigMgr* m_settings READ settings)

public:
    explicit PState(QObject *parent = nullptr);
    Q_INVOKABLE int launchZT();
    QString getGamePath();
    void setGamePath(QString);
    bool loadZooIni();
    QStringList getZtdList();
    Q_INVOKABLE PConfigMgr* settings() const { return m_pandacfg; }
    // Q_INVOKABLE void updateState();
    bool loadPandaCfg();
    QString pandaHomePath() { return m_configPath; }

    PConfigMgr *m_zooini;
    PConfigMgr *m_pandacfg;

signals:
    void pathChanged();
private:
    QString m_path;// = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.exe";
    QString m_resource_path;// = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\dlupdate\\";
    QString m_pandacfg_path;
    QVector<PModItem> m_mods;
    QString m_configPath = QDir::homePath() + "/.panda";

};

#endif // PSTATE_H
