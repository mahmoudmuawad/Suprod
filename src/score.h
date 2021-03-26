#ifndef SCORE_H
#define SCORE_H

#include <QDialog>
#include <QtWidgets>

namespace Ui {
class Score;
}

class Score : public QDialog
{
    Q_OBJECT

public:
    explicit Score(QWidget *parent = nullptr);
    ~Score();

    QString get_score();
    QString get_scoreDate();

    void check_score();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::Score *ui;
};

#endif // SCORE_H
