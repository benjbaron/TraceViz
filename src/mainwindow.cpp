#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "binaryevolvinggraphreader.h"
#include "graphviewer.h"
#include <QDebug>
#include <QLayout>
#include <QFileDialog>
#include <QStandardPaths>
#include "agent.h"
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));

    this->setGeometry(100,100,600,600);
    BinaryEvolvingGraphReader reader("/Users/quentinbramas/Projects/wsnet/wsnet-tools/test.beg");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open()
{
    QString filename = QFileDialog::getOpenFileName(this, "Ouvrir une trace", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), "Fichier trace (*.csv);;Tous les fichiers (*.*)");

    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return;
    }
    QTextStream in(&file);

    gv = new GraphViewer();

    while(!in.atEnd())
    {
        qDebug()<<in.readLine();
        gv->agents.append(new Agent());
    }
    this->setCentralWidget(gv);


}
