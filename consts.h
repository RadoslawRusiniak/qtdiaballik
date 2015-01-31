#ifndef CONSTS_H
#define CONSTS_H

#include <QGraphicsItem>

const int ROW_SIZE = 7;
const int COLUMN_SIZE = 7;

const int TEAMS_NR = 2;
const int HOLDERS_IN_TEAM = ROW_SIZE;

const int FIELD_WIDTH = 60;
const int FIELD_HEIGHT = 60;

/** rysowanie itemow, zwracanie boundingRectow na QGraphicsScene */
const int GRAPHICAL_MULTIPLIER_POSX = FIELD_WIDTH / 2;
const int GRAPHICAL_MULTIPLIER_POSY = FIELD_HEIGHT / 2;

/** uzywane w turn do sprawdzenia czy mozna jeszcze zrobic dany ruch
    i w Game by cofnac/ponowic cala runde */
const int MAX_MOVES_IN_ROUND = 3;
const int MAX_HOLDER_MOVES_IN_ROUND = 2;
const int MAX_PASSES_IN_ROUND = 1;

/** ilu przeciwnikow musi przylegac do nieprzekraczalnej linii */
const int OPP_FOR_IMP_LINE = 3;

enum MoveType { BALL_PASS = 0,
                HOLDER_MOVE = 1 };

enum PlayerTeam { WHITE_PLAYER = 0,
                  BLACK_PLAYER = 1 };

enum PlayerHumanCPU { HUMAN_PLAYER, CPU_PLAYER };

/** do poznawania co zostalo nacisniete i dostosowywanie reakcji do tego */
enum ItemType { TYPE_BALL = QGraphicsItem::UserType + 1,
                TYPE_HOLDER = QGraphicsItem::UserType + 2,
                TYPE_FIELD = QGraphicsItem::UserType + 3 };

#endif // CONSTS_H
