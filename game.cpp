#include "game.h"

Game::Game(QList<PlayerHumanCPU> *players, QObject *parent) :
    QObject(parent)
{
    ai = new AI(this);
    board = new Board(this);
    state = new GameState(WHITE_PLAYER, this);

    this->players = players;

    ball_pressed = NULL;
    holder_pressed = NULL;

    game_ended = false;
    in_edit_mode = false;

    makeConnections();

    if(players->at(state->getWhoIsMoving()) == CPU_PLAYER)
    {
        for(int i = 0; !game_ended && i < MAX_MOVES_IN_ROUND; ++i)
        {
            slotGenerateAIMove();
        }
    }
}

Game::Game(Board *board, GameState *state, QObject *parent) :
        QObject(parent)
{
   this->board = board;
   this->state = state;

   ball_pressed = NULL;
   holder_pressed = NULL;

   in_edit_mode = false;

   makeConnections();
}

void Game::makeConnections()
{
    /** reakcje gry na przycisniecia itemow graficznych - zaznaczanie, sprawdzanie poprawnosci ew. przesuniec */
    connect(board, SIGNAL(signalItemPressed(Ball *)), this, SLOT(slotReactOnPress(Ball *)));
    connect(board, SIGNAL(signalItemPressed(Holder *)), this, SLOT(slotReactOnPress(Holder *)));
    connect(board, SIGNAL(signalItemPressed(Field *)), this, SLOT(slotReactOnPress(Field *)));

    /** przekazywanie informacji o wykonanych ruchach do Board by moc je odwzorowac graficznie na planszy */
    connect(this, SIGNAL(signalMoveMade(Move *)), board, SLOT(slotExecuteMove(Move *)));
    /** przekazywanie informacji o wykonanych ruchach do GameState by moc je potem dodac do listy wykonanych */
    connect(this, SIGNAL(signalMoveMade(Move *)), state, SLOT(slotMoveMade(Move *)));

    /** sprawdzanie czy gra sie skonczyla po zrobionym ruchu */
    connect(this, SIGNAL(signalMoveMade(Move *)), this, SLOT(slotIsGameEnded()));

    /** obsluga stanu edycji */
    connect(this, SIGNAL(signalEditStateEntered()), state, SLOT(slotResetHistory()));
    connect(this, SIGNAL(signalChangePlayer()), state, SLOT(slotChangeMovingPlayer()));
    connect(this, SIGNAL(signalEditMoveMade(Move*)), board, SLOT(slotExecuteMove(Move*)));
    connect(this, SIGNAL(signalEditStateReturn()), state, SLOT(slotCreateTurnAndToList()));
}

void Game::slotReactOnPress(Ball *ball_pointed)
{
    if((!in_edit_mode) && (ball_pointed->getTeam() != state->getWhoIsMoving()))
    {
        return;
    }

//    this->holder_pressed->setSelected(false);
    holder_pressed = NULL;

    if(ball_pressed == ball_pointed)                // odznaczenie pilki
    {
        ball_pressed->setSelected(false);
//        qDebug() << "Game::slotReactOnPress(Ball *)" << ball_pressed->isSelected();
//        this->ball_pressed->update();
        ball_pressed = NULL;
    } else                                          // zaznaczenie pilki
    {
        ball_pressed = ball_pointed;
        ball_pressed->setSelected(true);
    }
//    qDebug() << "Ball" << ball_pressed;


}

void Game::slotReactOnPress(Holder *holder_pointed)
{
//    qDebug() << "Game::slotReactOnPress(Holder *)";
    if((!in_edit_mode) && (holder_pointed->getTeam() != state->getWhoIsMoving()))
    {
        return;
    }
    
    if(ball_pressed)                                // pilka do przeniesienia na innego zawodnika
    {
        if(ball_pressed->getTeam() == holder_pointed->getTeam())
        {
            Move *move = new Move(ball_pressed, holder_pointed);
            if(in_edit_mode)
            {
                emit signalEditMoveMade(move);
            }else if(slotIsMoveAllowed(move) && state->isMoveAllowed(move))
            {
                emit signalMoveMade(move);
            }
        }
        //        this->ball_pressed->setSelected(false);

    } else if(holder_pointed == holder_pressed)     // odznaczenie zawodnika
    {
//        this->holder_pressed->setSelected(false);
        holder_pressed = NULL;
    } else                                          // zaznaczenie nowego zawdonika
    {
        holder_pressed = holder_pointed;
//        this->holder_pressed->setSelected(true);
    }

    ball_pressed = NULL;
//    qDebug() << "Holder" << holder_pointed->getRow() << holder_pointed->getColumn();
}

