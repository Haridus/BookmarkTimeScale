#ifndef GENERATORTHREAD_H
#define GENERATORTHREAD_H

#include <QThread>
#include "bookmarkmodel.h"

class BookmarkGeneratorContext;

class BookmarkGeneratorThread : public QThread
{
    Q_OBJECT
public:
    BookmarkGeneratorThread(BookmarkGeneratorContext* context, int count)
        :pContext(context),
         mCount(count),
         mFinish(false)
    {}

    void finish()
    {
        mFinish = true;
    }

protected:
    void run() override;

private:
    BookmarkGeneratorContext* pContext;
    int mCount;
    bool mFinish;
};

#endif // GENERATORTHREAD_H
