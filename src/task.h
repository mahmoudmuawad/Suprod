#ifndef TASK_H
#define TASK_H

#include <QDialog>
#include <QtWidgets>
#include <QDebug>

namespace Ui {
class Task;
}

class Task : public QDialog
{
    Q_OBJECT

public:
    explicit Task(QWidget *parent = nullptr);
    ~Task();

    QString get_name();
    QString get_desc();
    QString get_dateFrom();
    QString get_dateTo();
    QString get_Done();
    QString get_prio();
    QList<QString>  get_checkList();

    QString get_checkItem();
    void update_checkList(QString itm);


    void set_task(QString name);
    void set_desc(QString desc);
    void set_timeTo(QTime t);
    void set_timeFrom(QTime t);
    void set_Done(int index);
    void set_prio(int index);
    void set_checklist(QString task_name);


private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void check_task();

    void on_pushButton_clicked();


private:
    Ui::Task *ui;
};

#endif // TASK_H

