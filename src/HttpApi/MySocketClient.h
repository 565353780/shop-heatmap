#ifndef MYSOCKETCLIENT_H
#define MYSOCKETCLIENT_H

#include <QObject>
#include <QQueue>
#include <QTimer>

#include <QTcpSocket>
#include <QJsonObject>
class MySocketClient : public QObject
{
    Q_OBJECT
public:
    MySocketClient(QObject* parent=0, int port=0);


public slots:
    Q_INVOKABLE void setUrl(QString);
    Q_INVOKABLE void setActive(bool);
    Q_INVOKABLE void sendTextMessage(QString);
    void sendObject(QJsonObject);

    Q_INVOKABLE void setAddress(QString);
    void slotReceiveMsg();
    void actualSendText();
signals:
    Q_INVOKABLE void currentStatus(int);
    void  clientReceiveTextMsg(QString);
private:
    int status_; //0:void 1:busy
    int port_;
    QString data_end_="#*#DataEnd#*#";
    QString address_;
    QString server_url_;
    QTcpSocket* client_socket_;
    QString msg_;
    QByteArray msg_array_;
    QTimer* check_status_timer_;
    QQueue<QString> sending_queue_;

};

#endif // MYSOCKETCLIENT_H
