#include "move.h"

Move::Move(Ball *ball, Holder *holder, QObject *parent) :
    QObject(parent)
{
    move_type = BALL_PASS;
    item_moved = ball;

//    from_pos = ball->getPosition();
//    to_pos = holder->getPosition();

    from_row = ball->getRow();
    from_column = ball->getColumn();
    to_row = holder->getRow();
    to_column = holder->getColumn();
}

Move::Move(Holder *holder, Field *field, QObject *parent) :
    QObject(parent)
{
    move_type = HOLDER_MOVE;
    item_moved = holder;

//    from_pos = holder->getPosition();
//    to_pos = field->getPosition();

    from_row = holder->getRow();
    from_column = holder->getColumn();
    to_row = field->getRow();
    to_column = field->getColumn();
}

Move::Move(Move *move)
{
    item_moved = move->getItemMoved();
    move_type = move->getType();

//    from_pos = move->getFromPosition();
//    to_pos = move->getToPosition();

    from_row = move->getFromRow();
    from_column = move->getFromColumn();

    to_row = move->getToRow();
    to_column = move->getToColumn();
}

/** odwraca ruch - tzn. zamienia miejsce docelowe ze zrodlowym */
void Move::revert_move()
{
    qSwap(from_row, to_row);
    qSwap(from_column, to_column);
}

QDataStream & operator<<(QDataStream &out, const Move &move)
{
    qDebug() << "Move::operator<<";
    out << (quint32 &)move.move_type;

    Ball *b;
    Holder *h;
    switch(move.move_type)
    {
    case BALL_PASS:
        b = dynamic_cast<Ball *>(move.item_moved);
        out << *b;
        break;
    case HOLDER_MOVE:
        h = dynamic_cast<Holder *>(move.item_moved);
        out << *h;
        break;
    }

    out << move.from_row << move.from_column
        << move.to_row << move.to_column;
    return out;
}

QDataStream & operator>>(QDataStream &in, Move &move)
{
    qDebug() << "Move::operator>>";
    in >> (quint32 &)move.move_type;
    qDebug() << move.move_type;
    Ball *b = NULL;
    Holder *h = NULL;
    switch(move.move_type)
    {
    case BALL_PASS:
        b = new Ball(WHITE_PLAYER, 0, 0);
        in >> *b;
        move.item_moved = b;
        break;
    case HOLDER_MOVE:
        h = new Holder(WHITE_PLAYER, 0, 0);
        in >> *h;
        move.item_moved = h;
        break;
    }

    in >> move.from_row >> move.from_column
       >> move.to_row >> move.to_column;
    return in;
}
