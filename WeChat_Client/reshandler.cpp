#include "client.h"
#include "index.h"
#include "onlineuser.h"
#include "reshandler.h"

#include <QMessageBox>
#include <QDebug>
#include <qdatetime.h>
#include <qdir.h>
#include "friend.h"
ResHandler::ResHandler(QObject *parent) : QObject(parent)
{
    m_bUpload = false;
}

void ResHandler::regist()
{
    bool ret;
    memcpy(&ret, pdu->caData, sizeof(bool));
    char caname[32];
    memcpy(caname, pdu->caData + 32, 32);

    if(ret)
    {
        QMessageBox::information(&Client::getInstance(), "注册", "注册成功");
    }
    else
    {
        QMessageBox::information(&Client::getInstance(), "注册", "非法");
    }

    if(ret)
    {
        QDir dir;
        dir.mkdir(QString("%1/%2").arg(Client::getInstance().getRootPath()).arg(QString(caname)));
    }
}

void ResHandler::login()
{
    bool ret;
    memcpy(&ret, pdu->caData, sizeof(bool));

    if(ret)
    {

        QMessageBox::information(&Client::getInstance(), "登录", "User: "
                                 + Client::getInstance().m_stringLoginName + " Login Success!");
        Index::getInstance().show();
        Client::getInstance().hide();
    }
    else
    {
        QMessageBox::information(&Client::getInstance(), "登录", "用户名或密码错误");
    }
}

void ResHandler::addFriendRequest()
{
    char cacurname[32] = {'\0'};
    memcpy(cacurname, pdu->caData, 32);
    int ret = QMessageBox::question(&Index::getInstance(), "Warning",
                                    QString("Whether to agree to the friend request from %1?").arg(cacurname));

    if(ret != QMessageBox::Yes)
    {
        return;
    }

    createFriendDir(cacurname);
    PDU* respdu = mkpdu(0);
    memcpy(respdu->caData, pdu->caData, 64);
    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST;
    Client::getInstance().sendPDU(respdu);
}

void ResHandler::addFriendAgree()
{
    QMessageBox::information(&Index::getInstance(), "Warning", "Success!");
    char cacurname[32] = {'\0'};
    memcpy(cacurname, pdu->caData, 32);
    char catarname[32] = {'\0'};
    memcpy(catarname, pdu->caData + 32, 32);

    Index::getInstance().refresh();

    if(Client::getInstance().m_stringLoginName == QString(cacurname))
    {
        createFriendDir(catarname);
    }
}

void ResHandler::createFriendDir(QString caname)
{
    QString path = QString("%1/%2/%3").arg(Client::getInstance().getRootPath())
                   .arg(Client::getInstance().m_stringLoginName)
                   .arg(caname);
    qDebug() << path;
    QDir dir;

    if(dir.mkdir(path))
    {
        qDebug() << "好友文件夹创建成功";
    }
    else
    {
        qDebug() << "好友文件夹创建失败或已存在";
    }

}

void ResHandler::deleteFriendRes()
{
    int ret;
    memcpy(&ret, pdu->caData, sizeof(int));

    if(ret == 1)
    {
        QMessageBox::information(&Friend::getInstance(), "Warning", "Delete success!");
        Friend::getInstance().refresh();
    }
    else if(ret == 0)
    {
        QMessageBox::information(&Friend::getInstance(), "Warning", "Delete Failed!");
    }
}

void ResHandler::addFriend()
{
    int ret;
    memcpy(&ret, pdu->caData, sizeof(int));

    if(ret == -2)
    {
        QMessageBox::information(&Friend::getInstance(), "Warning", "He is already your friend");
    }
    else if(ret == -1)
    {
        QMessageBox::information(&Friend::getInstance(), "Warning", "Internal Error");
    }
    else if(ret == 0)
    {
        QMessageBox::information(&Friend::getInstance(), "Warning", "Not online");
    }
}

void ResHandler::onlineUser()
{
    qDebug() << "pdu->camsg" << pdu->caMsg;
    uint uiSize = pdu->uiMsgLen / 32;
    QStringList list;
    char caTmp[32];

    for(uint i = 0; i < uiSize; i++)
    {
        memcpy(caTmp, pdu->caMsg + i * 32, 32);

        if(QString(caTmp) == Client::getInstance().m_stringLoginName)
        {
            continue;
        }

        list.append(QString(caTmp));
    }

    OnlineUser::getInstance().showOnlineUser(list);
}

void ResHandler::refreshfriend()
{
    uint uisize = pdu->uiMsgLen / 32;
    QStringList nameList;
    QStringList Signature = this->Signature;
    char catmp[32] = {'\0'};
    qDebug() << "********好友列表********";

    for(uint i = 0; i < uisize; i++)
    {
        memcpy(catmp, pdu->caMsg + i * 32, 32);
        nameList.append(QString(catmp));
        qDebug() << QString(catmp);
    }

    qDebug() << "************************";
    Index::getInstance().updateFriendLW(nameList, Signature);
    Friend::getInstance().updateFriendLW(nameList);
}

