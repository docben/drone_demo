#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    const Vector2D tabPos[5]={{60,80},{400,700},{50,250},{800,800},{700,50}};

    for (int i=0; i<5; i++) {
        QListWidgetItem *LWitems=new QListWidgetItem(ui->listDronesInfo);
        ui->listDronesInfo->addItem(LWitems);
        QString name="Drone"+QString::number(i+1);
        mapDrones[name]=new Drone(name);
        mapDrones[name]->setInitialPosition(tabPos[i]);
        ui->listDronesInfo->setItemWidget(LWitems,mapDrones[name]);
    }

    ui->widget->setMap(&mapDrones);
    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start();

    elapsedTimer.start();
}


MainWindow::~MainWindow() {
    delete ui;
    delete timer;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::update() {
    static int last=elapsedTimer.elapsed();
    static int steps=1000;
    int current=elapsedTimer.elapsed();
    int dt=current-last;
    // update positions of drones
    for (auto &drone:mapDrones) {
        // detect collisions between drones
        if (drone->getStatus()!=Drone::landed) {
            drone->initCollision();
            for (auto &obs:mapDrones) {
                if (obs->getStatus()!=Drone::landed && obs->getName()!=drone->getName()) {
                    Vector2D B=obs->getPosition();
                    drone->addCollision(B,ui->widget->droneCollisionDistance);
                }
            }
            drone->update(dt/1000.0,steps);
        }
    }
    int d = elapsedTimer.elapsed()-current;
    ui->statusbar->showMessage("duree:"+QString::number(d)+" steps="+QString::number(steps)+ " freq="+QString::number(dt));
    if (d>90) {
        steps/=2;
    } else {

        if (steps<10000) steps+=20;
    }
    last=current;
    ui->widget->repaint();
}
