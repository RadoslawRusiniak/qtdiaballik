#ifndef TURN_H
#define TURN_H

#include <QObject>
#include "consts.h"
#include "ball.h"
#include "holder.h"
#include "move.h"

class Turn : public QObject
{
    friend QDataStream & operator<<(QDataStream &out, const Turn &turn);
    friend QDataStream & operator>>(QDataStream &in, Turn &turn);

Q_OBJECT
public:
    explicit Turn(PlayerTeam player, QObject *parent = 0);

    bool isMoveAllowed(Move *move) const;

    Move * undoMove();
    Move * redoMove();

    void updateCounters(Move *move, int add_subtr);

    QList<Move *> * getMoves() const { return moves; }

    int getMovesMade() const { return moves_made; }
    int getHolderMovesMade() const { return holder_moves_made; }
    int getPassesMade() const { return passes_made; }

private:
    PlayerTeam whos_turn;

    QList<Move *> *moves;

    int moves_made;
    int holder_moves_made;
    int passes_made;

signals:

public slots:
    void slotReactOnMoveMade(Move *move);

private slots:

};

#endif // TURN_H
