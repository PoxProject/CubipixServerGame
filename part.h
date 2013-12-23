#ifndef PART_H
#define PART_H

#include "player.h"
#include <QMap>
#include <QVector>
#include <QStringList>
#include <QDebug>

class Part
{
public:
    Part(int partIdN = 0);

    void addPlayer(int id, QString username, int socketRow, int health, int posX, int posY);
    void updateInfoPlayer(int id, int health, int posX, int posY);
    void deletePlayer(int id);

    QStringList getInfoPlayer(int id);
    QList<int> getSocketRowPlayers();
    int numberPlayers();

    int getPartId();

private:
    QMap<QString, int> listPlayers;
    QVector<Player *> *playersClass;

    int partId;
};

#endif // PART_H
