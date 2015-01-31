#ifndef HOLDER_H
#define HOLDER_H

//#include <stdlib.h>
#include <QGraphicsItem>
#include <QPainter>
#include "consts.h"

#include <QDebug>

class Holder : public QGraphicsItem
{
    friend QDataStream & operator<<(QDataStream &out, const Holder &holder);
    friend QDataStream & operator>>(QDataStream &in, Holder &holder);

public:
    explicit Holder(PlayerTeam team, int row, int nr, QWidget *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    int type() const { return TYPE_HOLDER; }

    PlayerTeam getTeam() const { return team; }

    int getRow() const { return row; }
    int getColumn() const { return column; }

    void moveTo(int row, int column);

//    QPoint * getPosition() const { return board_pos; }

private:
    PlayerTeam team;
    int nr;

//    QPoint *board_pos;
    int row;
    int column;

    int posx;
    int posy;

signals:

public slots:

};

#endif // HOLDER_H
