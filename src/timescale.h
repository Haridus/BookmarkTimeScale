#ifndef TIMESCALE_H
#define TIMESCALE_H

#include <QWidget>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QWheelEvent>
#include "timestamp.h"

class TimeScalePrivate;

class TimeScale : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(TimeScale)
    Q_DECLARE_PRIVATE(TimeScale)

public:
    explicit TimeScale(QWidget *parent = 0);
    ~TimeScale();

signals:
    void rangeChanged(const Timestamp& t1, const  Timestamp& t2);

public slots:
    void setRange(const Timestamp& t1, const  Timestamp& t2);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

private:
    void zoomIn();
    void zoomOut();

private:
    TimeScalePrivate* d_ptr;
};

#endif // TIMESCALE_H
