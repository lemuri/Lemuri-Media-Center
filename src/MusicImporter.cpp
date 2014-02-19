#include "MusicImporter.h"

#include "MediaCenter.h"
#include "mimefileresolver.h"

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QFileSystemWatcher>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QStringBuilder>
#include <QDateTime>
#include <QDebug>
#include <QImage>

#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <fileref.h>
#include <tag.h>

MusicImporter::MusicImporter() :
    m_rwXapianDB(0),
    m_mediaPath(MediaCenter::pathMedia()),
    m_mediaCount(0)
{
    TagLib::FileRef::addFileTypeResolver(new Lemuri::FileTypeResolver());
}

void MusicImporter::init()
{
    bool isOpen = open();
    qWarning() << Q_FUNC_INFO << isOpen;
    if (!isOpen) {
        return;
    }

    QTime timer;
    timer.start();
    quint64 medieCount = m_mediaCount;
    if (validateEntries()) {
        commit();
        emit changed();
    }
    checkMediaCountChanged(medieCount);
    qWarning() << "Import validateEntries: " <<  timer.elapsed() << "ms";

    timer.start();
    if (scanPath(m_mediaPath)) {
        commit();
        emit changed();
    }
    checkMediaCountChanged(medieCount);
    qWarning() << "Import scanPath m_mediaPath: " <<  timer.elapsed() << "ms";

    emit finished();
}

void MusicImporter::markItem(const QString &filename)
{
//    QString uuid = uniqueId(filename);

//    Xapian::PostingIterator p = m_rwXapianDB->postlist_begin(uuid.toStdString());
//    if (p != m_rwXapianDB->postlist_end(uuid.toStdString())) {
//        Xapian::Document document = m_rwXapianDB->get_document(*p);

//        QDateTime dt = QDateTime::currentDateTime();
//        QString term("D");
//        term.append(dt.toString(Qt::ISODate));

//        document.add_boolean_term(term.toStdString());
//        m_rwXapianDB->replace_document(uuid.toStdString(), document);
//        qDebug() << term << document.termlist_count();

//        Xapian::TermIterator it = document.termlist_begin();
//        while (it != document.termlist_end()) {
////            qDebug() << QString::fromStdString(it.get_description()) << it.get_termfreq() << it.get_wdf() << QString::fromStdString(*it);
//            qDebug() << QString::fromStdString(*it);
//            ++it;
//        }
    //    }
}

bool MusicImporter::open()
{
    if (m_rwXapianDB == 0) {
        try {
            m_rwXapianDB = new Xapian::WritableDatabase(MediaCenter::xapianDBPath().toStdString(),
                                                        Xapian::DB_CREATE_OR_OPEN);
        } catch (const Xapian::Error &error) {
            qWarning() << "Exception: " << QString::fromStdString(error.get_msg());
            emit errorMessage(tr("Falha ao abrir índice: %1").arg(QString::fromStdString(error.get_msg())));
            return false;
        }
    }
    return true;
}

bool MusicImporter::validateEntries()
{
    bool changed = false;
    bool inited = m_mediaCount;

    QList<Xapian::docid> removeInvalidMedia;
    // Verify if the file is still valid or if it has been updated
    try {
        Xapian::PostingIterator it = m_rwXapianDB->postlist_begin(std::string());
        while (it != m_rwXapianDB->postlist_end(std::string())) {
            try {
                Xapian::Document doc = m_rwXapianDB->get_document(*it);

                QFileInfo fileInfo(QString::fromStdString(doc.get_value(AbsoluteFilePath)));

                bool validDocument = true;
                if (fileInfo.exists()) {
                    uint lastModified = atoi(doc.get_value(FileLastModified).c_str());
                    if (fileInfo.lastModified().toTime_t() != lastModified) {
                        // File has changed, update it's entry
//                        qDebug() << "File Changed: " << fileInfo.absoluteFilePath();
                        validDocument = addFile(fileInfo);
                    }

                    if (!inited && validDocument) {
                        ++m_mediaCount;
                    }
                } else {
                    validDocument = false;
                }

                if (!validDocument) {
//                    qDebug() << "Removing file:" << fileInfo.absoluteFilePath();
                    removeInvalidMedia << *it;
                }
            } catch (const Xapian::Error &error) {
                qWarning() << "Error getting document: " << QString::fromStdString(error.get_msg());
                emit errorMessage(tr("Erro ao obter o documento: %1").arg(QString::fromStdString(error.get_msg())));
            }

            ++it;
        }
    } catch (const Xapian::Error &error) {
        qWarning() << "Error while iterating: " << QString::fromStdString(error.get_msg());
        emit errorMessage(tr("Erro ao validar o index: %1").arg(QString::fromStdString(error.get_msg())));
    }

    // Remove the terms later to not corrupt the database
    foreach (const Xapian::docid &docid, removeInvalidMedia) {
        try {
            m_rwXapianDB->delete_document(docid);

            if (inited) {
                --m_mediaCount;
            }
            changed = true;
        } catch (const Xapian::Error &error) {
            qWarning() << "Error while deletting the document: " << QString::fromStdString(error.get_msg());
            emit errorMessage(tr("Erro ao apagar documento: %1").arg(QString::fromStdString(error.get_msg())));
        }
    }

    return changed;
}

