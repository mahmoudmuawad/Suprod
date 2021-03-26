#include "dayhabit.h"
#include "ui_dayhabit.h"

Dayhabit::Dayhabit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dayhabit)
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());
}

Dayhabit::~Dayhabit()
{
    delete ui;
}

void Dayhabit::closeEvent(QCloseEvent *event){
    db.exec("vacwm");
    event->accept();
}

QDate Dayhabit::get_day(){
    return ui->dateEdit->date();
}

void Dayhabit::update_checkList(QString itm, bool status)
{
    if(!itm.isEmpty()){
        QListWidgetItem* item = new QListWidgetItem(itm, ui->listWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        if(!status)
            item->setCheckState(Qt::Unchecked); // AND initialize check state
        else
            item->setCheckState(Qt::Checked);
    }
}


void Dayhabit::set_db(QDate dayInp)
{
    set_lock = QStandardPaths::standardLocations(QStandardPaths::DataLocation);

    qDebug()<<set_lock.at(0);
    db = QSqlDatabase::addDatabase("QSQLITE", "Connection");
    db.setDatabaseName(set_lock.at(0) + "/habits.db");
    if(db.open())
        qDebug()<<"db connections sucess";
    QSqlQuery queryo(db);
    queryo.prepare("SELECT * FROM habits");

    if(queryo.exec())
        while(queryo.next()){
            QString str = queryo.value("dateUntil").toString();
            QDate d = QDate::fromString(str);
            qDebug()<<dayInp.toString();
            qDebug()<<d.toString();
                if(dayInp <= d){
                      update_checkList(queryo.value("habit").toString(), queryo.value("status").toInt());
                }
            }
      else
            qDebug()<<"there is a problem in sql query";

    if(!ui->listWidget->count()>0){
        QSqlQuery queryli(db);
        queryli.prepare("SELECT * FROM habits");
        if(queryli.exec())
            while(queryli.next())
                update_checkList(queryli.value("habit").toString(), queryli.value("status").toInt());
        else
            qDebug()<<"there is a problem in sql query";
    }
}

void Dayhabit::on_pushButton_clicked()      // start tracking button
{
    if(!ui->listWidget->count()>0)
        if(QMessageBox::No == QMessageBox::question(this, "Start tracing", "in case of starting you can not add or remove habits in this day!\n Start ?"))
            return;
    else
        set_db(ui->dateEdit->date());
}


void Dayhabit::on_pushButton_3_clicked()
{
    set_lock = QStandardPaths::standardLocations(QStandardPaths::DataLocation);

    // this action should be seperated in a single button !
    db = QSqlDatabase::addDatabase("QSQLITE", "Connection");
    db.setDatabaseName(set_lock.at(0) + "/habits.db");
    if(db.open())
        qDebug()<<"Ok habit :  db connections success from habits";

        for(int i=0;i<ui->listWidget->count();i++){
            if(ui->listWidget->item(i)->checkState() == Qt::Checked){
                QSqlQuery querylis(db);
                querylis.prepare("UPDATE habits SET status = 1 WHERE habit = '"+ui->listWidget->item(i)->text()+"' ");
                if(querylis.exec())
                    qDebug()<< "Ok habit :  updated the value of checked list item";
                else
                    qDebug()<<"UPDATE habits SET status = 1 WHERE habit = '"+ui->listWidget->item(i)->text()+"' ";


            }
            else {

                QSqlQuery querylis(db);
                querylis.prepare("UPDATE habits SET status = 0 WHERE habit = '"+ui->listWidget->item(i)->text()+"' ");

                if(querylis.exec())
                    qDebug()<< "Ok habit :  updated the value of checked list item";
                else
                    qDebug()<<"UPDATE habits SET status = 0 WHERE habit = '"+ui->listWidget->item(i)->text()+"' ";
            }

        }
    Dayhabit::accept();
}

void Dayhabit::on_pushButton_2_clicked()
{
    Dayhabit::close();
}

