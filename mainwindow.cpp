#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Diaballik");

//    interface = new Interface(menuBar(), this);

    createMainMenu();
    createMovesBar();
    createEditorBar();

    current_game = NULL;
    game_played = false;

    in_edit_mode = false;

//    centralWindowOnScreen();
}

MainWindow::~MainWindow()
{
}

/** funkcja pomocnicza, centruje glowne okno na ekranie */

// TODO nie dziala do konca - naprawic lub usunac

void MainWindow::centralWindowOnScreen()
{
    QDesktopWidget *desktop = QApplication::desktop();
    int screenWidth, width;
    int screenHeight, height;
    int x, y;
    QSize windowSize;

    screenWidth = desktop->width();
    screenHeight = desktop->height();

    windowSize = size();
    width = windowSize.width();
    height = windowSize.height();

    x = (screenWidth - width) / 2;
    y = (screenHeight - height) / 2;

    move(x, y);
}

/** pokazuje dialog z wiadomoscia o koncu gry */
void MainWindow::showGameEnd(PlayerTeam winner, bool unfair_game_rule)
{
    QString s("Game ended. %1 player won");
    if(unfair_game_rule)
    {
        s.append(" due to \"unfair game\" rule");
    }
    s.append("!");
    if(winner == WHITE_PLAYER)
    {
        s = s.arg("White");
    } else
    {
        s = s.arg("Black");
    }

    QMessageBox msg_box;
    msg_box.setIcon(QMessageBox::Information);
    msg_box.setText(s);
    msg_box.exec();
}

/** ustawia menu glowne, uzywa funkcji pomocniczych:
    createMainMenuActions() i setMainMenuConnections() */
void MainWindow::createMainMenu()
{
    createMainMenuActions();

    QMenu *file = menuBar()->addMenu("&File");
    file->addAction(new_game_act);
    file->addSeparator();
    file->addAction(load_game_act);
    file->addAction(save_game_act);
    file->addSeparator();
    file->addAction(quit_act);

    QMenu *settings = menuBar()->addMenu("&Settings");

    QMenu *mode_menu = settings->addMenu("Mode");
    mode_menu->addAction(player_player_act);
    mode_menu->addAction(player_computer_act_white);
    mode_menu->addAction(player_computer_act_black);
    mode_menu->addAction(computer_computer_act);

    QMenu *infos = menuBar()->addMenu("&Info");
    infos->addAction(rules_act);
    infos->addMenu(information_act);
    information_act->addAction(edit_info_act);
    information_act->addAction(play_comp_info_act);
    information_act->addAction(about_info_act);

    setMainMenuConnections();\
}

/** funckja pomocnicza dla setMainMenu(),
    ustawia akcje dla glownego menu */
void MainWindow::createMainMenuActions()
{
    new_game_act = new QAction("New Game", this);
    load_game_act = new QAction("Load Game", this);
    save_game_act = new QAction("Save Game", this);
    quit_act = new QAction("&Quit", this);
    new_game_act->setShortcut(tr("CTRL+N"));
    load_game_act->setShortcut(tr("CTRL+L"));
    save_game_act->setShortcut(tr("CTRL+S"));
    quit_act->setShortcut(tr("CTRL+Q"));

    save_game_act->setDisabled(true);

    choose_mode_act = new QActionGroup(this);

    player_player_act = new QAction("Human/Human", this);
//    player_computer_act = new QActionGroup("Human/Computer", this);

    player_computer_act_white = new QAction("Human/Computer - you as whites", this);
    player_computer_act_black = new QAction("Human/Computer - you as blacks", this);

    computer_computer_act = new QAction("Computer/Computer", this);
    choose_mode_act->addAction(player_player_act);
    choose_mode_act->addAction(player_computer_act_white);
    choose_mode_act->addAction(player_computer_act_black);
    choose_mode_act->addAction(computer_computer_act);
    player_player_act->setCheckable(true);
    player_computer_act_white->setCheckable(true);
    player_computer_act_black->setCheckable(true);
    computer_computer_act->setCheckable(true);
    player_player_act->setChecked(true);

    rules_act = new QAction("Rules", this);
    rules_act->setShortcut(tr("CTRL+R"));

    information_act = new QMenu("Information", this);
//    information_act->setShortcut(tr("CTRL+I"));

    edit_info_act = new QAction("Editing game instructions", this);
    play_comp_info_act = new QAction("Different game modes", this);
    about_info_act = new QAction("About program", this);
}

