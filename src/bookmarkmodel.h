#ifndef BOOKMARKMODEL_H
#define BOOKMARKMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include "timestamp.h"
#include "bookmark.h"

class AbstractBookmarkModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit AbstractBookmarkModel(QObject *parent = nullptr) : QAbstractItemModel(parent)
    {}

    virtual ~AbstractBookmarkModel()
    {}

    Bookmark bookmark(int row){return Bookmark();}

    virtual int lower_bond(const Timestamp& t, const Timestamp& duration)const = 0;
    virtual int upper_bond(const Timestamp& t, const Timestamp& duration)const = 0;
};

//-----------------------------------------------------------------
class CustomBookmarkModelPrivate;

class CustomBookmarkModel : public AbstractBookmarkModel
{
    Q_OBJECT
    Q_DISABLE_COPY(CustomBookmarkModel)
    Q_DECLARE_PRIVATE(CustomBookmarkModel)

public:
    //  virtual bool canFetchMore(const QModelIndex &parent) const;
    //  virtual void fetchMore(const QModelIndex &parent);
    //  virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    //  virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const

    CustomBookmarkModel(QObject *parent = nullptr);
    ~CustomBookmarkModel();

    virtual int  columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &index) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual int lower_bond(const Timestamp& t, const Timestamp& duration)const;
    virtual int upper_bond(const Timestamp& t, const Timestamp& duration)const;

    void generate(int n);
    bool isGenerating()const;

private slots:
    void onGenerationFinish();

private:
    CustomBookmarkModelPrivate* d_ptr;
};



#endif // BOOKMARKMODEL_H