bool MusicImporter::scanPath(const QString &path)
{
    bool changed = false;

    QDir pathDir = path;
    if (!pathDir.exists() && !pathDir.mkpath(path)) {
        qWarning() << "Falha ao criar diretorio:" << path;
        emit errorMessage(tr("Falha ao criar diretorio: %1").arg(path));
        return false;
    }

    qWarning() << "diretorio:" << path;
    QDirIterator it(path, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        const QString &filePath = it.next();
        const QString &mime = m_db.mimeTypeForFile(filePath).name();
        qWarning() << "file & mime:" << filePath << mime;
        if (mime.startsWith(QLatin1String("video/")) ||
                mime.startsWith(QLatin1String("video/"))) {
            // Check if the document is on the DataBase
            const QString &uuid = uniqueId(filePath);
            if (!hasDocument(uuid) && addFile(it.fileInfo(), uuid)) {
                ++m_mediaCount;
                changed = true;
            }
        }
    }

    return changed;
}

void MusicImporter::commit()
{
    qDebug() << Q_FUNC_INFO;
    try {
        m_rwXapianDB->commit();
    } catch (const Xapian::Error &error) {
        qWarning() << "Error while commiting:" << QString::fromStdString(error.get_msg());
        emit errorMessage(tr("Erro ao persistir o banco de dados: %1").arg(QString::fromStdString(error.get_msg())));
    }
}

QString MusicImporter::uniqueId(const QString &filePath) const
{
    // Creates an unique id for the given path
    return QLatin1Char('Q') %
            QCryptographicHash::hash(filePath.toUtf8(),
                                     QCryptographicHash::Sha256).toBase64();
}

bool MusicImporter::addFile(const QFileInfo &fileInfo, const QString &uuid)
{
    QFile file(fileInfo.absoluteFilePath().toLocal8Bit());
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Failed to open file while importing:" << fileInfo.fileName() << file.errorString();
        return false;
    }

    Xapian::Document document;

    QString id = uuid;
    if (id.isNull()) {
        id = uniqueId(fileInfo.absoluteFilePath());
    }
    // Add the unique id term
    document.add_term(id.toStdString());

    // Store the information about the file
    const QString &absolutePath = fileInfo.absolutePath();
    QString album = movieSection(absolutePath);
    QString title = fileInfo.completeBaseName();
    document.add_value(AbsoluteFilePath, fileInfo.absoluteFilePath().toStdString());
    document.add_value(AbsolutePath, absolutePath.toStdString());
    document.add_value(AlbumDir, album.toStdString());
    document.add_value(FileLastModified, QString::number(fileInfo.lastModified().toTime_t()).toStdString());
    document.add_value(FileSize, QString::number(fileInfo.size()).toStdString());

    TagLib::FileRef fileTag(fileInfo.absoluteFilePath().toUtf8().constData());
    if (!fileTag.isNull()) {
        //    if (file.mimeType().startsWith(QLatin1String("video"))) {
        //        QFile dvd(absolutePath % QLatin1String("/.dvd"));
        //        if (!dvd.exists()) {
        //            dvd.open(QIODevice::WriteOnly);
        //        }
        //        document.add_value(IsVideo, "1");
        //    } else {
        document.add_value(IsVideo, "0");
        //    }

        qDebug() << fileInfo.absoluteFilePath();

        const TagLib::String &albumTag = fileTag.tag()->album();
        if (!albumTag.isEmpty() && !albumTag.isNull()) {
            album = albumTag.toCString(true);
        }
        document.add_value(Album, album.toStdString());

        const TagLib::String &artistTag = fileTag.tag()->artist();
        if (!artistTag.isEmpty() && !artistTag.isNull()) {
            document.add_value(Artist, artistTag.toCString(true));
        }

        const TagLib::String &titleTag = fileTag.tag()->title();
        if (!titleTag.isEmpty() && !titleTag.isNull()) {
            title = titleTag.toCString(true);
        } else {
            title = fileInfo.completeBaseName();
        }

        uint year = fileTag.tag()->year();
        document.add_value(AlbumYear, QString::number(year).toStdString());

        uint trackNumber = fileTag.tag()->track();
        document.add_value(TrackNumber, QString::number(trackNumber).toStdString());

        const TagLib::String &genreTag = fileTag.tag()->genre();
        if (!genreTag.isEmpty() && !genreTag.isNull()) {
            document.add_value(Genre, genreTag.toCString(true));
        } else {
            document.add_value(Genre, tr("Unknown").toStdString());
        }
    } else {
        qDebug() << "Null taglib for:" << fileInfo.absoluteFilePath();
    }

    document.add_value(Title, title.toStdString());

    try {
        // It's said that adding the docs sorted is faster.. (change in future)
        m_rwXapianDB->replace_document(id.toStdString(), document);
        createCoverFile(fileInfo.dir().path(), title);

        return true;
    } catch (const Xapian::Error &error) {
        qWarning() << "Erro while adding documment: " << QString::fromStdString(error.get_msg());
        emit errorMessage(tr("Erro ao adicionar documento: %1").arg(QString::fromStdString(error.get_msg())));
        return false;
    }
}

