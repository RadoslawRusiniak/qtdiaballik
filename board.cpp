#include "board.h"

Board::Board(QObject *parent)
//    : QGraphicsView(parent)
{
//    this->parent = parent;

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, FIELD_WIDTH * ROW_SIZE, FIELD_HEIGHT * COLUMN_SIZE);

    this->setSceneRect(scene->sceneRect());

    createArena();
    createTeams();

    setScene(scene);
//        this->setViewport(scene);
}

Board::Board(/*QGraphicsScene *scene,*/
             QList<QList<Field *> > *fields,
             QList<QList<Holder *> > *holders,
             QList<Ball *> *balls,
             QList<Holder *> *holders_with_balls,
             QObject *parent)
{
//    this->parent = parent;

//    this->scene = scene;
    this->fields = fields;
    this->holders = holders;
    this->balls = balls;
    this->holders_with_balls = holders_with_balls;

//    setScene(scene);
}

/** tworzy plansze, jej pola */
void Board::createArena()
{
    fields = new QList< QList <Field*> >();
    for(int i = 0; i < ROW_SIZE; ++i)
    {
        QList<Field*> *row = new QList<Field*>();
        for(int j = 0; j < COLUMN_SIZE; ++j)
        {
            Field *f = new Field(i, j, this);
            scene->addItem(f);
            row->append(f);
        }
        fields->append(*row);
    }

}

/** tworzy graficznie zespoly - zawodnikow i pilki */
void Board::createTeams()
{
    holders = new QList< QList <Holder *> >();
    balls = new QList<Ball *>();
    holders_with_balls = new QList<Holder *>();

    createTeam(WHITE_PLAYER);
    createTeam(BLACK_PLAYER);
}

/** tworzy zespol, umieszczajac pilke na srodkowym zawodniku */
void Board::createTeam(PlayerTeam team)
{
    bool with_ball;
    QList<Holder*> *holders_row = new QList<Holder*>();

    int row = (team == WHITE_PLAYER) ? COLUMN_SIZE - 1 : 0;
    for(int j = 0; j < HOLDERS_IN_TEAM; ++j)
    {
        with_ball = (j == HOLDERS_IN_TEAM / 2);
        Holder *h = new Holder(team, row, j, this);
        scene->addItem(h);

        fields->at(row).at(j)->setHolderOnField(h);
        holders_row->append(h);

        if(with_ball)
        {
            Ball *b = new Ball(team, row, j, this);
            scene->addItem(b);
            balls->append(b);

            holders_with_balls->append(h);
        }
    }
    holders->append(*holders_row);
}

