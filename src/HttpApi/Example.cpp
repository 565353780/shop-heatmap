#include "Example.h"

Example::Example(QObject *parent) : QObject(parent)
{
    simple_server_ = new SimpleServer(nullptr, 4400);
    connect(simple_server_, SIGNAL(requestReceived(QString, QJsonObject)), this, SLOT(slot_requestReceived(QString, QJsonObject)));
}

Example::~Example()
{

}

void Example::run_example()
{

}

void Example::slot_requestReceived(QString method, QJsonObject content)
{
    qDebug() << "get request :";
    qDebug() << method;
    qDebug() << content;
}
