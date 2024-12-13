#include "canvas.h"
#include <QPainter>

Canvas::Canvas(QWidget *parent)
    : QWidget{parent} {
    droneImg.load("../../media/drone.png");
    setMouseTracking(true);
}

void Canvas::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QBrush whiteBrush(Qt::SolidPattern);
    QPen penCol(Qt::DashDotDotLine);
    penCol.setColor(Qt::lightGray);
    penCol.setWidth(3);
    whiteBrush.setColor(Qt::white);
    painter.fillRect(0,0,width(),height(),whiteBrush);

    if (mapDrones) {
        Vector2D p;
        QRect rect(-droneIconSize/2,-droneIconSize/2,droneIconSize,droneIconSize);
        QRect rectCol(-droneCollisionDistance/2,-droneCollisionDistance/2,droneCollisionDistance,droneCollisionDistance);

        for (auto &drone:*mapDrones) {
            painter.save();
            painter.translate(drone->getPosition().x,drone->getPosition().y);
            painter.rotate(drone->getAzimut());
            painter.drawImage(rect,droneImg);
            if (drone->hasCollision()) {
                painter.setPen(penCol);
                painter.drawEllipse(rectCol);
            }
            painter.restore();
        }
    }
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    // search for a drone that is landed
    auto it = mapDrones->begin();
    while (it!=mapDrones->end() && (*it)->getStatus()!=Drone::landed) {
        it++;
    }
    // if found, ask for a motion to the mouse position
    if (it!=mapDrones->end()) {
        (*it)->setGoalPosition(Vector2D(event->pos().x(),event->pos().y()));
        (*it)->start();
    }
    repaint();
}
