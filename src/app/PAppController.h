#ifndef PSTATE_H
#define PSTATE_H

// Qt
#include <QObject>
#include <QProcess>
#include <QMutex>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QQmlEngine>
#include <QDir>
#include <QtQml/qqmlregistration.h>

// Project
#include "PModItem.h"
#include "PLauncher.h"
#include "PConfigMgr.h"

class PAppController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString path READ getGamePath WRITE setGamePath NOTIFY pathChanged)
    Q_PROPERTY(int dirty READ dirty NOTIFY dirtyChanged)

public:
    explicit PAppController(QObject *parent = nullptr);
    Q_INVOKABLE int launchZT();
    Q_INVOKABLE int launchScripting();
    QString getGamePath();
    void setGamePath(QString);
    bool loadZooIni();
    Q_INVOKABLE PConfigMgr* settings() const { return m_pandacfg.get(); }
    // Q_INVOKABLE void updateState();
    bool loadPandaCfg();
    Q_INVOKABLE QString pandaHomePath() { return m_configPath; }

    QSharedPointer<PConfigMgr> m_zooini;
    QSharedPointer<PConfigMgr> m_pandacfg;

    int dirty() const { return m_dirty; }
    void setDirty(int dirty) { m_dirty = dirty; }

signals:
    void pathChanged();
    void dirtyChanged();
private slots:
    // TODO: slot receives correct signals inside but does not emit to QML
    // not currently functional, but needs to be fixed for future INI editor
    void onConfigDirtyChanged(int dirty) {
        // track which config is dirty
        QObject* senderObj = sender();
        qDebug() << "Update dirty state from: " << senderObj << " with dirty state: " << dirty;

        // grab dirty data from the sender
        if (senderObj == m_zooini.get()) {
            m_zooiniDirty = dirty;
        } else if (senderObj == m_pandacfg.get()) {
            m_pandacfgDirty = dirty;
        }

        m_dirty = m_zooiniDirty + m_pandacfgDirty;
        emit dirtyChanged();
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
