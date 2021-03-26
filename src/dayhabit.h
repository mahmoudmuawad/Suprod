#ifndef DAYHABIT_H
#define DAYHABIT_H

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

namespace Ui {
class Dayhabit;
}

class Dayhabit : public QDialog
{
    Q_OBJECT

public:
    explicit Dayhabit(QWidget *parent = nullptr);
    ~Dayhabit();

    void set_db(QDate dayInp);

    QDate get_day();
    void update_checkList(QString itm, bool status);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dayhabit *ui;
    QSqlTableModel *sql_models;
    QStringList set_lock;
    QSqlDatabase db;
    void closeEvent(QCloseEvent *event);
    QSqlQueryModel search_model;
    QDir dir;
    QSettings *settings;

};

#endif // DAYHABIT_H
