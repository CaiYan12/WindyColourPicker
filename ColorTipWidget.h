#pragma once
#include <QWidget>
#include <QColor>

class ColorTipWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColorTipWidget(QWidget *parent = nullptr);
    void setColor(const QColor& color);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor m_color;
};