#include "agent.h"
#include <QTimer>
#include <QDebug>
#include "graphviewer.h"

Agent::Agent(QVector2D startPosition)
{
    position = destination = QVector2D(600.0* qrand()/RAND_MAX, 600.0*qrand()/RAND_MAX);
    if(!startPosition.isNull())
    {
        position = startPosition;
    }
    speed = 100;
}
void Agent::tick(quint64 elapsed)
{
    if((position - destination).length() < 2)
    {
        destination = QVector2D(600.0* qrand()/RAND_MAX, 600.0*qrand()/RAND_MAX);
    }

    position += (destination - position).normalized()*speed*elapsed/1000.0;
}
