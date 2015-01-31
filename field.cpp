#include "field.h"

Field::Field(int row, int column, QWidget *parent)
{
    this->row = row;
    this->column = column;

//    board_pos = new QPoint(row, column);
//    graphical_pos = new QPoint(column * GRAPHICAL_MULTIPLIER_POSX, row * GRAPHICAL_MULTIPLIER_POSY);
    posx = column * (FIELD_WIDTH / 2);
    posy = row * (FIELD_HEIGHT / 2);

    setPos(posx, posy);

    is_holder_on_field = false;
    holder_on_field = NULL;
}

QRectF Field::boundingRect() const
{
    return QRectF(QPoint(posx, posy), QPoint(posx + FIELD_WIDTH, posy + FIELD_HEIGHT));
//    return QRectF(graphical_pos->x(), graphical_pos->y(), graphical_pos->x() + FIELD_WIDTH, graphical_pos->y() + FIELD_HEIGHT);
}

void Field::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

//    qDebug() << "Field: " << posx << " " << posy << " " << boundingRect();

    QBrush brush(QColor(110, 139, 61, 127));

    QPainterPath path;
    path.addRoundedRect(boundingRect(), 15, 15);
//    painter->setRenderHint(QPainter::Antialiasing);
//    QPen pen;
//    pen.setWidth(3);
//    painter->setPen(pen);

    painter->fillPath(path, brush);

    painter->drawPath(path);
}

QDataStream & operator<<(QDataStream &out, const Field &field)
{
    qDebug() << "Field::operator<<";
    out << field.row << field.column
        << field.posx << field.posy
        << field.is_holder_on_field;
    if(field.is_holder_on_field)
    {
       out << *(field.holder_on_field);
    }
    return out;
}

QDataStream & operator>>(QDataStream &in, Field &field)
{
    in >> field.row >> field.column
       >> field.posx >> field.posy
       >> field.is_holder_on_field;
    if(field.is_holder_on_field)
    {
        in >> *(field.holder_on_field);
    }
    return in;
}
