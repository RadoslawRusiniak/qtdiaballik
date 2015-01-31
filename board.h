#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsView>
#include <QMouseEvent>
#include "consts.h"
#include "ball.h"
#include "holder.h"
#include "field.h"
#include "move.h"
#include "turn.h"

class Board : public QGraphicsView
{
    friend QDataStream &operator<<(QDataStream &out, const Board &board);
    friend QDataStream &operator>>(QDataStream &in, Board &board);

Q_OBJECT
public:
    explicit Board(QObject *parent = 0);
    explicit Board(/*QGraphicsScene *scene,*/
                   QList<QList<Field *> > *fields,
                   QList<QList<Holder *> > *holders,
                   QList<Ball *> *balls,
                   QList<Holder *> *holders_with_balls,
                   QObject *parent = 0);

    QGraphicsScene * getScene() const { return scene; }

    Holder * getHolderWithBall(PlayerTeam team) const { return holders_with_balls->at(team); }
    void setHolderWithBall(PlayerTeam team, Holder *h) { holders_with_balls->replace(team, h); }

    Ball * getBall(PlayerTeam team) const { return balls->at(team); }

    Holder * getHolderAt(int row, int column) const;

    int getStartingRow(PlayerTeam team) const { return (team == WHITE_PLAYER) ? COLUMN_SIZE - 1 : 0; }

    bool isPassAllowed(int from_row, int from_column, int to_row, int to_column, PlayerTeam who_moves) const;
    bool isHolderMoveAllowed(Holder *holder_moved, int to_row, int to_column, PlayerTeam who_moves) const;

    bool isImpassableLine(PlayerTeam team);

    QList<Move *> * getMovesList(PlayerTeam who_moves);
    QList<Move *> * returnPossibleHolderMoves(PlayerTeam who_moves);
    QList<Move *> * returnPossibleBallMoves(PlayerTeam who_moves);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
//    QObject *parent;

    QGraphicsScene *scene;

    QList<QList<Field *> > *fields;
    QList<QList<Holder *> > *holders;
    QList<Ball *> *balls;
    QList<Holder *> *holders_with_balls;

    void createArena();
    void createTeams();
    void createTeam(PlayerTeam team);

    void makeBoardConnections(QObject *parent);

    void changeHolderPosition(Holder *holder, Move *move);

    int checkDirection(int line_difference) const;

    bool isPartOfImpassableLine(Holder *holder, int &opp_in_adj, QList<bool> *cols_vis);
    Holder * findAdjHolder(Holder *holder, int search_column) const;

    bool isCorrectRow(int row) const { return row >= 0 && row < COLUMN_SIZE; }
    bool isCorrectColumn(int column) const { return column >= 0 && column < ROW_SIZE; }

signals:
    void signalItemPressed(Ball *ball_pointed);
    void signalItemPressed(Holder *holder_pointed);
    void signalItemPressed(Field *field_pointed);

public slots:
    void slotExecuteMove(Move *move);
    void slotUndoMove(Move * move);
    void slotRedoMove(Move * move);

private slots:

};

#endif // BOARD_H
