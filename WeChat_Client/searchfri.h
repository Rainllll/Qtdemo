#ifndef SEARCHFRI_H
#define SEARCHFRI_H

#include <QWidget>

namespace Ui
{
class SearchFri;
}

class SearchFri : public QWidget
{
    Q_OBJECT

public:
    static SearchFri& getInstance();
    ~SearchFri();
signals:
    void sendData(QString);   //用来传递数据的信号
private slots:
    void on_surePB_clicked();

private:
    Ui::SearchFri *ui;
    explicit SearchFri(QWidget *parent = nullptr);
    SearchFri(const SearchFri& instance) = delete;
    SearchFri& operator=(const SearchFri&) = delete;
};

#endif // SEARCHFRI_H
