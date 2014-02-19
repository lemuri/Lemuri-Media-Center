#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include <QObject>

class MediaCenter;
class KeyManager : public QObject
{
    Q_OBJECT
public:
    explicit KeyManager(MediaCenter *parent = 0);

    Q_PROPERTY(bool filterKeys READ filterKeys WRITE setFilterKeys NOTIFY filterKeysChanged)
    bool filterKeys() const;
    void setFilterKeys(bool filterKeys);

    Q_PROPERTY(int keyUp READ keyUp WRITE setKeyUp NOTIFY keysChanged)
    int keyUp() const;
    void setKeyUp(int key);

    Q_PROPERTY(int keyDown READ keyDown WRITE setKeyDown NOTIFY keysChanged)
    int keyDown() const;
    void setKeyDown(int key);

    Q_PROPERTY(int keyLeft READ keyLeft WRITE setKeyLeft NOTIFY keysChanged)
    int keyLeft() const;
    void setKeyLeft(int key);

    Q_PROPERTY(int keyRight READ keyRight WRITE setKeyRight NOTIFY keysChanged)
    int keyRight() const;
    void setKeyRight(int key);


    Q_PROPERTY(int keyChooseAlbum READ keyChooseAlbum WRITE setKeyChooseAlbum NOTIFY keysChanged)
    int keyChooseAlbum() const;
    void setKeyChooseAlbum(int key);

    Q_PROPERTY(int keyPlayMedia READ keyPlayMedia WRITE setKeyPlayMedia NOTIFY keysChanged)
    int keyPlayMedia() const;
    void setKeyPlayMedia(int key);


    Q_PROPERTY(int keyVolume READ keyVolume WRITE setKeyVolume NOTIFY keysChanged)
    int keyVolume() const;
    void setKeyVolume(int key);

    Q_PROPERTY(int keyConfigure READ keyConfigure WRITE setKeyConfigure NOTIFY keysChanged)
    int keyConfigure() const;
    void setKeyConfigure(int key);

    Q_PROPERTY(int keyLeave READ keyLeave WRITE setKeyLeave NOTIFY keysChanged)
    int keyLeave() const;
    void setKeyLeave(int key);

signals:
    void filterKeysChanged();
    void keysChanged();

public Q_SLOTS:
    QString keyString(int key);

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);

private:
    int saveKey(const QString &settingsKey, int value);
    QHash<QString, int> currentKeys() const;

    MediaCenter *m_mediaCenter;
    bool m_filterKeys;
    int m_keyUp;
    int m_keyDown;
    int m_keyLeft;
    int m_keyRight;
    int m_keyLeave;
    int m_keyChooseAlbum;
    int m_keyPlayMedia;
    int m_keyVolume;
    int m_keyConfigure;
};

#endif // KEYMANAGER_H
