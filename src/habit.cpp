#include "habit.h"
#include "ui_habit.h"

Habit::Habit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Habit)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());

    re_gui();
    set_db();
}

Habit::~Habit()
{
    delete ui;
    delete sql_models;
    delete settings;
}

QString Habit::get_name(){
    return ui->lineEdit->text();
}

QDate Habit::get_date(){
    return  ui->dateEdit->date();
}


void Habit::check_habit(){
    if(ui->lineEdit->text().isEmpty())
        QMessageBox::warning(this, "Error", "Please, Enter your Habit Name!");
}

void Habit::on_pushButton_3_clicked()
{
    check_habit();
    if(!ui->lineEdit->text().isEmpty()){
        QSqlQuery query ("insert into habits (habit, dateUntil, status) "
                         "values (:name, :dateu, :status);");
        query.bindValue(0, get_name());
        query.bindValue(1, get_date().toString());
        query.bindValue(2, 0);

        if(query.exec())
            update_table();
        else
            qDebug()<<"error in sql query";
    }
}



void Habit::closeEvent(QCloseEvent *event){
    db.exec("vacwm");
    db.close();
    settings->setValue("HabitGeometry", saveGeometry());
    settings->sync();
    event->accept();
}


void Habit::re_gui(){
    bool portable = false;
    if(QFile("portable.dat").exists())
    {
        qDebug() << "portable.dat detected!";
        portable = true;
        set_lock << "portable";
    }
    else
    {
        set_lock = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
        qDebug() << "Application Data Folder: " << QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    }
    bool ok = dir.mkpath(set_lock.at(0));
    if(!ok)
    {
        QMessageBox::critical(this, "Warning", "Could Not Create Storage Path");
        this->close();
    }
    settings = new QSettings(set_lock.at(0) + "/settings1.ini", QSettings::IniFormat);
    if(portable)
    {
        settings->setValue("portableMode", true);
        settings->sync();
    }
    restoreGeometry(settings->value("HabitGeometry").toByteArray());
}


void Habit::set_db()
{
    // create settings.ini file if it doesn't exist
    if(!QFile(set_lock.at(0) + "/settings1.ini").exists())
    {
        settings->setValue("HabitGeometry", saveGeometry());
        settings->setValue("Sorting/row", 0);
        settings->setValue("Sorting/type", 0);
        settings->setValue("AutoUpdate/enabled", true);
        settings->setValue("portableMode", false);
        settings->sync();
    }
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(set_lock.at(0) + "/habits.db");
    if(db.open())
        qDebug()<<"db is opened ";
    db.exec("create table if not exists habits (habit, dateUntil,status);");
    db.exec("create table if not exists sort_preferences (id PRIMARY KEY, row, type)");
    sql_models = new QSqlTableModel(this, db);
    ui->tableView->setModel(sql_models);
    update_table();
}


void Habit::update_table()
{
    sql_models->setTable("habits");
    sql_models->select();
    ui->tableView->setModel(sql_models);
    ui->tableView->resizeColumnsToContents();
    search_model.setQuery("SELECT id,row,type FROM sort_preferences", db);

    // ini settings test
    if(settings->value("sorting/type", 0).toInt() == 0)
        ui->tableView->sortByColumn(settings->value("sorting/row", 0).toInt(), Qt::AscendingOrder);
    else
        ui->tableView->sortByColumn(settings->value("sorting/row", 0).toInt(), Qt::DescendingOrder);
}

void Habit::on_pushButton_4_clicked()
{
    QString current_habit = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString().replace("'", "''");
    QSqlQuery query;
    query.prepare("delete from habits where habit = '" + current_habit +"'");

    if(!query.exec())
    {
        QMessageBox::warning(0,"Error", "can not be deleted !" + ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString() + ".");
        return;
    }
    else
        if(query.exec())
            qDebug()<<"deletion complete ";

    if(QMessageBox::No == QMessageBox::question(this, "Remove Task", "in case of deleting can not be resotored :("))
    {
        return;
    }

    update_table();

}

void Habit::on_pushButton_2_clicked()
{
    Habit::accept();
}
