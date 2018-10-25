#include "generatordialog.h"
#include "ui_generatordialog.h"

#include <QMovie>
#include <QPixmap>
#include <QInputDialog>

//[0]
class GeneratorDialogPrivate
{
public:
    GeneratorDialogPrivate():model(nullptr)
                            ,movie(":/images/loader_96.gif")
                            ,done(QPixmap( ":/images/standardbutton-apply-128.png") )
    {}

public:
    CustomBookmarkModel* model;
    QMovie movie;
    QPixmap done;
};

//[1]
GeneratorDialog::GeneratorDialog(QWidget *parent)
                :QDialog(parent)
                ,d_ptr(new GeneratorDialogPrivate())
                ,ui(new Ui::GeneratorDialog)
{
    ui->setupUi(this);
    connect(ui->generateButton,SIGNAL(clicked(bool)),this,SLOT(onGenerateRequest()));
}

GeneratorDialog::~GeneratorDialog()
{
    delete ui;
    delete d_ptr;
}

void GeneratorDialog::setModel(CustomBookmarkModel* model)
{
    d_ptr->model = model;
    connect(d_ptr->model,SIGNAL(modelReset()),this,SLOT(onGenerationFinished()));
}

void GeneratorDialog::onGenerateRequest()
{
    if(!d_ptr->model){
        return;
    }
    int count = QInputDialog::getInt(this,"Get items count","items count",1,1);
    ui->label->setMovie(&d_ptr->movie);
    d_ptr->movie.start();
    d_ptr->model->generate(count);
}

void GeneratorDialog::onGenerationFinished()
{
    d_ptr->movie.stop();
    ui->label->setPixmap(d_ptr->done);
}

