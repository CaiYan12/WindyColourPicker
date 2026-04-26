#pragma once

#include <QtWidgets/QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QKeyEvent>
#include <QSlider>

class ColorWheel;
class ColorTipWidget;

class WindyColourPicker : public QWidget
{
    Q_OBJECT

public:
    WindyColourPicker(QWidget *parent = nullptr);
    ~WindyColourPicker();
    void updateColorDisplay(const QColor& color);

private slots:
    void togglePickMode(bool checked); // 只保留这一个需要的

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    ColorWheel* m_colorWheel;
    QPushButton* m_btnPick;
    QLineEdit* m_lblRgb;
    QLineEdit* m_lblHex;
    QLineEdit* m_lblHsv;
    QLineEdit* m_lblRgba;
    QSlider* m_sliderValue;
    QSlider* m_sliderAlpha;
    ColorTipWidget* m_tipWidget;
    bool m_isPicking;
    QColor m_currentColor;
};