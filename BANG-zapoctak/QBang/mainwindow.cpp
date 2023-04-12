#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ai.h"

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
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ClearLabels();
    LoadLabels();
    g = new Game();

    //az PaintLayout() povoli co je treba
    FalseLabels();
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
    //cteme radku po radce a vytvarime karty, ktere jsou ulozeny v g->deck
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList list = line.split(",");
        std::vector<std::string> vec;
        foreach( QString str, list) {
          vec.push_back(str.toStdString());
        }
        GameTools::load_card(g, vec);
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
    QList<QGroupBox*> gblist = ui->centralwidget->findChildren<QGroupBox*>();
    for(auto l : gblist)
    {
        l->setTitle("");
    }
    ui->emporio->setTitle("Emporio");
    ui->pl_k->setTitle("Karty");
    ui->pl_m->setTitle("Modre");
}
void MainWindow::LoadLabels()
{
    emporio = ui->emporio->findChildren<QLabel*>();

    //postupne nacteni vsech lablu, ktere jsou potrebne pro hru
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
        //sami sebe nechceme v seznamu nepratel
        if((size_t)g->notai == i)
        {
            continue;
        }
        ui->choose_p->addItem(QIcon(QString::fromStdString(g->game_order[i]->file_loc())),
                              QString::fromStdString(g->game_order[i]->name));
    }
}
void MainWindow::Start(int players, const std::string& roles)
{
    delete g;
    g = new Game();

    GameTools::load_characters(g, g->characters);
    LoadCards();
    g->create(players, roles);
    GameTools::rotate_serif(g);//serif na pozici 0 v g->game_order
    g->draw_cards_start();//kazdy dostane tolik karet, kolik ma maximalne zivotu
    g->set_initial_enemies();
    g->set_distances();
    AddLayoutIndexes();//prirazeni lablu k postavam v g->game_order, hra probiha proti smeru hodinovych rucicek

    //tytyo 3 combo boxy pro vyber karet a vyber hrace jsou vzdy povoleny, protoze bez tlacitek je stejne nejde modifikovat
    ui->choose_c->setEnabled(true);
    ui->choose_p->setEnabled(true);

    PaintLayout();
}
void MainWindow::SetButtons()
{
    ui->choose_e->setEnabled(false);

    //emporio
    if(g->mode == EMPORIO && g->neu_turn != -1 && !g->game_order[g->neu_turn]->isai)
    {
        ui->choose_e->setEnabled(true);
        ui->play->setEnabled(false);
        ui->finish->setEnabled(false);
        ui->discard->setEnabled(false);
        ui->ability->setEnabled(false);
        ui->draw->setEnabled(false);
        ui->react->setEnabled(false);
    }
    //Kit Carlson Ability...notAI
    else if(g->mode == CARLSON)
    {
        ui->choose_e->setEnabled(true);
        ui->play->setEnabled(false);
        ui->finish->setEnabled(false);
        ui->discard->setEnabled(false);
        ui->ability->setEnabled(false);
        ui->draw->setEnabled(false);
        ui->react->setEnabled(false);
    }
    //indiani, kulomet...notAI reaguje
    else if(g->neu_turn != -1 && !g->game_order[g->neu_turn]->isai)
    {
        //dovolit jen bang, vedle, pivo, barel, jourd
        ui->play->setEnabled(true);
        ui->ability->setEnabled(g->game_order[g->notai]->has_notai_ability() && !g->game_order[g->notai]->ability_used);
        ui->draw->setEnabled(false);
        ui->react->setEnabled(false);
        ui->discard->setEnabled(false);
        ui->finish->setEnabled(true);
    }
    //hraje notAI, lize si na DYNAMIT
    else if(!g->game_order[g->active_player]->isai && g->game_order[g->active_player]->has_dyn())
    {
        ui->play->setEnabled(false);
        ui->finish->setEnabled(false);
        ui->discard->setEnabled(false);
        ui->ability->setEnabled(false);
        ui->draw->setEnabled(true);
        ui->react->setEnabled(false);
    }
    //hraje notAI, lize si na VEZENI
    else if(!g->game_order[g->active_player]->isai && g->game_order[g->active_player]->has_jail())
    {
        ui->play->setEnabled(false);
        ui->finish->setEnabled(false);
        ui->discard->setEnabled(false);
        ui->ability->setEnabled(false);
        ui->draw->setEnabled(true);
        ui->react->setEnabled(false);
    }
    //hraje notAI
    else if(!g->game_order[g->active_player]->isai && g->mode == NONE)
    {
        ui->play->setEnabled(true);
        ui->finish->setEnabled(true);
        ui->discard->setEnabled(true);
        ui->ability->setEnabled(g->game_order[g->active_player]->has_notai_ability() && !g->game_order[g->notai]->ability_used);
        ui->draw->setEnabled(!g->game_order[g->notai]->drawed);
        ui->react->setEnabled(false);
    }
    //AI bude reagovat na kartu od notAI
    else if(!g->game_order[g->active_player]->isai && g->mode != NONE && !g->duel_active_turn)
    {
        ui->play->setEnabled(false);
        ui->finish->setEnabled(false);
        ui->discard->setEnabled(false);
        ui->ability->setEnabled(false);
        ui->draw->setEnabled(false);
        ui->react->setEnabled(true);
    }
    //bang, vedle, Slab...hraje AI
    else if((g->mode == BANG || g->mode == VEDLE || g->mode == SLAB) && NotaiReact())
    {
        ui->play->setEnabled(true);
        ui->ability->setEnabled(g->game_order[g->notai]->has_notai_ability() && !g->game_order[g->notai]->ability_used);
        ui->draw->setEnabled(false);
        ui->react->setEnabled(false);
        ui->discard->setEnabled(false);
        ui->finish->setEnabled(true);
    }
    //duel, hraje notAI...reaguje
    else if(NotAiDuelReact())
    {
        ui->play->setEnabled(true);
        ui->ability->setEnabled(false);
        ui->draw->setEnabled(false);
        ui->react->setEnabled(false);
        ui->discard->setEnabled(false);
        ui->finish->setEnabled(true);
    }
    //hraje AI
    else
    {
        ui->play->setEnabled(false);
        ui->ability->setEnabled(false);
        ui->draw->setEnabled(false);
        ui->react->setEnabled(false);
        ui->discard->setEnabled(false);
        ui->finish->setEnabled(true);
    }
}
bool MainWindow::NotaiReact()
{
    //hraje-li notAI a ceka na reakci od AI
    return !g->game_order[GameTools::id_to_pos(g, g->game_order[g->active_player]->target_id)]->isai;
}
void MainWindow::FalseLabels()
{
    ui->play->setEnabled(false);
    ui->finish->setEnabled(false);
    ui->discard->setEnabled(false);
    ui->ability->setEnabled(false);
    ui->draw->setEnabled(false);
    ui->react->setEnabled(false);
    ui->finish->setEnabled(false);
    ui->choose_e->setEnabled(false);
    ui->choose_c->setEnabled(false);
    ui->choose_p->setEnabled(false);
}
void MainWindow::CheckFinished()
{
    //konec hry, nic nejde zmacknout
    if(g->finished())
    {
        FalseLabels();
        return;
    }
}
void MainWindow::AddLayoutIndexes()
{
    size_t count = g->game_order.size();
    for(; g->notai < count; g->notai++)
    {
        if(!g->game_order[g->notai]->isai)
        {
            break;
        }
    }
    g->game_order[g->notai]->layout_index = 6;


    for(size_t i = 1; i < count; i++)
    {
        g->game_order[(g->notai - i + count) % count]->layout_index = i - 1;
    }
}
bool MainWindow::NotAiDuelReact()
{
    //zda-li je na rade notAI v odehrani Bang pri duelu

    if(g->mode != DUEL)
    {
        return false;
    }


    return (g->game_order[g->active_player]->isai &&
            !g->duel_active_turn && !g->game_order[GameTools::id_to_pos(g, g->game_order[g->active_player]->target_id)]->isai) ||
           (!g->game_order[g->active_player]->isai && g->duel_active_turn);
}
QString MainWindow::ModeToText(Modes m)
{
    switch(m)
    {
    case BANG:
        return "Bang";
    case VEDLE:
        return "Bang";
    case EMPORIO:
        return "Hokynarstvi";
    case PANIKA:
        return "Panika";
    case BALOU:
        return "Cat Balou";
    case INDIANI:
        return "Indiani";
    case SALON:
        return "Salon";
    case KULOMET:
        return "Kulomet";
    case DUEL:
        return "Duel";
    default:
        return "";
    }
}
void MainWindow::PaintLayout()
{
    g->set_distances();
    AddLivePlayers();
    //chceme, aby byly jenom potrebne tlacitka enabled
    SetButtons();

    //CENTER
    ClearLabels();
    SetLabel(ui->deck, ":/cards/cards/back-playing.png");
    SetLabel(ui->discarded, QString::fromStdString(g->deck.back().file_loc()));
    SetLabel(ui->suit, QString::fromStdString(g->deck.back().suit_loc()));
    ui->rank->setText(QString::fromStdString(g->deck.back().rnk()));
    ui->target->setText(QString::fromStdString(GameTools::id_to_name(g, g->game_order[g->active_player]->target_id)));
    ui->label_target->setText("Target:");
    ui->label_react->setText("React:");
    ui->label_mode->setText("Mode:");
    ui->mode->setText(ModeToText(g->mode));
    if(g->neu_turn != -1)
    {
        ui->neu->setText(QString::fromStdString(GameTools::id_to_name(g, g->game_order[g->neu_turn]->id)));
    }

    //EMPORIO
    ui->choose_e->clear();
    for(size_t i = 0; i < g->emporio.size(); i++)
    {
        SetLabel(emporio[i], QString::fromStdString(g->emporio[i].file_loc()));
        ui->choose_e->addItem(QIcon(QString::fromStdString(g->emporio[i].file_loc())),
                              QString::fromStdString(g->emporio[i].name));
    }

    //clear cards, discard
    ui->choose_c->clear();

    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->isai)
        {
            //AI
            qobject_cast<QGroupBox*>(layout[g->game_order[i]->layout_index][0]->parent()->parent())
                    ->setTitle((size_t)g->active_player == i ? "PLAYING" : "");

            SetLabel(layout[g->game_order[i]->layout_index][0], QString::fromStdString(g->game_order[i]->file_loc()));
            layout[g->game_order[i]->layout_index][1]->setText(QString::number(g->game_order[i]->health));
            SetLabel(layout[g->game_order[i]->layout_index][2], ":/cards/cards/back-playing.png");
            layout[g->game_order[i]->layout_index][3]->setText(QString::number(g->game_order[i]->cards_hand.size()));
            SetLabel(layout[g->game_order[i]->layout_index][4], QString::fromStdString(g->game_order[i]->role_loc()));

            int start_index = 5;
            for(size_t j = 0; j < g->game_order[i]->cards_desk.size(); j++)
            {
                SetLabel(layout[g->game_order[i]->layout_index][j + start_index], QString::fromStdString(g->game_order[i]->cards_desk[j].file_loc()));
            }
        }
        else
        {
            //REAL PERSON CARDS
            for(size_t j = 0; j < g->game_order[i]->cards_hand.size(); j++)
            {
                ui->choose_c->addItem(QIcon(QString::fromStdString(g->game_order[i]->cards_hand[j].file_loc())), QString::fromStdString(g->game_order[i]->cards_hand[j].name));
            }
            for(size_t j = 0; j < g->game_order[i]->cards_desk.size(); j++)
            {
                ui->choose_c->addItem(QIcon(QString::fromStdString(g->game_order[i]->cards_desk[j].file_loc())), QString::fromStdString(g->game_order[i]->cards_desk[j].name));
            }

            //REAL PERSON
            SetLabel(layout[g->game_order[i]->layout_index][0], QString::fromStdString(g->game_order[i]->file_loc()));
            layout[g->game_order[i]->layout_index][1]->setText(QString::number(g->game_order[i]->health));
            SetLabel(layout[g->game_order[i]->layout_index][2], QString::fromStdString(g->game_order[i]->role_loc()));

            size_t cards = g->game_order[i]->cards_hand.size();
            if(cards > 10)
            {
                cards = 10;
            }

            int start_index = 9;
            for(size_t j = 0; j < cards; j++)
            {
                SetLabel(layout[g->game_order[i]->layout_index][j + start_index], QString::fromStdString(g->game_order[i]->cards_hand[j].file_loc()));
            }

            start_index = 3;
            for(size_t j = 0; j < g->game_order[i]->cards_desk.size(); j++)
            {
                SetLabel(layout[g->game_order[i]->layout_index][j + start_index], QString::fromStdString(g->game_order[i]->cards_desk[j].file_loc()));
            }
        }
    }
    //mrtvym hracum se jenom zobrazi postava, role v pripade AI oznameni, ze jsou mrtvi
    for(size_t i = 0; i < g->dead.size(); i++)
    {
        if(g->dead[i]->isai)
        {
            SetLabel(layout[g->dead[i]->layout_index][0], QString::fromStdString(g->dead[i]->file_loc()));
            SetLabel(layout[g->dead[i]->layout_index][4], QString::fromStdString(g->dead[i]->role_loc()));
            qobject_cast<QGroupBox*>(layout[g->dead[i]->layout_index][0]->parent()->parent())
                    ->setTitle("DEAD");
        }
        else
        {
            SetLabel(layout[g->dead[i]->layout_index][0], QString::fromStdString(g->dead[i]->file_loc()));
            SetLabel(layout[g->dead[i]->layout_index][2], QString::fromStdString(g->dead[i]->role_loc()));
        }
    }

    CheckFinished();
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
    std::string name = ui->choose_c->currentText().toStdString();
    //muzeme-li jako reakci na nejakou kartu zahrat tuto kartu
    bool can_play = g->can_respond_with_card(name);
    if(g->mode != NONE)
    {
        if(can_play)
        {
            int i = ui->choose_c->currentIndex();
            g->resolve_notai_react(i);
        }
        PaintLayout();
        return;
    }


    g->game_order[g->notai]->drawed = true;
    int i = ui->choose_c->currentIndex();
    int p = ui->choose_p->currentIndex();
    //nemuzeme zahrat kartu, ktera je polozena pred nama na stole
    if((size_t)i >= g->game_order[g->notai]->cards_hand.size())
    {
        return;
    }
    //pokud hrajeme kartu, ktera potrebuje cil
    if(i == -1 || (g->game_order[g->notai]->cards_hand[i].need_target() && p == -1))
    {
        return;
    }
    Ai::discard_card(g, g->game_order[g->notai]->cards_hand, g->game_order[g->notai]->cards_desk, i);
    g->game_order[g->notai]->set_target_id(ui->choose_p->currentText().toStdString());
    g->game_loop();
    PaintLayout();
}
void MainWindow::on_draw_clicked()
{
    //vyreseni dynamitu, vezeni a pripadna smrt po vybuchu
    if(g->game_order[g->active_player]->has_dyn())
    {
        if(g->game_order[g->active_player]->resolve_dyn())
        {
            if(g->game_order[g->active_player]->dec_hp(3))
            {
                int dead = g->game_order[g->active_player]->id;
                g->active_player = (g->active_player + 1) % g->player_alive;
                g->killed(dead);
            }
        }
        PaintLayout();
        return;
    }
    else if(g->game_order[g->active_player]->has_jail())
    {
        bool res = g->game_order[g->active_player]->resolve_jail();
        if(res)
        {
            PaintLayout();
            return;
        }
        else
        {
            g->game_order[g->active_player]->turn_reset();
            g->active_player = (g->active_player + 1) % g->player_alive;
            PaintLayout();
            return;
        }
    }

    g->game_order[g->notai]->drawed = true;
    g->game_order[g->notai]->draw_phase();
    PaintLayout();
}
void MainWindow::on_discard_clicked()
{
    g->game_order[g->notai]->drawed = true;
    int i = ui->choose_c->currentIndex();
    if(i == -1)
    {
        return;
    }
    Ai::discard_card(g, g->game_order[g->notai]->cards_hand, g->game_order[g->notai]->cards_desk, i);

    //pokud Sir Ketchum vyhodi 2 karty muze si dobyt 1 zivot, jeho schopnost
    g->game_order[g->notai]->discarded++;
    PaintLayout();
}
void MainWindow::on_finish_clicked()
{
    //nemuzeme ukoncit tah jestli mame v ruce vice karet nez zivotu
    if(!g->game_order[g->active_player]->isai &&
        g->game_order[g->active_player]->cards_hand.size() <= (size_t)g->game_order[g->active_player]->health &&
            g->mode == NONE)
    {
        g->game_order[g->active_player]->turn_reset();
        g->active_player = (g->active_player + 1) % g->player_alive;
        PaintLayout();
        return;
    }
    else if(!g->game_order[g->active_player]->isai && g->mode == NONE)
    {
        PaintLayout();
        return;
    }
    g->game_loop();
    PaintLayout();
}
void MainWindow::on_ability_clicked()
{
    if(g->game_order[g->active_player]->has_dyn() || g->game_order[g->active_player]->has_jail())
    {
        return;
    }
    g->game_order[g->notai]->set_target_id(ui->choose_p->currentText().toStdString());

    //pouziti schopnosti, je-li to mozne
    g->game_order[g->notai]->ability();
    PaintLayout();
}
void MainWindow::on_choose_e_activated(int index)
{
    //schopnost Kit Carlson, vybira 2 karty ze 3, ktere lezi v g->emporio
    if(g->mode == CARLSON)
    {
        Card c = g->emporio[index];
        g->emporio.erase(g->emporio.begin() + index);
        g->game_order[g->notai]->cards_hand.push_back(c);
        if(g->emporio.size() == 1)
        {
            g->emporio.clear();
            g->mode = NONE;
        }

        PaintLayout();
        return;
    }

    //notAI si vybral kartu z hokynarstvi
    Card c = g->emporio[index];
    g->emporio.erase(g->emporio.begin() + index);
    g->game_order[g->neu_turn]->cards_hand.push_back(c);

    g->neu_turn = (g->neu_turn + 1) % g->player_alive;
    if(g->active_player == g->neu_turn)
    {
        g->mode = NONE;
        g->neu_turn = -1;
    }

    PaintLayout();
}
void MainWindow::on_react_clicked()
{    
    //AI reaguje na kartu notAI
    g->resolve_played_card();
    PaintLayout();
}
