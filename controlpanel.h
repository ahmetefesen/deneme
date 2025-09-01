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
#include <QSpinBox>
#include <QMap>
#include "mapwidget.h"

class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = nullptr);
    ~ControlPanel();

    void setStatus(const QString &status);
    void addTarget(const Target &target);
    void removeTarget(const QString &targetName);
    void clearTargets();

signals:
    void startClicked();
    void stopClicked();
    void showWeatherConditionsChanged(bool checked);
    void showDTEDAreasChanged(bool checked);
    void showTargetsTrajChanged(bool checked);
    void targetPositionUpdated(const QString &targetName, double lat, double lon, double alt);
    void radarPositionUpdated(double lat, double lon, double alt);
    void namedRadarPositionUpdated(const QString &radarName, double lat, double lon, double alt);

private slots:
    void onStartClicked();
    void onStopClicked();
    void onShowDTEDAreasChanged(bool checked);
    void onShowWeatherConditionsChanged(bool checked);
    void onShowTargetsTrajChanged(bool checked);
    void updateElapsedTime();
    void updateTargetPositions();

public:
    void setRadarInitialKinematics(double lat, double lon, double alt, double velN, double velE, double velD);
    void setRadarRoute(const QVector<RadarRouteWaypoint> &route);
    void setTargetRoute(const QString &targetName, const QVector<RadarRouteWaypoint> &route);
    int hz() const; // current Hz at start
    bool calculateWeatherEnabled() const { return false; }
    bool showTargetsTrajEnabled() const { return showTargetsTrajCheckBox ? showTargetsTrajCheckBox->isChecked() : false; }

    // Multi-radar API
    void addRadarProfile(const QString &name, double lat, double lon, double alt, double velN, double velE, double velD, const QVector<RadarRouteWaypoint> &route);

    // UI mode
    void setRunningUI(bool running);

private:
    void setupUI();
    void createCheckboxes();
    void createHzControl();
    void createControlButtons();
    void createStatusDisplay();
    void createElapsedTimeDisplay();
    void updateTargetPosition(const Target &target, double deltaTime);
    void updateRadarPosition(double deltaTime);
    bool advanceTowardsWaypoint(double curLat, double curLon, double curAlt,
                                const RadarRouteWaypoint &wp,
                                double velN, double velE, double velD,
                                double deltaTime,
                                double &outLat, double &outLon, double &outAlt);

    // UI bileşenleri
    QHBoxLayout *mainLayout;
    
    // Checkbox'lar
    QCheckBox *showDTEDAreasCheckBox;
    QCheckBox *showWeatherConditionsCheckBox;
    QCheckBox *showTargetsTrajCheckBox;
    QCheckBox *calculateWeatherCheck;

    // Grup kutuları referansları (gizle/göster için)
    QGroupBox *checkboxGroup;
    QGroupBox *hzGroup;
    QGroupBox *controlGroup;
    QGroupBox *statusGroup;
    QGroupBox *elapsedTimeGroup;
    bool configGroupsRemoved{false};
    
    // Hz kontrolü
    QLabel *hzLabel;
    QSpinBox *hzSpinBox;
    
    // Kontrol butonları
    QPushButton *startButton;
    QPushButton *stopButton;
    
    // Durum gösterimi
    QLabel *statusLabel;
    QLabel *statusDisplay;
    
    // Geçen süre gösterimi
    QLabel *elapsedTimeLabel;
    QLabel *elapsedTimeDisplay;
    
    bool isRunning;
    QTimer *elapsedTimeTimer;
    QTimer *simulationTimer;  // Target simulation için
    QTime startTime;
    int currentHz;
    
    // Target simulation için
    QMap<QString, Target> targets;
    double simulationTime;  // Simülasyon süresi (saniye)

    struct TargetState {
        double X{0.0};
        double Y{0.0};
        double Z{0.0};
        double lat{0.0};
        double lon{0.0};
        double alt{0.0};
        double velN{0.0};
        double velE{0.0};
        double velD{0.0};
        QVector<RadarRouteWaypoint> route;
        int nextWpIndex{0};
    };
    QMap<QString, TargetState> targetStates;

    // Radar kinematikleri
    struct RadarKin {
        double lat{0.0};
        double lon{0.0};
        double alt{0.0};
        double velN{0.0};
        double velE{0.0};
        double velD{0.0};
        QVector<RadarRouteWaypoint> route;
        int nextWpIndex{0};
        bool initialized{false};
    } radar;

    QMap<QString, RadarKin> radars; // multi-radar

    double waypointArriveThresholdMeters{10.0};
};

#endif // CONTROLPANEL_H
