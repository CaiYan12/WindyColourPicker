#include "ColorTipWidget.h"
#include <QPainter>
#include <QPainterPath>

ColorTipWidget::ColorTipWidget(QWidget *parent) : QWidget(parent), m_color(Qt::black)
{
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setFixedSize(200, 60);
}

void ColorTipWidget::setColor(const QColor& color) { m_color = color; update(); }

void ColorTipWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this); painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path; path.addRoundedRect(rect(), 6, 6);
    painter.fillPath(path, QColor(40, 40, 40, 230));
    
    QRect colorRect(10, 10, 40, 40);
    painter.fillRect(colorRect, m_color);
    painter.setPen(QPen(Qt::white, 1)); painter.drawRect(colorRect);
    
    painter.setPen(Qt::white);
    QFont font("Consolas", 9); painter.setFont(font);
    QString text = QString("RGB: %1, %2, %3\nHEX: %4")
                      .arg(m_color.red()).arg(m_color.green()).arg(m_color.blue())
                      .arg(m_color.name().toUpper());
    painter.drawText(60, 15, 130, 35, Qt::AlignLeft, text);
}