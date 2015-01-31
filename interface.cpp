#include "interface.h"

Interface::Interface(QMenuBar *menuBar, QMainWindow *parent) :
    QWidget(parent)
{
//    setActions();
//
//    QMenu *file = menuBar->addMenu("&File");
//    file->addAction(newgame);
//    file->addAction(exitgame);
//    file->addSeparator();
//    file->addAction(quit);
//
//    QMenu *settings = menuBar->addMenu("&Settings");
//    settings->addAction(board);
//    settings->addAction(jewel);
//    settings->addAction(bonuses);
//
//    QMenu *diff = settings->addMenu("Difficulty");
//    diff->addAction(normal);
//    diff->addAction(extreme);
//
//    QMenu *modemenu = settings->addMenu("Mode");
//    modemenu->addAction(singleplayer);
//    modemenu->addAction(multiplayer);
//    modemenu->addAction(vsComputer);
//
//    QMenu *infos = menuBar->addMenu("&Info");
//    infos->addAction(rules);
//    infos->addAction(information);
//    infos->addSeparator();
//    infos->addAction(highscore);
//    infos->addAction(resethighscore);
}

//void Interface::setActions()
//{
//    newgame = new QAction("New", this);
//    exitgame = new QAction("Exit Game", this);
//    quit = new QAction("&Quit", this);
//    newgame->setShortcut(tr("CTRL+N"));
//    exitgame->setShortcut(tr("CTRL+E"));
//    exitgame->setDisabled(true);
//    quit->setShortcut(tr("CTRL+Q"));
//
//    board = new QAction("Set Board", this);
//    bonuses = new QAction("Set Bonuses' Frequency", this);
//    choosemode = new QAction("Mode", this);
//    difficulty = new QAction("Difficulty", this);
//    jewel = new QAction("Set number of Jewels", this);
//    board->setShortcut(tr("CTRL+B"));
//    bonuses->setShortcut(tr("CTRL+F"));
//    jewel->setShortcut(tr("CTRL+J"));
//
//    normal = new QAction("normal", this);
//    extreme = new QAction("extreme", this);
//    multiplayer = new QAction("multiplayer", this);
//    singleplayer = new QAction("singleplayer", this);
//    vsComputer = new QAction("vs Computer", this);
//    normal->setCheckable(true);
//    normal->setChecked(true);
//    extreme->setCheckable(true);
//    extreme->setChecked(false);
//    multiplayer->setCheckable(true);
//    multiplayer->setChecked(false);
//    singleplayer->setCheckable(true);
//    singleplayer->setChecked(true);
//    vsComputer->setCheckable(true);
//    vsComputer->setChecked(false);
//
//    rules = new QAction("Rules", this);
//    information = new QAction("Information", this);
//    highscore = new QAction("Highscores", this);
//    resethighscore = new QAction("Reset Highscores", this);
//    rules->setShortcut(tr("CTRL+R"));
//    information->setShortcut(tr("CTRL+I"));
//    highscore->setShortcut(tr("CTRL+H"));
//}
