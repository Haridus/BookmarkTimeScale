#ifndef BookmarkVIEW_H
#define BookmarkVIEW_H

#include <QWidget>
#include "bookmarkmodel.h"

namespace Ui {
class BookmarkView;
}

class BookmarkViewPrivate;

class BookmarkView : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(BookmarkView)
    Q_DECLARE_PRIVATE(BookmarkView)

public:
    explicit BookmarkView(QWidget *parent = 0);
    ~BookmarkView();

    void setModel(AbstractBookmarkModel* model);
    AbstractBookmarkModel* model()const;

protected slots:
    void onModelChanged();
    void onRangeChanged(const Timestamp& t1, const Timestamp& t2);

private:
    BookmarkViewPrivate* d_ptr;
    Ui::BookmarkView *ui;
};

#endif // BookmarkVIEW_H
