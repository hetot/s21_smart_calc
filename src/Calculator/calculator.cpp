#include "calculator.h"
#include "./ui_calculator.h"

QString init_str = "0.0";
bool is_empty = true;
bool div_trigger = false;
bool mul_trigger = false;
bool sub_trigger = false;
bool add_trigger = false;


Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);
    graph_window = new Graph();
    credit_window = new Credit();
    deposit_window = new Deposit();

    connect(graph_window, &Graph::firstWindow, this, &Calculator::show);
    connect(credit_window, &Credit::firstWindow, this, &Calculator::show);

    ui->Display->setText(init_str);
    QList<QPushButton *> all_buttons = Calculator::findChildren<QPushButton *>();
    for (int i = 0; i < 35; i++) {
        if (QString::compare("=", all_buttons[i]->text()) == 0) {
            connect(all_buttons[i], &QPushButton::released, this,
                    &Calculator::EqualButtonPressed);
        } else if (QString::compare("+/-", all_buttons[i]->text()) == 0) {
            connect(all_buttons[i], &QPushButton::released, this,
                    &Calculator::UnaryPressed);
        } else if (QString::compare("C", all_buttons[i]->text()) == 0) {
            connect(all_buttons[i], &QPushButton::released, this,
                    &Calculator::ClearButtonPressed);
        } else if (QString::compare("Credit", all_buttons[i]->text()) == 0) {
            connect(all_buttons[i], &QPushButton::released, this,
                    &Calculator::CreditButtonPressed);
        } else if (QString::compare("Deposit", all_buttons[i]->text()) == 0) {
            connect(all_buttons[i], &QPushButton::released, this,
                    &Calculator::DepositButtonPressed);
        } else {
            connect(all_buttons[i], &QPushButton::released, this,
                    &Calculator::AppenderButtonPressed);
        }
    }
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::CreditButtonPressed() {
    credit_window->show();
}

void Calculator::DepositButtonPressed() {
    deposit_window->show();
}

void Calculator::ClearButtonPressed() {
    ui->Display->setText(init_str);
    is_empty = true;
}

void Calculator::UnaryPressed() {
    if (is_empty == false) {
        QString display_val = ui->Display->text();
        QString add_val = "*(-1)";
        ui->Display->setText(display_val + add_val);
    }
}

void Calculator::EqualButtonPressed() {
    QString display_val = ui->Display->text();
    if (display_val.contains("x")) {
        QSplineSeries *series = new QSplineSeries();
        for (int i = -100; i <= 100; i++) {
            QString tmp = display_val;
            tmp.replace('x', "(" + QString::number(i) + ")");
            QByteArray ba = tmp.toLocal8Bit();
            char *c_str2 = ba.data();
            double ans;
            int r = s21_calc(c_str2, &ans);
            series->append(i, ans);
        }

        QChart *chart = new QChart();
        chart->legend()->hide();
        chart->addSeries(series);
        chart->createDefaultAxes();
        chart->setTitle("Graph");

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        graph_window->setCentralWidget(chartView);
        graph_window->show();
    } else {
        QByteArray ba = display_val.toLocal8Bit();
        char *c_str2 = ba.data();
        double ans;
        int r = s21_calc(c_str2, &ans);
        if (s21_calc(c_str2, &ans) == 1) {
            ui->Display->setText(QString::number(ans));
        } else {
            ui->Display->setText(init_str);
            QMessageBox msgBox;
            msgBox.setText("Invalid input");
            msgBox.exec();
        }

    }
}

void Calculator::AppenderButtonPressed() {
    QPushButton *button = (QPushButton *)sender();
    QString but_val = button->text();
    QString display_val = ui->Display->text();

    if (is_empty) {
        ui->Display->setText(but_val);
        is_empty = false;
    } else {
        QString new_val = display_val  + but_val;
        ui->Display->setText(new_val);
    }
}

