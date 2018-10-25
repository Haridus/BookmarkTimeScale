#ifndef BookmarkVIEWWIDGET_H
#define BookmarkVIEWWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QResizeEvent>
#include "timestamp.h"
#include "bookmarkmodel.h"
#include <QEvent>

class BookmarkViewWidgetPrivate;

class BookmarkViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BookmarkViewWidget(QWidget *parent = 0);
    ~BookmarkViewWidget();

    void setModel(AbstractBookmarkModel* model);
    AbstractBookmarkModel* model()const;

    void setRange(const Timestamp& t1, const Timestamp& t2);

signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

    virtual bool event(QEvent *e);

private slots:
    void onModelChanged();       

private:
    BookmarkViewWidgetPrivate* d_ptr;
};

#endif // BookmarkVIEWWIDGET_H
