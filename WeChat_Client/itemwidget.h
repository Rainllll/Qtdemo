#ifndef ITEMWIDGET_H
#define ITEMWIDGET_H

#include <QWidget>

namespace Ui
{
class ItemWidget;
}

class ItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ItemWidget(QWidget *parent = nullptr);
    ~ItemWidget();

    void SetData(const QString& qstrFileName, int iFileSize, const QString& qstrPic);

    QString getName();
    void SetData(const QString &qstrFileName, QString iFileSize, const QString &qstrPic);
private:
    Ui::ItemWidget *ui;
};

#endif // ITEMWIDGET_H
