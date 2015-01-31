#ifndef MOVE_H
#define MOVE_H

#include <QObject>
#include "ball.h"
#include "holder.h"
#include "field.h"

class Move: public QObject
{
    friend QDataStream & operator<<(QDataStream &out, const Move &move);
    friend QDataStream & operator>>(QDataStream &in, Move &move);

Q_OBJECT
public:
    explicit Move(Ball *ball, Holder *holder, QObject *parent = 0);
    explicit Move(Holder *holder, Field *field, QObject *parent = 0);
    explicit Move(Move *move);

    void revert_move();

    MoveType getType() const { return move_type; }
    QGraphicsItem * getItemMoved() const { return item_moved; }

    int getFromRow() const { return from_row; }
    int getFromColumn() const { return from_column; }
    int getToRow() const { return to_row; }
    int getToColumn() const { return to_column; }
//    QPoint * getFromPosition() const { return from_pos; }
//    QPoint * getToPosition() const { return to_pos; }

private:

    MoveType move_type;
    QGraphicsItem *item_moved;

//    QPoint *from_pos;
//    QPoint *to_pos;
    int from_row;
    int from_column;
    int to_row;
    int to_column;

signals:

public slots:

};

#endif // MOVE_H
