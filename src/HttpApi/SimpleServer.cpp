#include "SimpleServer.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <qjsondocument.h>
#include <QThread>
SimpleServer::SimpleServer(QObject *parent, int port) : QObject(parent),port_(port)
{
    server_=new QTcpServer(this);
    server_->listen(QHostAddress::AnyIPv4,port);
    connect(server_,&QTcpServer::newConnection, this, [=]{
        read_request_socket_=server_->nextPendingConnection();
        connect(read_request_socket_,&QTcpSocket::readyRead, this, &SimpleServer::slotReadRequest);
    });
}

void SimpleServer::slotReadRequest()
{
    qDebug()<<"new request in";
    QByteArray data;
    while(true){
        QByteArray data_new = read_request_socket_->readAll();
        qDebug() << data_new;
        qDebug() << data_new.size();
        if(data_new.size() == 0)
        {
            break;
        }
        data += data_new;
        QTime time;
        time.start();
        while(time.elapsed() < 100)
        {
            QCoreApplication::processEvents();
        }
    }
    QString content=QString(data);
    qDebug()<<content;
    QStringList splitList=content.split("\r\n");
    QStringList method_words=(splitList.at(0)).split(" ");
    QString method;
    if(method_words.size()>1)
        method=method_words[1];
    QString json_str=splitList.back();
    qDebug()<<"method,json_str"<<method<<json_str;
    QJsonDocument doc=QJsonDocument::fromJson(json_str.toUtf8());
//    QString result_string="OK";
//    QString http = "HTTP/1.1 200 OK\r\n";
//        http += "Server: nginx\r\n";
//        http += "Content-Type: application/octet-stream;charset=utf-8\r\n";
//        http += "Connection: close\r\n";
//        http += QString("Content-Length: %1\r\n\r\n").arg(result_string.toUtf8().size());
//        http += QString("%1\r\n").arg(result_string);

//    read_request_socket_->write(http.toUtf8());
//    read_request_socket_->flush();
//    read_request_socket_->waitForBytesWritten();
////    read_request_socket_->disconnectFromHost();
////    read_request_socket_->waitForDisconnected();
//    read_request_socket_->disconnectFromHost();
    emit requestReceived(method,doc.object());
}

void SimpleServer::slotSendTextMessage(QString text)
{
    QString result_string = text;
    QString http = "HTTP/1.1 200 OK\r\n";
        http += "Server: nginx\r\n";
        http += "Content-Type: plainText/text;charset=utf-8\r\n";
        http += "Connection: close\r\n";
        http += QString("Content-Length: %1\r\n\r\n").arg(result_string.toUtf8().size());
        http += QString("%1\r\n").arg(result_string);

    read_request_socket_->write(http.toUtf8());
    read_request_socket_->flush();
    read_request_socket_->waitForBytesWritten();
//    read_request_socket_->disconnectFromHost();
//    read_request_socket_->waitForDisconnected();
    read_request_socket_->disconnectFromHost();
}

HandleCLientServer::HandleCLientServer(QObject *parent, int port)
    :QObject(parent)
{
    server_=new QTcpServer(this);
    server_->listen(QHostAddress::AnyIPv4,port);
    connect(server_,&QTcpServer::newConnection, this, [=]{
        client_socket_=server_->nextPendingConnection();
        connect(client_socket_, &QTcpSocket::readyRead, this, &HandleCLientServer::slotReadRequest);
    });
}

void HandleCLientServer::slotReadRequest()
{
    QByteArray array=client_socket_->readAll();
    QString content=QString(array.right(16));
    //qDebug()<<array.size()<<content.remove(10,content.size()-20);
    content_data_.append(array);
    if(content.endsWith("\"}") || content.endsWith("\"\n}\n")){
        content_=QString::fromUtf8(content_data_).simplified();
        int index=content_.lastIndexOf("{");
        while (index!=-1) {
            emit receiveText(content_.right(content_.size()-index));
            emit signalReceiveText(content_.right(content_.size()-index));
            content_.remove(index,content_.size()-index);
            index=content_.lastIndexOf("{");
        }
        if(content_.size()>0){
            emit receiveText(content_);
            emit signalReceiveText(content_);
        }
        content_.clear();
        content_data_.clear();
    }
}

void HandleCLientServer::sendObject(QJsonObject msg)
{
    QJsonDocument doc(msg);
    client_socket_->write(doc.toJson());
    client_socket_->waitForBytesWritten();
}

void HandleCLientServer::sendTextMessage(QString msg)
{
    if(client_socket_){
        QByteArray data=msg.toUtf8();
        qDebug()<< "sending len:"<<msg.size();
        int num= data.length();
        int blockSize=1024*300;
        for(int low=0; low<num; low+=blockSize){
            client_socket_->write(data.mid(low,blockSize));
            client_socket_->waitForBytesWritten();
        }
    }

}
