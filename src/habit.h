#ifndef HABIT_H
#define HABIT_H

#include <QDialog>
#include <QtWidgets>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSettings>
#include <QFileDialog>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class Habit;
}

class Habit : public QDialog
{
    Q_OBJECT

public:
    explicit Habit(QWidget *parent = nullptr);
    ~Habit();

    QString get_name();
    QDate get_date();



private slots:
    void check_habit();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

protected slots:
    void closeEvent(QCloseEvent *event);
    void re_gui();
    void set_db();
    void update_table();


protected :
    Ui::Habit *ui;

    QStringList set_lock;
    QSqlDatabase db;
    QSqlTableModel *sql_models;
    QSqlQueryModel search_model;
    QDir dir;
    QSettings *settings;

private:
};

#endif // HABIT_H

