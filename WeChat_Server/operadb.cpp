#include "operadb.h"
#include <QDebug>
OperaDB::OperaDB()
{
    mydb = QSqlDatabase::addDatabase("QMYSQL");
}

OperaDB::~OperaDB()
{
    mydb.close();
}

void OperaDB::connect()
{
    mydb.setHostName("localhost");
    mydb.setUserName("root");
    mydb.setPassword("123456");
    mydb.setDatabaseName("server");
    mydb.setPort(3306);
    qDebug() << "\n******数据库连接******";

    if(!mydb.open())
    {
        qDebug() << "未成功链接到数据库";
    }
    else
    {
        qDebug() << "成功链接到数据库";
    }

    qDebug() << "*********************";
}

OperaDB& OperaDB::getinstance()
{
    static OperaDB m;
    return m;
}


bool OperaDB::handleRegist(const char *name, const char *pwd)
{
    if(name == NULL || pwd == NULL)
    {
        return false;
    }

    QString sql = QString("select * from user_info where name = '%1'").arg(name);
    QSqlQuery q;

    if(!q.exec(sql) || q.next())
    {
        return false;
    }

    sql = QString("insert into user_info(name,pwd) values('%1','%2')").arg(name).arg(pwd);
    return q.exec(sql);
}

bool OperaDB::handleLogin(const char *name, const char *pwd)
{
    if(name == NULL || pwd == NULL)
    {
        return false;
        qDebug() << 2;
    }

    QString sql = QString("select * from user_info where name = '%1' and pwd = '%2'").arg(name).arg(pwd);
    QSqlQuery q;

    if(q.exec(sql) && !q.next())
    {
        return false;
        qDebug() << 1;
    }

    sql = QString("update user_info set online = 1 where name = '%1' and pwd = '%2'").arg(name).arg(pwd);
    return q.exec(sql);
}

void OperaDB::handleoffline(const char *name)
{
    if(name == NULL)
    {
        qDebug() << "handleoffline name is null";
        return;
    }

    QString sql = QString("update user_info set online = 0 where name ='%1'").arg(name);

    QSqlQuery q;
    q.exec(sql);
}

int OperaDB::handlesearchuser(const char* name)
{
    if(name == NULL)
    {
        return -1;
    }

    QString data = QString("select online from user_info where name = '%1'").arg(name);
    QSqlQuery q;
    q.exec(data);

    if(q.next())
    {
        return q.value(0).toInt();

    }

    return -1;
}

QStringList OperaDB::getOnlineUser()
{
    QString data = QString("select name from user_info where online = 1");
    QSqlQuery q;
    q.exec(data);
    QStringList result;
    result.clear();

    while(q.next())
    {
        result.append(q.value(0).toString());
        qDebug() << q.value(0).toString();
    }

    return result;
}

int OperaDB::handleAddFriend(const char* cacurname, const char* catarname)
{
    if(cacurname == NULL || catarname == NULL)
    {
        return -1;
    }

    QString sql = QString(R"(
                          select* from friend
                          where
                          (
                          user_id = (select id from user_info where name ='%1')
                          and
                          friend_id = (select id from user_info where name ='%2')
                          )
                          or
                          (
                          friend_id = (select id from user_info where name ='%1')
                          and
                          user_id = (select id from user_info where name ='%2')
                          )
                          )").arg(cacurname).arg(catarname);
    QSqlQuery q;
    q.exec(sql);

    if(q.next())return -2;

    sql = QString("select online from user_info where name = '%1'").arg(catarname);
    q.exec(sql);

    if(q.next())
    {
        return q.value(0).toInt();
    }

    return -1;
}

void OperaDB::handleAddFriendAgree(const char* cacurname, const char* catarname)
{
    if(cacurname == NULL || catarname == NULL)
    {
        return;
    }

    QString sql = QString(R"(
                          insert into friend(user_id,friend_id)
                          select u1.id,u2.id
                          from user_info u1,user_info u2
                          where u1.name='%1' and u2.name = '%2'
                          )").arg(cacurname).arg(catarname);
    QSqlQuery q;
    q.exec(sql);
}

QStringList OperaDB::handleRefreshFriend(QString name)
{
    QStringList result;
    QString sql = QString(R"(
                          select name from user_info
                          where id in
                          (
                          select user_id from friend where friend_id = (select id from user_info where name = '%1')
                          union
                          select friend_id from friend where user_id = (select id from user_info where name = '%1')
                          )
                          )").arg(name);
    QSqlQuery q;
    q.exec(sql);

    while(q.next())
    {
        result.append(q.value(0).toString());
    }

    return result;
}

QMap<QString, QString> OperaDB::handleRefreshFriendSignature(QString name)
{
    qDebug() << "handleRefreshFriendSignature";

    QString sql = QString(R"(
                          select name,signature from user_info
                          where id in
                          (
                          select user_id from friend where friend_id = (select id from user_info where name = '%1')
                          union
                          select friend_id from friend where user_id = (select id from user_info where name = '%1')
                          )
                          )").arg(name);
    QSqlQuery q;
    q.exec(sql);
    QMap<QString, QString> m;

    while(q.next())
    {
        QString key = q.value(0).toString();
        QString T =  q.value(1).toString();
        m[key] = T;
        qDebug() << key << " " << T;
    }

    return m;
}

int OperaDB::handleDeleteFriend(QString curName, QString tarName)
{
    if(curName == NULL || tarName == NULL)return 0;

    QString sql = QString(R"(
                          select* from friend
                          where
                          (
                          user_id = (select id from user_info where name ='%1')
                          and
                          friend_id = (select id from user_info where name ='%2')
                          )
                          or
                          (
                          friend_id = (select id from user_info where name ='%1')
                          and
                          user_id = (select id from user_info where name ='%2')
                          )
                          )").arg(curName).arg(tarName);
    QSqlQuery q;
    q.exec(sql);

    if(!q.next())
    {
        return -1;
    }

    sql = QString(R"(
                  delete from friend
                  where
                  (
                  user_id = (select id from user_info where name ='%1')
                  and
                  friend_id = (select id from user_info where name ='%2')
                  )
                  or
                  (
                  friend_id = (select id from user_info where name ='%1')
                  and
                  user_id = (select id from user_info where name ='%2')
                  )
                  )").arg(curName).arg(tarName);
    return q.exec(sql);
}
