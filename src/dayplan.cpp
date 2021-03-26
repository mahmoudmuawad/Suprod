#include "dayplan.h"
#include "ui_dayplan.h"


Dayplan::Dayplan(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dayplan)
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());

    re_gui();
    set_db();

}

Dayplan::~Dayplan()
{
    delete ui;
    delete sql_models;
    delete settings;
}

void Dayplan::closeEvent(QCloseEvent *event){
    db.exec("vacwm");
    db.close();
    settings->setValue("DayplanGeometry", saveGeometry());
    settings->sync();
    event->accept();
}


void Dayplan::re_gui(){
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
    settings = new QSettings(set_lock.at(0) + "/settings.ini", QSettings::IniFormat);
    if(portable)
    {
        settings->setValue("portableMode", true);
        settings->sync();
    }
    restoreGeometry(settings->value("DayplanGeometry").toByteArray());
}


void Dayplan::on_pushButton_clicked()
{
    Task *task = new Task;
    task->setWindowTitle("New Task");
    if(task->exec()){
        QSqlQuery query ("insert into tasks3 (Task, Description, Start , End , Completed, Priority, Checklist, Day) "
                         "values (:name, :description, :start, :end, :completed, :priority, :checklist, :day);");
        query.bindValue(0, task->get_name());
        query.bindValue(1, task->get_desc());
        query.bindValue(2, task->get_dateFrom());
        query.bindValue(3, task->get_dateTo());
        query.bindValue(4, task->get_Done());
        query.bindValue(5,task->get_prio());
        query.bindValue(6, QString::number(task->get_checkList().count())+ " checks");
        query.bindValue(7, get_day());


        QList<QString> ls= task->get_checkList();
        QList<QString>::iterator i;
        for (i = ls.begin(); i != ls.end(); ++i){
            //cout << *i << Qt::endl;
            QSqlQuery queryli("insert into taskscl (Task, CLitems,status) "
                              " values (:Task,:CLitems,:status);");
            queryli.bindValue(0,task->get_name());
            queryli.bindValue(1,*i);
            queryli.bindValue(2, 0);
            if(queryli.exec())
                qDebug()<<"check list added ";
        }


        if(query.exec() )
            update_table();
    }
}

void Dayplan::set_db()
{
    // create settings.ini file if it doesn't exist
    if(!QFile(set_lock.at(0) + "/settings.ini").exists())
    {
        settings->setValue("DayplanGeometry", saveGeometry());
        //settings->setValue("DayplanState", saveState());
        settings->setValue("Sorting/row", 0);
        settings->setValue("Sorting/type", 0);
        settings->setValue("AutoUpdate/enabled", true);
        settings->setValue("portableMode", false);
        settings->sync();
    }
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(set_lock.at(0) + "/tasks3.db");
    if(db.open())
        qDebug()<<"db is opened ";

    db.exec("create table if not exists tasks3 (Task, Description, Start , End , Completed, Priority, Checklist, Day);");
    db.exec("create table if not exists taskscl (Task, CLitems,status);");
    db.exec("create table if not exists sort_preferences (id PRIMARY KEY, row, type)");


    sql_models = new QSqlTableModel(this, db);
    ui->tableView->setModel(sql_models);

    //ui->tableView->setModel(all_model);
    update_table();
}


void Dayplan::update_table()
{
    sql_models->setTable("tasks3");
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

QString Dayplan::get_day(){
    return ui->dateEdit->dateTime().toString("MM/dd/yyyy");
}


void Dayplan::on_pushButton_4_clicked()
{
    QSqlQuery query;
    QString cItem = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString().replace("'", "''");
    query.prepare("delete from tasks3 where Task = '" + cItem +"' ");
    if(!query.exec())
    {
        QMessageBox::warning(0,"Error", "can not be deleted !" + ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString() + ".");
        return;
    }
    if(QMessageBox::No == QMessageBox::question(this, "Remove Task", "in case of deleting can not be resotored :("))
    {
        return;
    }
    update_table();

    QSqlQuery queryli;
    queryli.prepare("DELETE FROM taskscl WHERE Task='" + cItem +"'");
    if(queryli.exec())
        qDebug()<<"related tasks deleted";
}

void Dayplan::on_pushButton_2_clicked()
{
    day_isAdd = true;
    Dayplan::accept();
}

void Dayplan::on_pushButton_5_clicked()
{
    Task *etask = new Task;
    etask->set_task(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString());
    etask->set_desc(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 1)).toString());
    etask->set_timeFrom(QTime::fromString(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 2)).toString(), "hh:mm:ss AP"));
    etask->set_timeTo(QTime::fromString(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 3)).toString(), "hh:mm:ss AP"));
    QString completed = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 4)).toString();
    if(completed == "Not Yet")
        etask->set_Done(0);
    else
        etask->set_Done(1);
    etask->set_prio(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 5)).toInt()); //set Priority

    QSqlQuery queryli(db);
    queryli.prepare("SELECT CLitems FROM taskscl WHERE Task =  '"+etask->get_name()+"'");
    if(queryli.exec())
        while(queryli.next())
            etask->update_checkList(queryli.value("CLitems").toString());
    else
        qDebug()<<"there is a problem in sql query";

    if(etask->exec())
    {
        QSqlQuery query("update tasks3 set Task=:name,Description=:description, Start=:start , End=:end , Completed=:completed, Priority=:priority, Checklist=:checklist, Day=:day WHERE Task=\'"
                        + ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString().replace("'", "''") + "\'");
        qDebug()<<"update tasks3 set Task=:name,Description=:description, Start=:start , End=:end , Completed=:completed, Priority=:priority, Checklist:checklist, Day:day WHERE Task=\'"
                  + ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString().replace("'", "''") + "\'";
        query.bindValue(0, etask->get_name());
        query.bindValue(1, etask->get_desc());
        query.bindValue(2, etask->get_dateFrom());
        query.bindValue(3, etask->get_dateTo());
        query.bindValue(4, etask->get_Done());
        query.bindValue(5, etask->get_prio());
        query.bindValue(6, QString::number(etask->get_checkList().count())+ " checks");
        query.bindValue(7, get_day());

        QSqlQuery queryli;
        queryli.prepare("DELETE FROM taskscl WHERE Task='" + etask->get_name() +"'");
        if(queryli.exec())
            qDebug()<<"related tasks deleted";

        QList<QString> ls= etask->get_checkList();
        QList<QString>::iterator i;
        for (i = ls.begin(); i != ls.end(); ++i){
            QSqlQuery queryli("insert into taskscl (Task, CLitems,status) "
                              " values (:Task,:CLitems,:status);");
            queryli.bindValue(0,etask->get_name());
            queryli.bindValue(1,*i);
            queryli.bindValue(2, 0);
            if(queryli.exec())
                qDebug()<<"check list added ";
        }
        if(query.exec())
            update_table();
        else
            qDebug()<<"error in query";
    }
}

void Dayplan::on_tableView_doubleClicked(const QModelIndex &index)
{
    on_pushButton_5_clicked();
}
