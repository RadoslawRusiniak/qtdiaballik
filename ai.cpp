#include "ai.h"

AI::AI(QObject *parent) :
    QObject(parent)
{
//    this->parent = dynamic_cast<Game *>(parent);
}

Move * AI::findBestMove(QList<Move *> *moves, PlayerTeam who_moves) const
{
    Move *best_move = NULL;
    int best_move_res = 0;
    int cur_res;

    for(int i = 0; i < moves->size(); ++i)
    {
        cur_res = calculateMove(moves->at(i), who_moves);
        if(cur_res > best_move_res)
        {
            best_move = moves->at(i);
            best_move_res = cur_res;
        }
    }

    return best_move;
}

/** zwraca ocene sytuacji w postaci inta - im wyzszy tym lepiej */
int AI::calculateSituation(QList<Holder *> *holders_list, Ball *ball,
                           int starting_row) const
{
    int res = 0;
    foreach(Holder *holder, *holders_list)
    {
        res += qAbs(holder->getRow() -  starting_row);
    }

    res += qAbs(ball->getRow() - starting_row);

    return res;
}

/** zwraca ocene ruchu w postaci inta - im wyzszy tym lepiej */
int AI::calculateMove(Move *move, PlayerTeam team) const
{
    int res = 0;
    res = (move->getToRow() - move->getFromRow()) * teamFactor(team);
//    res += qAbs(move->getFromColumn() - move->getToColumn());
    res += moveTypeFactor(move);
    res += qrand() % 3;
    return res;
}

int AI::teamFactor(PlayerTeam team) const
{
    int res;
    if(team == WHITE_PLAYER)
    {
        res = -1;
    } else
    {
        res = 1;
    }
    return res;
}

int AI::moveTypeFactor(Move *move) const
{
    int res;
    switch(move->getType())
    {
    case BALL_PASS:
        res = 3;
        break;
    case HOLDER_MOVE:
        res = 2;
        break;
    }
    return res;
}
