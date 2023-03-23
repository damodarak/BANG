#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include "game.h"
#include <QVector>
#include <QList>

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

private:
    Ui::MainWindow *ui;
    QVector<QList<QLabel*>> layout;
    QList<QLabel*> emporio;
    QLabel* discarded;
    QLabel* deck;
    Game* g;

    void SetLabel(QLabel* q, const QString& s);
    void LoadCards();
    void PaintLayout();
    void ClearLabels();
    void LoadLabels();
};
#endif // MAINWINDOW_H
