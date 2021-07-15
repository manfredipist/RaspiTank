#ifndef PIMAP_H
#define PIMAP_H

#include <QWidget>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QtMath>
#include <QHash>

#define CELL_SIZE 10
#define CELLS_COUNT 75
#define STARTX 10
#define STARTY 5

#define LASER_CONFIDENCE 0.1
#define SAFETY_CONFIDENCE 0.7

class PiMapCell : public QObject
{
    Q_OBJECT

public:
    explicit PiMapCell(const int &i, const int &j, QObject *parent = nullptr);
    ~PiMapCell();
public slots:
    double getConfidence()const{return confidence;}
    void setConfidence(const double &confidence);
    void decreaseConfidence();

    QGraphicsRectItem* getQGRectItem(){return qGRectItem;}
private slots:
    void updateQGRectItem();
private:
    const int x;
    const int y;
    double confidence;
    QGraphicsRectItem* qGRectItem;
};

namespace Ui {
    class PiMap;
}

class PiMap : public QWidget
{
    Q_OBJECT

public:
    explicit PiMap(const char &orientation='n', const int &startX=STARTX, const int &startY=STARTY, QWidget *parent = nullptr);
    ~PiMap();
signals:

public slots:
    bool moveIfSafe(const QString &direction);
    void updateObstaclesOnMap(const double &front_laser, const double &left_laser, const double &right_laser);
private slots:
    bool hasEnoughConfidenceToMove(const double &newX, const double &newY);
    void setNewHopePosition(const int &newX, const int &newY, const char &newOrientation);
    void changeOrientation(const char &orientation);
    void setObstacleOnMap(const double &newX, const double &newY);

private:
    Ui::PiMap *ui;

    char currentOrientation; // n,s,e,w
    int currentX;
    int currentY;

    QHash<int, QHash<int, PiMapCell*>> grid;
};



#endif // PIMAP_H
