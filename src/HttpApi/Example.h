#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <QObject>

#include "HttpApi/SimpleClient.h"
#include "HttpApi/SimpleServer.h"

class Example : public QObject
{
    Q_OBJECT
public:
    explicit Example(QObject *parent = nullptr);
    ~Example();

    void static run_example();

public slots:
    void slot_requestReceived(QString method, QJsonObject content);

private:
    SimpleServer* simple_server_;
};

#endif // EXAMPLE_H
