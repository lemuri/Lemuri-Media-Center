#include "KeyManager.h"

#include "MediaCenter.h"

#include <QSettings>
#include <QKeyEvent>
#include <QDebug>

#define KEY_UP      "Keys/NavUp"
#define KEY_UP_D     Qt::Key_Q
#define KEY_DOWN    "Keys/NavDown"
#define KEY_DOWN_D   Qt::Key_W
#define KEY_LEFT    "Keys/NavLeft"
#define KEY_LEFT_D   Qt::Key_E
#define KEY_RIGHT   "Keys/NavRight"
#define KEY_RIGHT_D  Qt::Key_R

#define KEY_CHOOSE_CD      "Keys/ChooseCD"
#define KEY_CHOOSE_CD_D     Qt::Key_I
#define KEY_PLAY_MEDIA     "Keys/PlayMedia"
#define KEY_PLAY_MEDIA_D    Qt::Key_O
#define KEY_CANCEL_MEDIA   "Keys/CancelMedia"
#define KEY_CANCEL_MEDIA_D  Qt::Key_U

#define KEY_VOLUME          "Keys/Volume"
#define KEY_VOLUME_D         Qt::Key_P
#define KEY_CONFIGURE       "Keys/Configure"
#define KEY_CONFIGURE_D      Qt::Key_X
#define KEY_LEAVE           "Keys/Leave"
#define KEY_LEAVE_D          Qt::Key_L
#define KEY_FULL_SCREEN     "Keys/FullScreen"
#define KEY_FULL_SCREEN_D    Qt::Key_F

KeyManager::KeyManager(MediaCenter *parent) :
    QObject(parent),
    m_mediaCenter(parent),
    m_filterKeys(true)
{
    QSettings settings;
    m_keyUp    = settings.value(QLatin1String(KEY_UP),    KEY_UP_D).toInt();
    m_keyDown  = settings.value(QLatin1String(KEY_DOWN),  KEY_DOWN_D).toInt();
    m_keyLeft  = settings.value(QLatin1String(KEY_LEFT),  KEY_LEFT_D).toInt();
    m_keyRight = settings.value(QLatin1String(KEY_RIGHT), KEY_RIGHT_D).toInt();

    m_keyChooseAlbum = settings.value(QLatin1String(KEY_CHOOSE_CD),    KEY_CHOOSE_CD_D).toInt();
    m_keyPlayMedia   = settings.value(QLatin1String(KEY_PLAY_MEDIA),   KEY_PLAY_MEDIA_D).toInt();
    m_keyCancelMedia = settings.value(QLatin1String(KEY_CANCEL_MEDIA), KEY_CANCEL_MEDIA_D).toInt();

    m_keyVolume       = settings.value(QLatin1String(KEY_VOLUME),        KEY_VOLUME_D).toInt();
    m_keyConfigure    = settings.value(QLatin1String(KEY_CONFIGURE),     KEY_CONFIGURE_D).toInt();
    m_keyLeave        = settings.value(QLatin1String(KEY_LEAVE),         KEY_LEAVE_D).toInt();
    m_keyFullscreen   = settings.value(QLatin1String(KEY_FULL_SCREEN),   KEY_FULL_SCREEN_D).toInt();
}

bool KeyManager::filterKeys() const
{
    return m_filterKeys;
}

void KeyManager::setFilterKeys(bool filterKeys)
{
    if (m_filterKeys != filterKeys) {
        m_filterKeys = filterKeys;
        emit filterKeysChanged();
    }
}

int KeyManager::keyUp() const
{
    return m_keyUp;
}

void KeyManager::setKeyUp(int key)
{
    if (m_keyUp != key) {
        int value = saveKey(QLatin1String(KEY_UP), key);
        if (value != -1) {
            m_keyUp = value;
            emit keysChanged();
        }
    }
}

int KeyManager::keyDown() const
{
    return m_keyDown;
}

void KeyManager::setKeyDown(int key)
{
    if (m_keyDown != key) {
        int value = saveKey(QLatin1String(KEY_DOWN), key);
        if (value != -1) {
            m_keyDown = value;
            emit keysChanged();
        }
    }
}

