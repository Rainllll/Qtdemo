#include "client.h"
#include "index.h"
#include "protocol.h"
#include "ui_index.h"
#include "friend.h"
#include "itemwidget.h"
#include <QTextDocument>
#include <QFileDialog>
#include <qmessagebox.h>
#include <QBuffer>
#include <QDateTime>

Index &Index::getInstance()
{
    static Index instance;
    return instance;
}

Index::Index(QWidget *parent) : QWidget(parent), ui(new Ui::Index)
{
    ui->setupUi(this);
    ui->listWidget->setResizeMode(QListView::Adjust);
    //ui->listWidget->setViewMode(QListView::IconMode);
    this->refreshSignature();
    this->refresh();
}

void Index::AddItem(const QString &qstrFileName, QString iFileSize, const QString &qstrPic)
{
    ItemWidget* pItemWidget = new ItemWidget(this);
    pItemWidget->SetData(qstrFileName, iFileSize, qstrPic);
    QListWidgetItem* pItem = new QListWidgetItem();
    pItem->setSizeHint(QSize(150, 150));
    ui->listWidget->addItem(pItem);
    ui->listWidget->setItemWidget(pItem, pItemWidget);

}

Index::~Index()
{

}

void Index::settarname(QString str)
{
    this->m_tarname = str;
}


