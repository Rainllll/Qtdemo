#include "searchfri.h"
#include "ui_searchfri.h"
#include "friend.h"
#include <QDebug>
SearchFri::SearchFri(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchFri)
{
    ui->setupUi(this);
    QObject::connect(this, SIGNAL(sendData(QString)), &Friend::getInstance(), SLOT(receiveData(QString)));
}

SearchFri &SearchFri::getInstance()
{
    static SearchFri instance;
    return instance;
}

SearchFri::~SearchFri()
{
    delete ui;
}

void SearchFri::on_surePB_clicked()
{
    qDebug() << "on_surePB_clicked";
    emit sendData(ui->lineEdit->text());  //获取lineEdit的输入并且传递出去
    this->hide();
}


