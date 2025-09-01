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
#include <QSpinBox>
#include <QMessageBox>
#include "geo.h"

ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent)
    , isRunning(false)
    , currentHz(10)
    , simulationTime(0.0)
{
    setFixedHeight(160);
    setupUI();
    if (mainLayout) { mainLayout->setContentsMargins(10, 6, 10, 6); mainLayout->setSpacing(20); }
    
    // Timer'ları başlat
    elapsedTimeTimer = new QTimer(this);
    connect(elapsedTimeTimer, &QTimer::timeout, this, &ControlPanel::updateElapsedTime);
    
    // Simulation timer'ı başlat
    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &ControlPanel::updateTargetPositions);
}

ControlPanel::~ControlPanel()
{
}

void ControlPanel::setupUI()
{
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 4, 10, 4);
    mainLayout->setSpacing(10);
    
    createCheckboxes();
    createHzControl();
    createControlButtons();
    createStatusDisplay();
    createElapsedTimeDisplay();
}

void ControlPanel::createCheckboxes()
{
    checkboxGroup = new QGroupBox("Display Options");
    QVBoxLayout *checkboxLayout = new QVBoxLayout(checkboxGroup);
    checkboxLayout->setContentsMargins(8, 6, 8, 6);
    checkboxLayout->setSpacing(6);

    checkboxGroup->setMinimumWidth(260);
    checkboxGroup->setStyleSheet(
        "QGroupBox{font-size:12px; font-weight:bold;}"
        "QCheckBox{font-size:11px;}"
    );

    showDTEDAreasCheckBox = new QCheckBox("Show DTED Areas");
    showDTEDAreasCheckBox->setChecked(false);
    showDTEDAreasCheckBox->setMinimumHeight(20);
    showDTEDAreasCheckBox->setStyleSheet("QCheckBox{padding:2px 6px;}");
    checkboxLayout->addWidget(showDTEDAreasCheckBox);
    
    showWeatherConditionsCheckBox = new QCheckBox("Show Weather Conditions");
    showWeatherConditionsCheckBox->setChecked(false);
    showWeatherConditionsCheckBox->setMinimumHeight(20);
    showWeatherConditionsCheckBox->setStyleSheet("QCheckBox{padding:2px 6px;}");
    checkboxLayout->addWidget(showWeatherConditionsCheckBox);
    
    showTargetsTrajCheckBox = new QCheckBox("Show Targets Traj");
    showTargetsTrajCheckBox->setChecked(false);
    showTargetsTrajCheckBox->setMinimumHeight(20);
    showTargetsTrajCheckBox->setStyleSheet("QCheckBox{padding:2px 6px;}");
    checkboxLayout->addWidget(showTargetsTrajCheckBox);

    // removed: calculateWeatherCheck moved to Sidebar Atmosphere
    
    mainLayout->addWidget(checkboxGroup);
    
    // Bağlantılar
    connect(showDTEDAreasCheckBox, &QCheckBox::toggled, this, &ControlPanel::onShowDTEDAreasChanged);
    connect(showWeatherConditionsCheckBox, &QCheckBox::toggled, this, &ControlPanel::onShowWeatherConditionsChanged);
    connect(showTargetsTrajCheckBox, &QCheckBox::toggled, this, &ControlPanel::onShowTargetsTrajChanged);
}

void ControlPanel::createHzControl()
{
    hzGroup = new QGroupBox("Hz Control");
    QVBoxLayout *hzLayout = new QVBoxLayout(hzGroup);
    hzGroup->setMinimumWidth(220);
    
    hzLabel = new QLabel("Hz Value:");
    hzLabel->setAlignment(Qt::AlignCenter);
    hzLayout->addWidget(hzLabel);
    
    hzSpinBox = new QSpinBox();
    hzSpinBox->setRange(1, 100);
    hzSpinBox->setValue(currentHz);
    hzSpinBox->setSuffix(" Hz");
    hzSpinBox->setStyleSheet(
        "QSpinBox {"
        "    background-color: #2a2a2a;"
        "    color: #00ff00;"
        "    padding: 2px;"
        "    border: 1px solid #555;"
        "    border-radius: 3px;"
        "    font-weight: bold;"
        "}"
    );
    hzLayout->addWidget(hzSpinBox);
    
    mainLayout->addWidget(hzGroup);
}



