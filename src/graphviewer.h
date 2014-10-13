#ifndef GRAPHVIEWER_H
#define GRAPHVIEWER_H

#include <QWidget>
#include <QMap>
#include <QElapsedTimer>
#include <QSlider>
#include <QTimer>
#include <QGeoCoordinate>
#include <QPoint>
class Agent;

class GraphViewer : public QWidget
{
    Q_OBJECT


public:
    explicit GraphViewer(QWidget *parent = 0);
    QSlider *slider;
    QMap<int, Agent*> agents;

    void startTimer();
    quint64 getTimestampFromSlider();
    int getSliderValueFromTimestamp();

    QGeoCoordinate getTopLeft() const;
    void setTopLeft(const QGeoCoordinate &value);

    QGeoCoordinate getBottomRight() const;
    void setBottomRight(const QGeoCoordinate &value);

    quint64 getEndTimestamp() const;
    void setEndTimestamp(const quint64 &value);

    quint64 getStartTimestamp() const;
    void setStartTimestamp(const quint64 &value);

protected:
    void paintEvent(QPaintEvent *);
    //    void mousePressEvent(QMouseEvent *);
signals:

public slots:
    void oneStep();
    void updateCurrentTimestamp();

private:
    int step; // Step to increment the timer (and the slider)
    QTimer *timer;
    QGeoCoordinate topLeft;
    QGeoCoordinate bottomRight;
    quint64 startTimestamp;
    quint64 endTimestamp;
    quint64 currentTimestamp;

    QPointF translatePosition(QGeoCoordinate);

};

#endif // GRAPHVIEWER_H
