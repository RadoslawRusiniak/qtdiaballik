#include "gamestate.h"

GameState::GameState(PlayerTeam who_starts, QObject *parent)
    : QObject(parent)
{
    who_to_move = who_starts;
    turns = new QList<Turn *>();

    current_turn_nr = 1;

    current_turn = new Turn(who_to_move, this);
    turns->append(current_turn);

    setGameStateConnections();

//    qDebug() << "GameState::GameState turns->size()" << turns->size();
}

/** sprawdza czy ruch pilka/zawodnikiem jest dozwolony pod wzgledem liczby ruchow w turze (zwracajac odpowiedz klasy Turn) */
bool GameState::isMoveAllowed(Move *move) const
{
    return current_turn->isMoveAllowed(move);
}

/** przesuwa w tyl obecna ture wobec tych na liscie tur */
void GameState::backTurn()
{
//    qDebug() << "GameState::backTurn";
    if(current_turn_nr < 2)
    {
        return;
    }

    unsetGameStateConnections();

    current_turn_nr -= 1;
    current_turn = turns->at(current_turn_nr - 1);

    setGameStateConnections();

//    qDebug() << "GameState::backTurn current_turn_nr" << current_turn_nr << "turns->size()" << turns->size()
//             << "current_turn->getMovesMade()" << current_turn->getMovesMade();
    slotChangeMovingPlayer();
}

/** przesuwa w przod obecna ture wobec tych na liscie tur */
void GameState::forwardTurn()
{
//    qDebug() << "GameState::forwardTurn";
    if(current_turn_nr >= turns->size())
    {
        return;
    }

    unsetGameStateConnections();

    current_turn_nr += 1;
    current_turn = turns->at(current_turn_nr - 1);

    setGameStateConnections();
    slotChangeMovingPlayer();
}

/** connectuje sygnaly dotyczace obecnej tury */
void GameState::setGameStateConnections()
{
    /** przekazywanie informacji o wykonanych ruchach z gry do Turn by moc je dodac do listy wykonanych w Turn */
    connect(this, SIGNAL(signalMoveMade(Move *)), current_turn, SLOT(slotReactOnMoveMade(Move *)));
}

/** disconnectuje sygnaly dotyczace obecnej tury */
void GameState::unsetGameStateConnections()
{
    disconnect(this, SIGNAL(signalMoveMade(Move*)), current_turn, SLOT(slotReactOnMoveMade(Move*)));
}

/** usuwa kolejno tury z listy tur tak aby nie bylo tam juz nieaktualnych tur
    (np. po cofnieciu tury i zrobieniu ruchu) */
void GameState::removeUnactualTurnsFromList()
{
    while(turns->size() > current_turn_nr)
    {
        turns->pop_back();
    }
}

/** zmienia ruszajacego sie na przeciwnika */
void GameState::slotChangeMovingPlayer()
{
    if(who_to_move == WHITE_PLAYER)
    {
        who_to_move = BLACK_PLAYER;
    } else // who_to_move == BLACK_PLAYER;
    {
        who_to_move = WHITE_PLAYER;
    }
}

/** slot - zmienia ture na ture przeciwnika */
void GameState::slotChangeTurn()
{
    slotChangeMovingPlayer();

    removeUnactualTurnsFromList();

    current_turn_nr += 1;

    slotCreateTurnAndToList();
//    qDebug() << "GameState::slotChangeTurn turns_size:" << turns->size();
}

/** slot - reguje na zrobiony ruch, usuwajac nieaktualne tury (np. w sytuacji cofniecie tury, zrobienie ruchu)
    i emituje sygnal o zrobionym ruchu */
void GameState::slotMoveMade(Move *move)
{
    removeUnactualTurnsFromList();
    emit signalMoveMade(move);
//    qDebug() << "GameState::slotMoveMade(...) current_turn->getMovesMade() "<< current_turn->getMovesMade();
}

/** slot - usuwa historie */
void GameState::slotResetHistory()
{
    turns->clear();
}

/** slot - tworzy nowa ture i dodaje ja do listy odpowiednio przestawiajac sygnaly na nowa ture */
void GameState::slotCreateTurnAndToList()
{
    unsetGameStateConnections();

    current_turn = new Turn(who_to_move, this);
    turns->append(current_turn);

    setGameStateConnections();
}

QDataStream & operator<<(QDataStream &out, const GameState &state)
{
    out << (quint32 &)state.who_to_move
        << state.current_turn_nr;

    foreach(Turn *turn, *(state.turns))
    {
        out << *turn;
    }

    return out;
}

QDataStream & operator>>(QDataStream &in, GameState &state)
{
    qDebug() << "GameState::operator>>";
    in >> (quint32 &)state.who_to_move
       >> state.current_turn_nr;

    for(int i = 0; i < state.current_turn_nr; ++i)
    {
        in >> *(state.turns->at(i));
    }

    state.current_turn = state.turns->at(state.current_turn_nr - 1);

    return in;
}
