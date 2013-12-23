#ifndef SERVERGAME_H
#define SERVERGAME_H

#include "part.h"
#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMap>

namespace Ui {
class ServerGame;
}

class ServerGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerGame(QWidget *parent = 0);
    ~ServerGame();

    void getListParts();

public slots:
    void returnListParts(QNetworkReply *reply);

    void newPlayer();
    void disconnectPlayer();

    void receiveData();
    void sendInformations(int partId);

    void on_pushButton_clicked();

private:
    Ui::ServerGame *ui;

    QTcpServer *server;
    quint16 sizeMessage;

    QMap<QString, int> listParts;
    QVector<Part *> *partsClass;

    QList<QTcpSocket *> listSocketsPlayers;

    QMap<QString, int> listUsers;

    QNetworkAccessManager *manager;
};

#endif // SERVERGAME_H
