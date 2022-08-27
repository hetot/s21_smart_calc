#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include "graph.h"
#include "credit.h"
#include "deposit.h"

extern "C" {
    #include"s21_smart_calc.h"
}


QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:
    Ui::Calculator *ui;
    Graph *graph_window;
    Credit *credit_window;
    Deposit *deposit_window;

private slots:
    void AppenderButtonPressed();
    void ClearButtonPressed();
    void EqualButtonPressed();
    void UnaryPressed();
    void CreditButtonPressed();
    void DepositButtonPressed();
};
#endif // CALCULATOR_H
