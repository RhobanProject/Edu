#ifndef FIELDINFORMATIONWIDGET_H
#define FIELDINFORMATIONWIDGET_H

#include <QWidget>

#include "Simulation.h"

namespace Ui {
    class FieldInformationWidget;
}

class FieldInformationWidget : public QWidget {
    Q_OBJECT
public:
    FieldInformationWidget(QWidget *parent = 0);
    ~FieldInformationWidget();
    void initialize( Simulation* simulation );

public slots:
    void refreshComponents(Simulation* simulation);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FieldInformationWidget *ui;
};

#endif // FIELDINFORMATIONWIDGET_H

