#include "startwindow.hh"
#include "ui_startwindow.h"


StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::StartWindow)
{
    difficult_ = 0;
    ui_->setupUi(this);
    ui_->difficultyLabel->setText(difficulties_[difficult_]);
}


StartWindow::~StartWindow()
{
    delete ui_;
    ui_ = nullptr;
}


void StartWindow::on_startGameButton_clicked()
{
    emit startButtonClicked(difficult_);
}


void StartWindow::on_decreaseDifficult_clicked()
{
    if (difficult_ > 0) {
        --difficult_;
        ui_->difficultyLabel->setText(difficulties_[difficult_]);
    }
}


void StartWindow::on_increaseDifficult_clicked()
{
    if (difficult_ < difficulties_.size() - 1) {
        ++difficult_;
        ui_->difficultyLabel->setText(difficulties_[difficult_]);
    }
}
