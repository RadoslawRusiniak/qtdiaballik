#include "turn.h"

Turn::Turn(PlayerTeam player, QObject *parent) :
    QObject(parent)
{
    whos_turn = player;
    moves_made = 0;
    holder_moves_made = 0;
    passes_made = 0;

    moves = new QList<Move *>();
}

/** sprawdza czy ruch pilka/zawodnikiem jest dozwolony pod wzgledem liczby ruchow w turze */
bool Turn::isMoveAllowed(Move *move) const
{
    switch(move->getType())
    {
    case BALL_PASS:
        return (passes_made < MAX_PASSES_IN_ROUND && passes_made < MAX_MOVES_IN_ROUND);
        break;
    case HOLDER_MOVE:
        return (holder_moves_made < MAX_HOLDER_MOVES_IN_ROUND && holder_moves_made < MAX_MOVES_IN_ROUND);
        break;
    }
}

/** cofa ruch, zwracajac go, by potem mogl zostac cofniety graficznie na planszy */
Move * Turn::undoMove()
{
    if(moves_made == 0)
    {
//        qDebug() << "Turn::undoMove" << "0 moves left";
        return NULL;
    }

    Move *ret_move = moves->at(moves_made - 1);
    updateCounters(ret_move, -1);

//    qDebug() << "Turn::undoMove moves_made" << moves_made;
    return ret_move;
}

/** ponawia ruch, zwracajac go, by potem mogl zostac ponowiony graficznie na planszy */
Move * Turn::redoMove()
{
    if(moves->size() == moves_made)
    {
        return NULL;
    }

    Move *ret_move = moves->at(moves_made);
    updateCounters(ret_move, 1);

    return ret_move;
}

/** uaktualnie liczniki, zwiekszajac/zmniejszajac o wartosc 2. argumentu - add_subtr */
void Turn::updateCounters(Move *move, int add_subtr)
{
    moves_made += add_subtr;
    switch(move->getType())
    {
    case BALL_PASS:
        passes_made += add_subtr;
        break;
    case HOLDER_MOVE:
        holder_moves_made += add_subtr;
        break;
    }
}

/** slot - uaktualnia liczbe zrobionych ruchow w danej turze po ruchu pilka/zawodnikiem */
void Turn::slotReactOnMoveMade(Move *move)
{

    /** usuwanie poprzednich ruchow, ktore juz sa nieaktualne po zrobieniu ostatniego
        (inaczej sytuacja ruch-undo-nowyruch-redo) moglaby spowodowalac crash */
    while(moves->size() > moves_made)
    {
      moves->pop_back();
    }
    moves->append(move);

    updateCounters(move, 1);
}

QDataStream & operator<<(QDataStream &out, const Turn &turn)
{
    qDebug() << "Turn::operator<<";
    out << (quint32 &)turn.whos_turn
        << turn.moves_made << turn.holder_moves_made << turn.passes_made;

    foreach(Move *move, *(turn.moves))
    {
        out << *move;
    }

    return out;
}

QDataStream & operator>>(QDataStream &in, Turn &turn)
{
    qDebug() << "Turn::operator>>";
    in >> (quint32 &)turn.whos_turn
        >> turn.moves_made >> turn.holder_moves_made >> turn.passes_made;

    for(int i = 0; i < turn.moves_made; ++i)
    {
        in >> *(turn.moves->at(i));
    }

    return in;
}
