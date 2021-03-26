#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tray = new QSystemTrayIcon;
    trayMenu = new QMenu;
    tray->setContextMenu(trayMenu);
    tray->setIcon(QIcon(":/images/imgs/habit.svg"));
    tray->show();
    connect(trayMenu, SIGNAL(triggered(QAction*)), this, SLOT(trayMenu_triggered(QAction*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_6_clicked()
{
    Dayplan *dayplan = new Dayplan;
    dayplan->setWindowTitle("Plan A day");
    dayplan->exec();
}


void MainWindow::on_pushButton_7_clicked()
{

}

void MainWindow::on_pushButton_8_clicked()
{
    Habit *habit = new Habit;
    habit->setWindowTitle("Add New Habit");
    habit->exec();

}


void MainWindow::on_pushButton_4_clicked()
{
    Days *days = new Days;
    days->setWindowTitle("Days");
    days->exec();

}

void MainWindow::on_pushButton_2_clicked()
{
    Dayhabit *dayhabit = new Dayhabit;
    dayhabit->setWindowTitle("habits");
    dayhabit->exec();
}




void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About", "Suprod : is an application for management and habit traking"
                                            "  \nCreated By Mahmoud Moawad \nVersion: " + QApplication::applicationVersion());
}


void MainWindow::on_actionReport_a_bug_triggered(){
    QMessageBox::information(this, "Bugs Report", "Please : send your bugs report to \n "
                                                  "mahmoudmuawad@gmail.com");
}
void MainWindow::on_actionUpdates_triggered(){
    QDesktopServices::openUrl(QUrl("https://github.com/mahmoudmuawad/Suprod"));
}
void MainWindow::on_actionLicence_triggered(){
    QDesktopServices::openUrl(QUrl("https://github.com/mahmoudmuawad/Suprod/blob/main/LICENSE"));
}


void  MainWindow::on_actionDays_triggered(){
    on_pushButton_4_clicked();
}

void MainWindow::on_actionHabit_tracker_triggered(){
    on_pushButton_2_clicked();
}

