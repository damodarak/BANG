#include "mainwindow.h"
#include "ui_ask.h"

Ask::Ask(QWidget *parent, MainWindow* mw) :
    QDialog(parent),
    ui(new Ui::Ask), mw(mw)
{
    ui->setupUi(this);
    //mw = parent;
    //mw = qobject_cast<QMainWindow*>(parent);
    mw->notai +=100;
}

Ask::~Ask()
{
    delete ui;
}