void Game::slotReactOnPress(Field *field_pointed)
{
//    qDebug() << "Field" << field_pointed->getRow() << field_pointed->getColumn();
    if(!holder_pressed)
    {
        return;
    }

    /** przesuniecie zawodnika na innego zawodnika */
    if(holder_pressed
       && board->getHolderAt(field_pointed->getRow(), field_pointed->getColumn()))
    {
        holder_pressed = board->getHolderAt(field_pointed->getRow(), field_pointed->getColumn());
        return;
    }

    Move *move = new Move(holder_pressed, field_pointed);
    if(in_edit_mode)
    {
//        qDebug() << "Game::slotReactOnPress(Field *)" << "edit mode";
        emit signalEditMoveMade(move);
        /** sprawdzanie czy nie przeniesiono zawodnika z pilka
                wtedy przeniesienie pilki z nim */
        if(holder_pressed == board->getHolderWithBall(holder_pressed->getTeam()))
        {
            Move *move_ball = new Move(board->getBall(holder_pressed->getTeam()), holder_pressed);
            emit signalEditMoveMade(move_ball);
        }
    } else if(slotIsMoveAllowed(move) && state->isMoveAllowed(move))
    {
        emit signalMoveMade(move);
//        this->holder_pressed->setSelected(false);
    }

    holder_pressed = NULL;
}

/** cofa cala runde */
void Game::undoTurn()
{
    for(int i = 0; i < MAX_MOVES_IN_ROUND; ++i)
    {
        slotMoveUndone();
    }
}

/** ponawia cala runde */
void Game::redoTurn()
{
    for(int i = 0; i < MAX_MOVES_IN_ROUND; ++i)
    {
        slotMoveRedone();
    }
}

/** czy zespol jest w pozycji wygrywajacej */
bool Game::isTeamInWinningPosition(PlayerTeam act_team) const
{
    PlayerTeam opponent = state->getOpponentTeam(act_team);
    if(board->getHolderWithBall(act_team)->getRow() == board->getStartingRow(opponent))
    {
        return true;
    }

    return false;
}

/** sprawdza poprawnosc ruchu zawodnikiem pod wzgledem przesuniecia (tzn. czy nie wiecej o jedno pole w gore, dol, bok) */
/** sprawdza poprawnosc ruchu pilka pod wzgledem przesuniecia (obecnie czy w gore, dol, bok, po skosie) */
bool Game::slotIsMoveAllowed(Move *move)
{
    int from_row = move->getFromRow();
    int from_column = move->getFromColumn();
    int to_row = move->getToRow();
    int to_column = move->getToColumn();
    Holder *holder_moved;

    switch(move->getType())
    {
    case BALL_PASS:
        return board->isPassAllowed(from_row, from_column, to_row, to_column, state->getWhoIsMoving());
        break;

    case HOLDER_MOVE:
        holder_moved = dynamic_cast<Holder *>(move->getItemMoved());
        return board->isHolderMoveAllowed(holder_moved, to_row, to_column, state->getWhoIsMoving());
        break;
    }
}

/** zwraca czy gra sie skonczyla i emituje sygnal o wygraniu aktualnego gracza */
bool Game::slotIsGameEnded()
{
    PlayerTeam act_team = state->getWhoIsMoving();
    PlayerTeam opp = state->getOpponentTeam(act_team);
    if(isTeamInWinningPosition(act_team))
    {
        if(!in_edit_mode)
        {
            game_ended = true;
            emit signalGameEnded(act_team, false);
        }
        return true;
    }

    if(board->isImpassableLine(act_team))
    {
        if(!in_edit_mode)
        {
            game_ended = true;
            emit signalGameEnded(opp, true);
        }
        return true;
    }

    if(board->isImpassableLine(opp))
    {
        if(!in_edit_mode)
        {
            emit signalGameEnded(act_team, true);
        }
        return true;
    }
    return false;
}

