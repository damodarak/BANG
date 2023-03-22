#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QString>
#include <string>
#include <vector>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
void MainWindow::LoadCards()
{
    QFile file(":/text/text/hraci_karty.txt");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,"title","file not open");
    }
    QTextStream in(&file);
    in.readLine();
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList list = line.split(",");
        std::vector<std::string> vec;
        foreach( QString str, list) {
          vec.push_back(str.toStdString());
        }
        g.load_card(vec);
    }
    file.close();
}
void MainWindow::ClearLabels()
{
    QList<QLabel *> list = ui->centralwidget->findChildren<QLabel *>();
    for(auto l : list)
    {
        l->clear();
    }
}
void MainWindow::ResetGame()
{
    g.clear();
    g.load_characters();
    LoadCards();
}
void MainWindow::on_actionStart_4_triggered()
{
//    4-SBBO
//    5-SBBOV
//    6-SBBOVB
//    7-SBBOVBV
    LoadLabels();
    ResetGame();
    g.create(4, "SBBO");
    g.rotate_serif();
    g.draw_cards_start();
    g.set_initial_enemies();
    g.set_distances();
    g.add_labels(layout);
    PaintLayout();
}
void MainWindow::LoadLabels()
{
    QList<QLabel *> list = ui->gb1->findChildren<QLabel *>();
    layout.append(list);
    list = ui->gb2->findChildren<QLabel *>();
    layout.append(list);
    list = ui->gb3->findChildren<QLabel *>();
    layout.append(list);
    list = ui->gb4->findChildren<QLabel *>();
    layout.append(list);
    list = ui->gb5->findChildren<QLabel *>();
    layout.append(list);
    list = ui->gb6->findChildren<QLabel *>();
    layout.append(list);

    QList<QLabel *> pl;
    pl.append(ui->pl_char);
    pl.append(ui->pl_hp);
    pl.append(ui->pl_role);
    pl.append(ui->pl_m->findChildren<QLabel*>());
    pl.append(ui->pl_k->findChildren<QLabel*>());
    layout.append(pl);
}
void MainWindow::PaintLayout()
{
    ClearLabels();
}
