#include "piplanner.h"

PiPlanner::PiPlanner(int delay, PiMap* piMap, PiMotion *piMotion, QObject *parent) : QObject(parent), delay(delay), piMap(piMap), piMotion(piMotion)
{
}

PiPlanner::~PiPlanner(){

}

void PiPlanner::loop(){
    forever{
        qInfo()<<"Running piWorker thread: "<<QThread::currentThread();

        if(QThread::currentThread()->isInterruptionRequested())
            return;

        if(piMap->moveIfSafe("forward"))
            piMotion->moveRight(90);

        QThread::msleep(3000);

        if(piMap->moveIfSafe("left"))
            piMotion->stepLeft();

        QThread::msleep(delay);

    }
}

