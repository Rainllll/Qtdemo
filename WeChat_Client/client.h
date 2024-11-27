#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QListWidget>
#include <QTextBrowser>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTcpSocket>
#include "protocol.h"
#include "reshandler.h"
QT_BEGIN_NAMESPACE
namespace Ui
{
class Client;
}

QT_END_NAMESPACE

class Client : public QMainWindow
{
    Q_OBJECT

public:
    QString ip;
    quint16 port;
    QString m_rootPath;
    QTcpSocket m_tcpSocket;
    QByteArray buff;
    ResHandler *rh;
    QString m_stringLoginName;
    ~Client();

    QTcpSocket& getTcpSocket();
    void loadConfig();
    static Client& getInstance();
    PDU* readPDU();
    void handleRes(PDU *pdu);
    void sendPDU(PDU *pdu);
    QString getRootPath();
public slots:
    void recvmsg();
    void showConnect();

private slots:
    void on_login_clicked();

    void on_register_2_clicked();

private:
    static Client instance;
    Ui::Client *ui;
    Client(QWidget *parent = nullptr);
    Client(const Client& instance) = delete;
    Client& operator=(const Client&) = delete;
};

#endif // CLIENT_H