/** slot - reaguje na nacisniecie potwierdzenia ruchow w menu ruchow, zmienia ture */
void Game::slotMovesConfirmed()
{
    ball_pressed = NULL;
    holder_pressed = NULL;
    state->slotChangeTurn();
    if(players->at(state->getWhoIsMoving()) == CPU_PLAYER)
    {
        for(int i = 0; !game_ended && i < MAX_MOVES_IN_ROUND; ++i)
        {
            slotGenerateAIMove();
        }
    }
}

/** slot - reaguje na nacisniecie cofniecia ruchu w menu ruchow */
void Game::slotMoveUndone()
{
    Move *move = state->undoMove();
    if(move)
    {
        board->slotUndoMove(move);
//        emit signalMoveUndone(move, state->getWhoIsMoving());
    }
    else
    {
//        qDebug() << "Game::slotMoveUndone()" << "0 moves left in turn";
    }
}

/** slot - reaguje na nacisniecie powtorzenia ruchu w menu ruchow */
void Game::slotMoveRedone()
{
    Move *move = state->redoMove();
    if(move)
    {
        board->slotRedoMove(move);
//        emit signalMoveRedone(move, state->getWhoIsMoving());
    }
}

/** slot - reaguje na nacisniecie cofniecia rundy w menu ruchow */
void Game::slotTurnUndone()
{
//    qDebug() << "Game::slotTurnUndone()";
    if(state->getCurrentTurnNr() < 3)
    {
        return;
    }

    for(int j = 0; j < 2; ++j)
    {
        undoTurn();
        state->backTurn();
    }

//    qDebug() << "Game::slotTurnUndone()" << state->getHolderMovesMade() << state->getHolderMovesMade();
}

/** slot - reaguje na nacisniecie powtorzenia rundy w menu ruchow */
void Game::slotTurnRedone()
{
//    qDebug() << "Game::slotTurnRedone()";
    if(state->getCurrentTurnNr() + 1 >= state->getTurnsDoneNr())
    {
        return;
    }

    for(int j = 0; j < 2; ++j)
    {
        redoTurn();
        state->forwardTurn();
    }
}

void Game::slotGenerateAIMove()
{
    if(state->getHolderMovesMade() + state->getPassesMade() == MAX_MOVES_IN_ROUND)
    {
        return;
    }

    Move *best_move = NULL;
    Move *holder_move;
    Move *ball_move;
    if(state->getHolderMovesMade() < MAX_HOLDER_MOVES_IN_ROUND)
    {
        QList<Move *> *holder_moves = board->returnPossibleHolderMoves(state->getWhoIsMoving());
        holder_move = ai->findBestMove(holder_moves, state->getWhoIsMoving());
        if(holder_move)
        {
            best_move = holder_move;
        }
    }

    if(state->getPassesMade() < MAX_PASSES_IN_ROUND)
    {
        QList<Move *> *ball_moves = board->returnPossibleBallMoves(state->getWhoIsMoving());
        ball_move = ai->findBestMove(ball_moves, state->getWhoIsMoving());
        if(ball_move)
        {
            best_move = ball_move;
        }
    }

    if(state->getHolderMovesMade() < MAX_HOLDER_MOVES_IN_ROUND && state->getPassesMade() < MAX_PASSES_IN_ROUND)
    {
        QList<Move *> *moves = new QList<Move *>();
        if(holder_move)
        {
            moves->append(holder_move);
        }
        if(ball_move)
        {
            moves->append(ball_move);
        }
        best_move = ai->findBestMove(moves, state->getWhoIsMoving());
    }

    if(best_move)
    {
        if(state->isMoveAllowed(best_move))
        {
            emit signalMoveMade(best_move);
        }
    }
}

void Game::slotEnterEditMode()
{
    in_edit_mode = true;
    emit signalEditStateEntered();
}

void Game::slotReturnFromEditMode()
{
    in_edit_mode = false;
    emit signalEditStateReturn();
}

QDataStream &operator<<(QDataStream &out, const Game &game)
{
    out << *(game.board) << *(game.state);
    return out;
}

QDataStream &operator>>(QDataStream &in, Game &game)
{
    in >> *(game.board) >> *(game.state);
    return in;
}
