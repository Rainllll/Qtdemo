#ifndef REQHANDLER_H
#define REQHANDLER_H

#include <QObject>
#include <qfile.h>
#include "protocol.h"
class ReqHandler : public QObject
{
    Q_OBJECT
public:

    PDU* pdu;

    explicit ReqHandler(QObject *parent = nullptr);
    PDU* regist();
    PDU* login(QString &loginname);
    PDU* findUser();
    PDU* onlineUser();
    PDU *addFriend();
    PDU *addFriendAgree();
    void setPDU(PDU *pdu);
    PDU *refreshFriend(QString loginname, int type);
    PDU *deleteFriend();
    PDU *getMsg(QString curname, QString tarname);
    PDU* chat();
    PDU *createDir(PDU *pdu);
    PDU *flushFile();
    PDU *delDir();
    PDU *renameFile();
    PDU *moveFile(PDU *pdu);
    PDU *uploadFile();
    PDU *uploadFileData();
    PDU *shareFile();
    PDU *shareFileAgree();
    PDU *chatImgReq();
    PDU *chatImg();
    PDU *chatImgData();
    PDU *preRefreshFriend(QString loginname);
    PDU *refreshFriendSignature(QString loginname);
private:
    QFile m_fUploadFile;
    qint64 m_iUploadTotalSize;
    qint64 m_iUploadReceived;
    bool m_bUpload;
signals:

};

#endif // REQHANDLER_H
