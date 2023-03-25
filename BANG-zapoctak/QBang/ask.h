#ifndef ASK_H
#define ASK_H

#include <QDialog>
#include <QMainWindow>

class MainWindow;

namespace Ui {
class Ask;
}

class Ask : public QDialog
{
    Q_OBJECT

public:
    explicit Ask(QWidget *parent = nullptr, MainWindow* mw = nullptr);
    ~Ask();

private:
    Ui::Ask *ui;
    MainWindow* mw;
};

#endif // ASK_H
