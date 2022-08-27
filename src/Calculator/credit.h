#ifndef CREDIT_H
#define CREDIT_H

#include <QMainWindow>
#include <QLayout>

namespace Ui {
class Credit;
}

class Credit : public QMainWindow
{
    Q_OBJECT

public:
    explicit Credit(QWidget *parent = nullptr);
    ~Credit();

signals:
    void firstWindow();

private:
    Ui::Credit *ui;

private slots:
    void Calculate();
};

#endif // CREDIT_H