void Index::refresh()
{
    //qDebug() << "1";
    PDU* pdu = mkpdu(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_REFRESH_REQUEST;
    memcpy(pdu->caData, Client::getInstance().m_stringLoginName.toStdString().c_str(), 32);
    //qDebug() << "2";
    Client::getInstance().sendPDU(pdu);
    //qDebug() << "3";
    //pdu->uiMsgType = ENUM_MSG_TYPE_REFRESH_SIGNATURE_REQUEST;
    //Client::getInstance().sendPDU(pdu);
    //qDebug() << "4";
}

void Index::refreshSignature()
{
    qDebug() << "1";
    PDU* pdu = mkpdu(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_REFRESH_SIGNATURE_REQUEST;
    memcpy(pdu->caData, Client::getInstance().m_stringLoginName.toStdString().c_str(), 32);
    qDebug() << "2";
    Client::getInstance().sendPDU(pdu);
    qDebug() << "3";
}

void Index::updateFriendLW(QStringList friendList, QStringList signatureList)
{
    ui->listWidget->clear();

    for(int i = 0; i < friendList.size(); i++)
    {

        AddItem(friendList[i], signatureList.isEmpty() ? "" : signatureList[i], "D:\\QTProject\\build-WeChat_Client-Desktop_Qt_5_14_2_MinGW_64_bit-Debug\\20240731124556.jpg");
    }

    //ui->listWidget->addItems(friendList);
}

void Index::on_sendPB_clicked()
{
    if(m_tarname == NULL)
    {
        QMessageBox::information(this, "Warning", "请选择好友");
        return;
    }

    QString strmsg = ui->MessageEdit->text();

    if(strmsg == NULL)
    {
        QMessageBox::information(this, "Warning", "消息不能为空");
        return;
    }

    PDU* pdu = mkpdu(strmsg.toStdString().size());
    pdu->uiMsgType = ENUM_MSG_TYPE_CHAT_REQUEST;
    memcpy(pdu->caData, Client::getInstance().m_stringLoginName.toStdString().c_str(), 32);
    memcpy(pdu->caData + 32, m_tarname.toStdString().c_str(), 32);
    memcpy(pdu->caMsg, strmsg.toStdString().c_str(), strmsg.toStdString().size());
    Client::getInstance().sendPDU(pdu);
    updateShowContent(QString(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss ddd\n")) +
                      QString("%1:%2").arg(Client::getInstance().m_stringLoginName).arg(strmsg));
    ui->MessageEdit->clear();
}

void Index::on_listWidget_itemClicked(QListWidgetItem *item)
{
    qDebug() << "on_listWidget_itemClicked";
    QWidget* widget = this->ui->listWidget->itemWidget(item);
    ItemWidget* itemWidget = qobject_cast<ItemWidget*>(widget);

    if (itemWidget)
    {
        QString info = itemWidget->getName();
        qDebug() << "Clicked information: " << info;
        showChatWidget(info);
    }


}

void Index::showChatWidget(QString tarName)
{
    this->ui->whoLB->setText("好友：" + tarName);
    this->m_tarname = tarName;
}

void Index::on_PhotoPB_clicked()
{
    if(m_tarname == NULL)
    {
        QMessageBox::information(this, "Warning", "请选择好友");
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this,
                       tr("图片选择对话框"),
                       "D:",
                       tr("*png *jpg;"));

    if(fileName.isEmpty())return;

    m_strUploadFilePath = fileName;
    /*
    QImage image(fileName);
    image = ImgHandler::getInstance().getResizedImage(fileName);
    QPixmap pixmap = QPixmap::fromImage(image);
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    QString html = QString("<img src='data:image/png;base64,%1' />").arg(QString::fromLatin1(byteArray.toBase64().data()));
    ui->Messagebox->insertHtml(html);

    QByteArray imgBy;
    QBuffer imgBuf(&imgBy);
    image.save(&imgBuf, "png");
    */
    showPicture(fileName, Client::getInstance().m_stringLoginName);

    int index = fileName.lastIndexOf('/');
    QString strFileName = fileName.right(fileName.size() - index - 1);
    qDebug() << strFileName;
    QFile file(fileName);
    qint64 fileSize = file.size();

    PDU* pdu = mkpdu(32);
    pdu->uiMsgType = ENUM_MSG_TYPE_CHAT_IMG_REQUEST;
    qDebug() << strFileName.size() << " " << sizeof(fileSize) << " " << sizeof(this->m_tarname);
    memcpy(pdu->caData, strFileName.toStdString().c_str(), 32);
    memcpy(pdu->caData + 32, &fileSize, sizeof(qint64));
    memcpy(pdu->caData + 48, m_tarname.toStdString().c_str(), 16);
    memcpy(pdu->caMsg, Client::getInstance().m_stringLoginName.toStdString().c_str(), 32);
    char caTarName[32] = {'\0'};
    memcpy(caTarName, pdu->caData + 48, 16);
    qDebug() << m_tarname;
    qDebug() << QString(caTarName);
    Client::getInstance().sendPDU(pdu);
}

void Index::showPicture(QString fileName, QString who)
{
    //图片显示
    QString str = QString(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss ddd\n"))
                  + who + ":\n";
    ui->Messagebox->append(QString(str));
    cv::Mat inputImage = cv::imread(fileName.toStdString().c_str());

    if (inputImage.empty())
    {
        qDebug() << "加载图像失败！";
        return;
    }
    else
    {
        qDebug() << "加载图像成功！";
    }

    // 调整图像大小
    cv::Mat resizedImage = ImgHandler::getInstance().resizeImageTo300(inputImage);

    // 验证返回的图像
    if (!resizedImage.empty())
    {
        qDebug() << "图像已成功调整大小，新的尺寸：" << resizedImage.cols << "x" << resizedImage.rows;
    }
    else
    {
        qDebug() << "resize加载图像失败！";
    }

    std::string html = ImgHandler::getInstance().matToHtml(resizedImage);
    ui->Messagebox->insertHtml(QString::fromStdString(html));
    //保持编辑器在光标最后一行
    QTextCursor cursor = ui->Messagebox->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->Messagebox->setTextCursor(cursor);

    //    QTextCursor cursor = QTextCursor();
    //    cursor.movePosition(QTextCursor::End);
    //    ui->Messagebox->setTextCursor(cursor);
    //    ui->Messagebox->insertHtml(imgPathToHtml(fileName, 200, 200));

}

QString Index::imgPathToHtml(const QString& imgPath, int width, int height)
{
    if (QFile::exists(imgPath))
    {
        QString sizeAttr;

        if (width > 0 && height > 0)
        {
            sizeAttr = QString(" width=\"%1\" height=\"%2\"").arg(width).arg(height);
        }

        return QString("<img src=\"%1\"%2 />").arg(imgPath).arg(sizeAttr);
    }
    else
    {
        return QString();
    }
}

void Index::uploadImgData()
{
    QFile file(this->m_strUploadFilePath);

    if(!(file.open(QIODevice::ReadOnly)))
    {
        QMessageBox::information(this, "Upload File", "Fail To Open File");
    }

    m_bUpload = true;

    qint64 ret = 0;
    PDU* datapdu = mkpdu(PARTIAL_FILE_SIZE);
    datapdu->uiMsgType = ENUM_MSG_TYPE_CHAT_IMG_DATA_REQUEST;

    while(true)
    {
        ret = file.read(datapdu->caMsg, PARTIAL_FILE_SIZE);

        if(ret == 0)
        {
            break;
        }

        if(ret < 0)
        {
            QMessageBox::warning(this, "Upload File", "Fail To Read File");
            break;
        }

        datapdu->uiMsgLen = ret;
        datapdu->uiPDUlen = ret + sizeof(PDU);
        memcpy(datapdu->caData, Index::getInstance().gettarname().toStdString().c_str(), 32);
        Client::getInstance().getTcpSocket().write((char*)datapdu, datapdu->uiPDUlen);
    }

    free(datapdu);
    datapdu = NULL;
    file.close();
    m_bUpload = false;
}

QString Index::gettarname()
{
    return this->m_tarname;
}

Ui::Index *Index::getui()
{
    return this->ui;
}

QString Index::whoLBtxt()
{
    return ui->whoLB->text();
}

void Index::setLBtxt(QString str)
{
    ui->whoLB->setText(str);
    ui->Messagebox->clear();
}

void Index::updateShowContent(QString strMsg)
{
    ui->Messagebox->append(strMsg);
}

void Index::on_friendCon_clicked()
{
    if(Friend::getInstance().isHidden())
    {
        Friend::getInstance().show();
    }
}
