#ifndef MAPCONTROLWIDGET_H
#define MAPCONTROLWIDGET_H

#include <QWidget>

#include "SimulationViewer.h"
#include "Simulation.h"
#include "Map.h"

namespace Ui {
    class MapControlWidget;
}

class MapControlWidget : public QWidget {
    Q_OBJECT
public:
    MapControlWidget(QWidget *parent = 0);
    ~MapControlWidget();
    void setSimulation( Simulation* simulation);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::MapControlWidget *ui;
    Simulation* _simulation;

private slots:
    void on_pushButton_clicked();
};

#endif // MAPCONTROLWIDGET_H
