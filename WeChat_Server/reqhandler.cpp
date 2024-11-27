#include "mytcpserver.h"
#include "operadb.h"
#include "reqhandler.h"
#include "server.h"
#include <QDebug>
#include <QDir>
#include <qmessagebox.h>
ReqHandler::ReqHandler(QObject *parent) : QObject(parent)
{

}

PDU *ReqHandler::regist()
{
    char caname[32] = {'\0'};
    char capwd[32] = {'\0'};
    memcpy(caname, pdu->caData, 32);
    memcpy(capwd, pdu->caData + 32, 32);
    bool ret = OperaDB::getinstance().handleRegist(caname, capwd);
    PDU* respdu = mkpdu(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_REGIST_RESPOND;
    memcpy(respdu->caData, &ret, sizeof(bool));
    memcpy(respdu->caData + 32, caname, 32);
    return respdu;
}

PDU *ReqHandler::login(QString &m_strname)
{
    char caname[32] = {'\0'};
    char capwd[32] = {'\0'};
    memcpy(caname, pdu->caData, 32);
    memcpy(capwd, pdu->caData + 32, 32);
    bool ret = OperaDB::getinstance().handleLogin(caname, capwd);

    if(ret)
    {
        m_strname = caname;
        qDebug() << m_strname << "Login!";
    }

    PDU* respdu = mkpdu(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPOND;
    memcpy(respdu->caData, &ret, sizeof(bool));
    return respdu;
}

PDU *ReqHandler::onlineUser()
{
    QStringList ret = OperaDB::getinstance().getOnlineUser();
    uint uiMesLen = ret.size() * 32;
    PDU* respdu = mkpdu(uiMesLen);
    respdu->uiMsgType = ENUM_MSG_TYPE_ONLINE_USER_RESPOND;
    qDebug() << "ret.size()" << ret.size();

    for(int i = 0; i < ret.size(); i++)
    {
        qDebug() << "ret.at(i)" << ret.at(i);
        memcpy(respdu->caMsg + i * 32, ret.at(i).toStdString().c_str(), ret.at(i).size());
    }

    return respdu;
}

PDU *ReqHandler::addFriend()
{
    char cacurname[32] = {'\0'};
    char catarname[32] = {'\0'};
    memcpy(cacurname, pdu->caData, 32);
    memcpy(catarname, pdu->caData + 32, 32);
    qDebug() << "addfriend cacurname" << cacurname
             << "catarname" << catarname;

    int ret = OperaDB::getinstance().handleAddFriend(cacurname, catarname);

    if(ret == 1)
    {
        MyTcpServer::getInstance().resend(catarname, pdu);
        return NULL;
    }

    PDU* respdu = mkpdu(0);
    respdu -> uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
    memcpy(respdu->caData, &ret, sizeof(int));
    return respdu;
}

PDU *ReqHandler::addFriendAgree()
{
    qDebug() << "agreeaddfriendcase";
    char cacurname[32] = {'\0'};
    char catarname[32] = {'\0'};
    memcpy(cacurname, pdu->caData, 32);
    memcpy(catarname, pdu->caData + 32, 32);
    OperaDB::getinstance().handleAddFriendAgree(cacurname, catarname);
    MyTcpServer::getInstance().resend(cacurname, pdu);
    PDU* respdu = mkpdu(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPOND;
    return respdu;
}

PDU *ReqHandler::preRefreshFriend(QString loginname)
{
    return refreshFriend(loginname, pdu->uiMsgType + 1);

    //    if(pdu->uiMsgType == ENUM_MSG_TYPE_REFRESH_SIGNATURE_REQUEST)
    //    {
    //        refreshFriendSignature(loginname);
    //    }
}

PDU *ReqHandler::refreshFriend(QString loginname, int type)
{
    qDebug() << "refreshFriend:" << loginname;
    QStringList result = OperaDB::getinstance().handleRefreshFriend(loginname);
    PDU* respdu = mkpdu(result.size() * 32);
    respdu->uiMsgType = type;

    for(int i = 0; i < result.size(); i++)
    {
        memcpy(respdu->caMsg + i * 32, result.at(i).toStdString().c_str(), 32);
        qDebug() << QString(respdu->caMsg + i * 32);
    }

    return respdu;
}

PDU *ReqHandler::refreshFriendSignature(QString loginname)
{
    qDebug() << "refreshFriendSignature:" << loginname;
    QStringList result = OperaDB::getinstance().handleRefreshFriend(loginname);
    QMap<QString, QString> resultmap = OperaDB::getinstance().handleRefreshFriendSignature(loginname);
    PDU* respdu = mkpdu(result.size() * 32);
    respdu->uiMsgType = ENUM_MSG_TYPE_REFRESH_SIGNATURE_RESPOND;

    for(int i = 0; i < result.size(); i++)
    {
        QString signature = resultmap[result.at(i)];
        memcpy(respdu->caMsg + i * 32, signature.toStdString().c_str(), 32);
        qDebug() << QString(respdu->caMsg + i * 32);
    }

    return respdu;
}

PDU *ReqHandler::deleteFriend()
{
    qDebug() << "deletefriendcase";
    char cacurname[32] = {'\0'};
    char catarname[32] = {'\0'};
    memcpy(cacurname, pdu->caData, 32);
    memcpy(catarname, pdu->caData + 32, 32);
    int ret = OperaDB::getinstance().handleDeleteFriend(cacurname, catarname);
    PDU* respdu = mkpdu(0);
    memcpy(respdu->caData, &ret, sizeof(int));
    respdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND;
    return respdu;
}

PDU* ReqHandler::chatImg()
{
    char caTarName[32] = {'\0'};
    memcpy(caTarName, pdu->caData + 48, 16);
    qDebug() << "chat image catarname:" << caTarName;
    pdu->uiMsgType = ENUM_MSG_TYPE_CHAT_IMG_RESPOND;
    MyTcpServer::getInstance().resend(caTarName, this->pdu);
    return NULL;
}

PDU* ReqHandler::chatImgReq()
{
    char caTarName[32] = {'\0'};
    memcpy(caTarName, pdu->caData + 32, 32);
    qDebug() << "chat image agree catarname:" << caTarName;
    pdu->uiMsgType = ENUM_MSG_TYPE_CHAT_IMG_AGREE_RESPOND;
    MyTcpServer::getInstance().resend(caTarName, this->pdu);
    return NULL;
}

PDU* ReqHandler::chatImgData()
{
    char caTarName[32] = {'\0'};
    memcpy(caTarName, pdu->caData, 32);
    qDebug() << "chat image data catarname:" << caTarName;
    pdu->uiMsgType = ENUM_MSG_TYPE_CHAT_IMG_DATA_RESPOND;
    MyTcpServer::getInstance().resend(caTarName, this->pdu);
    return NULL;
}

PDU *ReqHandler::chat()
{
    char caTarName[32] = {'\0'};
    memcpy(caTarName, pdu->caData + 32, 32);
    qDebug() << "chat catarname:" << caTarName;
    pdu->uiMsgType = ENUM_MSG_TYPE_CHAT_RESPOND;
    MyTcpServer::getInstance().resend(caTarName, pdu);
    return NULL;
}
