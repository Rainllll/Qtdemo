#ifndef ONLINEUSER_H
#define ONLINEUSER_H

#include <QWidget>

namespace Ui
{
class OnlineUser;
}

class OnlineUser : public QWidget
{
    Q_OBJECT

public:
    static OnlineUser& getInstance();
    ~OnlineUser();

    void showOnlineUser(QStringList userList);
private:
    Ui::OnlineUser *ui;
    explicit OnlineUser(QWidget *parent = nullptr);
    OnlineUser(const OnlineUser& instance) = delete;
    OnlineUser& operator=(const OnlineUser&) = delete;
};

#endif // ONLINEUSER_H
