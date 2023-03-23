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
    ClearLabels();
    LoadLabels();
    g = new Game();
}
MainWindow::~MainWindow()
{
    delete ui;
    delete g;
}
void MainWindow::SetLabel(QLabel* q, const QString& s)
{
    QPixmap px(s);
    q->setPixmap(px);
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
        g->load_card(vec);
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
void MainWindow::on_actionStart_4_triggered()
{
//    4-SBBO
//    5-SBBOV
//    6-SBBOVB
//    7-SBBOVBV
    delete g;
    g = new Game();

    g->load_characters();
    LoadCards();
    g->create(4, "SBBO");
    g->rotate_serif();
    g->draw_cards_start();
    g->set_initial_enemies();//todo
    g->set_distances();
    g->add_labels(layout);
    PaintLayout();
}
void MainWindow::LoadLabels()
{
    emporio = ui->emporio->findChildren<QLabel*>();
    discarded = ui->discarded;
    deck = ui->deck;

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
void MainWindow::PaintLayout()//emporio
{
    //CENTER
    ClearLabels();
    SetLabel(deck, ":/cards/cards/back-playing.png");
    SetLabel(discarded, g->deck.back().file_loc());

    //EMPORIO
    for(size_t i = 0; i < g->emporio.size(); i++)
    {
        SetLabel(emporio[i], g->emporio[i].file_loc());
    }

    //REAL PERSON
    SetLabel(g->game_order[g->notai]->char_l, g->game_order[g->notai]->file_loc());
    g->game_order[g->notai]->hp_l->setText(QString::number(g->game_order[g->notai]->health));
    SetLabel(g->game_order[g->notai]->role_l, g->game_order[g->notai]->role_loc());
    for(size_t i = 0; i < g->game_order[g->notai]->cards_hand.size(); i++)
    {
        SetLabel(g->game_order[g->notai]->cards_l[i], g->game_order[g->notai]->cards_hand[i].file_loc());
    }
    for(size_t i = 0; i < g->game_order[g->notai]->cards_desk.size(); i++)
    {
        SetLabel(g->game_order[g->notai]->m_l[i], g->game_order[g->notai]->cards_hand[i].file_loc());
    }

    //AI
    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(i != g->notai)
        {
            SetLabel(g->game_order[i]->char_l, g->game_order[i]->file_loc());
            g->game_order[i]->hp_l->setText(QString::number(g->game_order[i]->health));
            SetLabel(g->game_order[i]->card_l, ":/cards/cards/back-playing.png");
            g->game_order[i]->count_l->setText(QString::number(g->game_order[i]->cards_hand.size()));
            for(size_t j = 0; j < g->game_order[i]->cards_desk.size(); j++)
            {
                SetLabel(g->game_order[i]->m_l[j], g->game_order[i]->cards_desk[j].file_loc());
            }
        }
    }
}
