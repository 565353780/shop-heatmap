#include "MySocketServer.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

MySocketServer::MySocketServer(QObject *parent,int port) : QObject(parent),port_(port)
{
    server_=new QTcpServer(this);
    bool listening=server_->listen(QHostAddress::AnyIPv4,port);
    qDebug()<<"listening "<<port<<listening;
    connect(server_,&QTcpServer::newConnection, this, [=]{
        client_socket_=server_->nextPendingConnection();
        connect(client_socket_, &QTcpSocket::readyRead, this, &MySocketServer::slotReadRequest);
        connect(client_socket_, &QTcpSocket::disconnected,this,&MySocketServer::signalConnectClose);
    });
}

void MySocketServer::slotActualSendTextMsg()
{
    if(sending_data_queue_.size()>0 && client_socket_->isWritable()){
        QByteArray data=sending_data_queue_.front().append(data_end_.toUtf8());
        qDebug()<< "sending len:"<<data.size();
        int num= data.length();
        int blockSize=1024*300;
        for(int low=0; low<num; low+=blockSize){
            client_socket_->write(data.mid(low,blockSize));
            client_socket_->waitForBytesWritten();
        }
        sending_data_queue_.pop_front();
        slotActualSendTextMsg();
    }
}


void MySocketServer::slotReadRequest()
{

    QByteArray curData=client_socket_->readAll();
    QString curStringBack=(QString(curData.right(32)));
    content_data_.append(curData);
    qDebug()<<curStringBack;
    if(curStringBack.endsWith(data_end_)){
        content_=QString::fromUtf8(content_data_);

       //qDebug()<<msg_<<msg_.size();
        QStringList msglist= content_.split(data_end_);
        msglist.removeAll("");
        foreach (QString msg, msglist) {
            emit signalReceiveText(msg);
        }
        //qDebug()<<"finish receive msg len:"<<msg_.size();
        content_.clear();
        content_data_.clear();
    }
}

void MySocketServer::sendObject(QJsonObject msg)
{
    QJsonDocument doc(msg);
   sendTextMessage(QString(doc.toJson()));

}

void MySocketServer::sendTextMessage(QString msg)
{

    if(client_socket_){
        sending_data_queue_.push_back(msg.toUtf8());
        slotActualSendTextMsg();
    }

}
