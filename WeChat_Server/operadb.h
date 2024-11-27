#ifndef OPERADB_H
#define OPERADB_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
class OperaDB : public QObject
{
    Q_OBJECT
public:

    ~OperaDB();
    void connect();
    QSqlDatabase mydb;
    static OperaDB& getinstance();
    bool handleRegist(const char* name, const char* pwd);
    bool handleLogin(const char* name, const char* pwd);
    void handleoffline(const char* name);
    int handlesearchuser(const char* name);
    QStringList getOnlineUser();
    int handleAddFriend(const char *cacurname, const char *catarname);
    void handleAddFriendAgree(const char *cacurname, const char *catarname);
    QStringList handleRefreshFriend(QString name);
    int handleDeleteFriend(QString curName, QString tarname);
    QMap<QString, QString> handleRefreshFriendSignature(QString name);
private:
    OperaDB();
    OperaDB(const  OperaDB& instance) = delete;
    OperaDB& operator = (const OperaDB&) = delete;


};

#endif // OPERADB_H
