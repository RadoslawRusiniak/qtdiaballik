#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "ai.h"
#include "board.h"
#include "gamestate.h"
//#include "mainwindow.h"

class Game : public QObject
{
    friend QDataStream & operator<<(QDataStream &stream, const Game &game);
    friend QDataStream & operator>>(QDataStream &in, Game &game);

Q_OBJECT
public:
    explicit Game(QList<PlayerHumanCPU> *players, QObject *parent = 0);
    explicit Game(Board *board, GameState *state, QObject *parent = 0);

    Board * getBoard() const { return board; }

    PlayerTeam getWhoIsMoving() const { return state->getWhoIsMoving(); }
    PlayerTeam getOpponentTeam(PlayerTeam team) const { return state->getOpponentTeam(team); }
    int getCurrentTurnNr() const { return state->getCurrentTurnNr(); }
    int getHolderMovesMade() const { return state->getHolderMovesMade(); }
    int getPassesMade() const { return state->getPassesMade(); }

    bool isTeamInWinningPosition(PlayerTeam act_team) const;

private:
    AI *ai;
    Board *board;
    GameState *state;

    QList<PlayerHumanCPU> *players;
    Holder *holder_pressed;
    Ball *ball_pressed;

    bool game_ended;
    bool in_edit_mode;

//    bool isMoveValid(Holder *h, Field *f);
//    bool isMoveValid(Ball *b, Holder *h);

    void makeConnections();

    void undoTurn();
    void redoTurn();

signals:
    void signalMoveMade(Move *move);

    void signalGameEnded(PlayerTeam winner, bool unfair_game_rule);

    void signalEditStateEntered();
    void signalEditMoveMade(Move *move);
    void signalChangePlayer();
    void signalEditStateReturn();

private slots:
    void slotReactOnPress(Ball *ball_pointed);
    void slotReactOnPress(Holder *holder_pointed);
    void slotReactOnPress(Field *field_pointed);

public slots:
    bool slotIsMoveAllowed(Move *move);

    bool slotIsGameEnded();

    void slotMovesConfirmed();
    void slotMoveUndone();
    void slotMoveRedone();
    void slotTurnUndone();
    void slotTurnRedone();
    void slotGenerateAIMove();

    void slotEnterEditMode();
    void slotReturnFromEditMode();
};


#endif // GAME_H
