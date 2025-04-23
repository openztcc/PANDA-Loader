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

    Q_PROPERTY(QString path READ getGamePath WRITE setGamePath NOTIFY pathChanged)
    Q_PROPERTY(int dirty READ dirty NOTIFY dirtyChanged)

public:
    explicit PState(QObject *parent = nullptr);
    Q_INVOKABLE int launchZT();
    QString getGamePath();
    void setGamePath(QString);
    bool loadZooIni();
    QStringList getZtdList();
    Q_INVOKABLE PConfigMgr* settings() const { return m_pandacfg.get(); }
    // Q_INVOKABLE void updateState();
    bool loadPandaCfg();
    Q_INVOKABLE QString pandaHomePath() { return m_configPath; }

    std::unique_ptr<PConfigMgr> m_zooini;
    std::unique_ptr<PConfigMgr> m_pandacfg;

    int dirty() const { return m_dirty; }
    void setDirty(int dirty) { m_dirty = dirty; }

signals:
    void pathChanged();
    void dirtyChanged(int dirty);
private slots:
    void onConfigDirtyChanged(int dirty) {
        // track which config is dirty
        QObject* senderObj = sender();

        // grab dirty data from the sender
        if (senderObj == m_zooini.get()) {
            m_zooiniDirty = dirty;
        } else if (senderObj == m_pandacfg.get()) {
            m_pandacfgDirty = dirty;
        }
    
        // aggregate dirty data from all configs
        int totalDirty = m_zooiniDirty + m_pandacfgDirty;
        if (m_dirty != totalDirty) {
            m_dirty = totalDirty;
            emit dirtyChanged(m_dirty);
        }
    }
private:
    QString m_path;// = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.exe";
    QString m_resource_path;// = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\dlupdate\\";
    QString m_pandacfg_path;
    QVector<PModItem> m_mods;
    QString m_configPath = QDir::homePath() + "/.panda";
    int m_dirty;
    int m_zooiniDirty;
    int m_pandacfgDirty;

};

#endif // PSTATE_H
