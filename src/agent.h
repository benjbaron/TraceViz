#ifndef AGENT_H
#define AGENT_H
#include <QObject>
#include <QVector2D>
#include <QMap>

class Agent : public QObject
{
    Q_OBJECT
public:
    Agent(QVector2D startPosition = QVector2D());
    QVector2D destination;
    qreal speed;
    QMap<int, QVector2D> positions;
public slots:
    void tick(quint64 elapsed);
};

#endif // AGENT_H
