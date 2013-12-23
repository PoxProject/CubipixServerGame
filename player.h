#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class Player
{
public:
    Player(int idN = 0, QString usernameN = 0, int socketRowN = 0, int healthN = 0, int posXN = 0, int posYN = 0);

    int getId();
    QString getUsername();
    int getSocketRow();
    int getHealth();
    int getPosX();
    int getPosY();

    void updateHealth(int healthN);
    void updatePosX(int posXN);
    void updatePosY(int posYN);

private:
    int id;
    QString username;
    int socketRow;
    int health;

    int posX;
    int posY;
};

#endif // PLAYER_H
