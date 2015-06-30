#ifndef ROBOTINFORMATIONWIDGET_H
#define ROBOTINFORMATIONWIDGET_H

#include <QWidget>

#include "Robot.h"

namespace Ui {
    class RobotInformationWidget;
}

class RobotInformationWidget : public QWidget {
    Q_OBJECT
public:
    RobotInformationWidget(QWidget *parent = 0);
    ~RobotInformationWidget();

public slots:
    void refreshComponents(Robot* robot);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::RobotInformationWidget *ui;
};

#endif // ROBOTINFORMATIONWIDGET_H
