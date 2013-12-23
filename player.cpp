#include "player.h"

Player::Player(int idN, QString usernameN, int socketRowN, int healthN, int posXN, int posYN)
{
    id = idN;
    username = usernameN;
    socketRow = socketRowN;
    health = healthN;
    posX = posXN;
    posY = posYN;
}

int Player::getId()
{
    return id;
}

QString Player::getUsername()
{
    return username;
}

int Player::getSocketRow()
{
    return socketRow;
}

int Player::getHealth()
{
    return health;
}

int Player::getPosX()
{
    return posX;
}

int Player::getPosY()
{
    return posY;
}

void Player::updateHealth(int healthN)
{
    health = healthN;
}

void Player::updatePosX(int posXN)
{
    posX = posXN;
}

void Player::updatePosY(int posYN)
{
    posY = posYN;
}
