#ifndef DRONE_H
#define DRONE_H

#include <QWidget>
#include <QProgressBar>
#include <vector2d.h>
#include <QImage>

class Drone : public QWidget {

    Q_OBJECT
public:
    const double maxSpeed=50; // max speed in pixels per second
    const double maxPower=200;
    const double takeoffSpeed=1.5; // unit/s
    const double hoveringHeight=5; // units
    const double coefCollision=1000;
    const double damping=0.2;
    enum droneStatus { landed,takeoff,landing,hovering,turning,flying};
    explicit Drone(const QString &n,QWidget *parent = nullptr);
    ~Drone();
    inline void start() { status=takeoff; height=0; repaint(); }
    inline void stop() { status=landing; }
    inline void setSpeed(double s) { speedSetpoint=(s>maxSpeed?maxSpeed:s); }
    inline void setInitialPosition(const Vector2D& pos) { if (status==landed) position=pos; }
    inline void setGoalPosition(const Vector2D& pos) { goalPosition=pos; }
    inline Vector2D getPosition() { return position; }
    inline droneStatus getStatus() { return status; }
    inline QString getName() { return name; }
    inline double getAzimut() { return azimut; }
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent *event) override;

    void update(double dt,int steps);
    void initCollision();
    void addCollision(const Vector2D& A,float threshold);
    bool hasCollision() { return showCollision; }
signals:

private:
    const int compasSize = 48; //< size of the compas image
    const int barSpace = 150; //< minimum size of the ProgressBar
    droneStatus status;
    double height;            //< current height of the drone
    QString name;             //< name of the drone
    QProgressBar *speedPB;
    QProgressBar *powerPB;
    Vector2D position;        //< current position of the drone
    Vector2D goalPosition;    //< goal position for the drone
    Vector2D direction;       //< current direction
    Vector2D V;               //< current speed vector
    Vector2D ForceCollision;
    double speed;             //< current speed
    double speedSetpoint;     //< speed to reach if possible
    double power;
    double azimut;            // rotation angle of the drone
    QImage compasImg,stopImg,takeoffImg,landingImg;
    bool showCollision;

};

#endif // DRONE_H
