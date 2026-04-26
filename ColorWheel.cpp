#include "ColorWheel.h"
#include <QPainter>
#include <QConicalGradient>
#include <QRadialGradient>
#include <QtMath>
#include <QMouseEvent>

ColorWheel::ColorWheel(QWidget *parent) : QWidget(parent), m_currentColor(Qt::red), m_value(1.0)
{
    setMinimumSize(300, 300);
}

void ColorWheel::setValue(qreal value) {
    m_value = qBound(0.0, value, 1.0);
    int h, s, v, a;
    m_currentColor.getHsv(&h, &s, &v, &a);
    m_currentColor.setHsv(h, s, (int)(m_value * 255), a);
    emit colorChanged(m_currentColor);
}

void ColorWheel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());
    m_wheelRect = QRectF(0, 0, side, side);
    m_wheelRect.moveCenter(rect().center());

    // 绘制色相环
    QConicalGradient hueGrad(m_wheelRect.center(), 0);
    hueGrad.setColorAt(0.0, Qt::red);
    hueGrad.setColorAt(1.0/6.0, Qt::yellow);
    hueGrad.setColorAt(2.0/6.0, Qt::green);
    hueGrad.setColorAt(3.0/6.0, Qt::cyan);
    hueGrad.setColorAt(4.0/6.0, Qt::blue);
    hueGrad.setColorAt(5.0/6.0, Qt::magenta);
    hueGrad.setColorAt(1.0, Qt::red);
    painter.setBrush(hueGrad);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(m_wheelRect);

    // 绘制饱和度遮罩
    QRadialGradient satGrad(m_wheelRect.center(), m_wheelRect.width()/2.0);
    satGrad.setColorAt(0, QColor(255,255,255, 255));
    satGrad.setColorAt(1, QColor(255,255,255, 0));
    painter.setBrush(satGrad);
    painter.drawEllipse(m_wheelRect);

    // 绘制选择点
    painter.setPen(QPen(m_value > 0.5 ? Qt::black : Qt::white, 2));
    painter.setBrush(Qt::NoBrush);
    if (m_selectorPos.isNull()) updateSelectorFromColor(m_currentColor);
    painter.drawEllipse(m_selectorPos, 8, 8);
}

void ColorWheel::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) handleMouseEvent(event->pos());
}

void ColorWheel::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) handleMouseEvent(event->pos());
}

void ColorWheel::handleMouseEvent(const QPointF& pos) {
    QPointF center = m_wheelRect.center();
    qreal dx = pos.x() - center.x();
    qreal dy = pos.y() - center.y();
    qreal dist = qSqrt(dx * dx + dy * dy);
    qreal radius = m_wheelRect.width() / 2.0;
    qreal effectiveDist = qMin(dist, radius);
    qreal ratio = effectiveDist / qMax(dist, 1e-9);
    m_selectorPos = QPointF(center.x() + dx * ratio, center.y() + dy * ratio);
    m_currentColor = colorFromPos(m_selectorPos);
    update();
    emit colorChanged(m_currentColor);
}

QColor ColorWheel::colorFromPos(const QPointF &pos) const {
    QPointF center = m_wheelRect.center();
    qreal dx = pos.x() - center.x();
    qreal dy = pos.y() - center.y();
    qreal distance = qSqrt(dx * dx + dy * dy);
    qreal radius = m_wheelRect.width() / 2.0;
    qreal limitedDist = qMin(distance, radius);

    // 计算角度
    qreal angle = qAtan2(dy, dx) * 180.0 / M_PI;
    angle = angle + 180.0;
    angle = std::fmod(angle + 180.0, 360.0);
    qreal h = (360.0 - angle) / 360.0;

    QColor color;
    color.setHsvF(h, limitedDist / radius, m_value);
    return color;
}

void ColorWheel::setCurrentColor(const QColor& color) {
    m_currentColor = color;
    float h, s, v, a;
    color.getHsvF(&h, &s, &v, &a);
    m_value = v;
    updateSelectorFromColor(color);
    update();
}

void ColorWheel::updateSelectorFromColor(const QColor& color) {
    float h, s, v, a;
    color.getHsvF(&h, &s, &v, &a);
    QPointF center = m_wheelRect.center();
    qreal radius = m_wheelRect.width()/2.0;
    
    qreal angle = 360.0 - h * 360.0;
    qreal rad = angle * M_PI / 180.0;
    
    qreal x = center.x() + s * radius * qCos(rad);
    qreal y = center.y() + s * radius * qSin(rad);
    
    m_selectorPos = QPointF(x, y);
}