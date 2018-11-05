#include "bookmarkmodel.h"
#include <map>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>
#include "generatorthread_p.h"

//[0]
typedef std::vector<Bookmark> InternalData;

bool operator<(const Bookmark& b1, const Bookmark& b2)
{
    bool result = (b1.timestamp < b2.timestamp) || ( (b1.timestamp == b2.timestamp) && (b1.duration < b2.duration));
    return result;
}

//[1]
class BookmarkGeneratorContext
{
public:
    BookmarkGeneratorContext():dataptr(nullptr)
    {}
    BookmarkGeneratorContext(void *d): dataptr(d)
    {}

public:
    QMutex mutex;
    void*  dataptr;
};

void BookmarkGeneratorThread::run()
{
    if( pContext == nullptr || pContext->dataptr == nullptr) return;
    if( mCount == 0 ) return;
    mFinish = false;

    QMutexLocker lock(&pContext->mutex);
    InternalData* dataptr = (InternalData*)pContext->dataptr;
	
    for( int i = 0; i < dataptr->size(); i++ )
    {
        Bookmark& bookmark = dataptr->at(i);
        bookmark.name = QString("%1").arg(i);
        bookmark.timestamp = (qrand()/32765.0)*DAY;
        bookmark.duration  = (qrand()/32765.0)*(3*HOUR);

        if( i % 1000 == 0 ){
            qDebug()<<i;
        }

        if(mFinish){return;}
    }

    std::sort(dataptr->begin(),dataptr->end());
}

//[2]
class CustomBookmarkModelPrivate
{
public:
    CustomBookmarkModelPrivate() : generatorThread(nullptr)
                                  ,generatorContext((void*)&data)
    {}

public:
    QMutex mutex;
    InternalData data;
    BookmarkGeneratorThread* generatorThread;
    BookmarkGeneratorContext generatorContext;
};

//[3]
CustomBookmarkModel::CustomBookmarkModel(QObject *parent):AbstractBookmarkModel(parent)
                                                         ,d_ptr(new CustomBookmarkModelPrivate)
{}

CustomBookmarkModel::~CustomBookmarkModel()
{
    delete d_ptr;
}

int  CustomBookmarkModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant CustomBookmarkModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() ){
        return QVariant();
    }

    QVariant value;
    if(role == Qt::DisplayRole){
        int row  = index.row();
        int col  = index.column();
        Bookmark& bookmark = d_ptr->data.at(row);
        switch (col) {
        case 1:
            value = QVariant((int)bookmark.timestamp);
            break;
        case 2:
            value = QVariant((int)bookmark.duration);
            break;
        case 0:
        default:
            value = QString("Bookmark: ")+bookmark.name;
            break;
        }
    }
    return value;
}

QModelIndex CustomBookmarkModel::index(int row, int column, const QModelIndex &parent) const
{
    QModelIndex index;
    if( !isGenerating() ){
        if( row < d_ptr->data.size() ){
            index = createIndex(row,column,row);
        }
    }
    return index;
}

QModelIndex CustomBookmarkModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int CustomBookmarkModel::rowCount(const QModelIndex &parent) const
{
    int count = 0;
    if( !isGenerating() ){
        count = d_ptr->data.size();
    }
}

int CustomBookmarkModel::lower_bond(const Timestamp& t, const Timestamp& duration)const
{
    int bond = -1;
    if( !isGenerating() ){
        auto lbond = std::lower_bound(d_ptr->data.begin(),d_ptr->data.end(),Bookmark("",t,duration));
        bond =  lbond != d_ptr->data.end() ? lbond - d_ptr->data.begin() : -1;
    }
    return bond;
}

int CustomBookmarkModel::upper_bond(const Timestamp& t, const Timestamp& duration)const
{
    int bond = -1;
    if( !isGenerating() ){
        auto rbond = std::upper_bound(d_ptr->data.begin(),d_ptr->data.end(),Bookmark("",t,duration));
        bond =  rbond != d_ptr->data.end() ? rbond - d_ptr->data.begin() : -1;
    }
    return bond;
}

void CustomBookmarkModel::generate(int n)
{
    if( !isGenerating() ){
        if( d_ptr->generatorThread ){
            delete d_ptr->generatorThread;
        }
		beginResetModel();
		d_ptr->data.clear();
		d_ptr->data.resize( n );
        d_ptr->generatorThread = new BookmarkGeneratorThread(&d_ptr->generatorContext, n);
        connect(d_ptr->generatorThread,SIGNAL(finished()),this,SLOT(onGenerationFinish()));
        d_ptr->generatorThread->start();
    }
}

bool CustomBookmarkModel::isGenerating() const
{
    return d_ptr->generatorThread && d_ptr->generatorThread->isRunning();
}

void CustomBookmarkModel::onGenerationFinish()
{
    d_ptr->generatorThread->deleteLater();
    d_ptr->generatorThread = nullptr;

    //emit dataChanged(index(0,0),index(d_ptr->data.size(),2));
    endResetModel();
    qDebug()<<"done!";
}
