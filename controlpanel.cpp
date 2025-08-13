#include "controlpanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QTime>
#include <QTimer>
#include <QFont>
#include <QDebug>

ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent)
    , isRunning(false)
{
    setMinimumHeight(80);
    setMaximumHeight(100);
    setupUI();
}

ControlPanel::~ControlPanel()
{
}

void ControlPanel::setupUI()
{
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 5, 10, 5);
    mainLayout->setSpacing(15);
    
    createCheckboxes();
    createTimeDisplay();
    createControlButtons();
    createStatusDisplay();
}

void ControlPanel::createCheckboxes()
{
    QGroupBox *checkboxGroup = new QGroupBox("Display Options");
    QVBoxLayout *checkboxLayout = new QVBoxLayout(checkboxGroup);
    
    showDTEDAreasCheckBox = new QCheckBox("Show DTED Areas");
    showDTEDAreasCheckBox->setChecked(false);
    checkboxLayout->addWidget(showDTEDAreasCheckBox);
    
    showWeatherConditionsCheckBox = new QCheckBox("Show Weather Conditions");
    showWeatherConditionsCheckBox->setChecked(false);
    checkboxLayout->addWidget(showWeatherConditionsCheckBox);
    
    showTargetsTrajCheckBox = new QCheckBox("Show Targets Traj");
    showTargetsTrajCheckBox->setChecked(false);
    checkboxLayout->addWidget(showTargetsTrajCheckBox);
    
    mainLayout->addWidget(checkboxGroup);
    
    // Bağlantılar
    connect(showDTEDAreasCheckBox, &QCheckBox::toggled, this, &ControlPanel::onShowDTEDAreasChanged);
    connect(showWeatherConditionsCheckBox, &QCheckBox::toggled, this, &ControlPanel::onShowWeatherConditionsChanged);
    connect(showTargetsTrajCheckBox, &QCheckBox::toggled, this, &ControlPanel::onShowTargetsTrajChanged);
}

void ControlPanel::createTimeDisplay()
{
    QGroupBox *timeGroup = new QGroupBox("Time");
    QVBoxLayout *timeLayout = new QVBoxLayout(timeGroup);
    
    timeLabel = new QLabel("Current Time:");
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLayout->addWidget(timeLabel);
    
    timeDisplay = new QLabel("00:00:00");
    timeDisplay->setAlignment(Qt::AlignCenter);
    timeDisplay->setFont(QFont("Menlo", 12, QFont::Bold));
    timeDisplay->setStyleSheet("QLabel { background-color: #2a2a2a; color: #00ff00; padding: 5px; border: 1px solid #555; border-radius: 3px; }");
    timeLayout->addWidget(timeDisplay);
    
    mainLayout->addWidget(timeGroup);
}

void ControlPanel::createControlButtons()
{
    QGroupBox *controlGroup = new QGroupBox("Controls");
    QHBoxLayout *controlLayout = new QHBoxLayout(controlGroup);
    
    startButton = new QPushButton("Start");
    startButton->setMinimumSize(80, 30);
    startButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #2a82da;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 5px;"
        "    padding: 8px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1e6bb8;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #155a9e;"
        "}"
    );
    controlLayout->addWidget(startButton);
    
    stopButton = new QPushButton("Stop");
    stopButton->setMinimumSize(80, 30);
    stopButton->setEnabled(false);
    stopButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #dc3545;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 5px;"
        "    padding: 8px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #c82333;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #a71e2a;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #6c757d;"
        "    color: #adb5bd;"
        "}"
    );
    controlLayout->addWidget(stopButton);
    
    mainLayout->addWidget(controlGroup);
    
    // Bağlantılar
    connect(startButton, &QPushButton::clicked, this, &ControlPanel::onStartClicked);
    connect(stopButton, &QPushButton::clicked, this, &ControlPanel::onStopClicked);
}

void ControlPanel::createStatusDisplay()
{
    QGroupBox *statusGroup = new QGroupBox("Status");
    QVBoxLayout *statusLayout = new QVBoxLayout(statusGroup);
    
    statusLabel = new QLabel("Status:");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLayout->addWidget(statusLabel);
    
    statusDisplay = new QLabel("Paused");
    statusDisplay->setAlignment(Qt::AlignCenter);
    statusDisplay->setFont(QFont("Arial", 10, QFont::Bold));
    statusDisplay->setStyleSheet("QLabel { background-color: #2a2a2a; color: #ffc107; padding: 5px; border: 1px solid #555; border-radius: 3px; }");
    statusLayout->addWidget(statusDisplay);
    
    mainLayout->addWidget(statusGroup);
}

void ControlPanel::updateTime(const QString &time)
{
    timeDisplay->setText(time);
}

void ControlPanel::setStatus(const QString &status)
{
    statusDisplay->setText(status);
    
    if (status == "Running") {
        statusDisplay->setStyleSheet("QLabel { background-color: #2a2a2a; color: #28a745; padding: 5px; border: 1px solid #555; border-radius: 3px; }");
    } else {
        statusDisplay->setStyleSheet("QLabel { background-color: #2a2a2a; color: #ffc107; padding: 5px; border: 1px solid #555; border-radius: 3px; }");
    }
}

void ControlPanel::onStartClicked()
{
    isRunning = true;
    startButton->setEnabled(false);
    stopButton->setEnabled(true);
    emit startClicked();
}

void ControlPanel::onStopClicked()
{
    isRunning = false;
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
    emit stopClicked();
}

void ControlPanel::onShowDTEDAreasChanged(bool checked)
{
    qDebug() << "Show DTED Areas:" << checked;
    emit showDTEDAreasChanged(checked);
}

void ControlPanel::onShowWeatherConditionsChanged(bool checked)
{
    qDebug() << "Show Weather Conditions:" << checked;
    emit showWeatherConditionsChanged(checked);
}

void ControlPanel::onShowTargetsTrajChanged(bool checked)
{
    qDebug() << "Show Targets Traj.:" << checked;
}
