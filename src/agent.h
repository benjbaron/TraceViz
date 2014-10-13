#ifndef AGENT_H
#define AGENT_H
#include <QObject>
#include <QGeoCoordinate>
#include <QMap>

class GraphViewer;



class Agent : public QObject
{
    Q_OBJECT
public:
    Agent(int id, float range, GraphViewer * gv);

    void addPosition(quint64 timestamp, QGeoCoordinate position);
    QGeoCoordinate getPosition(quint64 timestamp);

    QGeoCoordinate currentPosition;
    QMap<quint64, QGeoCoordinate> positions;
    qreal range;
    int id;
public slots:
    void tick(quint64 currentTimestamp);

private:
    GraphViewer * gv;
};

#endif // AGENT_H
