#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QTime>
#include <QTimer>

class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = nullptr);
    ~ControlPanel();

    void updateTime(const QString &time);
    void setStatus(const QString &status);

signals:
    void startClicked();
    void stopClicked();
    void showWeatherConditionsChanged(bool checked);
    void showDTEDAreasChanged(bool checked);

private slots:
    void onStartClicked();
    void onStopClicked();
    void onShowDTEDAreasChanged(bool checked);
    void onShowWeatherConditionsChanged(bool checked);
    void onShowTargetsTrajChanged(bool checked);

private:
    void setupUI();
    void createCheckboxes();
    void createTimeDisplay();
    void createControlButtons();
    void createStatusDisplay();

    // UI bileşenleri
    QHBoxLayout *mainLayout;
    
    // Checkbox'lar
    QCheckBox *showDTEDAreasCheckBox;
    QCheckBox *showWeatherConditionsCheckBox;
    QCheckBox *showTargetsTrajCheckBox;
    
    // Zaman gösterimi
    QLabel *timeLabel;
    QLabel *timeDisplay;
    
    // Kontrol butonları
    QPushButton *startButton;
    QPushButton *stopButton;
    
    // Durum gösterimi
    QLabel *statusLabel;
    QLabel *statusDisplay;
    
    bool isRunning;
};

#endif // CONTROLPANEL_H