void ControlPanel::createControlButtons()
{
    controlGroup = new QGroupBox("Controls");
    QHBoxLayout *controlLayout = new QHBoxLayout(controlGroup);
    controlGroup->setMinimumWidth(240);
    
    startButton = new QPushButton("Start");
    startButton->setMinimumSize(72, 26);
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
    stopButton->setMinimumSize(68, 24);
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
    statusGroup = new QGroupBox("Status");
    QVBoxLayout *statusLayout = new QVBoxLayout(statusGroup);
    statusGroup->setMinimumWidth(220);
    
    statusLabel = new QLabel("Status:");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLayout->addWidget(statusLabel);
    
    statusDisplay = new QLabel("Paused");
    statusDisplay->setAlignment(Qt::AlignCenter);
    statusDisplay->setFont(QFont("Arial", 10, QFont::Bold));
    statusDisplay->setStyleSheet("QLabel { background-color: #2a2a2a; color: #ffc107; padding: 3px; border: 1px solid #555; border-radius: 3px; }");
    statusLayout->addWidget(statusDisplay);
    
    mainLayout->addWidget(statusGroup);
}

void ControlPanel::createElapsedTimeDisplay()
{
    elapsedTimeGroup = new QGroupBox("Elapsed Time");
    QVBoxLayout *elapsedTimeLayout = new QVBoxLayout(elapsedTimeGroup);
    elapsedTimeGroup->setMinimumWidth(220);
    
    elapsedTimeLabel = new QLabel("Time:");
    elapsedTimeLabel->setAlignment(Qt::AlignCenter);
    elapsedTimeLayout->addWidget(elapsedTimeLabel);
    
    elapsedTimeDisplay = new QLabel("00:00:00");
    elapsedTimeDisplay->setAlignment(Qt::AlignCenter);
    elapsedTimeDisplay->setFont(QFont("Menlo", 12, QFont::Bold));
    elapsedTimeDisplay->setStyleSheet("QLabel { background-color: #2a2a2a; color: #00ff00; padding: 3px; border: 1px solid #555; border-radius: 3px; }");
    elapsedTimeLayout->addWidget(elapsedTimeDisplay);
    
    mainLayout->addWidget(elapsedTimeGroup);
}

void ControlPanel::setRunningUI(bool running)
{
    qDebug() << "setRunningUI running=" << running;
    if (running) {
        if (!configGroupsRemoved) {
            if (checkboxGroup) {
                mainLayout->removeWidget(checkboxGroup);
                checkboxGroup->hide();
                checkboxGroup->setParent(nullptr);
            }
            if (hzGroup) {
                mainLayout->removeWidget(hzGroup);
                hzGroup->hide();
                hzGroup->setParent(nullptr);
            }
            configGroupsRemoved = true;
        }
        setFixedHeight(100);
    } else {
        if (configGroupsRemoved) {
            if (checkboxGroup && checkboxGroup->parent() == nullptr) {
                checkboxGroup->setParent(this);
                checkboxGroup->show();
                mainLayout->insertWidget(0, checkboxGroup);
            }
            if (hzGroup && hzGroup->parent() == nullptr) {
                hzGroup->setParent(this);
                hzGroup->show();
                mainLayout->insertWidget(1, hzGroup);
            }
            configGroupsRemoved = false;
        }
        setFixedHeight(160);
    }
    mainLayout->invalidate();
    mainLayout->activate();
    updateGeometry();
    adjustSize();
}

void ControlPanel::onStartClicked()
{
    isRunning = true;
    startButton->setEnabled(false);
    stopButton->setEnabled(true);
    
    // Süre hesaplamayı başlat
    startTime = QTime::currentTime();
    currentHz = hzSpinBox->value();
    simulationTime = 0.0;
    
    // Hz değerine göre timer'ları başlat
    int interval = 1000 / currentHz;
    // Elapsed time artık Hz'le aynı hızda güncellensin ve simulationTime göstersin
    elapsedTimeTimer->start(interval);
    simulationTimer->start(interval);
    
    setRunningUI(true);
    
    emit startClicked();
}

void ControlPanel::onStopClicked()
{
    isRunning = false;
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
    
    // Timer'ları durdur
    elapsedTimeTimer->stop();
    simulationTimer->stop();
    
    setRunningUI(false);
    
    emit stopClicked();
}

void ControlPanel::updateElapsedTime()
{
    if (isRunning) {
        // simulationTime'i göster (Hz'e bağlı artıyor)
        int totalSeconds = static_cast<int>(simulationTime);
        int hours = totalSeconds / 3600;
        int minutes = (totalSeconds % 3600) / 60;
        int seconds = totalSeconds % 60;
        QString timeStr = QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
        elapsedTimeDisplay->setText(timeStr);
    }
}

