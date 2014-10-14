#include "graphviewer.h"
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QDebug>
#include <QSlider>
#include <QtCore>
#include <QGeoCoordinate>
#include "agent.h"

GraphViewer::GraphViewer(QWidget *parent) :
    QWidget(parent)
{
    this->step = 1e6;
    this->timer = new QTimer();
    this->slider = new QSlider(Qt::Horizontal, this);
    this->slider->setMinimumWidth(300);
    this->slider->setRange(0, 1e6);
    this->timer->setInterval(1); // Updated each 1 millisecond

    connect(timer, SIGNAL(timeout()), this, SLOT(oneStep()));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(update()));
//    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(changeCurrentTimestamp()));
}

void GraphViewer::startTimer() {
    qDebug() << this->slider->minimum() << this->slider->maximum();
    this->currentTimestamp = this->startTimestamp;
    this->timer->start();
    this->slider->setValue(0);
}

void GraphViewer::updateCurrentTimestamp() {
    // Update the current timestamp
    currentTimestamp = getTimestampFromSlider();
}

quint64 GraphViewer::getTimestampFromSlider() {
    // Code taken from http://stackoverflow.com/questions/929103/convert-a-number-range-to-another-range-maintaining-ratio
    quint64 oldRange = this->slider->maximum() - this->slider->minimum();
    quint64 newRange = this->endTimestamp - this->startTimestamp;
    return (quint64) (((this->slider->value() - this->slider->minimum()) * newRange) / oldRange) + this->startTimestamp;
}

int GraphViewer::getSliderValueFromTimestamp() {
    // Code taken from http://stackoverflow.com/questions/929103/convert-a-number-range-to-another-range-maintaining-ratio
    quint64 oldRange = this->endTimestamp - this->startTimestamp;
    quint64 newRange = this->slider->maximum() - this->slider->minimum();
    return (int) (((currentTimestamp - this->startTimestamp) * newRange) / oldRange) + this->slider->minimum();
}

void GraphViewer::oneStep()
{
//    qDebug() << slider->minimum() << slider->maximum();

    if(getTimestampFromSlider() + step <= endTimestamp) {
        // The time can be incremented
        slider->setValue(getSliderValueFromTimestamp() + 1);
    } else {
        // The simulation must restart
        startTimer();
    }
}
quint64 GraphViewer::getStartTimestamp() const { return startTimestamp; }
void GraphViewer::setStartTimestamp(const quint64 &value){ startTimestamp = value; }

quint64 GraphViewer::getEndTimestamp() const { return endTimestamp;}
void GraphViewer::setEndTimestamp(const quint64 &value) { endTimestamp = value; }

QGeoCoordinate GraphViewer::getBottomRight() const { return bottomRight; }
void GraphViewer::setBottomRight(const QGeoCoordinate &value) { bottomRight = value; }

QGeoCoordinate GraphViewer::getTopLeft() const{ return topLeft; }
void GraphViewer::setTopLeft(const QGeoCoordinate &value) { topLeft = value; }

QPointF GraphViewer::translatePosition(QGeoCoordinate position) {
//    qDebug() << position << " | " << topLeft << " | " << bottomRight;

    // Height and width of the QWidget
    qreal height1 = this->height();
    qreal width1  = this->width();

    // Height and width of the coordinates to display
    // Take into account the negative values
    qreal width2 = 0.0;
    if(topLeft.latitude() < 0.0 && bottomRight.latitude() > 0.0) {
        width2  = qAbs(topLeft.latitude()) + bottomRight.latitude();
    } else {
        width2  = qAbs(bottomRight.latitude() - topLeft.latitude());
    }

    qreal height2 = 0.0;
    if(bottomRight.longitude() < 0.0 && topLeft.longitude() > 0.0) {
        height2 = qAbs(bottomRight.longitude()) + topLeft.longitude();
    } else {
        height2 = qAbs(topLeft.longitude()-bottomRight.longitude());
    }

//    qDebug() << height1 << " | " << height2 << " | " << width1 << " | " << width2;

    qreal xdist = 0.0;
    if(topLeft.latitude() < 0.0 && position.latitude() > 0.0) {
        xdist = qAbs(topLeft.latitude()) + position.latitude();
    } else {
        xdist = qAbs(position.latitude() - topLeft.latitude());
    }

    qreal ydist = 0.0;
    if(bottomRight.longitude() < 0.0 && position.longitude() > 0.0) {
        ydist = qAbs(bottomRight.longitude()) + position.longitude();
    } else {
        ydist = qAbs(position.longitude() - bottomRight.longitude());
    }

    qreal xpos = xdist * (width1/width2);
    qreal ypos = ydist * (height1/height2);

    return QPointF(xpos,ypos);
}


