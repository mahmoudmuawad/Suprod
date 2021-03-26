#include "otask.h"
#include "ui_otask.h"

Otask::Otask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Otask)
{
    ui->setupUi(this);
}

Otask::~Otask()
{
    delete ui;
}

void Otask::closeEvent(QCloseEvent *event){
    db.exec("suprod");
    //db.close();
    event->accept();
}



void Otask::set_taskName(QString task_name){
    ui->taskLabel->setText(task_name);
}

void Otask::set_timeFrom(QString t){
    ui->startLabel->setText(t);
}

void Otask::set_timeTo(QString t){
    ui->endLabel->setText(t);
}

void Otask::set_desc(QString desc){
    ui->descLabel->setText(desc);
}

void Otask::update_checkList(QString itm, bool status)
{
    if(!itm.isEmpty()){
        QListWidgetItem* item = new QListWidgetItem(itm, ui->listWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        if(!status)
            item->setCheckState(Qt::Unchecked); // AND initialize check state
        else
            item->setCheckState(Qt::Checked);
        //ui->listWidget->addItem(checkbox);
    }
}

QListWidget* Otask::get_checkList(){
    return ui->listWidget;
}

void Otask::on_pushButton_clicked()
{
    set_lock = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    db = QSqlDatabase::addDatabase("QSQLITE", "Connection");
    db.setDatabaseName(set_lock.at(0) + "/tasks3.db");
    if(db.open())
        qDebug()<<"db connections success from opentasks";

        for(int i=0;i<ui->listWidget->count();i++){
            if(ui->listWidget->item(i)->checkState() == Qt::Checked){
                QSqlQuery querylis(db);
                querylis.prepare("DELETE FROM taskscl WHERE CLitems = '"+ui->listWidget->item(i)->text()+"' ");
                querylis.exec();

                QSqlQuery queryli(db);
                queryli.prepare("insert into taskscl (Task, CLitems,status) "
                                  " values (:Task,:CLitems,:status);");
                queryli.bindValue(0, ui->taskLabel->text());
                queryli.bindValue(1, ui->listWidget->item(i)->text());
                queryli.bindValue(2, 1);
                if(!queryli.exec())
                    qDebug()<<"error in sql query in otask";
                else
                    qDebug()<<"excuted successfully in otask";

            }
            else {
                QSqlQuery querylis(db);
                querylis.prepare("DELETE FROM taskscl WHERE CLitems = '"+ui->listWidget->item(i)->text()+"' ");
                querylis.exec();

                QSqlQuery queryli(db);
                queryli.prepare("insert into taskscl (Task, CLitems,status) "
                                  " values (:Task,:CLitems,:status);");
                queryli.bindValue(0, ui->taskLabel->text());
                queryli.bindValue(1, ui->listWidget->item(i)->text());
                queryli.bindValue(2, 0);
                if(!queryli.exec())
                    qDebug()<<"error in sql query in otask";
                else
                    qDebug()<<"excuted successfully in otask";
            }

        }
    fch = true;
    Otask::accept();
}
