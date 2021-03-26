#ifndef OTASK_H
#define OTASK_H

#include <QDialog>
#include <QtWidgets>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlRecord>

namespace Ui {
class Otask;
}

class Otask : public QDialog
{
    Q_OBJECT

public:
    explicit Otask(QWidget *parent = nullptr);
    ~Otask();

    void set_taskName(QString task_name);
    void set_timeFrom(QString t);
    void set_timeTo(QString t);
    void set_desc(QString desc);
    QListWidget* get_checkList();
    void update_checkList(QString itm, bool status);

    bool fch = false;


private slots:
    void on_pushButton_clicked();

private:
    Ui::Otask *ui;

    QStringList set_lock;
    QSqlDatabase db;

    QDir dir;
    void closeEvent(QCloseEvent *event);
};

#endif // OTASK_H