int KeyManager::keyLeft() const
{
    return m_keyLeft;
}

void KeyManager::setKeyLeft(int key)
{
    if (m_keyLeft != key) {
        int value = saveKey(QLatin1String(KEY_LEFT), key);
        if (value != -1) {
            m_keyLeft = value;
            emit keysChanged();
        }
    }
}

int KeyManager::keyRight() const
{
    return m_keyRight;
}

void KeyManager::setKeyRight(int key)
{
    if (m_keyRight != key) {
        int value = saveKey(QLatin1String(KEY_RIGHT), key);
        if (value != -1) {
            m_keyRight = value;
            emit keysChanged();
        }
    }
}

int KeyManager::keyChooseAlbum() const
{
    return m_keyChooseAlbum;
}

void KeyManager::setKeyChooseAlbum(int key)
{
    if (m_keyChooseAlbum != key) {
        int value = saveKey(QLatin1String(KEY_CHOOSE_CD), key);
        if (value != -1) {
            m_keyChooseAlbum = value;
            emit keysChanged();
        }
    }
}

int KeyManager::keyPlayMedia() const
{
    return m_keyPlayMedia;
}

void KeyManager::setKeyPlayMedia(int key)
{
    if (m_keyPlayMedia != key) {
        int value = saveKey(QLatin1String(KEY_PLAY_MEDIA), key);
        if (value != -1) {
            m_keyPlayMedia = value;
            emit keysChanged();
        }
    }
}

int KeyManager::keyCancelMedia() const
{
    return m_keyCancelMedia;
}

void KeyManager::setKeyCancelMedia(int key)
{
    if (m_keyCancelMedia != key) {
        int value = saveKey(QLatin1String(KEY_CANCEL_MEDIA), key);
        if (value != -1) {
            m_keyCancelMedia = value;
            emit keysChanged();
        }
    }
}

int KeyManager::keyVolume() const
{
    return m_keyVolume;
}

void KeyManager::setKeyVolume(int key)
{
    if (m_keyVolume != key) {
        int value = saveKey(QLatin1String(KEY_VOLUME), key);
        if (value != -1) {
            m_keyVolume = value;
            emit keysChanged();
        }
    }
}

int KeyManager::keyConfigure() const
{
    return m_keyConfigure;
}

void KeyManager::setKeyConfigure(int key)
{
    if (m_keyConfigure != key) {
        int value = saveKey(QLatin1String(KEY_CONFIGURE), key);
        if (value != -1) {
            m_keyConfigure = value;
            emit keysChanged();
        }
    }
}

int KeyManager::keyLeave() const
{
    return m_keyLeave;
}

void KeyManager::setKeyLeave(int key)
{
    if (m_keyLeave != key) {
        int value = saveKey(QLatin1String(KEY_LEAVE), key);
        if (value != -1) {
            m_keyLeave = value;
            emit keysChanged();
        }
    }
}

QString KeyManager::keyString(int key)
{
    QKeySequence seq(key);
    return seq.toString();
}

