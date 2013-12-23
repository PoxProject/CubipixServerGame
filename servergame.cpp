#include "servergame.h"
#include "ui_servergame.h"

ServerGame::ServerGame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerGame)
{
    ui->setupUi(this);

    //ui->logServerGame->append("Initialisation des paramètres ...");

    server = new QTcpServer(this);

    if (!server->listen(QHostAddress::Any, 50885))
    {
        ui->logServerGame->setText(tr("<span style=\"color: red;\">Erreur :") + server->errorString() + tr(".</span>"));
    }
    else
    {
        ui->logServerGame->setText(tr("<span style=\"color: green;\">Démarrage réussi sur le port : <strong>") + QString::number(server->serverPort()) + tr("</strong>.</span>"));
        connect(server, SIGNAL(newConnection()), this, SLOT(newPlayer()));
    }

    manager = new QNetworkAccessManager(this);
    partsClass = new QVector<Part *>();

    sizeMessage = 0;

    getListParts();
}

void ServerGame::getListParts()
{
    manager->get(QNetworkRequest(QUrl("http://pox.alwaysdata.net/other/CubipixAPI/getParts.php")));
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(returnListParts(QNetworkReply*)));
}

void ServerGame::returnListParts(QNetworkReply *reply)
{
    ui->logServerGame->append("Recherche de parties ...");

    QByteArray data = reply->readAll();
    QString str(data);

    QStringList returnParts = str.split(";;");

    /*
    listParts->clear();
    partsClass->clear();
    */

    //listParts.clear();

    for(int i = 0; i < returnParts.count(); i++)
    {
        Part *part = new Part(returnParts.value(i).toInt());

        partsClass->append(part);

        listParts.insert(returnParts.value(i), i);

        ui->logServerGame->append("Add part : " + returnParts.value(i) + ".");
    }
}

void ServerGame::newPlayer()
{
    ui->logServerGame->append("Nouvel utilisateur connecté.");

    QTcpSocket *newConnection = server->nextPendingConnection();

    listSocketsPlayers << newConnection;

    connect(newConnection, SIGNAL(disconnected()), this, SLOT(disconnectPlayer()));
    connect(newConnection, SIGNAL(readyRead()), this, SLOT(receiveData()));
}

void ServerGame::disconnectPlayer()
{
    ui->logServerGame->append("Déconnection d'un utilisateur.");
}

void ServerGame::receiveData()
{
    ui->logServerGame->append("Données reçues.");

    qDebug() << "dataReceive";

    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());

    if (socket == 0)
    {
        ui->logServerGame->append("Aucun socket.");
        qDebug() << "no socket";
        return;
    }

    QDataStream in(socket);

    if (sizeMessage == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
        {
            ui->logServerGame->append("Erreur bytesAvailable.");
            qDebug() << "bytesAvailable";
            return;
        }

        in >> sizeMessage;
    }

    if (socket->bytesAvailable() < sizeMessage)
    {
        ui->logServerGame->append("Erreur taille du message.");
        qDebug() << "sizeMessage";
        return;
    }

    QString message;
    in >> message;

    ui->logServerGame->append("Message : " + message + ".");

    qDebug() << message;

    QStringList data = message.split(";;");

    //listParts.insert(4, 5);

    int partRow = listParts[QString(data.value(0))];

    //qDebug() << "if" << partsClass->at(partRow)->getInfoPlayer(data.value(1).toInt()).value(0);

    qDebug() << "test" << partsClass->at(partRow)->getInfoPlayer(data.value(1).toInt()).value(0);

    if(partsClass->at(partRow)->getInfoPlayer(data.value(1).toInt()).value(0) != "NotExist")
    {
        qDebug() << "updateInfoPlayer";

        partsClass->at(partRow)->updateInfoPlayer(data.value(1).toInt(), data.value(3).toInt(), data.value(4).toInt(), data.value(5).toInt());

        ui->logServerGame->append("Mise à jour des informations de :" + data.value(1) + ".");
    }
    else
    {
        qDebug() << "addPlayer";

        int socketRow;

        for(int i = 0; i < listSocketsPlayers.count(); i++)
        {
            if(listSocketsPlayers.at(i) == socket)
            {
                qDebug() << "Position :" << i;

                socketRow = i;
            }
        }

        partsClass->at(partRow)->addPlayer(data.value(1).toInt(), data.value(2), socketRow, data.value(3).toInt(), data.value(4).toInt(), data.value(5).toInt());

        listUsers.insert(QString::number(listUsers.count() + 1), data.value(1).toInt());

        qDebug() << listUsers;

        ui->logServerGame->append("Ajout de :" + data.value(1) + ".");
    }

    sendInformations(data.value(0).toInt());

    sizeMessage = 0;

    qDebug() << "numberPlayers" << partsClass->at(0)->numberPlayers();
}

void ServerGame::sendInformations(int partId)
{
    qDebug() << "sendInformations";

    int partRow = listParts[QString::number(partId)];

    int numberPlayer = partsClass->at(partRow)->numberPlayers();
    QList<int> listSocketsRow = partsClass->at(partRow)->getSocketRowPlayers();

    QString messageToSend;

    messageToSend += QString::number(partsClass->at(partRow)->getPartId()) + ";;" + QString::number(partsClass->at(partRow)->numberPlayers()) + ";;";

    int z = 0;

    for(int i = 0; i < partsClass->at(partRow)->numberPlayers(); i++)
    {
        z = i + 1;

        QStringList infoPlayer = partsClass->at(partRow)->getInfoPlayer(listUsers[QString::number(z)]);

        messageToSend += infoPlayer.value(0) + "||" + infoPlayer.value(1) + "||" + infoPlayer.value(2) + "||" + infoPlayer.value(3) + "||" + infoPlayer.value(4);

        if(z != partsClass->at(partRow)->numberPlayers())
        {
            messageToSend += ";;";
        }
    }

    ui->logServerGame->append("Envoi d'un message :" + messageToSend);

    qDebug() << listSocketsRow;

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    //QString messageToSend = "Test";

    out << (quint16) 0;
    out << messageToSend;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    qDebug() << "beforeSend";

    for(int i = 0; i < listSocketsRow.count(); i++)
    {
        listSocketsPlayers[i]->write(paquet);
    }
}

ServerGame::~ServerGame()
{
    delete ui;
}

void ServerGame::on_pushButton_clicked()
{
    qDebug() << partsClass->at(0)->getInfoPlayer(1);
}
