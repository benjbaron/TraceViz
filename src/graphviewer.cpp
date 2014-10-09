#include "graphviewer.h"
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include "agent.h"

GraphViewer::GraphViewer(QWidget *parent) :
    QWidget(parent)
{
    QTimer *timer = new QTimer();
    timer->setInterval(1);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start();
    _elapsedTimer.start();
}


void GraphViewer::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    quint64 elapsed = _elapsedTimer.elapsed();
    _elapsedTimer.start();
    //painter.setRenderHint(QPainter::Antialiasing);

    foreach(Agent* agent, agents)
    {
        agent->tick(elapsed);
    }
    painter.setPen(QPen(QColor(0,0,0,255)));
    painter.setBrush(QBrush(QColor(200,200,200,255)));
    foreach(Agent* agent, agents)
    {
        painter.drawEllipse(agent->position.toPoint(), 2,2);
    }
    painter.setPen(QPen(QColor(0,0,0,80)));
    foreach(Agent* agent, agents)
    {
        foreach(Agent* neig, agents)
        {
            if(agent->position != neig->position && agent->position.distanceToPoint(neig->position) < 25)
            {
                painter.drawLine(agent->position.toPoint(), neig->position.toPoint());
            }
        }
    }
    painter.drawText(0, 30, QString::number(1000.0/elapsed));
}

void GraphViewer::mousePressEvent(QMouseEvent * event)
{
    Agent * a = new Agent(QVector2D(event->pos()));
    agents.append(a);
    update();
}
