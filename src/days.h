#ifndef DAYS_H
#define DAYS_H

#include <QDialog>
#include <QListWidget>
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

#include "otask.h"

namespace Ui {
class Days;
}

class Days : public QDialog
{
    Q_OBJECT

public:
    explicit Days(QWidget *parent = nullptr);
    ~Days();

    //QListWidget *days_list();
    QTableWidget *table();

    void re_gui();
    void set_db(QString dayInp);

    QString get_day();

protected slots:

    //void re_gui();
    //void set_sett();
    void closeEvent(QCloseEvent *event);
    void update_table();

protected :
    Ui::Days *ui;



private slots:
    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();


private:
    QSqlTableModel *sql_models;
    QStringList set_lock;
    QSqlDatabase db;

    QSqlQueryModel search_model;
    QDir dir;
    QSettings *settings;
};

#endif // DAYS_H
