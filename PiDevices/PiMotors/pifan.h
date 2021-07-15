#ifndef PIFAN_H
#define PIFAN_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QThread>

#include <pigpiod_if2.h>

class PiFan : public QObject
{
    Q_OBJECT
public:
    explicit PiFan(const int &pi, const int &en, QObject *parent = nullptr);
public slots:
    void toggleFan(const bool &enabled);
signals:
private:
    const int pi;

    const int en;
};

#endif // PIFAN_H
