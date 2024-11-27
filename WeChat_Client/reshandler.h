#ifndef RESHANDLER_H
#define RESHANDLER_H
#include <QObject>
#include <qfile.h>
#include "protocol.h"
class ResHandler : public QObject
{
    Q_OBJECT
public:
    PDU* pdu;
    QFile m_fUploadFile;
    qint64 m_iUploadTotalSize;
    qint64 m_iUploadReceived;
    QStringList Signature;
    bool m_bUpload;

    explicit ResHandler(QObject *parent = nullptr);

    void regist();
    void login();
    void refreshfriend();

    void chatRequest();
    PDU* chatImgRequest();
    void chatImgAgreeRes();
    PDU *chatImgData();
    void deleteFriendRes();
    void addFriend();
    void addFriendRequest();
    void addFriendAgree();
    void onlineUser();
    void createFriendDir(QString caname);
    void refreshfriendSignature();
signals:

};

#endif // RESHANDLER_H
