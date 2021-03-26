#include "task.h"
#include "ui_task.h"
#include <iostream>

Task::Task(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Task)
{
    ui->setupUi(this);
    ui->timeEdit->setTime(QTime::currentTime());
    ui->timeEdit_2->setTime(QTime::currentTime());
    connect(ui->pushButton, SIGNAL(accepted()), this, SLOT(check_task()));
}

Task::~Task()
{
    delete ui;
}

QString Task::get_name(){
    return  ui->lineEdit->text();
}

QString Task::get_desc(){
    return  ui->plainTextEdit->toPlainText();
}

QString Task::get_dateTo(){
    return ui->timeEdit->time().toString("hh:mm:ss AP");
}

QString Task::get_dateFrom(){
    return ui->timeEdit_2->time().toString("hh:mm:ss AP");
}

QString Task::get_Done(){
    return  ui->comboBox->currentText();
}

QString Task::get_prio(){
    return ui->spinBox->text();
}

QString Task::get_checkItem(){
    return ui->lineEdit_2->text();
}




QList<QString>  Task::get_checkList(){
    QList<QString> chtxs;
    for (int i = 0; i < ui->listWidget->count(); i++){
        chtxs.append(ui->listWidget->item(i)->text());
    }
    return chtxs;
}

void Task::update_checkList(QString itm)
{
    if(!itm.isEmpty())
        ui->listWidget->addItem(itm);
}

// for editing taks
void Task::set_task(QString name)
{
    ui->lineEdit->setText(name);
}

void Task::set_desc(QString desc){
    ui->plainTextEdit->setPlainText(desc);
}

void Task::set_timeFrom(QTime t)
{
    ui->timeEdit->setTime(t);
}

void Task::set_timeTo(QTime t)
{
    ui->timeEdit->setTime(t);
}

void Task::set_Done(int index)
{
    ui->comboBox->setCurrentIndex(index);
}

void Task::set_prio(int index)
{
    ui->spinBox->setValue(index);
}

void Task::set_checklist(QString task_name)
{

}

void Task::check_task(){

    if(ui->lineEdit->text().isEmpty())
        QMessageBox::warning(this, "Error", "Please, Enter your Task Name!");
    else
        Task::accept();
}

void Task::on_pushButton_3_clicked()
{
    update_checkList(get_checkItem());
}

void Task::on_pushButton_4_clicked()
{
    ui->listWidget->takeItem(ui->listWidget->currentRow());
}

void Task::on_pushButton_clicked()
{
    check_task();
}