void ControlPanel::updateTargetPositions()
{
    if (!isRunning) return;
    
    // Hz değerine göre delta time hesapla
    double deltaTime = 1.0 / currentHz;
    simulationTime += deltaTime;
    
    // Radar hareketini güncelle (tekil)
    updateRadarPosition(deltaTime);

    // Çoklu radarlar
    for (auto it = radars.begin(); it != radars.end(); ++it) {
        RadarKin &rk = it.value();
        if (!rk.initialized) continue;
        if (rk.nextWpIndex < rk.route.size()) {
            const auto &wp = rk.route[rk.nextWpIndex];
            double newLat, newLon, newAlt;
            bool arrived = advanceTowardsWaypoint(rk.lat, rk.lon, rk.alt, wp,
                                                  rk.velN, rk.velE, rk.velD,
                                                  deltaTime, newLat, newLon, newAlt);
            rk.lat = newLat; rk.lon = newLon; rk.alt = newAlt;
            emit namedRadarPositionUpdated(it.key(), rk.lat, rk.lon, rk.alt);
            if (arrived) { rk.velN = wp.velN; rk.velE = wp.velE; rk.velD = wp.velD; rk.nextWpIndex++; }
        } else {
            double newLat, newLon, newAlt; RadarRouteWaypoint dummy{rk.lat, rk.lon, rk.alt, 0,0,0};
            advanceTowardsWaypoint(rk.lat, rk.lon, rk.alt, dummy, rk.velN, rk.velE, rk.velD, deltaTime, newLat, newLon, newAlt);
            rk.lat = newLat; rk.lon = newLon; rk.alt = newAlt;
            emit namedRadarPositionUpdated(it.key(), rk.lat, rk.lon, rk.alt);
        }
    }
    
    // Tüm target'ların pozisyonlarını güncelle
    for (auto it = targets.begin(); it != targets.end(); ++it) {
        updateTargetPosition(it.value(), deltaTime);
    }
}

bool ControlPanel::advanceTowardsWaypoint(double curLat, double curLon, double curAlt,
                                const RadarRouteWaypoint &wp,
                                double velN, double velE, double velD,
                                double deltaTime,
                                double &outLat, double &outLon, double &outAlt)
{
    // ENU adımını uygula
    double dE = velE * deltaTime;
    double dN = velN * deltaTime;
    double dU = -velD * deltaTime; // D aşağı (+) kabul: u = -D
    double Xn, Yn, Zn;
    Geo::enuToECEF(dE, dN, dU, curLat, curLon, curAlt, Xn, Yn, Zn);
    double nlat, nlon, nalt;
    Geo::ecefToGeodetic(Xn, Yn, Zn, nlat, nlon, nalt);

    // Varış kontrolü (yakınsaklık): mevcut nokta ile hedef WP arasındaki mesafe < threshold?
    double Xe, Ye, Ze, Xw, Yw, Zw;
    Geo::geodeticToECEF(nlat, nlon, nalt, Xe, Ye, Ze);
    Geo::geodeticToECEF(wp.lat, wp.lon, wp.alt, Xw, Yw, Zw);
    double dist = std::sqrt((Xe-Xw)*(Xe-Xw) + (Ye-Yw)*(Ye-Yw) + (Ze-Zw)*(Ze-Zw));

    outLat = nlat; outLon = nlon; outAlt = nalt;
    return dist <= waypointArriveThresholdMeters;
}

void ControlPanel::updateRadarPosition(double deltaTime)
{
    if (!radar.initialized) return;

    // Hedef WP var ise
    if (radar.nextWpIndex < radar.route.size()) {
        const auto &wp = radar.route[radar.nextWpIndex];
        double newLat, newLon, newAlt;
        bool arrived = advanceTowardsWaypoint(radar.lat, radar.lon, radar.alt,
                                              wp, radar.velN, radar.velE, radar.velD,
                                              deltaTime, newLat, newLon, newAlt);
        radar.lat = newLat; radar.lon = newLon; radar.alt = newAlt;
        emit radarPositionUpdated(radar.lat, radar.lon, radar.alt);
        if (arrived) {
            // WP hızlarını devral
            radar.velN = wp.velN; radar.velE = wp.velE; radar.velD = wp.velD;
            radar.nextWpIndex++;
        }
    } else {
        // Son hızla devam
        double newLat, newLon, newAlt;
        RadarRouteWaypoint dummy{radar.lat, radar.lon, radar.alt, 0,0,0};
        advanceTowardsWaypoint(radar.lat, radar.lon, radar.alt, dummy,
                               radar.velN, radar.velE, radar.velD,
                               deltaTime, newLat, newLon, newAlt);
        radar.lat = newLat; radar.lon = newLon; radar.alt = newAlt;
        emit radarPositionUpdated(radar.lat, radar.lon, radar.alt);
    }
}

