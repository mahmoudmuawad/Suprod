#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSettings>
#include <QFileDialog>
#include <QSystemTrayIcon>
#include <QMenu>

#include <QMessageBox>
#include <QDebug>

#include "dayplan.h"
#include "habit.h"
#include "score.h"
#include "days.h"
#include "dayhabit.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:

    void on_actionAbout_triggered();
    void on_actionReport_a_bug_triggered();
    void on_actionUpdates_triggered();
    void on_actionLicence_triggered();

    void on_actionDays_triggered();
    void on_actionHabit_tracker_triggered();


protected :
    Ui::MainWindow *ui;
    QSystemTrayIcon *tray;
    QMenu *trayMenu;


private slots:
    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_4_clicked();


    void on_pushButton_2_clicked();

private:
    QString added_day;
};
#endif // MAINWINDOW_H
