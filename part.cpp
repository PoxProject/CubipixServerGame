#include "part.h"

Part::Part(int partIdN)
{
    partId = partIdN;

    playersClass = new QVector<Player *>();
}

void Part::addPlayer(int id, QString username, int socketRow, int health, int posX, int posY)
{
    Player *player = new Player(id, username, socketRow, health, posX, posY);

    playersClass->append(player);

    listPlayers.insert(QString::number(id), playersClass->count());

    qDebug() << listPlayers;
}

void Part::updateInfoPlayer(int id, int health, int posX, int posY)
{
    int i = listPlayers[QString::number(id)] - 1;

    playersClass->at(i)->updateHealth(health);
    playersClass->at(i)->updatePosX(posX);
    playersClass->at(i)->updatePosY(posY);
}

void Part::deletePlayer(int id)
{
    playersClass->remove(listPlayers[QString::number(id)] - 1);

    listPlayers.clear();

    for(int i = 0; i < playersClass->count(); i++)
    {
        listPlayers.insert(QString::number(playersClass->at(i)->getId()), i + 1);
    }
}

QStringList Part::getInfoPlayer(int id)
{
    int i = listPlayers[QString::number(id)];

    qDebug() << i - 1;

    QStringList infoPlayer;

    if(i != NULL)
    {
        infoPlayer << QString::number(playersClass->at(i - 1)->getId())
                   << QString(playersClass->at(i - 1)->getUsername())
                   << QString::number(playersClass->at(i - 1)->getHealth())
                   << QString::number(playersClass->at(i - 1)->getPosX())
                   << QString::number(playersClass->at(i - 1)->getPosY());
    }
    else
    {
        infoPlayer << QString("NotExist");
    }

    return infoPlayer;
}

QList<int> Part::getSocketRowPlayers()
{
    QList<int> listSocketRow;

    for(int i = 0; i < playersClass->count(); i++)
    {
        listSocketRow.append(playersClass->at(i)->getSocketRow());
    }

    return listSocketRow;
}

int Part::numberPlayers()
{
    return playersClass->count();
}

int Part::getPartId()
{
    return partId;
}
