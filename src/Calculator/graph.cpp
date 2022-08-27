#include "graph.h"
#include "ui_graph.h"

Graph::Graph(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Graph)
{
    ui->setupUi(this);
}

Graph::~Graph()
{
    delete ui;
}
