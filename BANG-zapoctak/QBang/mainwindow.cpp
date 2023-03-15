#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"

#include <QPixmap>
#include <QString>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SetLabel(ui->label1_char, ":/char_img/char_img/paul.png");
    SetLabel(ui->label1_cards, ":/cards/cards/back-playing.png");
    SetLabel(ui->label1_b_1, ":/cards/cards/appaloosa.png");
    SetLabel(ui->label1_b_2, ":/cards/cards/appaloosa.png");
    SetLabel(ui->label1_b_3, ":/cards/cards/appaloosa.png");
    SetLabel(ui->label1_b_4, ":/cards/cards/appaloosa.png");
    SetLabel(ui->label1_b_5, ":/cards/cards/appaloosa.png");
    SetLabel(ui->label1_b_6, ":/cards/cards/appaloosa.png");
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetLabel(QLabel* q, const QString& s)
{
    QPixmap px(s);
    int w = q->width();
    int h = q->height();
    q->setPixmap(px.scaled(w,h,Qt::KeepAspectRatio));
}


void MainWindow::on_pushButton_clicked()
{
    Game g;
    g.load_characters();
    std::string a = g.characters[0]->name;
    QString qstr = QString::fromStdString(a);
    ui->label1_hp_2->setText(qstr);
}

