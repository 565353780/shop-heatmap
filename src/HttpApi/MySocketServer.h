#ifndef MYSOCKETSERVER_H
#define MYSOCKETSERVER_H

#include <QObject>
#include <QQueue>
#include <QJsonObject>
class QTcpServer;
class QTcpSocket;
class MySocketServer : public QObject
{
    Q_OBJECT
public:
    explicit MySocketServer(QObject *parent = nullptr, int port=0);
signals:
    void signalReceiveText(QString);
    void signalConnectClose();
public slots:
    void slotActualSendTextMsg();
    void slotReadRequest();
    Q_INVOKABLE void sendObject(QJsonObject);
    Q_INVOKABLE void sendTextMessage(QString);
private:
    int port_;
    QQueue<QByteArray> sending_data_queue_;
    int status_=0;//0:available 1:busy
    QString data_end_="#*#DataEnd#*#";
    QTcpServer* server_;
    QTcpSocket* client_socket_=nullptr;
    QString  content_;
    QByteArray content_data_;
};

#endif // MYSOCKETSERVER_H
