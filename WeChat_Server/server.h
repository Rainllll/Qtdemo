#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class Server;
}

QT_END_NAMESPACE

class Server : public QMainWindow
{
    Q_OBJECT

public:
    void loadConfig();
    static Server& getInstance();
    ~Server();
    QString getRootPath();
private:
    QString ip;
    quint16 port;
    QString m_rootPath;
    Ui::Server *ui;
    Server(QWidget *parent = nullptr);
    Server(const Server& other) = delete;
    Server& operator=(const Server& other) = delete;
};

#endif // MAINWINDOW_H
