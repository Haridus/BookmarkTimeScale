#ifndef GENERATORDIALOG_H
#define GENERATORDIALOG_H

#include <QDialog>
#include "bookmarkmodel.h"

namespace Ui {
class GeneratorDialog;
}

class GeneratorDialogPrivate;

class GeneratorDialog : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(GeneratorDialog)
    Q_DECLARE_PRIVATE(GeneratorDialog)

public:
    explicit GeneratorDialog(QWidget *parent = 0);
    ~GeneratorDialog();

    void setModel(CustomBookmarkModel* model);

private slots:
    void onGenerateRequest();
    void onGenerationFinished();

private:
    GeneratorDialogPrivate* d_ptr;
    Ui::GeneratorDialog *ui;
};

#endif // GENERATORDIALOG_H