/** funckja pomocnicza dla setMainMenu(),
    ustawia connecty dla actionow z glownego menu */
void MainWindow::setMainMenuConnections()
{
    connect(new_game_act, SIGNAL(triggered()), this, SLOT(createNewGame()));
    connect(load_game_act, SIGNAL(triggered()), this, SLOT(loadGame()));
    connect(save_game_act, SIGNAL(triggered()), this, SLOT(saveGame()));
    connect(quit_act, SIGNAL(triggered()), this, SLOT(closeApp()));

    connect(rules_act, SIGNAL(triggered()), this, SLOT(slotShowRules()));

    connect(edit_info_act, SIGNAL(triggered()), this, SLOT(slotShowEditInfo()));
    connect(play_comp_info_act, SIGNAL(triggered()), this, SLOT(slotShowPlayvsComputerInfo()));
    connect(about_info_act, SIGNAL(triggered()), this, SLOT(slotShowAboutInfo()));
}

/** ustawia menu ruchow po stworzeniu nowej gry */
void MainWindow::createMovesBar()
{
    moves_bar = new QToolBar(tr("&Move"));
    moves_bar->setMovable(false);

    confirm_moves_act = new QAction("Confirm moves", this);
    undo_act = new QAction("Undo move", this);
    redo_act = new QAction("Redo move", this);
    back_turn_act = new QAction("Go back to my previous turn", this);
    forward_turn_act = new QAction("Go forward to my next turn", this);
    hint_act = new QAction("Hint", this);

    moves_bar->addAction(confirm_moves_act);
    moves_bar->addSeparator();
    moves_bar->addAction(undo_act);
    moves_bar->addAction(redo_act);
    moves_bar->addSeparator();
    moves_bar->addAction(back_turn_act);
    moves_bar->addAction(forward_turn_act);
    moves_bar->addSeparator();
    moves_bar->addAction(hint_act);
}

/** ustawia connecty dla menu ruchow
    WAZNE - connecty z gra PRZED connectami z glownym oknem (aktualnosc informacji) */
void MainWindow::setMovesBarConnections()
{
    connect(confirm_moves_act, SIGNAL(triggered()), current_game, SLOT(slotMovesConfirmed()));
    connect(undo_act, SIGNAL(triggered()), current_game, SLOT(slotMoveUndone()));
    connect(redo_act, SIGNAL(triggered()), current_game, SLOT(slotMoveRedone()));
    connect(back_turn_act, SIGNAL(triggered()), current_game, SLOT(slotTurnUndone()));
    connect(forward_turn_act, SIGNAL(triggered()), current_game, SLOT(slotTurnRedone()));
    connect(hint_act, SIGNAL(triggered()), current_game, SLOT(slotGenerateAIMove()));

    setStatusBarConnections();
}

/** disconnectuje menu ruchow - uzywane podczas wejscia w stan edycji */
void MainWindow::unsetMovesBarConnections()
{
    disconnect(confirm_moves_act, SIGNAL(triggered()), current_game, SLOT(slotMovesConfirmed()));
    disconnect(undo_act, SIGNAL(triggered()), current_game, SLOT(slotMoveUndone()));
    disconnect(redo_act, SIGNAL(triggered()), current_game, SLOT(slotMoveRedone()));
    disconnect(back_turn_act, SIGNAL(triggered()), current_game, SLOT(slotTurnUndone()));
    disconnect(forward_turn_act, SIGNAL(triggered()), current_game, SLOT(slotTurnRedone()));
    disconnect(hint_act, SIGNAL(triggered()), current_game, SLOT(slotGenerateAIMove()));
}

/** tworzy menu edycji pokazujac na poczatku tylko przycisk umozliwiajacy wejscie w tryb edycji */
void MainWindow::createEditorBar()
{
    editor_bar = new QToolBar(tr("&Editor"));
    editor_bar->setMovable(false);

    edit_act = new QAction("Edit game", this);

    confirm_edit_act = new QAction("Confirm editing", this);
//    cancel_edit_act = new QAction("Cancel editing", this);
    change_who_moves_act = new QAction("Change moving player", this);

    editor_bar->addAction(edit_act);
    connect(edit_act, SIGNAL(triggered()), this, SLOT(slotEditStateClicked()));
}

