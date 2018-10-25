#include "timescale.h"
#include <QPair>
#include <QPainter>
#include <QTextOption>
#include <QRubberBand>
#include <QDebug>
#include <QFontMetrics>

class TimeScalePrivate
{
public:
    TimeScalePrivate(): range(DEFAULT_TIMESTAMP_RANGE)
                       ,rubberBand(nullptr)
    {}

    ~TimeScalePrivate()
    {
        if( rubberBand ){
            delete rubberBand;
        }
    }

public:
    TimeStampRange range;
    QRubberBand*   rubberBand;
    QPoint         anchorPos;
};

//-----------------------------------------------------------------
TimeScale::TimeScale(QWidget *parent)
          :d_ptr(new TimeScalePrivate)
{}

TimeScale::~TimeScale()
{
    delete d_ptr;
}

void TimeScale::setRange(const Timestamp& t1, const  Timestamp& t2)
{
    if( t1 == t2 ){
        d_ptr->range = DEFAULT_TIMESTAMP_RANGE;
    }
    else{
        Timestamp _t1 = t1 < t2 ? t1 : t2;
        Timestamp _t2 = t1 < t2 ? t2 : t1;
        _t1 = _t1 < DEFAULT_TIMESTAMP_BEGIN ? DEFAULT_TIMESTAMP_BEGIN : _t1;
        _t2 = _t2 > DEFAULT_TIMESTAMP_END ? DEFAULT_TIMESTAMP_END : _t2;

        if( (_t2 - _t1) < 24*MILLISECOND ){
            Timestamp _center = (_t1+_t2)/2;
            _t1 = _center - 12*MILLISECOND;
            _t2 = _center + 12*MILLISECOND;
        }

        d_ptr->range = qMakePair(_t1,_t2);
    }
    emit rangeChanged(d_ptr->range.first,d_ptr->range.second);
    update();
}

void TimeScale::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);
}

void TimeScale::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    if( d_ptr->rubberBand && (event->buttons() & Qt::RightButton) ){
        QRect rubberRect = QRect( QPoint(d_ptr->anchorPos.x(),0), QPoint(event->pos().x(),height()) ).normalized();
        d_ptr->rubberBand->setGeometry(rubberRect);
        qDebug()<<rubberRect;
    }
    if( event->buttons() & Qt::LeftButton){
        int diff = d_ptr->anchorPos.x() - event->pos().x();
        Timestamp tdiff = ( diff/float( width() ) )*(d_ptr->range.second-d_ptr->range.first);
        setRange(d_ptr->range.first+tdiff,d_ptr->range.second+tdiff);
        d_ptr->anchorPos = event->pos();
    }
}

void TimeScale::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    d_ptr->anchorPos = event->pos();
    if( event->button() == Qt::RightButton ){
       if( !d_ptr->rubberBand ){
           d_ptr->rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
       }
       d_ptr->rubberBand->setGeometry(QRect(QPoint(d_ptr->anchorPos.x(),0), QSize()) );
       d_ptr->rubberBand->show();
    }
}

void TimeScale::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if( d_ptr->rubberBand && ( event->button() == Qt::RightButton) ){
        d_ptr->rubberBand->hide();
        int left = d_ptr->rubberBand->geometry().left();
        int right = d_ptr->rubberBand->geometry().right();

        Timestamp t1 = posToTimestamp(left,width(),d_ptr->range);
        Timestamp t2 = posToTimestamp(right,width(),d_ptr->range);
        setRange(t1,t2);
        qDebug()<<left<<right<<t1<<t2;
    }
}

inline int getOptimatTimeparts(const TimeStampRange& range)
{
    int result = TimestampAllParts;
    int diff = range.second - range.first;
    if( diff > 12*HOUR ){
        result = Hours;
    }
    else if( diff > 30*MINUTE ){
        result = Hours | Minutes;
    }
    else if( diff > 30*SECOND ){
        result = Hours | Minutes | Seconds;
    }
    return result;
}

void TimeScale::paintEvent(QPaintEvent *event)
{
    const int parts = 24;
    Timestamp element = (d_ptr->range.second - d_ptr->range.first)/parts;
    int       elementWidth  = width()/24;
    int       elementHeight = height()/2;
    int       textHeight    = height()/2;
    int       timeParts = getOptimatTimeparts(d_ptr->range);

    QFont previewFont = font();
    QString sampleText = makeString( ( d_ptr->range.second + d_ptr->range.first) /2 ,timeParts);
    QFont f = previewFont;
    QFontMetrics fm( f );
    while( fm.width(sampleText) > elementWidth ){
        f.setPointSize( f.pointSize() - 1);
        fm = QFontMetrics(f);
    }
    previewFont = f;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();
        painter.setFont(previewFont);
        painter.setPen(QPen(palette().foreground(),3) );
        Timestamp tcursor = element;
        int       wcursor = elementWidth;
        for( int part = 1; part < parts; part++ ){
            tcursor = d_ptr->range.first+element*part;
            wcursor = elementWidth*part;
            painter.drawLine( QPointF(wcursor,0), QPointF(wcursor,elementHeight) );

            QString text = makeString(tcursor,timeParts);
            QRectF  textRect = QRectF(wcursor-elementWidth/2,elementHeight,elementWidth,textHeight);
            QTextOption textOpt = QTextOption(Qt::AlignHCenter | Qt::AlignBottom);
            painter.drawText(textRect,text,textOpt);
        }
    painter.restore();

    QWidget::paintEvent(event);
}

void TimeScale::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void TimeScale::zoomIn()
{
    Timestamp diff   = (d_ptr->range.second - d_ptr->range.first);
    Timestamp center = (d_ptr->range.second + d_ptr->range.first)/2;
    setRange(center - diff/4, center+diff/4 );
}

void TimeScale::zoomOut()
{
    Timestamp diff   = (d_ptr->range.second - d_ptr->range.first);
    Timestamp center = (d_ptr->range.second + d_ptr->range.first)/2;
    setRange(center - diff*2, center+diff*2 );
}

void TimeScale::wheelEvent(QWheelEvent *event)
{
    QWidget::wheelEvent(event);
    QPoint numDegrees = event->angleDelta() / 8;
    QPoint numSteps = numDegrees / 15;
    if( numSteps.y() > 0  ){
        zoomIn();
    }
    else{
        zoomOut();
    }
}


