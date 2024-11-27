#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include "mytcpsocket.h"

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    static MyTcpServer& getInstance();
    void incomingConnection(qintptr handle) override;
    void resend(char *tarname, PDU *pdu);
private:
    MyTcpServer();
    MyTcpServer(const MyTcpServer& instance) = delete;
    MyTcpServer& operator=(const MyTcpServer&) = delete;
    QList<MyTcpSocket *> m_tcpsocket;
public slots:
    void deleteSocket(MyTcpSocket* mysocket);
};

#endif // MYTCPSERVER_H
