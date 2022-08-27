#include "credit.h"
#include "ui_credit.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <cmath>
#include <QLayout>

Credit::Credit(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Credit)
{
    ui->setupUi(this);

    QPushButton *but = Credit::findChild<QPushButton *>();
    connect(but, &QPushButton::released, this, &Credit::Calculate);
}

Credit::~Credit()
{
    delete ui;
}

void Credit::Calculate() {
    double loan = ui->loan_amount->text().toDouble();
    double interest = ui->interest_rate->text().toDouble();
    double duration = ui->loan_term->text().toDouble();

    QMessageBox msgBox;
    QString ans = "";

    if (loan >= 0 && interest >= 0 && duration >= 0) {
        if (ui->equal_payments->isChecked()) {
            interest = interest / (100 * 12);
            double coef = interest / (1 - pow(1 + interest, -(duration * 12)));
            double payment = loan * coef;
            for (int i = 0; i < (int)(duration * 12); i++) {
                ans = ans + QString::number(payment) + "\n";
            }
            ui->textEdit->setText(ans);
        } else if (ui->diff_payments->isChecked()) {
            double monthly = loan / (duration * 12);
            for (int i = 0; i < (int)(duration * 12); i++) {
                double payment = monthly + (loan - monthly * i) * interest / (100 * 12);
                ans = ans + QString::number(payment) + "\n";
            }
            ui->textEdit->setText(ans);
        } else {
            msgBox.setText("Choose payment method");
            msgBox.exec();
        }
    } else {
        msgBox.setText("Invalid input");
        msgBox.exec();
    }
}
