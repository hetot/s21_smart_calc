#include "deposit.h"
#include "ui_deposit.h"
#include <QDateEdit>
#include <QMessageBox>
#include <QLayoutItem>

Deposit::Deposit(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Deposit)
{
    ui->setupUi(this);

    ui->lineEdit->setValidator(new QDoubleValidator(0, 10000000000, 5, this));
    ui->lineEdit_2->setValidator(new QIntValidator(0, 3650, this));
    ui->lineEdit_3->setValidator(new QDoubleValidator(0, 50, 5, this));
    ui->lineEdit_4->setValidator(new QDoubleValidator(0, 50, 5, this));

    QObject::connect(ui->add_fill, &QPushButton::clicked, this, &Deposit::onAddfirst);
    QObject::connect(ui->add_check, &QPushButton::clicked, this, &Deposit::onAddsecond);
    QObject::connect(ui->pushButton, &QPushButton::clicked, this, &Deposit::calculate);
    ui->comboBox_3->addItem("Каждый день");
    ui->comboBox_3->addItem("Каждую неделю");
    ui->comboBox_3->addItem("Раз в месяц");

    ui->comboBox->addItem("Дней");
    ui->comboBox->addItem("Месяцев");
    ui->comboBox->addItem("лет");
}

Deposit::~Deposit()
{
    delete ui;
}

void Deposit::calculate() {
    double deposit_num = ui->lineEdit->text().toDouble();
    int period_num = ui->lineEdit_2->text().toInt();
    double interest_num = ui->lineEdit_3->text().toDouble();
    double tax_num = ui->lineEdit_4->text().toDouble();

    QMessageBox msgBox;
    if (deposit_num >= 0 && period_num >= 0 && interest_num >= 0 && tax_num >= 0) {
        QDate date_now = QDate::currentDate();

        QList<QLineEdit *> lines = ui->frame->findChildren<QLineEdit *>();
        double ins[100];
        QList<QDate> in_dates;
        for (int i = 0; i < lines.count(); i++) {
            if (i % 2 == 0) {
                ins[i / 2] = lines[i]->text().toDouble();
            } else {
                in_dates.append(QDate::fromString(lines[i]->text(), "dd.MM.yyyy"));
            }
        }

        QList<QLineEdit *> lines_del = ui->frame_2->findChildren<QLineEdit *>();
        double ins_del[100];
        QList<QDate> in_dates_del;
        for (int i = 0; i < lines_del.count(); i++) {
            if (i % 2 == 0) {
                ins_del[i / 2] = lines_del[i]->text().toDouble();
            } else {
                in_dates_del.append(QDate::fromString(lines_del[i]->text(), "dd.MM.yyyy"));
            }
        }

        QDate final_date;
        if (QString::compare(ui->comboBox->currentText(), "Дней") == 0) {
            final_date = date_now.addDays(period_num);
        } else if (QString::compare(ui->comboBox->currentText(), "Месяцев") == 0) {
            final_date = date_now.addMonths(period_num);
        } else {
            final_date = date_now.addYears(period_num);
        }

        QDate started_date = date_now;
        date_now = date_now.addDays(1);
        QString ans = "";
        double earned = 0;
        double overal = 0;
        double taxed = 0;
        double taxed_overal = 0;
        while (date_now <= final_date) {
            for (int i = 0; i < lines.count() / 2; i++) {
                if (in_dates[i] == date_now) {
                    deposit_num += ins[i];
                }
            }
            for (int i = 0; i < lines_del.count() / 2; i++) {
                if (in_dates_del[i] == date_now) {
                    deposit_num -= ins_del[i];
                }
            }
            if (deposit_num < 0) {
                break;
            }
            earned += (deposit_num * interest_num) / 36500;
            taxed += earned * tax_num / 100;
            taxed_overal += taxed;
            overal += deposit_num * interest_num / 36500;
            if (ui->checkBox->isChecked()) {
                deposit_num += earned;
            }
            if (QString::compare(ui->comboBox_3->currentText(), "Каждый день") == 0) {
                ans = ans + date_now.toString("dd.MM.yyyy");
                ans = ans + "->" + QString::number(earned) + "\n";
                earned = 0;
                taxed = 0;
            } else if (QString::compare(ui->comboBox_3->currentText(), "Каждую неделю") == 0) {
                if (date_now == started_date.addDays(7) || date_now == final_date) {
                    started_date = date_now;
                    ans = ans + date_now.toString("dd.MM.yyyy");
                    ans = ans + "->" + QString::number(earned) + "\n";
                    earned = 0;
                    taxed = 0;
                }
            } else {
                if (date_now == started_date.addMonths(1) || date_now == final_date) {
                    started_date = date_now.addMonths(1);
                    ans = ans + date_now.toString("dd.MM.yyyy");
                    ans = ans + "->" + QString::number(earned) + "\n";
                    earned = 0;
                    taxed = 0;
                }
            }
            date_now = date_now.addDays(1);
        }
        ans = ans + "Остаток на вкладе->" + QString::number(deposit_num) + ", Начисленные проценты->" + QString::number(overal);
        ans = ans + " ,Налог->" + QString::number(taxed_overal);
        if (deposit_num < 0) {
            msgBox.setText("Invalid computation");
            msgBox.exec();
        } else {
            ui->answer_display->setText(ans);
        }
    }
}

void Deposit::onAddfirst() {
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->frame->layout());

    QHBoxLayout *h_layout = new QHBoxLayout(ui->frame);

    QLineEdit *line = new QLineEdit(ui->frame);
    line->setValidator(new QDoubleValidator(0, 10000000000, 5, this));
    h_layout->addWidget(line);

    QDateEdit *date_edit = new QDateEdit(ui->frame);
    date_edit->setDisplayFormat("dd.MM.yyyy");
    h_layout->addWidget(date_edit);

    QString button_text = "Удалить";
    QPushButton *button = new QPushButton(button_text, ui->frame);
    h_layout->addWidget(button);

    layout->insertLayout(0, h_layout);
    hash_add.insert(button, h_layout);

    QObject::connect(button, &QPushButton::clicked, this, &Deposit::onDeletefirst);
}

void Deposit::onDeletefirst() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    QHBoxLayout *h_layout = hash_add.take(button);

    QLayoutItem *item = NULL;
    while ((item = h_layout->takeAt(0)) != 0) {
        if (item->widget() != 0) {
            delete item->widget();
        }
        delete item;
    }
    delete h_layout;
}

void Deposit::onAddsecond() {
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->frame_2->layout());

    QHBoxLayout *h_layout = new QHBoxLayout(ui->frame_2);

    QLineEdit *line = new QLineEdit(ui->frame_2);
    line->setValidator(new QDoubleValidator(0, 10000000000, 5, this));
    h_layout->addWidget(line);

    QDateEdit *date_edit = new QDateEdit(ui->frame_2);
    date_edit->setDisplayFormat("dd.MM.yyyy");
    h_layout->addWidget(date_edit);

    QString button_text = "Удалить";
    QPushButton *button = new QPushButton(button_text, ui->frame_2);
    h_layout->addWidget(button);

    layout->insertLayout(0, h_layout);
    hash_get.insert(button, h_layout);

    QObject::connect(button, &QPushButton::clicked, this, &Deposit::onDeletesecond);
}

void Deposit::onDeletesecond() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    QHBoxLayout *h_layout = hash_get.take(button);

    QLayoutItem *item = NULL;
    while ((item = h_layout->takeAt(0)) != 0) {
        if (item->widget() != 0) {
            delete item->widget();
        }
        delete item;
    }
    delete h_layout;
}
