#include "onlineuser.h"
#include "ui_onlineuser.h"

OnlineUser::OnlineUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineUser)
{
    ui->setupUi(this);
}

OnlineUser &OnlineUser::getInstance()
{
    static OnlineUser instance;
    return instance;
}

void OnlineUser::showOnlineUser(QStringList userList)
{
    ui->onlineuserlw->clear();
    ui->onlineuserlw->addItems(userList);
}

OnlineUser::~OnlineUser()
{
    delete ui;
}
