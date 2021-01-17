#ifndef STARTWINDOW_HH
#define STARTWINDOW_HH

#include <vector>

#include <QMainWindow>

/**
 * @brief Used to ask difficulty level from player.
 * @pre -
 * @post Difficulty is set
 */

namespace Ui {
class StartWindow;
}

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

private:
    Ui::StartWindow* ui_;
    unsigned long int difficult_;
    const std::vector<QString> difficulties_ = {"Easy", "Medium", "Hard", "Insane"};

signals:
    void startButtonClicked(unsigned int);

private slots:
    void on_startGameButton_clicked();

    // Scrolls through the difficulties.
    void on_decreaseDifficult_clicked();
    void on_increaseDifficult_clicked();

};

#endif // STARTWINDOW_HH

