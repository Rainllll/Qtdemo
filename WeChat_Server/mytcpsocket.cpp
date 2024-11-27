#include "mytcpsocket.h"
#include "operadb.h"

PDU *MyTcpSocket::handleReq(PDU *pdu)
{
    reqhandler->pdu = pdu;

    switch (pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_LOGIN_REQUEST:
        return reqhandler->login(m_strname);

    case ENUM_MSG_TYPE_REGIST_REQUEST:
        return reqhandler->regist();

    case ENUM_MSG_TYPE_REFRESH_REQUEST:
        return reqhandler->preRefreshFriend(m_strname);

    case ENUM_MSG_TYPE_REFRESH_SIGNATURE_REQUEST:
        return reqhandler->refreshFriendSignature(m_strname);

    case ENUM_MSG_TYPE_CHAT_REQUEST:
        return reqhandler->chat();

    case ENUM_MSG_TYPE_CHAT_IMG_REQUEST:
        return reqhandler->chatImg();

    case ENUM_MSG_TYPE_CHAT_IMG_AGREE_REQUEST:
        return reqhandler->chatImgReq();

    case ENUM_MSG_TYPE_CHAT_IMG_DATA_REQUEST:
        return reqhandler->chatImgData();

    case ENUM_MSG_TYPE_FRIEND_MANAGE_REFRESH_REQUEST:
        return reqhandler->preRefreshFriend(m_strname);

    case ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST:
        return reqhandler->deleteFriend();

    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
        return reqhandler->addFriend();

    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST:
        return reqhandler->addFriendAgree();

    case ENUM_MSG_TYPE_ONLINE_USER_REQUEST:
        return reqhandler->onlineUser();

    default:
        break;
    }

    return NULL;
}

void MyTcpSocket::sendPDU(PDU *respdu)
{
    if(respdu == NULL)return;

    write((char*)respdu, respdu->uiPDUlen);
    free(respdu);
    respdu = NULL;
}

MyTcpSocket::MyTcpSocket()
{
    connect(this, &QTcpSocket::readyRead, this, &MyTcpSocket::recvmsg);

    if(connect(this, &QTcpSocket::disconnected, this, &MyTcpSocket::clientoffline))
    {
        qDebug() << "Connect success";
    }

    this->reqhandler = new ReqHandler();
}

MyTcpSocket::~MyTcpSocket()
{

}

void MyTcpSocket::recvmsg()
{
    QByteArray data = this->readAll();
    this->buff.append(data);
    qDebug() << "\n***********接收数据单元************";
    qDebug() << "readPDU 接收消息的长度：" << this->bytesAvailable();

    while(buff.size() >= (int)(sizeof(PDU)))
    {
        PDU* pdu = (PDU*)buff.data();

        if(buff.size() < int(pdu->uiPDUlen))break;

        qDebug() << "readPDU Type:" << pdu->uiMsgType
                 << "Par1:" << pdu->caData
                 << "Par2:" << pdu->caData + 32
                 << "Message:" << pdu->caMsg;
        PDU* respdu = handleReq(pdu);
        sendPDU(respdu);
        buff.remove(0, pdu->uiPDUlen);
    }

    qDebug() << "************************************";
}

void MyTcpSocket::clientoffline()
{
    qDebug() << "off";
    OperaDB::getinstance().handleoffline(m_strname.toStdString().c_str());
    emit offline(this);
}

