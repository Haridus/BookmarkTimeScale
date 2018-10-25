#include "bookmarkviewwidget.h"
#include <qDebug>
#include <QMap>
#include <QPainter>
#include <QTimer>
#include <QToolTip>

enum BookmarkViewItemType
{
    BookmarkItem = 0,
    GroupItem
};

struct BookmarkViewItem
{
    BookmarkViewItem()
      : type(BookmarkItem)
      ,posStart(-1)
      ,posEnd(-1)
      ,begin(-1)
      ,end(-1)
    {}

    BookmarkViewItem(int type
                    ,int ps
                    ,int pe
                    ,int bg
                    ,int end
                     ): type(type)
                       ,posStart(ps)
                       ,posEnd(pe)
                       ,begin(bg)
                       ,end(end)
    {

    }

    bool isValid()const
    {
        return posStart>=0 && posEnd > 0 && begin>=0  && end >= 0;
    }

    int type;
    int posStart;
    int posEnd;
    int begin;
    int end;
};

class BookmarkViewWidgetPrivate
{
public:
    BookmarkViewWidgetPrivate() : model(nullptr), range(DEFAULT_TIMESTAMP_RANGE)
    {}

    void recalculateElements(const QRect& geometry)
    {
        qDebug()<<"begin recalculate";
        if( !model  ){
            return;
        }
        if(model->rowCount() == 0 ){
            //clear;
        }
        items.clear();

        int width = geometry.width();
        auto lbond = model->lower_bond(range.first,0);
        auto rbond =  model->upper_bond(range.second,0) ;
        rbond = rbond == -1 ? model->rowCount() : rbond;
        for(auto srow = lbond;(srow !=-1) && (rbond - srow > 0) && ( srow < model->rowCount() ) ; ){
            Timestamp _t1 = model->index(srow,1).data().toInt();
            double posStart  = timestampToPos(_t1,width,range);
            double posEnd    = posStart+100;
            Timestamp _t2 = posToTimestamp(posEnd,width,range);

            auto  erow = model->upper_bond(_t2,0);
            erow = erow == -1 ? model->rowCount() : erow;

            int diff = erow - srow;

            items[posStart] = BookmarkViewItem(diff < 2 ? BookmarkItem : GroupItem, posStart, posEnd, srow, erow);
            qDebug()<<"item:"<<items[posStart].type<<items[posStart].posStart<<items[posStart].posEnd;

            srow = erow;
        }
        qDebug()<<items.count();
    }

    BookmarkViewItem itemAt(const QPoint& p)
    {
        BookmarkViewItem item;
        auto i = items.find(p.x());
        if(i == items.end() ){
            i = items.lowerBound(p.x()) - 1;//see QMap description
            if( ( i.value().posStart < p.x() ) && ( i.value().posEnd > p.x() ) ){
                item = i.value();
            }
        }
        return item;
    }

public:
    AbstractBookmarkModel* model;
    TimeStampRange range;
    QMap<int,BookmarkViewItem> items;
};

//------------------------------------
BookmarkViewWidget::BookmarkViewWidget(QWidget *parent)
                   :QWidget(parent)
                   ,d_ptr(new BookmarkViewWidgetPrivate)
{}

BookmarkViewWidget::~BookmarkViewWidget()
{
    delete d_ptr;
}

void BookmarkViewWidget::setModel(AbstractBookmarkModel* model)
{
    if( d_ptr->model ){
        disconnect(d_ptr->model,SIGNAL(modelReset()),this,SLOT(onModelChanged()));
    }
    d_ptr->model = model;
    if( d_ptr->model ){
        connect(d_ptr->model,SIGNAL(modelReset()),this,SLOT(onModelChanged()));
    }
}

AbstractBookmarkModel* BookmarkViewWidget::model()const
{
    return d_ptr->model;
}

void BookmarkViewWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();
    painter.setPen(QPen(Qt::black,1));
        for( auto iter = d_ptr->items.begin(); iter != d_ptr->items.end() ; iter++ ){
            QString text;
            switch (iter->type) {
            case GroupItem:
                painter.setBrush(Qt::green);
                text = QString("%1").arg(iter->end-iter->begin);
                break;
            case BookmarkItem:
                painter.setBrush(Qt::blue);
                text = QString("%1").arg(d_ptr->model->index(iter->begin,0).data().toString());
                break;
            }
            QRect itemRect = QRect( iter->posStart, 0, 100, height() );
            painter.drawRect( itemRect );

            QRectF  textRect = itemRect;
            QTextOption textOpt = QTextOption(Qt::AlignHCenter | Qt::AlignVCenter);
            painter.drawText(textRect,text,textOpt);
        }
    painter.restore();

    QWidget::paintEvent(event);
}

void BookmarkViewWidget::resizeEvent(QResizeEvent *event)
{
    d_ptr->recalculateElements(geometry());
    update();
}

bool BookmarkViewWidget::event(QEvent* e)
{
    bool result = false;
    switch (e->type()) {
    case QEvent::ToolTip:
    case QEvent::WhatsThis:
    {
        QHelpEvent* he = (QHelpEvent*)e;
        BookmarkViewItem item = d_ptr->itemAt( he->pos() );
        if( item.isValid() ){
            qDebug()<<"item found";
            switch (item.type) {
                case BookmarkItem:
                {
                    QString name = d_ptr->model->index(item.begin,0).data().toString();
                    Timestamp t  = d_ptr->model->index(item.begin,1).data().toInt();
                    Timestamp d  = d_ptr->model->index(item.begin,2).data().toInt();
                    QToolTip::showText(he->globalPos(),QString("%1[%2,%3]").arg(name).arg(makeString(t)).arg(makeString(d)));
                }
                    break;
                case GroupItem:
                {
                    QStringList entryes;
                    for(int i = 0; (i < (item.end-item.begin) ) && (i < 15) ; i++){
                        entryes << d_ptr->model->index(item.begin+i,0).data().toString();
                    }
                    int remain = item.end -item.begin - 15;
                    if( remain > 0){
                        entryes<<QString("+%1 bookmarks").arg(remain);
                    }
                    QToolTip::showText(he->globalPos(),entryes.join("\n"));
                }
                    break;
            }
        }
        else{
            qDebug()<<"no item here";
        }
    }
        break;
    default:
        result = QWidget::event(e);
        break;
    }
    return result;
}

void BookmarkViewWidget::onModelChanged()
{
    d_ptr->recalculateElements(geometry());
    update();
}

void BookmarkViewWidget::setRange(const Timestamp& t1, const Timestamp& t2)
{
    d_ptr->range = TimeStampRange(t1,t2);
    d_ptr->recalculateElements(geometry());
    update();
}
