#include "bookmarkview.h"
#include "ui_bookmarkview.h"

class BookmarkViewPrivate
{

};

//----------------------
BookmarkView::BookmarkView(QWidget *parent)
             :QWidget(parent)
             ,d_ptr(new BookmarkViewPrivate)
             ,ui(new Ui::BookmarkView)
{
    ui->setupUi(this);
    connect(ui->scale,SIGNAL(rangeChanged(Timestamp,Timestamp)),this, SLOT(onRangeChanged(Timestamp,Timestamp)));
}

BookmarkView::~BookmarkView()
{
    delete ui;
    delete d_ptr;
}

void BookmarkView::setModel(AbstractBookmarkModel* model)
{
    ui->view->setModel(model);
}

AbstractBookmarkModel* BookmarkView::model()const
{
    return ui->view->model();
}

void BookmarkView::onModelChanged()
{

}

void BookmarkView::onRangeChanged(const Timestamp &t1, const Timestamp &t2)
{
    ui->view->setRange(t1,t2);
}



