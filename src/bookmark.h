#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QString>
#include "timestamp.h"

class Bookmark
{
public:
    Bookmark():timestamp(DEFAULT_TIMESTAMP_BEGIN)
              ,duration(0)
    {}

    Bookmark(const QString& name, const Timestamp& begin, const Timestamp& duration )
            :name(name)
            ,timestamp(begin)
            ,duration(duration)
    {}

public:
    QString name;
    Timestamp timestamp;
    Timestamp duration;
};

#endif // BOOKMARK_H
