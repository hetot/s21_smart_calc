#ifndef GRAPH_H
#define GRAPH_H

#include <QMainWindow>
#include <QtCharts>
#include <QSplineSeries>

namespace Ui {
class Graph;
}

class Graph : public QMainWindow
{
    Q_OBJECT

public:
    explicit Graph(QWidget *parent = nullptr);
    ~Graph();

signals:
    void firstWindow();

private:
    Ui::Graph *ui;
};

#endif // GRAPH_H
