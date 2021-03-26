#ifndef DAYPLAN_H
#define DAYPLAN_H

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

#include "task.h"

namespace Ui {
class Dayplan;
}

class Dayplan : public QDialog
{
    Q_OBJECT

public:
    explicit Dayplan(QWidget *parent = nullptr);
    ~Dayplan();

    QString get_day();
    bool day_isAdd = false;

protected slots:
    void closeEvent(QCloseEvent *event);
    void re_gui();

    void on_pushButton_clicked();
    void set_db();
    void update_table();


protected :
    Ui::Dayplan *ui;

    QStringList set_lock;
    QSqlDatabase db;
    QSqlTableModel *sql_models;
    QSqlQueryModel search_model;
    QDir dir;
    QSettings *settings;

private slots:
    void on_pushButton_4_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_5_clicked();
    void on_tableView_doubleClicked(const QModelIndex &index);
};

#endif // DAYPLAN_H
