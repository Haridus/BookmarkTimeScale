#include "bookmarkview.h"
#include <QApplication>

#include <QDebug>
#include "bookmarkmodel.h"
#include <QTableView>
#include "generatordialog.h"
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CustomBookmarkModel model;

    BookmarkView w;
    w.setModel(&model);

    QTableView view;
    view.setModel(&model);

    GeneratorDialog dlg;
    dlg.setModel(&model);

    QDesktopWidget dw;
    QPoint center = dw.screen(0)->geometry().center();
    w.setGeometry(QRect(center.x()-400,center.y()-30,800,60));
    view.setGeometry(QRect(center.x()+410,center.y()/2,300,center.y()));
    dlg.move(QPoint(center.x()-75,center.y()-250));

    w.show();
    view.show();
    dlg.show();


    return a.exec();
}