void MusicImporter::createCoverFile(const QString &absolutePath, const QString &title)
{
    QStringList nameFilters;
    nameFilters << QLatin1String("*.png");
    nameFilters << QLatin1String("*.jpeg");
    nameFilters << QLatin1String("*.jpg");
    nameFilters << QLatin1String("*.pamp");

    QFileInfoList infoList;

    bool localFound = false;
    QDir dir(absolutePath);
    infoList = dir.entryInfoList(nameFilters,
                                 QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                                 QDir::Size);
    foreach (const QFileInfo &fileInfo, infoList) {
        const QString &absoluteFilePath = fileInfo.absoluteFilePath();
        QImage image(fileInfo.absoluteFilePath());
        if (image.format() != QImage::Format_Invalid &&
                QFile::copy(absoluteFilePath, MediaCenter::coverPath(absolutePath))) {
            localFound = true;
            break;
        }
    }

    if (localFound) {
        return;
    }

    QNetworkAccessManager nam;
    QUrl url("https://api.themoviedb.org/3/search/movie");
    QUrlQuery query;
    query.addQueryItem(QLatin1String("api_key"), QLatin1String("3e4b22d9919ae1fcaada321737e19dba"));
    query.addQueryItem(QLatin1String("query"), title);
    query.addQueryItem(QLatin1String("language"), QLatin1String("pt_BR"));
    url.setQuery(query);

    qDebug() << url << url.toString();
    QNetworkRequest req(url);
    req.setRawHeader("Accept", "application/json");

    QNetworkReply *reply = nam.get(req);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished,
            &loop, &QEventLoop::quit);
    loop.exec();

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    if (document.isNull()) {
        qDebug() << "Null document for" << title;
        return;
    }

//    document.object()["results"]
    QJsonArray array = document.object()["results"].toArray();
    if (array.isEmpty()) {
        qDebug() << "Null array for" << title;
        return;
    }

    QString posterPath = array.first().toObject()["poster_path"].toString();
    if (!posterPath.isEmpty()) {
        QUrl urlImg("http://image.tmdb.org/t/p/w500/" % posterPath);
        QNetworkRequest reqImg(urlImg);
        QNetworkReply *replyImg = nam.get(reqImg);
        QEventLoop loop;
        connect(replyImg, &QNetworkReply::finished,
                &loop, &QEventLoop::quit);
        loop.exec();

        if (replyImg->error()) {
            qDebug() << replyImg->errorString();
            return;
        }

        QFile imgFile(MediaCenter::coverPath(absolutePath));
        if (imgFile.open(QFile::ReadWrite)) {
            imgFile.write(replyImg->readAll());
        } else {
            qDebug() << "Failed to open" << imgFile.fileName();
        }
    } else {
        qDebug() << "Null results for" << title;
    }
}

void MusicImporter::checkMediaCountChanged(quint64 &medieCount)
{
    if (medieCount != m_mediaCount) {
        medieCount = m_mediaCount;

        emit mediaCountChanged(medieCount);
    }
}

QString MusicImporter::movieSection(const QString &absolutePath)
{
    return absolutePath.section(QLatin1Char('/'), -1);
}

bool MusicImporter::hasDocument(const QString &uniqueId)
{
    // Try to find the item
    return m_rwXapianDB->postlist_begin(uniqueId.toStdString())
            != m_rwXapianDB->postlist_end(uniqueId.toStdString());
}
