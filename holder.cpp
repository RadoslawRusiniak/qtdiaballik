#include "holder.h"

Holder::Holder(PlayerTeam team, int row, int nr, QWidget *parent)
{
    this->team = team;
    this->nr = nr;

    this->row = row;
    this->column = nr;

//    board_pos = new QPoint(row, column);

    moveTo(row, column);

    setFlag(QGraphicsItem::ItemIsSelectable);
    setSelected(false);
}

QRectF Holder::boundingRect() const
{
    return QRectF(QPoint(posx + FIELD_WIDTH / 2 - FIELD_WIDTH / 3, posy + FIELD_HEIGHT / 2 - FIELD_HEIGHT / 3),
                  QPoint(posx + FIELD_WIDTH / 2 + FIELD_WIDTH / 3, posy + FIELD_HEIGHT / 2 + FIELD_HEIGHT / 3));
}

void Holder::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    qDebug() << "Holder: " << posx << " " << posy << " " << boundingRect();

    QBrush brush(Qt::black);
/*    if(isSelected())
    {
        brush.setColor(Qt::yellow);
    } else */if(team)
    {
        brush.setColor(Qt::black);
    } else
    {
        brush.setColor(Qt::white);
    }

    QPainterPath path;
    path.addRoundedRect(boundingRect(), 5, 5);
//    painter->setRenderHint(QPainter::Antialiasing);
//    QPen pen;
//    pen.setWidth(3);
//    painter->setPen(pen);

    painter->fillPath(path, brush);

    painter->drawPath(path);
}

/** zmienia polozenie zawodnika na rzad i kolumne podane w parametrach */
void Holder::moveTo(int row, int column)
{
    this->row = row;
    this->column = column;

    posx = column * (FIELD_WIDTH / 2);
    posy = row * (FIELD_HEIGHT / 2);

    setPos(posx, posy);
}

QDataStream &operator<<(QDataStream &out, const Holder &holder)
{
    qDebug() << "Holder::operator<<";
    out << (quint32 &)holder.team << holder.nr
        << holder.row << holder.column
        << holder.posx << holder.posy;
    return out;
}

QDataStream &operator>>(QDataStream &in, Holder &holder)
{
    in >> (quint32 &)holder.team >> holder.nr
       >> holder.row >> holder.column
       >> holder.posx >> holder.posy;
    return in;
}

