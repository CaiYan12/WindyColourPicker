#pragma once
#include <QWidget>
#include <QColor>

class ColorWheel : public QWidget
{
    Q_OBJECT
public:
    explicit ColorWheel(QWidget *parent = nullptr);
    void setCurrentColor(const QColor& color);
    void setValue(qreal value); // 新增：设置明度 (0.0 - 1.0)

signals:
    void colorChanged(const QColor& color);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QColor m_currentColor;
    QPointF m_selectorPos;
    QRectF m_wheelRect;
    qreal m_value; // 新增：存储当前明度
    QColor colorFromPos(const QPointF &pos);
    void updateSelectorFromColor(const QColor& color);
};