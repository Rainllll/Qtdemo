#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include "qtcpsocket.h"
#include "reqhandler.h"
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    ReqHandler* reqhandler;
    //    MsgHandler* msghandler;
    QString m_strname;
    QByteArray buff;
    PDU* readPDU();
    PDU* handleReq(PDU* pdu);
    void sendPDU(PDU* pdu);
    MyTcpSocket();
    ~MyTcpSocket();

public slots:
    void recvmsg();
    void clientoffline();
signals:
    void offline(MyTcpSocket* mysocket);
};

#endif // MYTCPSOCKET_H
