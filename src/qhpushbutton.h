#ifndef QHPUSHBUTTON_H
#define QHPUSHBUTTON_H

#include <QtWidgets>

class QHPushButton : public QPushButton
{
    Q_OBJECT;

public:
    explicit QHPushButton(QWidget *parent = 0);

protected:
    void hoverEnter(QHoverEvent *event);
    void hoverLeave(QHoverEvent *event);
    void hoverMove(QHoverEvent *event);
    bool event(QEvent *event);

};

#endif // QHPUSHBUTTON_H
