#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>
#include "consts.h"
#include "turn.h"
#include "ball.h"
#include "holder.h"
#include "field.h"

class GameState : public QObject
{
    friend QDataStream & operator<<(QDataStream &out, const GameState &state);
    friend QDataStream & operator>>(QDataStream &in, GameState &state);

Q_OBJECT
public:
    explicit GameState(PlayerTeam who_starts, QObject *parent = 0);

    PlayerTeam getWhoIsMoving() const { return who_to_move; }
    int getCurrentTurnNr() const { return current_turn_nr; }
    int getTurnsDoneNr() const { return turns->size(); }

    bool isMoveAllowed(Move *move) const;

    void backTurn();
    void forwardTurn();

    Move * undoMove() { return current_turn->undoMove(); }
    Move * redoMove() { return current_turn->redoMove(); }

    int getHolderMovesMade() const { return current_turn->getHolderMovesMade(); }
    int getPassesMade() const { return current_turn->getPassesMade(); }

    PlayerTeam getOpponentTeam(PlayerTeam team) const { return (team == WHITE_PLAYER) ? BLACK_PLAYER : WHITE_PLAYER; }

private:
    PlayerTeam who_to_move;
    int current_turn_nr;

    QList<Turn *> *turns;

    Turn *current_turn;

    void setGameStateConnections();
    void unsetGameStateConnections();
    void removeUnactualTurnsFromList();

signals:
    void signalMoveMade(Move *move);

public slots:
    void slotChangeMovingPlayer();
    void slotChangeTurn();
    void slotMoveMade(Move *);

    void slotResetHistory();

    void slotCreateTurnAndToList();

private slots:
};

#endif // GAMESTATE_H
