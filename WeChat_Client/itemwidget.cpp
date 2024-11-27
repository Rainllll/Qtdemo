#include "itemwidget.h"
#include "ui_itemwidget.h"

ItemWidget::ItemWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ItemWidget)
{
    ui->setupUi(this);
    //ui->label_pic->setScaledContents(true);
    ui->label_pic->resize(100, 100);
    this->resize(340, 90);
}

ItemWidget::~ItemWidget()
{
    delete ui;
}

void ItemWidget::SetData(const QString& qstrFileName, QString iFileSize, const QString& qstrPic)
{
    ui->label_fileName->setText(qstrFileName);
    ui->label_fileSize->setText(iFileSize);

    QPixmap pixmapPic(qstrPic);
    int iWidth = ui->label_pic->width();
    int iHeight = ui->label_pic->height();
    QPixmap pixmapPicFit = pixmapPic.scaled(iWidth, iHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//饱满填充
    ui->label_pic->setPixmap(pixmapPicFit);
    //    QImage *img_mainicon;//主图标显示在右上角lable中
    //    img_mainicon = new QImage; //新建一个image对象
    //    img_mainicon->load(qstrPic); //载入图片到img对象中
    //    img_mainicon->scaled(QSize(100, 100), Qt::KeepAspectRatio); //把图片
    //    ui->label_pic->setScaledContents(true);
    //    ui->label_pic->setPixmap(QPixmap::fromImage(*img_mainicon)); //将图片放入label，使用setPixmap

}

QString ItemWidget::getName()
{
    return this->ui->label_fileName->text();
}
