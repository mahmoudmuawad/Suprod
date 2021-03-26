#include "days.h"
#include "ui_days.h"

Days::Days(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Days)
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());

    re_gui();
    set_db(get_day());

}

Days::~Days()
{
    delete ui;
    delete sql_models;
    delete settings;
}

void Days::closeEvent(QCloseEvent *event){
    db.exec("suprod");
    db.close();
    settings->setValue("DaysGeometry", saveGeometry());
    settings->sync();
    event->accept();
}


void Days::re_gui(){
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
    settings = new QSettings(set_lock.at(0) + "/settings0.ini", QSettings::IniFormat);
    if(portable)
    {
        settings->setValue("portableMode", true);
        settings->sync();
    }
    restoreGeometry(settings->value("DaysGeometry").toByteArray());
}

void Days::set_db(QString dayInp)
{
    // create settings.ini file if it doesn't exist
    if(!QFile(set_lock.at(0) + "/settings0.ini").exists())
    {
        settings->setValue("DaysGeometry", saveGeometry());
        //settings->setValue("DayplanState", saveState());
        settings->setValue("Sorting/row", 0);
        settings->setValue("Sorting/type", 0);
        settings->setValue("AutoUpdate/enabled", true);
        settings->setValue("portableMode", false);
        settings->sync();
    }
    db = QSqlDatabase::addDatabase("QSQLITE", "Connection");
    db.setDatabaseName(set_lock.at(0) + "/tasks3.db");
    if(db.open())
        qDebug()<<"db connections sucess";

    db.exec("DROP TABLE daytasks0;");
    db.exec("CREATE TABLE daytasks0 AS SELECT Task,Start,End,Priority,Checklist FROM tasks3 WHERE Day = '"+dayInp+"' AND Completed = 'Not Yet';");
    qDebug()<<"CREATE TABLE IF NOT EXISTS daytasks0 AS SELECT Task,Start,End,Priority,Checklist FROM tasks3 WHERE Day = '"+get_day()+"' AND Completed = 'Not Yet')";
    sql_models = new QSqlTableModel(this, db);
    ui->tableView->setModel(sql_models);
    update_table();
}


void Days::update_table()
{
    sql_models->setTable("daytasks0");
    sql_models->select();
    ui->tableView->setModel(sql_models);
    ui->tableView->resizeColumnsToContents();
    if(settings->value("sorting/type", 0).toInt() == 0)
        ui->tableView->sortByColumn(settings->value("sorting/row", 0).toInt(), Qt::AscendingOrder);
    else
        ui->tableView->sortByColumn(settings->value("sorting/row", 0).toInt(), Qt::DescendingOrder);
}

QString Days::get_day(){
    return ui->dateEdit->dateTime().toString("MM/dd/yyyy");
}


void Days::on_pushButton_6_clicked()
{
    re_gui();
    set_db(get_day());
}

void Days::on_pushButton_5_clicked()
{
    QString current_task = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString().replace("'", "''");
    QSqlQuery query1, query2;
    query1.prepare("delete from tasks3 where Task = \'" + current_task +"\'");
    query2.prepare("delete from daytasks0 where Task = \'" + current_task +"\'");

    if(!query1.exec())
    {
        QMessageBox::warning(0,"Error", "can not be deleted !" + ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString() + ".");
        return;
    }
    else
        if(query2.exec())
            qDebug()<<"deletion complete ";

    if(QMessageBox::No == QMessageBox::question(this, "Remove Task", "in case of deleting can not be resotored :("))
    {
        return;
    }


    QSqlQuery querylis;
    querylis.prepare("DELETE FROM taskscl WHERE Task='" + current_task +"'");
    qDebug()<<"DELETE FROM taskscl WHERE Task='" + current_task +"'";
    if(querylis.exec())
        qDebug()<<"related tasks deleted";

    update_table();

}

void Days::on_pushButton_3_clicked()
{
    Otask *otask = new Otask;
    QString current_task = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString().replace("'", "''");
    otask->setWindowTitle(current_task);
    otask->set_taskName(current_task);
    QSqlQuery queryo(db);
    queryo.prepare("SELECT * FROM tasks3 WHERE Task = '"+current_task+"'");
    if(queryo.exec())
        while(queryo.next()){
            otask->set_timeFrom(queryo.value("Start").toString());
            otask->set_timeTo(queryo.value("End").toString());
            otask->set_desc(queryo.value("Description").toString());
        }
    else
        qDebug()<<"there is a problem in sql query";

    QSqlQuery queryli(db);
    queryli.prepare("SELECT CLitems,status FROM taskscl WHERE Task =  '"+current_task+"'");
    if(queryli.exec())
        while(queryli.next())
            otask->update_checkList(queryli.value("CLitems").toString(), queryli.value("status").toInt());
    else
        qDebug()<<"there is a problem in sql query";
    otask->exec();
    if(otask->close()){
            re_gui();
            set_db(get_day());
    }
}



void Days::on_pushButton_4_clicked()
{
    QString current_task = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString().replace("'", "''");
    QSqlQuery query(db);
    query.prepare("UPDATE tasks3 SET Completed = 'Yes' WHERE Task = '" + current_task + "'");
    if(query.exec())
        on_pushButton_6_clicked();
    else
        qDebug()<<"There is error in complete some task";

}
