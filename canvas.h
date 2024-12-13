#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <drone.h>
#include <QMouseEvent>
#include <QPaintEvent>

class Canvas : public QWidget {
    Q_OBJECT
public:
    const int droneIconSize=64;
    const double droneCollisionDistance=droneIconSize*1.5;
    explicit Canvas(QWidget *parent = nullptr);

    inline void setMap(QMap<QString,Drone*> *map) { mapDrones=map; }
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:

private:
    QMap<QString,Drone*> *mapDrones=nullptr;
    QImage droneImg;
};

#endif // CANVAS_H