void GraphViewer::paintEvent(QPaintEvent * event)
{
    currentTimestamp = getTimestampFromSlider();
    QPainter painter(this);
    painter.setPen(QPen(QColor(0,0,0,255))); // Contours
    painter.setBrush(QBrush(QColor(0,0,0,255))); // Filling

    //painter.setRenderHint(QPainter::Antialiasing);

    // Update the current position of each agent
    foreach(Agent* agent, agents)
    {
        agent->tick(currentTimestamp);
//        qDebug() << currentTimestamp;
    }

    // Draw each active agent
    foreach(Agent* agent, agents)
    {
        // Draw the past trajectory for the current examined agent
        QList <quint64> timestampsList = agent->positions.keys();
        quint64 prevTimestamp = timestampsList[0];
        for (int i = 1; i < timestampsList.size(); ++i) {
            quint64 nextTimestamp = timestampsList[i];
            QGeoCoordinate prevPosition = agent->positions[prevTimestamp];
            QPointF prevPoint = translatePosition(prevPosition);
            QGeoCoordinate nextPosition = agent->positions[nextTimestamp];
            QPointF nextPoint = translatePosition(nextPosition);

            if (prevTimestamp > currentTimestamp) {
                // Positions that are before the current position
                painter.setPen(QPen(QColor(255,0,0,125)));
                painter.setBrush(QBrush(QColor(255,0,0,125)));
                painter.drawEllipse(prevPoint, 2, 2);
                painter.drawEllipse(nextPoint, 2, 2);
                painter.drawLine(prevPoint, nextPoint);
            }

            if (nextTimestamp < currentTimestamp) {
                // Positions that are after the current position
                painter.setPen(QPen(QColor(0,255,0,125)));
                painter.setBrush(QBrush(QColor(0,255,0,125)));
                painter.drawEllipse(prevPoint, 2, 2);
                painter.drawEllipse(nextPoint, 2, 2);
                painter.drawLine(prevPoint, nextPoint);
            }

            // Increment the timestamps
            prevTimestamp = nextTimestamp;
        }

        // Transform the current position of the node to fit the canevas
        QGeoCoordinate agentPostion = agent->currentPosition;
        painter.setPen(QPen(QColor(0,0,0,255))); // Contours
        painter.setBrush(QBrush(QColor(0,0,0,255))); // Filling
        painter.drawEllipse(translatePosition(agentPostion), 5, 5);

    }

    // Draw the contacts between neighbor active agents
    painter.setPen(QPen(QColor(0,0,0,80)));
    foreach(Agent* agent, agents)
    {
        // Not optimized: only the top half of the loop
        foreach(Agent* neig, agents)
        {
            if(agent->currentPosition.distanceTo(neig->currentPosition) < qMin(agent->range, neig->range))
            {
                // Draw a line between the two nodes if they are in each other's transmission range
                QPointF agentPoint = translatePosition(agent->currentPosition);
                QPointF neigPoint = translatePosition(neig->currentPosition);
                painter.drawLine(agentPoint, neigPoint);
                qDebug() << "Connection between bus 1 and 2 at " << currentTimestamp;
            }
        }
    }
    painter.drawText(0, 40, QString::number(currentTimestamp));
}

//void GraphViewer::mousePressEvent(QMouseEvent * event)
//{
//    Agent * a = new Agent(QVector2D(event->pos()));
//    agents.append(a);
//    update();
//}
