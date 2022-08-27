#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QHash>
#include <QList>

namespace Ui {
class Deposit;
}

class Deposit : public QMainWindow
{
    Q_OBJECT

private:
    QHash<QPushButton *, QHBoxLayout *> hash_add;
    QHash<QPushButton *, QHBoxLayout *> hash_get;

public:
    explicit Deposit(QWidget *parent = nullptr);
    ~Deposit();

    void onAddfirst();
    void onAddsecond();
    void onDeletefirst();
    void onDeletesecond();

    void calculate();

signals:
    void firstWindow();

private:
    Ui::Deposit *ui;
};

#endif // DEPOSIT_H