/** ustawia connecty dla menu edycji */
void MainWindow::setEditorBarConnections()
{
    connect(confirm_edit_act, SIGNAL(triggered()), this, SLOT(slotEditStateConfirmed()));
    connect(this, SIGNAL(signalReturnFromEditMode()), current_game, SLOT(slotReturnFromEditMode()));
    connect(this, SIGNAL(signalReturnFromEditMode()), this, SLOT(slotUpdateStatusBar()));

    connect(change_who_moves_act, SIGNAL(triggered()), this, SLOT(slotEditStatePlayerChange()));

    connect(this, SIGNAL(signalChangePlayerClicked()), current_game, SIGNAL(signalChangePlayer()));
}

/** tworzy statusBar umieszczajac na nim informacje o tym kto sie rusza, ktora jest tura, ile odbylo sie ruchow w turze */
void MainWindow::createStatusBar()
{
    who_moves = new QLabel();
    turn_nr = new QLabel();
    holder_moves_made = new QLabel();
    passes_made = new QLabel();
    edit_mode_info = new QLabel("EDIT MODE");

    statusBar()->addWidget(turn_nr);
    statusBar()->addWidget(holder_moves_made);
    statusBar()->addWidget(passes_made);
    statusBar()->addPermanentWidget(who_moves);

    statusBar()->setMinimumHeight(50);

    slotUpdateStatusBar();
}

/** ustawia connecty dla statusBar */
void MainWindow::setStatusBarConnections()
{
    connect(confirm_moves_act, SIGNAL(triggered()), this, SLOT(slotUpdateStatusBar()));
    connect(undo_act, SIGNAL(triggered()), this, SLOT(slotUpdateMovesNr()));
    connect(redo_act, SIGNAL(triggered()), this, SLOT(slotUpdateMovesNr()));
    connect(back_turn_act, SIGNAL(triggered()), this, SLOT(slotUpdateTurn()));
    connect(forward_turn_act, SIGNAL(triggered()), this, SLOT(slotUpdateTurn()));

    connect(current_game, SIGNAL(signalMoveMade(Move *)), this, SLOT(slotUpdateMovesNr()));
}

/** ustawia na statusBarze informacje o tym kto sie rusza */
void MainWindow::updateWhoMoves()
{
    QString s("Now moves: %1 (%2)");
    if(current_game->getWhoIsMoving() == WHITE_PLAYER)
    {
        s = s.arg("Whites").arg(white_human_cpu == HUMAN_PLAYER ? "Human" : "CPU");
        who_moves->setText(s);
    } else // current_game->getWhoIsMoving() == BLACK_PLAYER
    {
        s = s.arg("Blacks").arg(black_human_cpu == HUMAN_PLAYER ? "Human" : "CPU");
        who_moves->setText(s);
    }
}

/** zmienia menu edycji w zaleznosci od tego czy okno jest w trybie edycji czy nie */
void MainWindow::changeEditorBar()
{
    if(in_edit_mode)
    {
        editor_bar->removeAction(edit_act);

        editor_bar->addAction(confirm_edit_act);
//        editor_bar->addAction(cancel_edit_act);
        editor_bar->addAction(change_who_moves_act);
    } else
    {
        editor_bar->removeAction(confirm_edit_act);
//        editor_bar->removeAction(cancel_edit_act);
        editor_bar->removeAction(change_who_moves_act);

        editor_bar->addAction(edit_act);
    }
}

/** zmienia statusBar w zaleznosci od tego czy okno jest w trybie edycji czy nie */
void MainWindow::changeStatusBar()
{
    if(in_edit_mode)
    {
        statusBar()->insertWidget(0, edit_mode_info);

    } else
    {
        statusBar()->removeWidget(edit_mode_info);
    }
}

/** tworzy nowa gre dodajac ja do listy gier, ustawia plansze tej gry jako centralny widget,
    inicjuje towrzenie menu ruchow, dostosowuje rozmiar glownego okna */
