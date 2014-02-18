#ifndef MIMEFILERESOLVER_H
#define MIMEFILERESOLVER_H

#include <QMimeDatabase>

#include <fileref.h>

using namespace TagLib;

namespace Lemuri
{

class FileTypeResolver : public TagLib::FileRef::FileTypeResolver
{
public:
    virtual ~FileTypeResolver() {}

    TagLib::File *createFile(TagLib::FileName fileName,
                             bool readProperties,
                             TagLib::AudioProperties::ReadStyle propertiesStyle) const;

private:
    QMimeDatabase m_mimeDb;
};

}

#endif // MIMEFILERESOLVER_H
