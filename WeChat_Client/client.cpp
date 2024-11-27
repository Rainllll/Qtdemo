#include "client.h"
#include "ui_client.h"
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QHostAddress>
Client::Client(QWidget *parent): QMainWindow(parent), ui(new Ui::Client)
{
    ui->setupUi(this);
    loadConfig();
    connect(&m_tcpSocket, &QTcpSocket::connected, this, &Client::showConnect);
    m_tcpSocket.connectToHost(QHostAddress(this->ip), this->port);
    connect(&m_tcpSocket, &QTcpSocket::readyRead, this, &Client::recvmsg);
    rh = new ResHandler();
}

Client::~Client()
{
    delete ui;
}

void Client::loadConfig()
{
    QFile file(":/client.config");

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "打开配置文件", "打开配置文件失败");
        return;
    }

    QByteArray baData = file.readAll();
    QString strdata = QString(baData);
    QStringList ql = strdata.split("\r\n");
    ip = ql.at(0);
    port = ql.at(1).toUShort();
    m_rootPath = ql.at(2);
    qDebug() << "\n******配置文件加载******";
    qDebug() << "IP:" << ip;
    qDebug() << "Localhost:" << port;
    qDebug() << "My rootPath:" << m_rootPath;
    file.close();
    qDebug() << "***********************";
    file.close();

}

Client &Client::getInstance()
{
    static Client instance;
    return instance;
}

QTcpSocket& Client::getTcpSocket()
{
    return m_tcpSocket;
}

void Client::showConnect()
{
    qDebug() << "\n服务器连接成功";
}

void Client::recvmsg()
{
    QByteArray data = m_tcpSocket.readAll();
    this->buff.append(data);

    while(buff.size() >= (int)(sizeof(PDU)))
    {
        PDU* pdu = (PDU*)buff.data();

        if(buff.size() < int(pdu->uiPDUlen))break;

        qDebug() << "\n***********接收数据单元************";
        qDebug() << "readPDU Type:" << pdu->uiMsgType
                 << "\nPar1:" << pdu->caData
                 << "\nPar2:" << pdu->caData + 32
                 << "\nMessage:" << pdu->caMsg;
        qDebug() << "************************************";
        handleRes(pdu);

        buff.remove(0, pdu->uiPDUlen);
    }
}


void Client::handleRes(PDU *pdu)
{
    rh->pdu = pdu;

    switch (pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_RESPOND:
    {
        rh->regist();
        break;
    }

    case ENUM_MSG_TYPE_LOGIN_RESPOND:
    {
        rh->login();
        break;
    }

    case ENUM_MSG_TYPE_REFRESH_RESPOND:
    {
        rh->refreshfriend();
        break;
    }

    case ENUM_MSG_TYPE_REFRESH_SIGNATURE_RESPOND:
    {
        rh->refreshfriendSignature();
        break;
    }

    case ENUM_MSG_TYPE_CHAT_RESPOND:
    {
        rh->chatRequest();
        break;
    }

    case ENUM_MSG_TYPE_CHAT_IMG_RESPOND:
    {
        rh->chatImgRequest();
        break;
    }

    case ENUM_MSG_TYPE_CHAT_IMG_AGREE_RESPOND:
    {
        rh->chatImgAgreeRes();
        break;
    }

    case ENUM_MSG_TYPE_CHAT_IMG_DATA_RESPOND:
    {
        rh->chatImgData();
        break;
    }

    case ENUM_MSG_TYPE_FRIEND_MANAGE_REFRESH_RESPOND:
    {
        rh->refreshfriend();
        break;
    }

    case ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND:
    {
        rh->deleteFriendRes();
        break;
    }

    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
    {
        rh->addFriendRequest();
        break;
    }

    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPOND:
    {
        rh->addFriendAgree();
        break;
    }

    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST:
    {
        rh->addFriendAgree();
        break;
    }

    case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:
    {
        rh->addFriend();
        break;
    }

    case ENUM_MSG_TYPE_ONLINE_USER_RESPOND:
    {
        rh->onlineUser();
        break;
    }

    default:
        break;
    }
}

void Client::on_login_clicked()
{
    //获取账号和密码
    QString strname = ui->name->text();
    QString strpwd = ui->pwd->text();

    //特判
    if(strname.isEmpty() || strpwd.isEmpty() ||
        strname.toStdString().size() > 32 || strpwd.toStdString().size() > 32)
    {
        QMessageBox::information(this, "登录", "用户名或密码非法");
        return;
    }

    //构造结构体
    PDU* pdu = mkpdu(0);
    //设置消息类型
    pdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_REQUEST;
    //设置消息内容
    memcpy(pdu->caData, strname.toStdString().c_str(), 32);
    memcpy(pdu->caData + 32, strpwd.toStdString().c_str(), 32);
    m_stringLoginName = strname;
    sendPDU(pdu);
}

void Client::sendPDU(PDU* pdu)
{
    m_tcpSocket.write((char*)pdu, pdu->uiPDUlen);
    qDebug() << "\n*******发送数据单元*******" ;
    qDebug() << "caData:" << pdu->caData;
    qDebug() << "caData+32:" << pdu->caData + 32;
    qDebug() << "caMsg:" << pdu->caMsg;
    qDebug() << "**************************" ;
    free(pdu);
    pdu = NULL;
}

QString Client::getRootPath()
{
    return this->m_rootPath;
}

void Client::on_register_2_clicked()
{
    //获取账号和密码
    QString strname = ui->name->text();
    QString strpwd = ui->pwd->text();

    //特判
    if(strname.isEmpty() || strpwd.isEmpty() ||
        strname.toStdString().size() > 32 || strpwd.toStdString().size() > 32)
    {
        QMessageBox::information(this, "注册", "用户名或密码非法");
        return;
    }

    //构造结构体
    PDU* pdu = mkpdu(0);
    //设置消息类型
    pdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;
    //设置消息内容
    memcpy(pdu->caData, strname.toStdString().c_str(), 32);
    memcpy(pdu->caData + 32, strpwd.toStdString().c_str(), 32);
    sendPDU(pdu);
}
