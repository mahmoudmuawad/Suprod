#include "qhpushbutton.h"

QHPushButton::QHPushButton(QWidget *parent) : QPushButton(parent)
{
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
}


void QHPushButton::hoverEnter(QHoverEvent *)
{
    this->setStyleSheet(this->styleSheet()+ "background-color : rgb(138, 165, 255);");
}

void QHPushButton::hoverLeave(QHoverEvent *)
{
    this->setStyleSheet(this->styleSheet() + "background-color : none;");
}

void QHPushButton::hoverMove(QHoverEvent *)
{
    /*
    QFont font = this->font();
    font.setBold(true);
    this->setFont(font);
    repaint();
    */
}

bool QHPushButton::event(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::HoverEnter:
        hoverEnter(static_cast<QHoverEvent*>(event));
        return true;
        break;
    case QEvent::HoverLeave:
        hoverLeave(static_cast<QHoverEvent*>(event));
        return true;
        break;
    case QEvent::HoverMove:
        hoverMove(static_cast<QHoverEvent*>(event));
        return true;
        break;
    default:
        break;
    }
    return QWidget::event(event);
}
