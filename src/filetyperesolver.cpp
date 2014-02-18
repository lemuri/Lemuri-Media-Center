#include "filetyperesolver.h"

FileTypeResolver::FileTypeResolver()
{
}

class MyFileTypeResolver : FileTypeResolver
{
  TagLib::File *createFile(TagLib::FileName *fileName, bool, AudioProperties::ReadStyle)
  {
    if(someCheckForAnMP3File(fileName))
      return new TagLib::MPEG::File(fileName);
    return 0;
  }
}
FileRef::addFileTypeResolver(new MyFileTypeResolver);
