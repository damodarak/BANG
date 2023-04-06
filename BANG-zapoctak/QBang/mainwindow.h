#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include <QVector>
#include <QList>

#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionStart_4_triggered();

    void on_actionStart_7_triggered();

    void on_play_clicked();

    void on_draw_clicked();

    void on_discard_clicked();

    void on_finish_clicked();

    void on_ability_clicked();

    void on_actionStart_5_triggered();

    void on_actionStart_6_triggered();

    friend class Ask;
    void on_choose_e_activated(int index);

    void on_react_clicked();

private:
    Ui::MainWindow *ui;
    QVector<QList<QLabel*>> layout;
    QList<QLabel*> emporio;
    Game* g;

    void SetLabel(QLabel* q, const QString& s);
    void LoadCards();
    void PaintLayout();//po odehrani jakehokoliv tahu
    void ClearLabels();
    void LoadLabels();
    void AddLivePlayers();
    void Start(int players, const std::string& roles);
    void SetButtons();
    bool NotaiReact();
    void FalseLabels();
    void CheckFinished();
    void AddLayoutIndexes();
};
#endif // MAINWINDOW_H
