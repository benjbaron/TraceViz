#include "binaryevolvinggraphreader.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>

BinaryEvolvingGraphReader::BinaryEvolvingGraphReader(QString filename)
{
    QFile file(filename);
    if(!file.open(QFile::WriteOnly))
    {
        return;
    }
    QDataStream out(&file);
    QString metaData;
    //in >> metaData;
    //qDebug()<<metaData;
    qint32 b=10;
    out << b;

    /*QDataStream in(&file);
    QString metaData;
    //in >> metaData;
    //qDebug()<<metaData;
    qint32 a;
    in >> a;
    qDebug()<<a;*/
}
