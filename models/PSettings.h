#ifndef PSETTINGS_H
#define PSETTINGS_H

#include <QObject>
#include <QString>

class PSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString zooGamePath READ zooGamePath WRITE setZooGamePath NOTIFY zooGamePathChanged)
    Q_PROPERTY(bool useIsoMounting READ useIsoMounting WRITE setUseIsoMounting NOTIFY useIsoMountingChanged)
    Q_PROPERTY(QString isoPath READ isoPath WRITE setIsoPath NOTIFY isoPathChanged)

public:
    explicit PSettings(QObject* parent = nullptr);

    QString zooGamePath() const;
    void setZooGamePath(const QString& path);

    bool useIsoMounting() const;
    void setUseIsoMounting(bool use);

    QString isoPath() const;
    void setIsoPath(const QString& path);

    bool loadFromToml(const QString& filePath);

signals:
    void zooGamePathChanged();
    void useIsoMountingChanged();
    void isoPathChanged();

private:
    QString m_zooGamePath;
    bool m_useIsoMounting = false;
    QString m_isoPath;
};

#endif // PSETTINGS_H
