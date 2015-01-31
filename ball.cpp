#include "ball.h"

Ball::Ball(PlayerTeam team, int row, int column, QWidget *parent)
{
    this->team = team;

    this->row = row;

//    board_pos = new QPoint();
//    graphical_pos = new QPoint();
//    QPoint graphical_pos;

    moveTo(row, column);

    setZValue(zValue() + 1);

    setFlags(QGraphicsItem::ItemIsSelectable);
    setSelected(false);
}

QRectF Ball::boundingRect() const
{
//    QPoint *ball_point_mid = point_mid;
//    qDebug() << "aaa" << posx << posy;
//    QPoint *point_mid = new QPoint(GRAPHICAL_MULTIPLIER_POSX, GRAPHICAL_MULTIPLIER_POSY);
//    QPoint *point_reach = new QPoint(FIELD_WIDTH / 5, FIELD_HEIGHT / 5);

//    const QPoint *top_left = new QPoint(posx + FIELD_WIDTH / 2 - FIELD_WIDTH / 5, posy + FIELD_HEIGHT / 2 - FIELD_HEIGHT / 5);
//    const QPoint *bottom_right = new QPoint(posy + FIELD_WIDTH / 2 + FIELD_WIDTH / 5, posy + FIELD_HEIGHT / 2 + FIELD_HEIGHT / 5);

//    return QRectF(graphical_pos + point_mid - point_reach,
//                  graphical_pos + point_mid + point_reach);

//    return QRectF(graphical_pos + FIELD_WIDTH / 2 - point_reach,
//                  graphical_pos + FIELD/ + point_reach);

//    return QRectF(QPoint(top_left->x(), top_left->y()), QPoint(bottom_right->x(), bottom_right->y()));

    return QRectF(QPoint(posx + FIELD_WIDTH / 2 - FIELD_WIDTH / 5, posy + FIELD_HEIGHT / 2 - FIELD_HEIGHT / 5),
                  QPoint(posx + FIELD_WIDTH / 2 + FIELD_WIDTH / 5, posy + FIELD_HEIGHT / 2 + FIELD_HEIGHT / 5));
}

void Ball::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);

//    qDebug() << "Ball: " << posx << " " << posy << " " << boundingRect();
    QBrush brush(Qt::black);

//    qDebug() << "Ball::paint(...)" << isSelected();

/*    if(isSelected())
    {
        brush.setColor(Qt::yellow);
    } else */if(team)
    {
        brush.setColor(Qt::green);
    } else
    {
        brush.setColor(Qt::red);
    }


    QPainterPath path;
    path.addEllipse(boundingRect());
//    QPen pen;
//    pen.setWidth(3);
//    painter->setPen(pen);

    painter->fillPath(path, brush);

    painter->drawPath(path);
}

/** zmienia polozenie pilki na rzad i kolumne podane w parametrach */
void Ball::moveTo(int row, int column)
{
//    board_pos->setX(row);
//    board_pos->setY(column);
    this->row = row;
    this->column = column;

//    graphical_pos->setX(board_pos->x() * GRAPHICAL_MULTIPLIER_POSX);
//    graphical_pos->setY(board_pos->y() * GRAPHICAL_MULTIPLIER_POSY);
    posx = column * (FIELD_WIDTH / 2);
    posy = row * (FIELD_HEIGHT / 2);

    setPos(posx, posy);
}

QDataStream &operator<<(QDataStream &out, const Ball &ball)
{
    qDebug() << "Ball::operator<<";
    out << (quint32 &)ball.team
        << ball.row << ball.column
        << ball.posx << ball.posy;
    return out;
}

QDataStream &operator>>(QDataStream &in, Ball &ball)
{
    in >> (quint32 &)ball.team
       >> ball.row >> ball.column
       >> ball.posx >> ball.posy;
    return in;
}
