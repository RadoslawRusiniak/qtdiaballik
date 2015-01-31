#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "consts.h"
#include "interface.h"
#include "game.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool IsInEditMode() const { return in_edit_mode; }

private:
//    Interface *interface;
    bool game_played;
    Game *current_game;

    PlayerHumanCPU white_human_cpu;
    PlayerHumanCPU black_human_cpu;

    void centralWindowOnScreen();

    void showGameEnd(PlayerTeam winner, bool unfair_game_rule);

    /** menu glowne */
    QAction *new_game_act;
    QAction *load_game_act;
    QAction *save_game_act;
    QAction *quit_act;

    QActionGroup *choose_mode_act;
        QAction *player_player_act;
//        QActionGroup *player_computer_act;
            QAction *player_computer_act_white;
            QAction *player_computer_act_black;
        QAction *computer_computer_act;

    QAction *rules_act;
    QMenu *information_act;
        QAction *edit_info_act;
        QAction *play_comp_info_act;
        QAction *about_info_act;

    void createMainMenu();
    void createMainMenuActions();
    void setMainMenuConnections();
    void unsetMovesBarConnections();

    /** menu z opcjami dotyczacymi ruchow - potwierdzenie, cofniecie, ponowienie */
    QToolBar *moves_bar;
    QAction *confirm_moves_act;
    QAction *undo_act;
    QAction *redo_act;
    QAction *back_turn_act;
    QAction *forward_turn_act;
    QAction *hint_act;

    void createMovesBar();
    void setMovesBarConnections();

    /** menu z edycja stanu gry */
    QToolBar *editor_bar;
    QAction *edit_act;

    QAction *confirm_edit_act;
//    QAction *cancel_edit_act;
    QAction *change_who_moves_act;

    void createEditorBar();
    void setEditorBarConnections();

    /** statusBar - informacja o przebiegu gry - kto sie rusza, ktora runda itd. */
    QLabel *who_moves;
    QLabel *turn_nr;
    QLabel *holder_moves_made;
    QLabel *passes_made;
    QLabel *edit_mode_info;

    void createStatusBar();
    void setStatusBarConnections();
    void updateWhoMoves();

    /** informacje i obsluga trybu edycji */
    bool in_edit_mode;

    void changeStatusBar();
    void changeEditorBar();

signals:
    void signalChangePlayerClicked();

    void signalReturnFromEditMode();

private slots:
    void createNewGame();
    void loadGame();
    void saveGame();
//    void exitGame();
    void closeApp();

    void slotEditStateClicked();
    void slotGoIntoEditState();
    void slotEditStateConfirmed();
    void slotEditStatePlayerChange();

    void slotGameEnded(PlayerTeam winner, bool unfair_game_rule);

    void slotUpdateStatusBar();
    void slotUpdateTurn();
    void slotUpdateMovesNr();

    void slotShowRules();
    void slotShowEditInfo();
    void slotShowPlayvsComputerInfo();
    void slotShowAboutInfo();
};

#endif // MAINWINDOW_H
