#include "operadb.h"
#include "server.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OperaDB::getinstance().connect();
    Server::getInstance();
    return a.exec();
}
