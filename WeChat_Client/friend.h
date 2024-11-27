#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>

namespace Ui
{
class Friend;
}

class Friend : public QWidget
{
    Q_OBJECT

public:
    static Friend& getInstance();
    ~Friend();

    void refresh();
    void updateFriendLW(QStringList friendList);
private slots:
    void on_freshFriend_clicked();

    void on_deleteFriend_clicked();

    void on_addFriend_clicked();
    void receiveData(QString data);   //接收传递过来的数据的槽
    void on_onlineUser_clicked();

private:
    Ui::Friend *ui;
    QString addFriendName;
    Friend(QWidget *parent = nullptr);
    Friend(const Friend& instance) = delete;
    Friend& operator=(const Friend&) = delete;
};

#endif // FRIEND_H
