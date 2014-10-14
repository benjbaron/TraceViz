#include "agent.h"
#include <QTimer>
#include <QDebug>
#include <algorithm>
#include <cmath>
#include <QtCore>
#include <QGeoCoordinate>
#include "graphviewer.h"

Agent::Agent(int id, float range)
{
    this->id = id;
    this->range = range;
}

void Agent::addPosition(quint64 timestamp, QGeoCoordinate position) {
    positions.insert(timestamp, position);
}

QGeoCoordinate Agent::getPosition(quint64 timestamp) {

    // If the timestamp is an index in the positions of the agent
    if(positions.contains(timestamp)) {
        return positions[timestamp];
    }

    // Interpolate the position if the timestamp is not in the QMap
    QList<quint64> timestampsList = positions.keys();

    // Test if the timestamp is within the bounds of the position
    if(timestamp < timestampsList.first() || timestamp > timestampsList.last()) {
        return QGeoCoordinate();
    }

    quint64 prevTimestamp = timestampsList.first();
    for (int i = 1; i < timestampsList.size(); ++i) {
        quint64 nextTimestamp = timestampsList[i];

        // Find the two tmestamps in the position list that bound the timestamp we want to interpolate
        if(timestamp >= prevTimestamp && timestamp <= nextTimestamp) {

            QGeoCoordinate prevPosition = positions[prevTimestamp];
            QGeoCoordinate nextPosition = positions[nextTimestamp];

            // Case if the agent is not active
            if (!prevPosition.isValid()|| !nextPosition.isValid()) {
                // Then the current position is not yet active
                return QGeoCoordinate();
            }

            // Interpolate the current position of the agent based on the bounding positions
            double coefficient = ((timestamp - qMin(prevTimestamp, nextTimestamp)) / qAbs(nextTimestamp - prevTimestamp));
            double lat = qMin(prevPosition.latitude(),  nextPosition.latitude())  + qAbs(nextPosition.latitude()  - prevPosition.latitude())  * coefficient;
            double lon = qMin(prevPosition.longitude(), nextPosition.longitude()) + qAbs(nextPosition.longitude() - prevPosition.longitude()) * coefficient;
            return QGeoCoordinate(lat,lon);
        }
        // Increment the timestamps
        prevTimestamp = nextTimestamp;
    }
    return QGeoCoordinate();
}

void Agent::tick(quint64 currentTimestamp)
{
    // This function is executed at each tick
    // Aim: compute the current position of the agent
    currentPosition = this->getPosition(currentTimestamp);
}

void Agent::contacts(Agent * agent, qint64 timePrecision) {
    qDebug() << "Examining contacts between " << this->id << " and " << agent->id;
    // Print the contacts between the current agent and agent (passed in parameter)
    bool continuity = false;
    quint64 contactLength = 0;
    quint64 start = qMax(this->positions.keys().first(), agent->positions.keys().first());
    quint64 end  = qMin(this->positions.keys().last(), agent->positions.keys().last());

    quint64 contactStart = 0;

    for (quint64 ts = start; ts <= end; ts += timePrecision) {
        QGeoCoordinate pos1 = this->getPosition(ts);
        QGeoCoordinate pos2 = agent->getPosition(ts);
        qreal distance = pos1.distanceTo(pos2);
        if(distance < qMin(this->range, agent->range)) {
//            qDebug() << "## Contact between bus" << this->id << "and" << agent->id << "at" << ts;
            if (!continuity) {
                continuity = true;
                contactStart = ts;
            }
            // Increment the contact length counter
            contactLength += timePrecision;
        } else {
            if (continuity) {
                // only if the continuity is still true
                qDebug() << "Contact between bus " << this->id << " and " << agent->id << " -- " << contactLength << contactStart << contactStart+contactLength;
                continuity = false;
                contactLength = 0;
            }
        }
    }
    if (continuity) {
        // only if the continuity is still true after the end of the loop
        qDebug() << "Contact between bus " << this->id << " and " << agent->id << " -- " << contactLength;
    }
    qDebug() << "No more";
}
