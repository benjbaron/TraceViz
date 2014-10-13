#include "agent.h"
#include <QTimer>
#include <QDebug>
#include <algorithm>
#include <cmath>
#include <QtCore>
#include <QGeoCoordinate>
#include "graphviewer.h"

Agent::Agent(int id, float range, GraphViewer * gv)
{
    this->id = id;
    this->range = range;
    this->gv = gv;
}

void Agent::addPosition(quint64 timestamp, QGeoCoordinate position) {
    positions.insert(timestamp, position);
}

QGeoCoordinate Agent::getPosition(quint64 timestamp) {
    return positions[timestamp];
}

void Agent::tick(quint64 currentTimestamp)
{
    // This function is executed at each tick
    // Aim: compute the current position of the agent

    // Find the two timestamps closest to the currentTimestamp
    QList<quint64> timestampsList = positions.keys();
    quint64 prevTimestamp = timestampsList[0];
    for (int i = 1; i < timestampsList.size(); ++i) {
        quint64 nextTimestamp = timestampsList[i];
        if(currentTimestamp >= prevTimestamp && currentTimestamp <= nextTimestamp) {

            QGeoCoordinate prevPosition = positions[prevTimestamp];
            QGeoCoordinate nextPosition = positions[nextTimestamp];

            // Case if the agent is not active
            if (!prevPosition.isValid()|| !nextPosition.isValid()) {
                // Then the current position is not yet active
                currentPosition = QGeoCoordinate();
                qDebug() << "position not valid";
                break;
            }

            // Interpolate the current position of the agent based on the bounding
            double coefficient = ((currentTimestamp - qMin(prevTimestamp, nextTimestamp)) / qAbs(nextTimestamp - prevTimestamp));
            double lat = qMin(prevPosition.latitude(),  nextPosition.latitude())  + qAbs(nextPosition.latitude()  - prevPosition.latitude())  * coefficient;
            double lon = qMin(prevPosition.longitude(), nextPosition.longitude()) + qAbs(nextPosition.longitude() - prevPosition.longitude()) * coefficient;
            currentPosition = QGeoCoordinate(lat,lon);
            break; // Exit the loop, as we found the two bounding timestamps
        }
        // Increment the timestamps
        prevTimestamp = nextTimestamp;
    }
}
