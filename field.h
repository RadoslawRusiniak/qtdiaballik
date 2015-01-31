#ifndef FIELD_H
#define FIELD_H

#include <QGraphicsRectItem>
#include <QPainter>
#include "consts.h"
#include "holder.h"

#include <QDebug>

class Field : public QGraphicsItem
{
    friend QDataStream & operator<<(QDataStream &out, const Field &field);
    friend QDataStream & operator>>(QDataStream &in, Field &field);

public:
    explicit Field(int row, int column, QWidget *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    int type() const { return TYPE_FIELD;};

    int getRow() const { return row; };
    int getColumn() const { return column; };

//    QPoint * getPosition() const { return board_pos; }

    Holder * getHolderOnField() const { return holder_on_field; }
    void setHolderOnField(Holder *holder) { holder_on_field = holder;
                                            is_holder_on_field = (holder != NULL); }

private:
//    QPoint *board_pos;
//    QPoint *graphical_pos;
    int row;
    int column;

    int posx;
    int posy;

    bool is_holder_on_field;
    Holder *holder_on_field;

signals:

public slots:

};

#endif // FIELD_H
