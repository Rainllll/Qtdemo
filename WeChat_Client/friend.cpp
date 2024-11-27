#include "client.h"
#include "friend.h"
#include "protocol.h"
#include "searchfri.h"
#include "ui_friend.h"
#include "onlineuser.h"
#include <QMessageBox>

Friend::Friend(QWidget *parent) : QWidget(parent), ui(new Ui::Friend)
{
    ui->setupUi(this);
    this->refresh();
}

void Friend::refresh()
{
    PDU* pdu = mkpdu(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_FRIEND_MANAGE_REFRESH_REQUEST;
    memcpy(pdu->caData, Client::getInstance().m_stringLoginName.toStdString().c_str(), 32);
    Client::getInstance().sendPDU(pdu);
}

Friend &Friend::getInstance()
{
    static Friend instance;
    return instance;
}

void Friend::updateFriendLW(QStringList friendList)
{
    ui->friendList->clear();
    ui->friendList->addItems(friendList);
}

Friend::~Friend()
{
    delete ui;
}

void Friend::on_freshFriend_clicked()
{
    refresh();
}

void Friend::on_deleteFriend_clicked()
{
    QListWidgetItem* pItem = ui->friendList->currentItem();

    if(!pItem)
    {
        QMessageBox::information(this, "Delete Friend", "Please select item!");
        return;
    }

    QString strTarName = pItem->text();
    int ret = QMessageBox::question(this, "Delete", QString("Delete your friend '%1'").arg(strTarName));

    if(ret == QMessageBox::Yes)
    {
        PDU* pdu = NULL;
        pdu = mkpdu(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST;
        memcpy(pdu->caData, Client::getInstance().m_stringLoginName.toStdString().c_str(), 32);
        memcpy(pdu->caData + 32, strTarName.toStdString().c_str(), 32);
        Client::getInstance().sendPDU(pdu);
    }
    else
    {
        return;
    }
}

void Friend::on_addFriend_clicked()
{
    SearchFri& dlg = SearchFri::getInstance();

    if(dlg.isHidden())
    {
        dlg.show();
    }
}

void Friend::receiveData(QString data)
{
    addFriendName = data;
    qDebug() << addFriendName;
    QString strcurname = Client::getInstance().m_stringLoginName;
    QString strtarname = this->addFriendName;
    qDebug() << strcurname << " " << strtarname;
    PDU* pdu = mkpdu(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
    memcpy(pdu->caData, strcurname.toStdString().c_str(), 32);
    memcpy(pdu->caData + 32, strtarname.toStdString().c_str(), 32);
    Client::getInstance().sendPDU(pdu);
}

void Friend::on_onlineUser_clicked()
{
    OnlineUser& instance = OnlineUser::getInstance();

    if(instance.isHidden())
    {
        instance.show();
    }

    PDU* pdu = mkpdu(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_ONLINE_USER_REQUEST;
    Client::getInstance().sendPDU(pdu);
}
