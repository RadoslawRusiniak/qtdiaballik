#ifndef BALL_H
#define BALL_H

#include <QGraphicsItem>
#include <QPainter>
#include "consts.h"

#include <QDebug>

class Ball : public QGraphicsItem
{
    friend QDataStream & operator<<(QDataStream &out, const Ball &ball);
    friend QDataStream & operator>>(QDataStream &in, Ball &ball);

public:
    explicit Ball(PlayerTeam team, int row, int column, QWidget *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    int type() const { return TYPE_BALL; }
    PlayerTeam getTeam() const { return team; }

    int getRow() const { return row; };
    int getColumn() const { return column; };

//    QPoint * getPosition() const { return board_pos; }

    void moveTo(int row, int column);

private:
    PlayerTeam team;

//    QPoint *board_pos;
//    QPoint *graphical_pos;
    int row;
    int column;

    int posx;
    int posy;

//    Holder *holder;

signals:

public slots:

};

#endif // BALL_H