bool KeyManager::eventFilter(QObject *watched, QEvent *event)
{
//    qDebug() << Q_FUNC_INFO << event;

    if (!m_mediaCenter->showVideo()) {
        m_mediaCenter->showVideoStart();
    }

    if (!m_filterKeys || event->type() != QEvent::KeyPress) {
        return QObject::eventFilter(watched, event);
    }

    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
    if (m_mediaCenter->configuring()) {
        if (keyEvent->key() == m_keyDown) {
            QKeyEvent pressEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
            QApplication::sendEvent(watched, &pressEvent);
            return true;
        } else if (keyEvent->key() == m_keyUp) {
            QKeyEvent pressEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::ShiftModifier);
            QApplication::sendEvent(watched, &pressEvent);
            return true;
        } else if (keyEvent->key() == m_keyRight) {
            QKeyEvent pressEvent(QEvent::KeyPress, Qt::Key_Plus, Qt::NoModifier);
            QApplication::sendEvent(watched, &pressEvent);
            return true;
        } else if (keyEvent->key() == m_keyLeft) {
            QKeyEvent pressEvent(QEvent::KeyPress, Qt::Key_Minus, Qt::NoModifier);
            QApplication::sendEvent(watched, &pressEvent);
            return true;
        }
    } else {
        if (keyEvent->key() == m_keyDown) {
            QKeyEvent pressEvent(QEvent::KeyPress,
                                 Qt::Key_Down,
                                 Qt::NoModifier,
                                 QString(),
                                 keyEvent->isAutoRepeat());
            QApplication::sendEvent(watched, &pressEvent);
            return true;
        } else if (keyEvent->key() == m_keyUp) {
            QKeyEvent pressEvent(QEvent::KeyPress,
                                 Qt::Key_Up,
                                 Qt::NoModifier,
                                 QString(),
                                 keyEvent->isAutoRepeat());
            QApplication::sendEvent(watched, &pressEvent);
            return true;
        } else if (keyEvent->key() == m_keyRight) {
            QKeyEvent pressEvent(QEvent::KeyPress,
                                 Qt::Key_Right,
                                 Qt::NoModifier,
                                 QString(),
                                 keyEvent->isAutoRepeat());
            QApplication::sendEvent(watched, &pressEvent);
            return true;
        } else if (keyEvent->key() == m_keyLeft) {
            QKeyEvent pressEvent(QEvent::KeyPress,
                                 Qt::Key_Left,
                                 Qt::NoModifier,
                                 QString(),
                                 keyEvent->isAutoRepeat());
            QApplication::sendEvent(watched, &pressEvent);
            return true;
        }
    }

    if (keyEvent->key() == m_keyChooseAlbum) {
        QKeyEvent pressEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
        QApplication::sendEvent(watched, &pressEvent);
        QKeyEvent releaseEvent(QEvent::KeyRelease, Qt::Key_Enter, Qt::NoModifier);
        QApplication::sendEvent(watched, &releaseEvent);
        return true;
    } else if (keyEvent->key() == m_keyPlayMedia) {
        QKeyEvent pressEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
        QApplication::sendEvent(watched, &pressEvent);
        QKeyEvent releaseEvent(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
        QApplication::sendEvent(watched, &releaseEvent);
        return true;
    } else if (keyEvent->key() == m_keyFullscreen) {
        m_mediaCenter->setShowFullScreen(!m_mediaCenter->showFullScreen());
        return true;
    } else if (keyEvent->key() == m_keyLeave) {
        m_mediaCenter->close();
        return true;
    } else if (keyEvent->key() == m_keyCancelMedia) {
        m_mediaCenter->nextMedia();
        return true;
    }

    return QObject::eventFilter(watched, event);
}

int KeyManager::saveKey(const QString &settingsKey, int value)
{
    int key = value;
    QHash<QString, int> keys = currentKeys();
    QHash<QString, int>::Iterator it = keys.begin();
    while (it != keys.end()) {
        if (settingsKey != it.key() && value == it.value()) {
            key = -1;
            break;
        }
        ++it;
    }

    if (key != -1) {
        QSettings settings;
        settings.setValue(settingsKey, key);
    }

    return key;
}

QHash<QString, int> KeyManager::currentKeys() const
{
    QHash<QString, int> keys;
    keys[QLatin1String(KEY_UP)]    = m_keyUp;
    keys[QLatin1String(KEY_DOWN)]  = m_keyDown;
    keys[QLatin1String(KEY_LEFT)]  = m_keyLeft;
    keys[QLatin1String(KEY_RIGHT)] = m_keyRight;

    keys[QLatin1String(KEY_CHOOSE_CD)]    = m_keyChooseAlbum;
    keys[QLatin1String(KEY_PLAY_MEDIA)]   = m_keyPlayMedia;
    keys[QLatin1String(KEY_CANCEL_MEDIA)] = m_keyCancelMedia;

    keys[QLatin1String(KEY_VOLUME)]        = m_keyVolume;
    keys[QLatin1String(KEY_CONFIGURE)]     = m_keyConfigure;
    keys[QLatin1String(KEY_LEAVE)]         = m_keyLeave;
    keys[QLatin1String(KEY_FULL_SCREEN)]   = m_keyFullscreen;

    return keys;
}
