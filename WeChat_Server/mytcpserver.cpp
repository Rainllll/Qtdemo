#include "mytcpserver.h"


MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr handle)
{
    qDebug() << "\n******************";
    qDebug() << "新客户端连接";
    MyTcpSocket *ptcpsocket = new MyTcpSocket;
    ptcpsocket->setSocketDescriptor(handle);
    m_tcpsocket.append(ptcpsocket);
    connect(ptcpsocket, &MyTcpSocket::offline, this, &MyTcpServer::deleteSocket);
    qDebug() << "端口号" << handle;
}

void MyTcpServer::resend(char *tarname, PDU *pdu)
{
    foreach(MyTcpSocket* pTcpSocket, m_tcpsocket)
    {
        if(pTcpSocket->m_strname == tarname)
        {
            pTcpSocket->write((char*)pdu, pdu->uiPDUlen);
            qDebug() << "find:" + pTcpSocket->m_strname;
        }
    }
}

MyTcpServer::MyTcpServer()
{

}

void MyTcpServer::deleteSocket(MyTcpSocket *mysocket)
{
    m_tcpsocket.removeOne(mysocket);
    mysocket->deleteLater();
    mysocket = NULL;
    qDebug() << m_tcpsocket.size();

    for(int i = 0; i < m_tcpsocket.size(); i++)
    {
        qDebug() << m_tcpsocket.at(i)->m_strname;
    }
}