void MainWindow::createNewGame()
{
    if(current_game)
    {
        delete current_game;
    }

    QList<PlayerHumanCPU> *players = new QList<PlayerHumanCPU>();
    QAction *act = choose_mode_act->checkedAction();
    if(act == player_player_act)
    {
        white_human_cpu = HUMAN_PLAYER;
        black_human_cpu = HUMAN_PLAYER;
        players->append(HUMAN_PLAYER);
        players->append(HUMAN_PLAYER);
    } else if(act == player_computer_act_white)
    {
        white_human_cpu = HUMAN_PLAYER;
        black_human_cpu = CPU_PLAYER;
        players->append(HUMAN_PLAYER);
        players->append(CPU_PLAYER);
    } else if(act == player_computer_act_black)
    {
        white_human_cpu = CPU_PLAYER;
        black_human_cpu = HUMAN_PLAYER;
        players->append(CPU_PLAYER);
        players->append(HUMAN_PLAYER);
    } else if(act == computer_computer_act)
    {
        white_human_cpu = CPU_PLAYER;
        black_human_cpu = CPU_PLAYER;
        players->append(CPU_PLAYER);
        players->append(CPU_PLAYER);
    }

    current_game = new Game(players, this);

//    main_layout->addWidget(game->getBoard());
    setCentralWidget(current_game->getBoard());
    centralWidget()->setMinimumSize(centralWidget()->sizeHint());

    if(!game_played)
    {
        addToolBar(Qt::RightToolBarArea, moves_bar);
        addToolBar(Qt::BottomToolBarArea, editor_bar);
        createStatusBar();

        save_game_act->setDisabled(false);

        game_played = true;
    } else
    {
        slotUpdateStatusBar();
    }

    setMovesBarConnections();
    setEditorBarConnections();

    connect(current_game, SIGNAL(signalGameEnded(PlayerTeam, bool)), this, SLOT(slotGameEnded(PlayerTeam, bool)));

    if(in_edit_mode)
    {
        in_edit_mode = false;
        changeEditorBar();
    }

    resize(sizeHint());
    setFixedSize(size());
}

void MainWindow::loadGame()
{
    QFile file("file.dat");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);    // read the data serialized from the file
//    qDebug() << "MainWindow::loadGame()";
    in >> *current_game;
//    setCentralWidget(current_game->getBoard());
}

