#ifndef AI_H
#define AI_H

#include <QObject>
#include "field.h"
#include "move.h"

class AI : public QObject
{
Q_OBJECT
public:
    explicit AI(QObject *parent = 0);

    Move * findBestMove(QList<Move *> *moves, PlayerTeam who_moves) const;

private:
    int calculateSituation(QList<Holder *> *holders_list, Ball *ball,
                           int starting_row) const;
    int calculateMove(Move *move, PlayerTeam team) const;

    int moveTypeFactor(Move *move) const;
    int teamFactor(PlayerTeam team) const;
signals:

public slots:

};

#endif // AI_H
