#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QString>
#include <string>
#include <vector>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QGroupBox>
#include <QWidget>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), notai(0)
{
    ui->setupUi(this);

    qt = new QTimer(this);
    connect(qt, SIGNAL(timeout()), this, SLOT(on_finish_clicked()));

    ClearLabels();
    LoadLabels();
    g = new Game();
    SetButtons(false);
    ui->choose_e->setEnabled(false);
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
    ui->gb1->setTitle("");
    ui->gb2->setTitle("");
    ui->gb3->setTitle("");
    ui->gb4->setTitle("");
    ui->gb5->setTitle("");
    ui->gb6->setTitle("");
}
void MainWindow::LoadLabels()
{
    emporio = ui->emporio->findChildren<QLabel*>();
    discarded = ui->discarded;
    deck = ui->deck;
    suit = ui->suit;
    rank = ui->rank;

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
void MainWindow::AddLivePlayers()
{
    ui->choose_p->clear();
    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if((size_t)notai == i)
        {
            continue;
        }
        ui->choose_p->addItem(QIcon(g->game_order[i]->file_loc()), QString::fromStdString(g->game_order[i]->name));
    }
}
void MainWindow::Start(int players, const std::string& roles)
{
    delete g;
    g = new Game();

    g->load_characters();
    LoadCards();
    g->create(players, roles);
    g->rotate_serif();
    g->draw_cards_start();
    g->set_initial_enemies();
    g->set_distances();
    g->add_labels(layout);
    notai = g->notai;
    PaintLayout();
}

void MainWindow::SetButtons(bool state)
{
    ui->play->setEnabled(state);
    ui->finish->setEnabled(state);
    ui->discard->setEnabled(state);
    ui->ability->setEnabled(state);
    ui->draw->setEnabled(state);
}
void MainWindow::PaintLayout()
{
    AddLivePlayers();

    //CENTER
    ClearLabels();
    SetLabel(deck, ":/cards/cards/back-playing.png");
    SetLabel(discarded, g->deck.back().file_loc());
    SetLabel(suit, g->deck.back().suit_loc());
    rank->setText(g->deck.back().rnk());
    ui->target->setText(g->id_to_name(g->game_order[g->active_player]->target_id));
    ui->label->setText("Target:");

    //EMPORIO
    ui->choose_e->setEnabled(g->emporio.size() != 0);
    for(size_t i = 0; i < g->emporio.size(); i++)
    {
        SetLabel(emporio[i], g->emporio[i].file_loc());
        ui->choose_e->addItem(QIcon(g->emporio[i].file_loc()), QString::fromStdString(g->emporio[i].name));
    }

    //REAL PERSON CARDS
    ui->choose_c->clear();
    ui->choose_d->clear();
    for(size_t i = 0; i < g->game_order[notai]->cards_hand.size(); i++)
    {
        ui->choose_c->addItem(QIcon(g->game_order[notai]->cards_hand[i].file_loc()), QString::fromStdString(g->game_order[notai]->cards_hand[i].name));
        ui->choose_d->addItem(QIcon(g->game_order[notai]->cards_hand[i].file_loc()), QString::fromStdString(g->game_order[notai]->cards_hand[i].name));
    }
    for(size_t i = 0; i < g->game_order[notai]->cards_desk.size(); i++)
    {
        ui->choose_c->addItem(QIcon(g->game_order[notai]->cards_desk[i].file_loc()), QString::fromStdString(g->game_order[notai]->cards_desk[i].name));
        ui->choose_d->addItem(QIcon(g->game_order[notai]->cards_desk[i].file_loc()), QString::fromStdString(g->game_order[notai]->cards_desk[i].name));
    }


    //REAL PERSON
    SetLabel(g->game_order[notai]->char_l, g->game_order[notai]->file_loc());
    g->game_order[notai]->hp_l->setText(QString::number(g->game_order[notai]->health));
    SetLabel(g->game_order[notai]->role_l, g->game_order[notai]->role_loc());
    for(size_t i = 0; i < g->game_order[notai]->cards_hand.size(); i++)
    {
        SetLabel(g->game_order[notai]->cards_l[i], g->game_order[notai]->cards_hand[i].file_loc());
    }
    for(size_t i = 0; i < g->game_order[notai]->cards_desk.size(); i++)
    {
        SetLabel(g->game_order[notai]->m_l[i], g->game_order[notai]->cards_desk[i].file_loc());
    }
    SetButtons(notai == g->active_player);

    //AI
    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(i != (size_t)notai)
        {
            qobject_cast<QGroupBox*>(g->game_order[i]->char_l->parent()->parent())
                    ->setTitle((size_t)g->active_player == i ? "PLAYING" : "");


            SetLabel(g->game_order[i]->char_l, g->game_order[i]->file_loc());
            g->game_order[i]->hp_l->setText(QString::number(g->game_order[i]->health));
            SetLabel(g->game_order[i]->card_l, ":/cards/cards/back-playing.png");
            g->game_order[i]->count_l->setText(QString::number(g->game_order[i]->cards_hand.size()));
            SetLabel(g->game_order[i]->role_l, g->game_order[i]->role_loc());
            for(size_t j = 0; j < g->game_order[i]->cards_desk.size(); j++)
            {
                SetLabel(g->game_order[i]->m_l[j], g->game_order[i]->cards_desk[j].file_loc());
            }
        }
    }


    ui->finish->setEnabled(true);
}
void MainWindow::on_actionStart_4_triggered()
{
    Start(4, "SBBO");
}
void MainWindow::on_actionStart_5_triggered()
{
    Start(5, "SBBOV");
}
void MainWindow::on_actionStart_6_triggered()
{
    Start(6, "SBBOVB");
}
void MainWindow::on_actionStart_7_triggered()
{
    Start(7, "SBBOVBV");
}
void MainWindow::on_play_clicked()
{
    g->game_order[notai]->drawed = true;
    int i = ui->choose_c->currentIndex();
    int p = ui->choose_p->currentIndex();
    if(i == -1 || (g->game_order[notai]->cards_hand[i].need_target() && p == -1) ||
            (size_t)i >= g->game_order[notai]->cards_hand.size())//barel
    {
        return;
    }
    if(p != -1)
    {
        g->game_order[notai]->target_id = g->name_to_id(ui->choose_p->currentText());
    }
    g->game_order[notai]->discard_card(i);
    g->game_order[notai]->set_target_id(ui->choose_p->currentText().toStdString());
    g->game_loop();
    PaintLayout();
}
void MainWindow::on_draw_clicked()
{
    if(g->game_order[notai]->drawed)
    {
        return;
    }
    g->game_order[notai]->drawed = true;
    g->game_order[notai]->draw_phase();
    PaintLayout();
}
void MainWindow::on_discard_clicked()
{
    g->game_order[notai]->drawed = true;
    int i = ui->choose_d->currentIndex();
    if(i == -1)
    {
        return;
    }
    g->game_order[notai]->discard_card(i);
    PaintLayout();
}
void MainWindow::on_finish_clicked()
{
    qt->start(1000);
    g->game_loop();
    PaintLayout();





//    Ask b(this, this);
//    b.setModal(true);
//    b.exec();
}
void MainWindow::on_ability_clicked()
{
    g->game_order[notai]->drawed = true;
    g->game_order[notai]->ability();
}