void MainWindow::saveGame()
{
    QFile file("file.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);   // we will serialize the data into the file
//    qDebug() << "MainWindow::saveGame()";
    out << *current_game;
}

void MainWindow::closeApp()
{
//    exitGame();
//    saveSettings();
    qApp->quit();
}

/** slot - reaguje na klikniecie polecenia wejscia do trybu edycji
    wyswietla message box z informacje o utracie historii w przypadku przejscia dalej
    i reguje na decyzje uzytkownika */
void MainWindow::slotEditStateClicked()
{
    QMessageBox msg_box;
    msg_box.setIcon(QMessageBox::Question);
    msg_box.setContentsMargins(10, 10, 10, 10);
    msg_box.setText("You are about to enter edit mode.");
    msg_box.setInformativeText("If you continue your history will be lost.\nDo you want to proceed?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg_box.setDefaultButton(QMessageBox::Yes);
    int ret = msg_box.exec();

    switch(ret)
    {
    case QMessageBox::Yes:
        slotGoIntoEditState();
        break;
    case QMessageBox::No:

        break;
    }
}

/** slot - przestawia okno na tryb edycji, uaktualnia menu edycji, statusBar, wylacza menu ruchow */
void MainWindow::slotGoIntoEditState()
{
//    qDebug() << "Edit state";

    in_edit_mode = true;
    changeEditorBar();
    changeStatusBar();

    connect(edit_act, SIGNAL(triggered()), current_game, SLOT(slotEnterEditMode()));

    unsetMovesBarConnections();
}

/** slot - powraca z trybu edycji do normalnego trybu, uaktualnia menu edycji, statusBar, wlacza menu ruchow */
void MainWindow::slotEditStateConfirmed()
{
    disconnect(edit_act, SIGNAL(triggered()), current_game, SLOT(slotEnterEditMode()));

//    PlayerTeam act_team = current_game->getWhoIsMoving();
    if(current_game->slotIsGameEnded())
    {
        QMessageBox msg_box;
        msg_box.setIcon(QMessageBox::Warning);
        msg_box.setContentsMargins(10, 10, 10, 10);
        msg_box.setText("The game is now in ending configuration. Change the position of the ball before carring on.");
        msg_box.setStandardButtons(QMessageBox::Ok);
        msg_box.setDefaultButton(QMessageBox::Ok);
        msg_box.exec();

        return;
    }

    in_edit_mode = false;

    setMovesBarConnections();

    changeEditorBar();
    changeStatusBar();

    emit signalReturnFromEditMode();
}

/** slot - reaguje na polecenie zmiany aktualnego gracza*/
void MainWindow::slotEditStatePlayerChange()
{
    emit signalChangePlayerClicked();
    updateWhoMoves();
}

void MainWindow::slotGameEnded(PlayerTeam winner, bool unfair_game_rule)
{
    showGameEnd(winner, unfair_game_rule);
    unsetMovesBarConnections();
}

/** slot - uaktualnia caly statusBar, reaguje na potwierdzenie ruchow z danej tury */
void MainWindow::slotUpdateStatusBar()
{
    updateWhoMoves();
    slotUpdateTurn();
//    slotUpdateMovesNr(); - teraz wywolywane ;przez updateTurn
}

/** slot - uaktualnia nr tury na statusBarze */
void MainWindow::slotUpdateTurn()
{
    turn_nr->setText(QString("Turn nr: %1").arg(current_game->getCurrentTurnNr()));
    slotUpdateMovesNr();
}

/** slot - uaktualnia liczby ruchow w turze na statusBarze
    w formie slotu, bo reguje na zrobienie ruchu, undo, redo */
void MainWindow::slotUpdateMovesNr()
{
//    qDebug() << "MainWindow::slotUpdateNMovesNr()";
    holder_moves_made->setText(QString("Holder moves made: %1").arg(current_game->getHolderMovesMade()));
    passes_made->setText(QString("Passes made: %1").arg(current_game->getPassesMade()));
}

void MainWindow::slotShowRules()
{
    QString s("Diaballik's rules are simple. Each opponent has a team composed of seven players, one of which has a ball."
              " There is no mechanism to remove players, so there are always 14 players in the game."
              " The goal of the game is to bring a player to the opponent's side with the ball."
              " Opponents take turns and can optionally take three actions during their turn in any order;"
              " two moves and one ball throw. A move is to move one player just one horizontal or vertical space."
              " The player with the ball can throw his ball to another player of his team who is located in the vertical line,"
              " the horizontal line or a diagonal line and if no opposing player is in the way."
              " As in handball, the player with the ball cannot move while he has the ball."
              " So to move, the ball holder must pass to one of his partners."
              " Moves can only be made in the horizontal or vertical direction and only if the destination is unoccupied."
              " Moving diagonally requires two moves, since one must move horizontally, then vertically,"
              " in two stages (as such one cannot move diagonally between two pieces)."
              "\nThere is also rule called \"unfair game\". When player\'s holders form an impassable line"
              " and opponent touches this line with at least 3 holders, player \"with\" impassable line loses automatically."
              "\nRules from: Wikipedia."
                );
    QMessageBox msg_box;
    msg_box.setIcon(QMessageBox::Information);
    msg_box.setContentsMargins(10, 10, 10, 10);
    msg_box.setText(s);
    msg_box.exec();
}

void MainWindow::slotShowEditInfo()
{
    QString s("You can switch to edit mode by clicking \"edit game\" button."
              " On the left side of the status bar \"EDIT MODE\" message should appear."
              " Then, the following actions will be available:"
              "\n-change moving player - click on \"Change moving player\" button,"
              "\n-change ball position - click on the ball and then on the holder you want to put the ball on,"
              "\n-change holder without ball position - click on the holder and then on field you want to put it on,"
              "\n-change holder with ball and ball position - click on the holder with ball"
              " (but NOT on the ball as it will cause \"change ball position\" action) and then on the field you want to put it on."
              "\nWhen you finish editing, just click on \"Confirm editing\" button."
              );
    QMessageBox msg_box;
    msg_box.setIcon(QMessageBox::Information);
    msg_box.setContentsMargins(10, 10, 10, 10);
    msg_box.setText(s);
    msg_box.exec();
}

void MainWindow::slotShowPlayvsComputerInfo()
{
    QString s("Go to Settings Menu and choose mode you want to play."
              " If you select any of the players as CPU you still have to confirm copmuter moves"
              " by clicking on \"Confirm moves\". This gives you also opportunity to \"undo\" and \"redo\" some of computer moves.");
    QMessageBox msg_box;
    msg_box.setIcon(QMessageBox::Information);
    msg_box.setContentsMargins(10, 10, 10, 10);
    msg_box.setText(s);
    msg_box.exec();
}

void MainWindow::slotShowAboutInfo()
{
    QString s("Author: Radosław Rusiniak.");
    QMessageBox msg_box;
    msg_box.setIcon(QMessageBox::Information);
    msg_box.setText(s);
    msg_box.exec();
}