void Board::mousePressEvent(QMouseEvent *event)
{
    QGraphicsItem *item = scene->itemAt(event->pos());

    Ball *ball_pointed = NULL;
    Holder *holder_pointed = NULL;
    Field *field_pointed = NULL;

//    qDebug() << event->pos() << this->viewport()->size() << this->viewport()->geometry().topLeft();
    if(item)
    {
        switch(item->type())
        {
        case TYPE_BALL:
            ball_pointed = dynamic_cast<Ball *>(item);
            emit signalItemPressed(ball_pointed);

            break;
        case TYPE_HOLDER:
            holder_pointed = dynamic_cast<Holder *>(item);
            emit signalItemPressed(holder_pointed);

            break;
        case TYPE_FIELD:
            field_pointed = dynamic_cast<Field *>(item);
            emit signalItemPressed(field_pointed);

            break;
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void Board::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
}

Holder * Board::getHolderAt(int row, int column) const
{
    if(row < 0 || row >= COLUMN_SIZE || column < 0 || column >= ROW_SIZE)
    {
        return NULL;
    }
    return fields->at(row).at(column)->getHolderOnField();
}

/** ustawia polom posiadanie zawodnika po ruchu */
void Board::changeHolderPosition(Holder *holder, Move *move)
{
    fields->at(move->getFromRow()).at(move->getFromColumn())->setHolderOnField(NULL);
    fields->at(move->getToRow()).at(move->getToColumn())->setHolderOnField(holder);
}

/** funckja pomocnicza dla sprawdzania poprawnosci ruchu - zwraca kierunek przesuwania sie itemu
    w pionie/poziomie w zaleznosci od podanego w argumencie parametru */
int Board::checkDirection(int diff) const
{
    if(diff == 0)
    {
        return 0;
    } else if(diff > 0)
    {
        return 1;
    } else // diff < 0
    {
        return -1;
    }
}

bool Board::isPassAllowed(int from_row, int from_column, int to_row, int to_column, PlayerTeam who_moves) const
{
    int diff_row = to_row - from_row;
    int diff_column = to_column - from_column;
    int row_dir, col_dir;

    row_dir = checkDirection(diff_row);
    col_dir = checkDirection(diff_column);
    int cur_row, cur_col;

    /** czy podanie jest w pionie/poziomie/po ukosie */
    if(qAbs(diff_row) != 0 && qAbs(diff_column) != 0 && qAbs(diff_row) != qAbs(diff_column))
    {
        return false;
    }

    /** czy na linii podania jest przeciwnik */
    for(cur_row = from_row, cur_col = from_column;
        cur_row != to_row || cur_col != to_column; cur_row += row_dir, cur_col += col_dir)
    {
        Holder *holder = getHolderAt(cur_row, cur_col);
        if(holder)
        {
            if(holder->getTeam() != who_moves)
            {
                return false;
            }
        }
    }
    return true;
}

bool Board::isHolderMoveAllowed(Holder *holder_moved, int to_row, int to_column, PlayerTeam who_moves) const
{
    Holder *holder_with_ball = holders_with_balls->at(who_moves);
    int from_row = holder_moved->getRow();
    int from_column = holder_moved->getColumn();

    /** czy zawodnik jest bez pilki */
    if(holder_moved == holder_with_ball)
    {
        return false;
    }

    /** czy ruch jest o 1 pole w pionie/poziomie */
    if(qAbs(from_row - to_row) + qAbs(from_column - to_column) > 1)
    {
        return false;
    }
    return true;
}

/** sprawdza czy nastapila regula "unfair game"
    uzywa funkcji isPartOfImpassableLine */
bool Board::isImpassableLine(PlayerTeam team)
{
    int opp_in_adj = 0;
    QList<bool> *cols_vis = new QList<bool>();
    for(int i = 0; i < ROW_SIZE; ++i)
    {
        cols_vis->append(false);
    }
    bool imp_line = isPartOfImpassableLine(holders->at(team).at(0), opp_in_adj, cols_vis);

    return (imp_line && opp_in_adj >= OPP_FOR_IMP_LINE);
}

/** funkcja pomocnicza dla sprawdzania "unfair game" sprawdzajaca czy dany holder jest czescia nieprzekraczalnej linii
    sprawdza czy dany holder ma w sasiednich kolumnach holderow z wlasnej druzyny blisko siebie
    i jesli tak to wywoluje dla nich rekrurenycjnie sprawdzanie czy sa czescia i=nieprzekraczalnej linii */
bool Board::isPartOfImpassableLine(Holder *holder, int &opp_in_adj, QList<bool> *cols_vis)
{
    if(cols_vis->at(holder->getColumn()))
    {
        return true;
    }

//    qDebug() << holder->getColumn();

    cols_vis->replace(holder->getColumn(), true);
    Holder *opp_holder;
    Holder *adj;
    bool left_adj = true;
    bool right_adj = true;

    for(int i = -1; i <= 1; i += 2)
    {
        opp_holder = getHolderAt(holder->getRow() + i, holder->getColumn());
        if(opp_holder)
        {
            if(opp_holder->getTeam() != holder->getTeam())
            {
                opp_in_adj += 1;
            }
        }
    }

    if(holder->getColumn() > 0)
    {
        adj = findAdjHolder(holder, holder->getColumn() - 1);
        if(adj)
        {
            left_adj = isPartOfImpassableLine(adj, opp_in_adj, cols_vis);
        } else
        {
            left_adj = false;
        }
    }
    if(holder->getColumn() < ROW_SIZE - 1)
    {
        adj = findAdjHolder(holder, holder->getColumn() + 1);
        if(adj)
        {
            right_adj = isPartOfImpassableLine(adj, opp_in_adj, cols_vis);
        } else
        {
            right_adj = false;
        }
    }

    return (left_adj && right_adj);
}

/** funkcja pomocnicza dla isPartOfImpassableLine
    znajduje holdera bedacego w sasiedztwie holdera z 1. argumentu
    w sasiedniej kolumnie (search_column) i zwraca go jesli jest
    wpp zwraca NULL */
Holder * Board::findAdjHolder(Holder *holder, int search_column) const
{
    Holder *adj;
    for(int i = -1; i <= 1; ++i)
    {
        adj = getHolderAt(holder->getRow() + i, search_column);
        if(adj)
        {
            if(adj->getTeam() == holder->getTeam())
            {
                return adj;
            }
        }
    }

    return NULL;
}

/** zwraca liste ruchow dla zespolu */
QList<Move *> * Board::getMovesList(PlayerTeam who_moves)
{
    QList<Move *> *moves = returnPossibleHolderMoves(who_moves);
    QList<Move *> *ball_moves = returnPossibleBallMoves(who_moves);
    moves->append(*ball_moves);
    return moves;
}

QList<Move *> * Board::returnPossibleHolderMoves(PlayerTeam who_moves)
{
    QList<Move *> *moves = new QList<Move *>();
    Field *move_field;
    int i, j;

    foreach(Holder *holder, (holders->at(who_moves)))
    {
//        qDebug() << "Board::returnPossibleHolderMoves";
        if(holder != holders_with_balls->at(who_moves))
        {
            for(i = -1; i <= 1; ++i)
            {
                for(j = -1; j <= 1; ++j)
                {
//                    qDebug() << "aa" << i << j;
                    if(qAbs(i + j) == 1 && isCorrectRow(holder->getRow() + i) && isCorrectColumn(holder->getColumn() + j))
                    {
                        move_field = fields->at(holder->getRow() + i).at(holder->getColumn() + j);
                        if(!move_field->getHolderOnField())
                        {
                            Move *move = new Move(holder, move_field, this);
                            moves->append(move);
                        }
                    }
                }
            }
        }
    }

    return moves;
}

QList<Move *> * Board::returnPossibleBallMoves(PlayerTeam who_moves)
{
    QList<Move *> *moves = new QList<Move *>();
    Holder *holder_with_ball = holders_with_balls->at(who_moves);
    foreach(Holder *holder, (holders->at(who_moves)))
    {
        if(holder != holder_with_ball)
        {
            if(this->isPassAllowed(holder_with_ball->getRow(), holder_with_ball->getColumn(),
                                   holder->getRow(), holder->getColumn(), who_moves))
            {
                Move *move = new Move(balls->at(who_moves), holder, this);
                moves->append(move);
            }
        }
    }

    return moves;
}

/** slot - przesuwa na planszy pilke/zawodnika i zmienia zawodnika trzymajacego pilke */
void Board::slotExecuteMove(Move *move)
{
    Ball *ball;
    Holder *holder;
//    Field *field;

    switch(move->getType())
    {
    case BALL_PASS:
        ball = dynamic_cast<Ball *>(move->getItemMoved());
//        holder = dynamic_cast<Holder*>(move->getDestinationItem());

        ball->moveTo(move->getToRow(), move->getToColumn());
        setHolderWithBall(ball->getTeam(), getHolderAt(move->getToRow(), move->getToColumn()));
        break;
    case HOLDER_MOVE:
        holder = dynamic_cast<Holder *>(move->getItemMoved());
//        field = dynamic_cast<Field *>(move->getDestinationItem());

        holder->moveTo(move->getToRow(), move->getToColumn());
        changeHolderPosition(holder, move);
        break;
    }
}

void Board::slotUndoMove(Move *move)
{
//    qDebug() << "Board::slotUndoMove(...)";
    Move * rev_move = new Move(move);
    rev_move->revert_move();
    slotExecuteMove(rev_move);
}

void Board::slotRedoMove(Move *move)
{
//    qDebug() << "Board::slotRedoMove(...)";
    slotExecuteMove(move);
}

QDataStream &operator<<(QDataStream &out, const Board &board)
{
    qDebug() << "Board::operator<<";
    foreach(QList<Field *> fields_list, *(board.fields))
    {
        foreach(Field *field, fields_list)
        {
            out << *field;
        }
    }

    foreach(QList<Holder *> holders_list, *(board.holders))
    {
        foreach(Holder *holder, holders_list)
        {
            out << *holder;
        }
    }

    foreach(Ball * ball, *(board.balls))
    {
        out << *ball;
    }

    foreach(Holder * holder, *(board.holders_with_balls))
    {
        out << *holder;
    }

    return out;
}

QDataStream &operator>>(QDataStream &in, Board &board)
{
    qDebug() << "Board::operator>>";
    int i,j;
    for(i = 0; i < ROW_SIZE; ++i)
    {
        for(j = 0; j < COLUMN_SIZE; ++j)
        {
            in >> *(board.fields->at(i).at(j));
        }
    }

    for(i = 0; i < 2; ++i)
    {
        for(j = 0; j < HOLDERS_IN_TEAM; ++j)
        {
            in >> *(board.holders->at(i).at(j));
        }
    }

    for(i = 0; i < 2; ++i)
    {
        in >> *(board.balls->at(i));
    }

    for(i = 0; i < 2; ++i)
    {
        in >> *(board.holders_with_balls->at(i));
    }

    return in;
}
