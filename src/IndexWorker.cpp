#include "IndexWorker.h"

#include "MusicImporter.h"
#include "MediaCenter.h"

#include <QDir>
#include <QXmlStreamWriter>
#include <QStringBuilder>
#include <QDebug>

bool trackCaseInsensitiveLessThan(const TrackInfo &s1, const TrackInfo &s2)
{
    if (s1.trackNumber == 0 && s2.trackNumber > 0) {
        return false;
    } else if (s1.trackNumber == s2.trackNumber) {
        return QString::localeAwareCompare(s1.title, s2.title) < 0;
    }
    return s1.trackNumber < s2.trackNumber;
}

bool genreCaseInsensitiveLessThan(const GenreInfo &s1, const GenreInfo &s2)
{
    return QString::localeAwareCompare(s1.name, s2.name) < 0;
}

bool albumCaseInsensitiveLessThan(const AlbumInfo &s1, const AlbumInfo &s2)
{
    int artistRes = QString::localeAwareCompare(s1.artist, s2.artist);
    if (artistRes < 0) {
        return true;
    } else if (artistRes == 0) {
        int nameRes = QString::localeAwareCompare(s1.name, s2.name);
        if (nameRes < 0) {
            return true;
        } else if (nameRes == 0) {
            return s1.path.toLower() < s2.path.toLower();
        }
    }
    return false;
}

IndexWorker::IndexWorker():
    m_database(0)
{
}

void IndexWorker::close()
{
    if (m_database) {
        m_database->close();
        delete m_database;
        m_database = 0;
    }
}

void IndexWorker::facetedSearch(quint64 id, int slot, const QStringList &genres)
{
    if (!open(id)) {
        return;
    }

    Xapian::Enquire enquire(*m_database);
    Xapian::ValueCountMatchSpy spy0(slot);
    enquire.add_matchspy(&spy0);

    enquire.set_query(filter(genres));

    try {
        enquire.get_mset(0, 1, m_database->get_doccount());
    } catch (const Xapian::Error &xError) {
        qWarning() << "Exception: " << QString::fromStdString(xError.get_msg());
        QString errorMsg = QString::fromUtf8("Erro ao obter dados: %1")
                .arg(QString::fromStdString(xError.get_msg()));
        emit failed(id, errorMsg);
        return;
    }

    if (slot == MusicImporter::Genre) {
        GenreInfoList ret;
        // Iterate on the spies
        Xapian::TermIterator it = spy0.values_begin();
        while (it != spy0.values_end()) {
            GenreInfo info;
            info.name = QString::fromStdString(*it);
            info.mediaCount = it.get_termfreq();

            ret << info;

            ++it;
        }
        qSort(ret.begin(), ret.end(), genreCaseInsensitiveLessThan);
//        qDebug() << Q_FUNC_INFO << "genre finished";
        emit getGenresFinished(id, ret);
    } else if (slot == MusicImporter::AbsolutePath) {
        AlbumInfoList ret;
        // Iterate on the spies
        for (Xapian::TermIterator i = spy0.values_begin(); i != spy0.values_end(); ++i) {
            AlbumInfo info;
            info.path = QString::fromStdString(*i);
//            info.artist = CryptoFile::artistSection(info.path);
//            info.name = CryptoFile::albumSection(info.path);

            QFile dvd(info.path % QLatin1String("/.dvd"));
            info.isDvd = dvd.exists();

            ret << info;
        }
        qSort(ret.begin(), ret.end(), albumCaseInsensitiveLessThan);
//        qDebug() << Q_FUNC_INFO << "album finished";
        emit getAlbumsFinished(id, ret);
    } else {
        QStringList ret;
        // Iterate on the spies
        for (Xapian::TermIterator i = spy0.values_begin(); i != spy0.values_end(); ++i) {
            ret << QString::fromStdString(*i);
        }
        ret.sort();
        emit facetedSearchFinished(id, ret);
    }
}

void IndexWorker::getTracks(quint64 id, const QString &albumPath, const QStringList &genreFilters)
{
    if (!open(id)) {
        return;
    }

    TrackInfoList ret;

    Xapian::Enquire enquire(*m_database);
    Xapian::Query categoryPathQuery(Xapian::Query::OP_VALUE_RANGE,
                                    MusicImporter::AbsolutePath,
                                    albumPath.toStdString(),
                                    albumPath.toStdString());
    Xapian::Query query(Xapian::Query::OP_AND,
                        categoryPathQuery,
                        filter(genreFilters));
    enquire.set_query(query);

    // Get matches one result is enough
    Xapian::MSet matches = enquire.get_mset(0, 60, m_database->get_doccount());
    for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i) {
        Xapian::Document doc = i.get_document();

        TrackInfo info;
        info.title = QString::fromStdString(doc.get_value(MusicImporter::Title));
        info.absoluteFilePath = QString::fromStdString(doc.get_value(MusicImporter::AbsoluteFilePath));
        info.artist = QString::fromStdString(doc.get_value(MusicImporter::Artist));
        info.isVideo = doc.get_value(MusicImporter::IsVideo).compare("0");
        info.trackNumber = std::atoi(doc.get_value(MusicImporter::TrackNumber).c_str());

        ret << info;
    }
    qSort(ret.begin(), ret.end(), trackCaseInsensitiveLessThan);

    emit getTracksFinished(id, ret);
}

bool IndexWorker::open(quint64 id)
{
    if (m_database) {
        return true;
    }

    try {
        m_database = new Xapian::Database(MediaCenter::xapianDBPath().toStdString());
    } catch (const Xapian::Error &error) {
        qWarning() << "Exception: " << QString::fromStdString(error.get_msg());
        QString errorMsg  = QString::fromUtf8("Erro ao abrir índice: %1")
                .arg(QString::fromStdString(error.get_msg()));
        emit failed(id, errorMsg);
        return false;
    }

    return true;
}

Xapian::Query IndexWorker::filter(const QStringList &genres) const
{
    if (genres.isEmpty()) {
        return Xapian::Query::MatchAll;
    }

    Xapian::Query query(Xapian::Query::OP_VALUE_RANGE,
                        MusicImporter::Genre,
                        genres.first().toStdString(),
                        genres.first().toStdString());
    if (genres.size() >= 2) {
        return Xapian::Query(Xapian::Query::OP_AND_NOT,
                             filter(genres.mid(1)),
                             query);
    } else {
        return Xapian::Query(Xapian::Query::OP_AND_NOT,
                             Xapian::Query::MatchAll,
                             query);
    }
}
