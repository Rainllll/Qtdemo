#include "mytcpserver.h"
#include "server.h"
#include "ui_server.h"
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <qhostaddress.h>

Server::Server(QWidget *parent): QMainWindow(parent), ui(new Ui::Server)
{
    ui->setupUi(this);
    loadConfig();
    MyTcpServer::getInstance().listen(QHostAddress(ip), port);
}

Server::~Server()
{
    delete ui;
}

void Server::loadConfig()
{
    QFile file(":/server.config");

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

}

Server &Server::getInstance()
{
    static Server instance;
    return instance;
}
