#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <QPair>
#include <QString>

typedef long Timestamp;
typedef QPair<Timestamp,Timestamp> TimeStampRange;

const Timestamp MILLISECOND = 1;
const Timestamp SECOND = 1000;
const Timestamp MINUTE = 60*SECOND;
const Timestamp HOUR   = 60*MINUTE;
const Timestamp DAY    = 24*HOUR;

const Timestamp DEFAULT_TIMESTAMP_BEGIN = 0;
const Timestamp DEFAULT_TIMESTAMP_END   = DAY;//1 day in milliseconds

const TimeStampRange DEFAULT_TIMESTAMP_RANGE = TimeStampRange(DEFAULT_TIMESTAMP_BEGIN,DEFAULT_TIMESTAMP_END);

enum TimestampParts
{
    Days      = 0,
    Hours     = 0x1,
    Minutes   = 0x2,
    Seconds   = 0x4,
    Milliseconds = 0x8,
    TimestampAllParts = 0xf
};

struct TimestampComposition
{
    int days; //always 0 yet
    int hours;
    int minutes;
    int seconds;
    int milliseconds;
};

inline TimestampComposition splitTimeStamp(const Timestamp& t)
{
    TimestampComposition result;
    result.days = 0;
    result.hours   =  t / HOUR ;
    result.minutes = (t % HOUR )  / MINUTE;
    result.seconds = (t % MINUTE )/ SECOND;
    result.milliseconds = t % SECOND;
    return result;
}


inline Timestamp posToTimestamp(double pos, int width, const TimeStampRange& range)
{
    Timestamp result = range.first+(pos/width)*(range.second-range.first);
    return result;
}

inline double timestampToPos(const Timestamp& t, int width, const TimeStampRange& range)
{
    double f = double(t-range.first)/double(range.second-range.first);
    double result = width*f;
    return result;
}

inline QString makeString(const Timestamp& t, int parts = TimestampAllParts)
{
    QString result;
    TimestampComposition tp = splitTimeStamp(t);
    if(( parts & Hours ) ){
        result += QString("%1h").arg(tp.hours,2,10,QChar('0'));
    }
    if( ( parts & Minutes ) ){
        result += QString("%1m").arg(tp.minutes,2,10,QChar('0'));
    }
    if( ( parts & Seconds ) ){
        result += QString("%1s").arg(tp.seconds,2,10,QChar('0'));
    }
    if( ( parts & Milliseconds ) ){
        result += QString("%1ms").arg(tp.milliseconds,3,10,QChar('0'));
    }
    return result;
}

#endif // TIMESTAMP_H
