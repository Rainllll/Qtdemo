#ifndef INDEX_H
#define INDEX_H

#include <QListWidget>
#include <QWidget>
#include "imghandler.h"
namespace Ui
{
class Index;
}

class Index : public QWidget
{
    Q_OBJECT
public:
    static Index& getInstance();
    ~Index();
    void refresh();
    void getFriend();
    void updateFriendLW(QStringList friendList, QStringList signatureList);
    void updateShowContent(QString strMsg);
    QString imgPathToHtml(const QString &imgPath, int width = -1, int height = -1);
    Ui::Index* getui();
    QString msgbxisv();
    QString whoLBtxt();
    void setLBtxt(QString str);
    void settarname(QString str);
    bool m_bUpload;
    QString m_strCurPath;
    QString m_strUploadFilePath;
    QString gettarname();
    void uploadFile();
    void uploadImgData();
    void showPicture();
    void showPicture(QString fileName, QString who);
    void showChatWidget(QString tarName);
    void AddItem(const QString &qstrFileName, QString iFileSize, const QString &qstrPic);
    void refreshSignature();
private slots:

    void on_sendPB_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_PhotoPB_clicked();

    void on_friendCon_clicked();

private:
    QString m_tarname;
    Index(QWidget *parent = nullptr);
    Ui::Index *ui;
    Index(const Index& instance) = delete;
    Index& operator=(const Index&) = delete;
    //添加Item
    void AddItem(const QString& qstrFileName, int iFileSize, const QString& qstrPic);
};

#endif // INDEX_H
