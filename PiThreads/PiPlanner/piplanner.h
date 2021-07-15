#ifndef PIWORKER_H
#define PIWORKER_H

#include <QObject>
#include <QDebug>
#include <QThread>

#include "PiWidgets/PiMap/pimap.h"
#include "PiThreads/PiMotion/pimotion.h"

class PiPlanner : public QObject
{
    Q_OBJECT
public:
    explicit PiPlanner(int delay, PiMap* piMap, PiMotion *piMotion, QObject *parent = nullptr);
    ~PiPlanner();
public slots:
    void loop();
signals:
private slots:
private:
    const int delay;
    PiMap* piMap;
    PiMotion *piMotion;
};

#endif // PIWORKER_H
