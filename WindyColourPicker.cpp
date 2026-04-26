#include "WindyColourPicker.h"
#include "ColorWheel.h"
#include "ColorTipWidget.h"
#include <windows.h>
#include <QScreen>
#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>
#include <QMouseEvent>
#include <QGroupBox>
#include <QLineEdit>
#include <QFrame>
#include <QLabel>


// 全局钩子与窗口指针
static HHOOK g_mouseHook = nullptr;
static WindyColourPicker* g_mainWindow = nullptr;

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_MOUSEMOVE && g_mainWindow) {
        MSLLHOOKSTRUCT* pMouse = (MSLLHOOKSTRUCT*)lParam;
        QMetaObject::invokeMethod(g_mainWindow, [=]() {
            QPoint cursorPos = QCursor::pos();
            HDC hdcScreen = GetDC(NULL);
            COLORREF color = GetPixel(hdcScreen, pMouse->pt.x, pMouse->pt.y);
            ReleaseDC(NULL, hdcScreen);
            g_mainWindow->updateColorDisplay(QColor(GetRValue(color), GetGValue(color), GetBValue(color), 255));
            g_mainWindow->moveTipTo(cursorPos + QPoint(20, 20));
        }, Qt::QueuedConnection);
    }
    return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
}

// 辅助函数：创建 Fluent 风格的 LineEdit
static QLineEdit* createFluentLineEdit(QWidget* parent = nullptr) {
    QLineEdit* le = new QLineEdit(parent);
    le->setReadOnly(true);
    le->setMinimumWidth(170);
    le->setMaximumWidth(170);
    le->setCursor(Qt::IBeamCursor);
    le->setStyleSheet(R"(
        QLineEdit {
            background-color: #ffffff;
            border: 1px solid #d1d1d1;
            border-radius: 5px;
            padding: 7px 10px;
            font-family: "Consolas", "Segoe UI";
            font-size: 13px;
            color: #222;
        }
        QLineEdit:hover { border: 1px solid #0078D4; }
        QLineEdit:focus { border: 2px solid #0078D4; padding: 6px 9px; }
    )");
    return le;
}

// 辅助函数：创建统一宽度的标签（文字居中）
static QLabel* createFixedLabel(const QString& text, QWidget* parent = nullptr) {
    QLabel* lbl = new QLabel(text, parent);
    lbl->setFixedWidth(60);
    lbl->setAlignment(Qt::AlignCenter); // 修复：文字居中
    lbl->setStyleSheet(R"(
        QLabel {
            font-size: 13px;
            color: #666;
            font-weight: 600;
            background-color: #f8f8f8;
            border-radius: 5px;
            padding: 7px 0;
        }
    )");
    return lbl;
}

WindyColourPicker::WindyColourPicker(QWidget *parent) : QWidget(parent), m_isPicking(false), m_currentColor(Qt::red)
{
    g_mainWindow = this;
    resize(720, 540); // 稍微增加高度，给色盘底部留出空间
    setWindowTitle("Windy Colour Picker");
    setMinimumSize(720, 540);

    // 全局样式
    this->setStyleSheet(R"(
        QWidget {
            background-color: #f3f3f3;
            font-family: "Segoe UI", "Microsoft YaHei UI";
        }
        QSlider::groove:horizontal {
            height: 4px;
            background: #d5d5d5;
            border-radius: 2px;
        }
        QSlider::handle:horizontal {
            background: #0078D4;
            width: 20px;
            height: 20px;
            margin: -8px 0;
            border-radius: 10px;
            border: 2px solid white;
        }
        QSlider::handle:horizontal:hover { background: #1084D9; }
        QSlider { min-height: 30px; max-height: 30px; }
        QGroupBox {
            border: none;
            margin-top: 10px;
            font-weight: 600;
            color: #333;
            font-size: 14px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 0px;
            padding: 0 5px 15px 0px;
        }
    )");

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(24);
    mainLayout->setContentsMargins(24, 24, 24, 24);

    // === 左侧面板：色盘 ===
    QWidget* leftPanel = new QWidget(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0,0,0,0);
    leftLayout->setSpacing(16);
    
    // 修复色盘居中：增加底部内边距
    QFrame* wheelContainer = new QFrame(this);
    wheelContainer->setStyleSheet(R"(
        QFrame { background-color: white; border-radius: 12px; border: 1px solid rgba(0,0,0,0.06); }
    )");
    QVBoxLayout* wheelLayout = new QVBoxLayout(wheelContainer);
    wheelLayout->setContentsMargins(20, 20, 20, 24); // 底部增加4px边距

    m_colorWheel = new ColorWheel(this);
    m_colorWheel->setFixedSize(300, 300); // 固定大小，确保居中
    wheelLayout->addWidget(m_colorWheel, 0, Qt::AlignCenter);
    leftLayout->addWidget(wheelContainer, 0, Qt::AlignHCenter);

    QGroupBox* sliderGroup = new QGroupBox("调整", this);
    QVBoxLayout* sliderLayout = new QVBoxLayout(sliderGroup);
    sliderLayout->setSpacing(12);
    
    QWidget* valRow = new QWidget();
    QHBoxLayout* valRowLayout = new QHBoxLayout(valRow);
    valRowLayout->setSpacing(8);
    QLabel* lblVal = new QLabel("明度");
    lblVal->setStyleSheet("font-size: 13px; color: #444; font-weight: 500; min-width: 50px;");
    m_sliderValue = new QSlider(Qt::Horizontal, this);
    m_sliderValue->setRange(0, 255);
    m_sliderValue->setValue(255);
    m_lblValueNum = new QLabel("255");
    m_lblValueNum->setFixedWidth(30);
    m_lblValueNum->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lblValueNum->setStyleSheet("font-size: 13px; color: #333; font-weight: 500;");
    valRowLayout->addWidget(lblVal);
    valRowLayout->addWidget(m_sliderValue);
    valRowLayout->addWidget(m_lblValueNum);

    QWidget* alphaRow = new QWidget();
    QHBoxLayout* alphaRowLayout = new QHBoxLayout(alphaRow);
    alphaRowLayout->setSpacing(8);
    QLabel* lblAlpha = new QLabel("透明度");
    lblAlpha->setStyleSheet("font-size: 13px; color: #444; font-weight: 500; min-width: 50px;");
    m_sliderAlpha = new QSlider(Qt::Horizontal, this);
    m_sliderAlpha->setRange(0, 255);
    m_sliderAlpha->setValue(255);
    m_lblAlphaNum = new QLabel("255");
    m_lblAlphaNum->setFixedWidth(30);
    m_lblAlphaNum->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lblAlphaNum->setStyleSheet("font-size: 13px; color: #333; font-weight: 500;");
    alphaRowLayout->addWidget(lblAlpha);
    alphaRowLayout->addWidget(m_sliderAlpha);
    alphaRowLayout->addWidget(m_lblAlphaNum);

    sliderLayout->addWidget(valRow);
    sliderLayout->addWidget(alphaRow);
    leftLayout->addWidget(sliderGroup);
    leftLayout->addStretch();

    mainLayout->addWidget(leftPanel, 1);

    // === 右侧面板：控制与信息 ===
    QWidget* rightPanel = new QWidget(this);
    rightPanel->setMinimumWidth(300);
    rightPanel->setMaximumWidth(300);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setSpacing(16);

    // 按钮
    m_btnPick = new QPushButton("🎯 开始取色 (空格)", this);
    m_btnPick->setCheckable(true);
    m_btnPick->setCursor(Qt::PointingHandCursor);
    m_btnPick->setFocusPolicy(Qt::NoFocus);
    
    m_btnPick->setStyleSheet(R"(
        QPushButton {
            background-color: #0078D4;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 12px 20px;
            font-size: 14px;
            font-weight: 600;
        }
        QPushButton:hover { background-color: #1084D9; }
        QPushButton:pressed { background-color: #006CC1; }
        QPushButton:checked { background-color: #FF4343; }
        QPushButton:checked:hover { background-color: #FF5A5A; }
    )");

    connect(m_btnPick, &QPushButton::toggled, this, &WindyColourPicker::togglePickMode);
    rightLayout->addWidget(m_btnPick);

    // 颜色预览
    QFrame* previewCard = new QFrame(this);
    previewCard->setStyleSheet(R"(
        QFrame { background-color: white; border-radius: 12px; border: 1px solid rgba(0,0,0,0.06); }
    )");
    QVBoxLayout* previewLayout = new QVBoxLayout(previewCard);
    previewLayout->setContentsMargins(14, 14, 14, 14);
    
    QLabel* colorPreview = new QLabel(this);
    colorPreview->setMinimumHeight(90);
    colorPreview->setObjectName("ColorPreview");
    colorPreview->setStyleSheet("background-color: #FF0000; border-radius: 8px;");
    previewLayout->addWidget(colorPreview);
    m_colorPreview = colorPreview;
    rightLayout->addWidget(previewCard);

    // 颜色信息
    QFrame* infoCard = new QFrame(this);
    infoCard->setStyleSheet(R"(
        QFrame { background-color: white; border-radius: 12px; border: 1px solid rgba(0,0,0,0.06); }
    )");
    QFormLayout* infoLayout = new QFormLayout(infoCard);
    infoLayout->setSpacing(10);
    infoLayout->setContentsMargins(16, 18, 16, 18);
    infoLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    
    infoLayout->addRow(createFixedLabel("HEX:"), m_lblHex = createFluentLineEdit());
    infoLayout->addRow(createFixedLabel("RGB:"), m_lblRgb = createFluentLineEdit());
    infoLayout->addRow(createFixedLabel("RGBA:"), m_lblRgba = createFluentLineEdit());
    infoLayout->addRow(createFixedLabel("HSV:"), m_lblHsv = createFluentLineEdit());

    rightLayout->addWidget(infoCard);
    rightLayout->addStretch();

    mainLayout->addWidget(rightPanel);

    m_tipWidget = new ColorTipWidget(this);
    
    // 逻辑连接
    connect(m_colorWheel, &ColorWheel::colorChanged, this, [this](const QColor& c){
        int alpha = m_sliderAlpha->value();
        m_currentColor = QColor(c.red(), c.green(), c.blue(), alpha);
        int h, s, v; m_currentColor.getHsv(&h, &s, &v);
        { QSignalBlocker b(m_sliderValue); m_sliderValue->setValue(v); }
        this->updateColorDisplay(m_currentColor);
    });
    
    connect(m_sliderValue, &QSlider::valueChanged, this, [this](int value){
        m_lblValueNum->setText(QString::number(value));
        m_colorWheel->setValue(value / 255.0);
        int h, s, v, a;
        m_currentColor.getHsv(&h, &s, &v, &a);
        m_currentColor.setHsv(h, s, value, a);
        this->updateColorDisplay(m_currentColor);
    });
    
    connect(m_sliderAlpha, &QSlider::valueChanged, this, [this](int value){
        m_lblAlphaNum->setText(QString::number(value));
        m_currentColor.setAlpha(value);
        this->updateColorDisplay(m_currentColor);
    });
}

WindyColourPicker::~WindyColourPicker() {
    if (g_mouseHook) UnhookWindowsHookEx(g_mouseHook);
}

void WindyColourPicker::moveTipTo(const QPoint& pos) {
    if (m_tipWidget) m_tipWidget->move(pos);
}

void WindyColourPicker::stopPicking() {
    if (m_btnPick) m_btnPick->setChecked(false);
}

void WindyColourPicker::togglePickMode(bool checked) {
    m_isPicking = checked;
    if (checked) {
        m_btnPick->setText("⏹️ 停止取色 (Esc)");
        g_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, GetModuleHandle(NULL), 0);
        m_tipWidget->show();
        setFocus(Qt::OtherFocusReason);
        grabKeyboard();
    } else {
        m_btnPick->setText("🎯 开始取色 (空格)");
        if (g_mouseHook) UnhookWindowsHookEx(g_mouseHook);
        m_tipWidget->hide();
        releaseKeyboard();
    }
}

void WindyColourPicker::updateColorDisplay(const QColor& color) {
    m_currentColor = color;
    
    m_lblHex->setText(color.name().toUpper());
    m_lblRgb->setText(QString("%1, %2, %3").arg(color.red()).arg(color.green()).arg(color.blue()));
    m_lblRgba->setText(QString("%1, %2, %3, %4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
    
    int h, s, v; color.getHsv(&h, &s, &v);
    m_lblHsv->setText(QString("%1°, %2%, %3%").arg(h).arg(s/255.0*100, 0, 'f', 0).arg(v/255.0*100, 0, 'f', 0));

    m_colorPreview->setStyleSheet(
        QString("background-color: rgba(%1, %2, %3, %4); border-radius: 8px;")
            .arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));

    // 同步滑块
    {
        QSignalBlocker b1(m_sliderValue);
        QSignalBlocker b2(m_sliderAlpha);
        m_sliderValue->setValue(v);
        m_sliderAlpha->setValue(color.alpha());
        m_lblValueNum->setText(QString::number(v));
        m_lblAlphaNum->setText(QString::number(color.alpha()));
    }

    // 取色模式下同步色盘指针
    if (m_isPicking) {
        QSignalBlocker blocker(m_colorWheel);
        m_colorWheel->setCurrentColor(color);
    }

    m_tipWidget->setColor(color);
}

void WindyColourPicker::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape && m_isPicking) {
        stopPicking();
        return;
    }
    if (event->key() == Qt::Key_Space && !m_isPicking) {
        m_btnPick->setChecked(true);
        return;
    }
    QWidget::keyPressEvent(event);
}