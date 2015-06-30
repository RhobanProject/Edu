#include "MainWindow.h"
#include "../bin/ui_MainWindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //_timer_refresh.setInterval( 100 );
    _timer_refresh.start( 10 );
    connect(&_timer_refresh,SIGNAL(timeout()),this,SLOT(refreshComponents()));

    /* Creation de la simulation et du widget d'affichage de la simu*/
    Robot* robot = new Robot(70,35,35,2);
    Simulation* simulation = new Simulation( robot );
    simulation->initializeMap(500, 3, 0.5, 100, 150, 5, 4);
    simulation->getMap()->processToFieldMap();
    simulation->getMap()->generateLane(150/SimulationViewer::cote, 600/SimulationViewer::cote, 100/SimulationViewer::cote);
    ui->centralwidget->addSimulation( simulation );
    ui->centralwidget->initialization();
    ui->controlDockContent->setSimulationViewer( ui->centralwidget );
    ui->mapControlDockContents->setSimulation( simulation );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::refreshComponents()
{    
    ui->fieldDockContent->initialize( ui->centralwidget->getSimulation() );
    ui->controlDockContent->refreshComponents( ui->centralwidget );
    ui->fieldDockContent->refreshComponents( ui->centralwidget->getSimulation() );
    ui->robotDockContent->refreshComponents( ui->centralwidget->getSimulation()->getRobots()->at(0) );
    //ui->controlDockContent->refreshComponents( centralWidget->get );
}