void ControlPanel::updateTargetPosition(const Target &target, double deltaTime)
{
    // ECEF state yoksa başlat
    if (!targetStates.contains(target.name)) {
        TargetState st;
        st.lat = target.initLatitude;
        st.lon = target.initLongitude;
        st.alt = target.initAltitude;
        st.velN = target.initVelocityN;
        st.velE = target.initVelocityE;
        st.velD = target.initVelocityD;
        Geo::geodeticToECEF(st.lat, Geo::wrapLon(st.lon), st.alt, st.X, st.Y, st.Z);
        targetStates[target.name] = st;
    }
    TargetState &st = targetStates[target.name];

    // Waypoint varsa, sıradaki WP'ye doğru ilerle
    if (st.nextWpIndex < st.route.size()) {
        const auto &wp = st.route[st.nextWpIndex];
        double newLat, newLon, newAlt;
        bool arrived = advanceTowardsWaypoint(st.lat, st.lon, st.alt, wp,
                                              st.velN, st.velE, st.velD,
                                              deltaTime, newLat, newLon, newAlt);
        st.lat = newLat; st.lon = newLon; st.alt = newAlt;
        emit targetPositionUpdated(target.name, st.lat, st.lon, st.alt);
        if (arrived) {
            // WP hızlarını devral
            st.velN = wp.velN; st.velE = wp.velE; st.velD = wp.velD;
            st.nextWpIndex++;
        }
    } else {
        // Son hızla devam
        double newLat, newLon, newAlt;
        RadarRouteWaypoint dummy{st.lat, st.lon, st.alt, 0,0,0};
        advanceTowardsWaypoint(st.lat, st.lon, st.alt, dummy,
                               st.velN, st.velE, st.velD,
                               deltaTime, newLat, newLon, newAlt);
        st.lat = newLat; st.lon = newLon; st.alt = newAlt;
        emit targetPositionUpdated(target.name, st.lat, st.lon, st.alt);
    }
}

void ControlPanel::addTarget(const Target &target)
{
    targets[target.name] = target;
    // ECEF init
    TargetState st;
    st.lat = target.initLatitude;
    st.lon = target.initLongitude;
    st.alt = target.initAltitude;
    Geo::geodeticToECEF(st.lat, Geo::wrapLon(st.lon), st.alt, st.X, st.Y, st.Z);
    targetStates[target.name] = st;
    qDebug() << "Added target:" << target.name << "to simulation";
}

void ControlPanel::removeTarget(const QString &targetName)
{
    if (targets.remove(targetName) > 0) {
        targetStates.remove(targetName);
        qDebug() << "Removed target:" << targetName << "from simulation";
    }
}

void ControlPanel::clearTargets()
{
    targets.clear();
    targetStates.clear();
    qDebug() << "Cleared all targets from simulation";
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
    emit showTargetsTrajChanged(checked);
}

void ControlPanel::setRadarInitialKinematics(double lat, double lon, double alt, double velN, double velE, double velD)
{
    radar.lat = lat; radar.lon = lon; radar.alt = alt;
    radar.velN = velN; radar.velE = velE; radar.velD = velD;
    radar.initialized = true;
}

void ControlPanel::setRadarRoute(const QVector<RadarRouteWaypoint> &route)
{
    radar.route = route;
    radar.nextWpIndex = 0;
}

void ControlPanel::setTargetRoute(const QString &targetName, const QVector<RadarRouteWaypoint> &route)
{
    if (!targetStates.contains(targetName)) {
        TargetState st; targetStates[targetName] = st;
    }
    targetStates[targetName].route = route;
    targetStates[targetName].nextWpIndex = 0;
}

int ControlPanel::hz() const
{
    return hzSpinBox ? hzSpinBox->value() : currentHz;
}

void ControlPanel::addRadarProfile(const QString &name, double lat, double lon, double alt, double velN, double velE, double velD, const QVector<RadarRouteWaypoint> &route)
{
    RadarKin rk; rk.lat = lat; rk.lon = lon; rk.alt = alt; rk.velN = velN; rk.velE = velE; rk.velD = velD; rk.route = route; rk.nextWpIndex = 0; rk.initialized = true;
    radars[name] = rk;
}

void ControlPanel::setStatus(const QString &status)
{
    if (!statusDisplay) return;
    statusDisplay->setText(status);
    if (status.compare("Running", Qt::CaseInsensitive) == 0) {
        statusDisplay->setStyleSheet("QLabel { background-color: #2a2a2a; color: #28a745; padding: 5px; border: 1px solid #555; border-radius: 3px; }");
    } else {
        statusDisplay->setStyleSheet("QLabel { background-color: #2a2a2a; color: #ffc107; padding: 5px; border: 1px solid #555; border-radius: 3px; }");
    }
}