void ResHandler::refreshfriendSignature()
{
    uint uisize = pdu->uiMsgLen / 32;
    QStringList SignatureList;
    char catmp[32] = {'\0'};
    qDebug() << "********好友Signature列表********";

    for(uint i = 0; i < uisize; i++)
    {
        memcpy(catmp, pdu->caMsg + i * 32, 32);
        SignatureList.append(QString(catmp));
        qDebug() << QString(catmp);
    }

    this->Signature = SignatureList;
    qDebug() << "************************";
}

void ResHandler::chatRequest()
{
    char cachatname[32] = {'\0'};
    memcpy(cachatname, pdu->caData, 32);

    if(Index::getInstance().whoLBtxt() != "好友：" + QString(cachatname))
    {
        Index::getInstance().setLBtxt("好友：" + QString(cachatname));
    }

    Index::getInstance().settarname(QString(cachatname));
    Index::getInstance().updateShowContent(QString(QDateTime::currentDateTime()
                                           .toString("yyyy.MM.dd hh:mm:ss ddd\n")) +
                                           QString("%1:%2").arg(cachatname).arg((char*)pdu->caMsg));
}

PDU* ResHandler::chatImgRequest()
{
    PDU* respdu = mkpdu(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_CHAT_IMG_AGREE_REQUEST;
    bool res = false;

    if (m_bUpload)
    {
        qDebug() << "uploadFile m_bUpload";
        memcpy(respdu->caData, &res, sizeof(bool));
        return respdu;
    }

    char tarname[32];
    memcpy(tarname, pdu->caMsg, 32);
    Index::getInstance().settarname(QString(tarname));
    char caFileName[32] = {'\0'};
    qint64 fileSize = 0;
    memcpy(caFileName, pdu->caData, 32);
    memcpy(&fileSize, pdu->caData + 32, sizeof(qint64));
    qDebug() << Index::getInstance().gettarname();
    QString strtarnamepath =  QString("%1/%2/%3/%4").arg(Client::getInstance().getRootPath())
                              .arg(Client::getInstance().m_stringLoginName)
                              .arg(Index::getInstance().gettarname())
                              .arg(caFileName);

    m_fUploadFile.setFileName(strtarnamepath);

    if(m_fUploadFile.exists())
    {
        QString tarname = Index::getInstance().gettarname();

        if(Index::getInstance().whoLBtxt() != "好友：" + tarname)
        {
            Index::getInstance().setLBtxt("好友：" + tarname);
        }

        Index::getInstance().showPicture(m_fUploadFile.fileName(), Index::getInstance().gettarname());
        return NULL;
    }

    qDebug() << m_fUploadFile.fileName();

    if (m_fUploadFile.open(QIODevice::WriteOnly))
    {
        m_bUpload = true;
        m_iUploadReceived = 0;
        m_iUploadTotalSize = fileSize;
        res = true;
    }

    memcpy(respdu->caData, &res, sizeof(bool));
    qDebug() << res;
    memcpy(respdu->caData + 32, Index::getInstance().gettarname().toStdString().c_str(), 32);
    qDebug() << Index::getInstance().gettarname();
    Client::getInstance().sendPDU(respdu);
    return NULL;
}

void ResHandler::chatImgAgreeRes()
{
    bool ret;
    memcpy(&ret, pdu->caData, sizeof(bool));

    if(ret)
    {
        Index::getInstance().uploadImgData();
    }
    else
    {
        QMessageBox::information(&Client::getInstance(), "Upload File Request", "Fail");
    }

    return;
}

PDU* ResHandler::chatImgData()
{
    m_fUploadFile.write(pdu->caMsg, pdu->uiMsgLen);
    m_iUploadReceived += pdu->uiMsgLen;

    if(m_iUploadReceived < m_iUploadTotalSize)
    {
        return NULL;
    }

    m_fUploadFile.close();
    QString tarname = Index::getInstance().gettarname();

    if(Index::getInstance().whoLBtxt() != "好友：" + tarname)
    {
        Index::getInstance().setLBtxt("好友：" + tarname);
    }

    Index::getInstance().showPicture(m_fUploadFile.fileName(), Index::getInstance().gettarname());
    m_bUpload = false;
    PDU* respdu = mkpdu(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_CHAT_IMG_DATA_RES_RESPOND;
    bool res = m_iUploadReceived == m_iUploadTotalSize;
    memcpy(respdu->caData, &res, sizeof(bool));
    return respdu;
}
