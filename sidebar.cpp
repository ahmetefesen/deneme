#include "sidebar.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QTreeWidget>
#include <QSlider>
#include <QProgressBar>
#include <QFormLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QRegularExpression>
#include <QHeaderView>
#include <QTabBar>

static QList<Sidebar::RadarProfile> s_radarProfiles;

Sidebar::Sidebar(QWidget *parent)
    : QWidget(parent)
{
    setMinimumWidth(300);
    setMaximumWidth(400);
    setupUI();
}

Sidebar::~Sidebar()
{
}

void Sidebar::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(5);
    
    // Tab widget oluştur
    tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);
    
    // Tab'ları oluştur
    createGeneralTab();
    createAdvancedPropertiesTab();
    createTerrainTab();
    createAtmosphereTab();
    createRadarTab();
    createTargetTab();
    
    // Tab'ları ekle
    tabWidget->addTab(generalTab, "General");
    tabWidget->addTab(advancedPropertiesTab, "Advanced Propagations");
    tabWidget->addTab(terrainTab, "Terrain");
    tabWidget->addTab(atmosphereTab, "Atmosphere");
    tabWidget->addTab(radarTab, "Radar");
    tabWidget->addTab(targetTab, "Target");
    
    connect(tabWidget, &QTabWidget::currentChanged, this, &Sidebar::onTabChanged);

    // Tab bar scroll butonlarını açık tutuyoruz, genişliği kilitlemiyoruz
    if (QTabBar *tb = tabWidget->tabBar()) {
        tb->setExpanding(false);
        tb->setUsesScrollButtons(true);
        tabWidget->setElideMode(Qt::ElideNone);
    }
}

int Sidebar::preferredWidthForFirstTabs(int count) const
{
    if (!tabWidget || !tabWidget->tabBar()) return 300;
    QTabBar *tb = tabWidget->tabBar();
    int w = 0;
    for (int i = 0; i < count && i < tb->count(); ++i) {
        w += tb->tabRect(i).width();
    }
    return qMax(300, w + 24);
}

void Sidebar::createGeneralTab()
{
    generalTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(generalTab);

    generalSubTabs = new QTabWidget(generalTab);
    layout->addWidget(generalSubTabs);

    // -------- Initial Alt Sekmesi --------
    generalInitialTab = new QWidget();
    QVBoxLayout *mainLay = new QVBoxLayout(generalInitialTab);

    // Active radar label + name
    QHBoxLayout *hdr = new QHBoxLayout();
    activeRadarLabel = new QLabel(QString("Active Radar: #%1").arg(radarProfileCounter));
    radarNameEdit = new QLineEdit(QString("Radar %1").arg(radarProfileCounter));
    radarNameEdit->setPlaceholderText("Radar name");
    hdr->addWidget(activeRadarLabel, 0);
    hdr->addStretch(1);
    hdr->addWidget(new QLabel("Name:"));
    hdr->addWidget(radarNameEdit, 1);
    mainLay->addLayout(hdr);

    // Radar Section
    QGroupBox *radarGroup = new QGroupBox("Radar");
    QFormLayout *radarLayout = new QFormLayout(radarGroup);
    QLineEdit *radarName = new QLineEdit();
    radarName->setPlaceholderText("Enter radar name");
    radarLayout->addRow("Radar Name:", radarName);
    mainLay->addWidget(radarGroup);

    // Initial Position Section
    QGroupBox *initialPositionGroup = new QGroupBox("Initial Position");
    QFormLayout *initialPositionLayout = new QFormLayout(initialPositionGroup);
    generalLatSpin = new QDoubleSpinBox();
    generalLatSpin->setRange(-90.0, 90.0);
    generalLatSpin->setValue(47.93943);
    generalLatSpin->setDecimals(6);
    generalLatSpin->setSuffix(" Deg");
    initialPositionLayout->addRow("Latitude (N):", generalLatSpin);
    generalLonSpin = new QDoubleSpinBox();
    generalLonSpin->setRange(-180.0, 180.0);
    generalLonSpin->setValue(3.68785);
    generalLonSpin->setDecimals(6);
    generalLonSpin->setSuffix(" Deg");
    initialPositionLayout->addRow("Longitude (W):", generalLonSpin);
    generalAltSpin = new QDoubleSpinBox();
    generalAltSpin->setRange(0.0, 50000.0);
    generalAltSpin->setValue(1000.0);
    generalAltSpin->setSuffix(" m");
    initialPositionLayout->addRow("Altitude:", generalAltSpin);
    generalRcsSpin = new QDoubleSpinBox();
    generalRcsSpin->setRange(-50.0, 50.0);
    generalRcsSpin->setValue(10.0);
    generalRcsSpin->setSuffix(" dBsm");
    initialPositionLayout->addRow("RCS:", generalRcsSpin);
    generalVelNSpin = new QDoubleSpinBox();
    generalVelNSpin->setRange(-1000.0, 1000.0);
    generalVelNSpin->setValue(0.0);
    generalVelNSpin->setSuffix(" mps");
    initialPositionLayout->addRow("Velocity N:", generalVelNSpin);
    generalVelESpin = new QDoubleSpinBox();
    generalVelESpin->setRange(-1000.0, 1000.0);
    generalVelESpin->setValue(0.0);
    generalVelESpin->setSuffix(" mps");
    initialPositionLayout->addRow("Velocity E:", generalVelESpin);
    generalVelDSpin = new QDoubleSpinBox();
    generalVelDSpin->setRange(-1000.0, 1000.0);
    generalVelDSpin->setValue(0.0);
    generalVelDSpin->setSuffix(" mps");
    initialPositionLayout->addRow("Velocity D:", generalVelDSpin);

    mainLay->addWidget(initialPositionGroup);

    // Initial position değişikliklerini dinle
    connect(generalLatSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Sidebar::onInitialPositionChanged);
    connect(generalLonSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Sidebar::onInitialPositionChanged);
    connect(generalAltSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Sidebar::onInitialPositionChanged);

    mainLay->addStretch();

    // -------- Route Alt Sekmesi --------
    generalRouteTab = new QWidget();
    QVBoxLayout *routeLay = new QVBoxLayout(generalRouteTab);
    QHBoxLayout *top = new QHBoxLayout();
    QLabel *routeTitle = new QLabel("Radar's Route");
    routeTitle->setFont(QFont("Arial", 10, QFont::Bold));
    top->addWidget(routeTitle, 1);
    routeLay->addLayout(top);

    QGroupBox *wpGroup = new QGroupBox("Waypoints");
    QVBoxLayout *wpLay = new QVBoxLayout(wpGroup);
    radarWaypointList = new QListWidget();
    radarWaypointList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    radarWaypointList->setTextElideMode(Qt::ElideNone);
    radarWaypointList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    radarWaypointList->setResizeMode(QListView::Adjust);
    radarWaypointList->setMaximumHeight(180);
    wpLay->addWidget(radarWaypointList);

    QFormLayout *wpForm = new QFormLayout();
    radarWpLatSpin = new QDoubleSpinBox();
    radarWpLatSpin->setRange(-90.0, 90.0);
    radarWpLatSpin->setDecimals(6);
    radarWpLatSpin->setSuffix(" Deg");
    wpForm->addRow("Latitude (N):", radarWpLatSpin);
    radarWpLonSpin = new QDoubleSpinBox();
    radarWpLonSpin->setRange(-180.0, 180.0);
    radarWpLonSpin->setDecimals(6);
    radarWpLonSpin->setSuffix(" Deg");
    wpForm->addRow("Longitude (W):", radarWpLonSpin);
    radarWpAltSpin = new QDoubleSpinBox();
    radarWpAltSpin->setRange(0.0, 50000.0);
    radarWpAltSpin->setValue(1000.0);
    radarWpAltSpin->setSuffix(" m");
    wpForm->addRow("Altitude:", radarWpAltSpin);
    radarWpVelNSpin = new QDoubleSpinBox();
    radarWpVelNSpin->setRange(-1000.0, 1000.0);
    radarWpVelNSpin->setValue(0.0);
    radarWpVelNSpin->setSuffix(" mps");
    wpForm->addRow("Velocity N:", radarWpVelNSpin);
    radarWpVelESpin = new QDoubleSpinBox();
    radarWpVelESpin->setRange(-1000.0, 1000.0);
    radarWpVelESpin->setValue(0.0);
    radarWpVelESpin->setSuffix(" mps");
    wpForm->addRow("Velocity E:", radarWpVelESpin);
    radarWpVelDSpin = new QDoubleSpinBox();
    radarWpVelDSpin->setRange(-1000.0, 1000.0);
    radarWpVelDSpin->setValue(0.0);
    radarWpVelDSpin->setSuffix(" mps");
    wpForm->addRow("Velocity D:", radarWpVelDSpin);
    wpLay->addLayout(wpForm);

    QHBoxLayout *wpBtns = new QHBoxLayout();
    radarAppendBtn = new QPushButton("Append");
    radarDeleteBtn = new QPushButton("Delete");
    wpBtns->addWidget(radarAppendBtn);
    wpBtns->addWidget(radarDeleteBtn);
    wpLay->addLayout(wpBtns);
    routeLay->addWidget(wpGroup);
    routeLay->addStretch();

    // Alt sekmeleri ekle
    generalSubTabs->addTab(generalInitialTab, "Initial");
    generalSubTabs->addTab(generalRouteTab, "Route");

    // Connections (Route sekmesi)
    connect(radarAppendBtn, &QPushButton::clicked, this, &Sidebar::onAppendRadarWaypoint);
    connect(radarDeleteBtn, &QPushButton::clicked, this, &Sidebar::onDeleteRadarWaypoint);

    // Route sekmesi seçildiğinde tabloyu yenile ve durumu yayınla
    connect(generalSubTabs, &QTabWidget::currentChanged, this, [this](int idx){
        if (generalSubTabs->widget(idx) == generalRouteTab) {
            radarWaypointList->clear();
            for (const auto &rw : radarWaypointsData) {
                QString wp = QString("Lat=%1, Lon=%2, Alt=%3m, VelN=%4, VelE=%5, VelD=%6")
                    .arg(rw.lat, 0, 'f', 6)
                    .arg(rw.lon, 0, 'f', 6)
                    .arg(rw.alt, 0, 'f', 1)
                    .arg(rw.velN, 0, 'f', 1)
                    .arg(rw.velE, 0, 'f', 1)
                    .arg(rw.velD, 0, 'f', 1);
                radarWaypointList->addItem(wp);
            }
            QVector<QPair<double,double>> latLon;
            latLon.push_back({ generalLatSpin->value(), generalLonSpin->value() });
            for (const auto &rw : radarWaypointsData) latLon.push_back({ rw.lat, rw.lon });
            emit radarRouteChanged(latLon);
        }
    });
}

void Sidebar::createAdvancedPropertiesTab()
{
    advancedPropertiesTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(advancedPropertiesTab);
    
    // Advanced Propagation Section
    QGroupBox *propagationGroup = new QGroupBox("Advanced Propagation");
    QFormLayout *propagationLayout = new QFormLayout(propagationGroup);
    
    QComboBox *advancedPropagation = new QComboBox();
    advancedPropagation->addItem("Radar is not moving: Calculate Propagation Factor (PPF) 360 degrees for once");
    advancedPropagation->addItem("Radar is moving: Calculate Propagation Factor (PPF) 360 degree for step");
    advancedPropagation->addItem("Radar is moving: Calculate Propagation Factor (PPF) only target direction");
    advancedPropagation->addItem("Skip: Path Propagation Factor (PPF) calculation");
    advancedPropagation->addItem("Skip: Path Propagation Factor (PPF) and Terrain calculation");
    propagationLayout->addRow("Advanced Propagation:", advancedPropagation);
    
    layout->addWidget(propagationGroup);
    
    // Calculation Parameters Section
    QGroupBox *calcGroup = new QGroupBox("Calculation Parameters");
    QFormLayout *calcLayout = new QFormLayout(calcGroup);
    
    QDoubleSpinBox *maxAltitude = new QDoubleSpinBox();
    maxAltitude->setRange(0.0, 50000.0);
    maxAltitude->setValue(10000.0);
    maxAltitude->setSuffix(" m");
    calcLayout->addRow("Max Altitude:", maxAltitude);
    
    QDoubleSpinBox *maxDistance = new QDoubleSpinBox();
    maxDistance->setRange(0.1, 1000.0);
    maxDistance->setValue(100.0);
    maxDistance->setSuffix(" km");
    calcLayout->addRow("Max Distance:", maxDistance);
    
    QDoubleSpinBox *altitudeStepSize = new QDoubleSpinBox();
    altitudeStepSize->setRange(1.0, 1000.0);
    altitudeStepSize->setValue(100.0);
    altitudeStepSize->setSuffix(" m");
    calcLayout->addRow("Altitude Step Size:", altitudeStepSize);
    
    QDoubleSpinBox *distanceStepSize = new QDoubleSpinBox();
    distanceStepSize->setRange(1.0, 10000.0);
    distanceStepSize->setValue(1000.0);
    distanceStepSize->setSuffix(" m");
    calcLayout->addRow("Distance Step Size:", distanceStepSize);
    
    layout->addWidget(calcGroup);
    
    // Ground Profile Section
    QGroupBox *groundGroup = new QGroupBox("Ground Profile");
    QFormLayout *groundLayout = new QFormLayout(groundGroup);
    
    QComboBox *groundProfile = new QComboBox();
    groundProfile->addItem("PEC Surface");
    groundProfile->addItem("Impedance Surface");
    groundLayout->addRow("Ground Profile:", groundProfile);
    
    QDoubleSpinBox *groundConductivity = new QDoubleSpinBox();
    groundConductivity->setRange(0.001, 100.0);
    groundConductivity->setValue(0.005);
    groundConductivity->setSuffix(" S/m");
    groundLayout->addRow("Ground Conductivity:", groundConductivity);
    
    QDoubleSpinBox *dielectricConstant = new QDoubleSpinBox();
    dielectricConstant->setRange(1.0, 100.0);
    dielectricConstant->setValue(15.0);
    dielectricConstant->setDecimals(2);
    groundLayout->addRow("Dielectric Constant:", dielectricConstant);
    
    layout->addWidget(groundGroup);
    
    // Terrain Profile Section
    QGroupBox *terrainProfileGroup = new QGroupBox("Terrain Profile");
    QFormLayout *terrainProfileLayout = new QFormLayout(terrainProfileGroup);
    
    QComboBox *terrainProfile = new QComboBox();
    terrainProfile->addItem("Rough Terrain");
    terrainProfile->addItem("Flat Terrain: not use DTED data");
    terrainProfileLayout->addRow("Terrain Profile:", terrainProfile);
    
    QDoubleSpinBox *halfBeamWidth = new QDoubleSpinBox();
    halfBeamWidth->setRange(0.1, 90.0);
    halfBeamWidth->setValue(1.0);
    halfBeamWidth->setDecimals(1);
    halfBeamWidth->setSuffix(" deg");
    terrainProfileLayout->addRow("Half Beam Width:", halfBeamWidth);
    
    layout->addWidget(terrainProfileGroup);
    
    layout->addStretch();
}

void Sidebar::createTerrainTab()
{
    terrainTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(terrainTab);
    
    // DTED Files List Section
    QGroupBox *dtedFilesGroup = new QGroupBox("DTED Files");
    QVBoxLayout *dtedFilesLayout = new QVBoxLayout(dtedFilesGroup);
    
    dtedFilesList = new QListWidget();
    dtedFilesList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    dtedFilesList->setTextElideMode(Qt::ElideNone);
    dtedFilesList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    dtedFilesList->setResizeMode(QListView::Adjust);
    dtedFilesList->setMaximumHeight(150);
    dtedFilesLayout->addWidget(dtedFilesList);
    
    // Browse button for DTED files
    QPushButton *browseDTEDButton = new QPushButton("Browse DTED Files");
    dtedFilesLayout->addWidget(browseDTEDButton);
    
    layout->addWidget(dtedFilesGroup);
    
    // DTED File Properties Section
    QGroupBox *dtedPropertiesGroup = new QGroupBox("DTED File Properties");
    QFormLayout *dtedPropertiesLayout = new QFormLayout(dtedPropertiesGroup);
    
    QComboBox *dtedLevelComboBox = new QComboBox();
    dtedLevelComboBox->addItems({"DTED-0", "DTED-1", "DTED-2"});
    dtedPropertiesLayout->addRow("DTED Level:", dtedLevelComboBox);
    
    QDoubleSpinBox *terrainResolutionSpinBox = new QDoubleSpinBox();
    terrainResolutionSpinBox->setRange(1.0, 1000.0);
    terrainResolutionSpinBox->setValue(30.0);
    terrainResolutionSpinBox->setSuffix(" m");
    dtedPropertiesLayout->addRow("Resolution:", terrainResolutionSpinBox);
    
    layout->addWidget(dtedPropertiesGroup);
    
    // DTED File Options Section
    QGroupBox *dtedOptionsGroup = new QGroupBox("DTED File Options");
    QVBoxLayout *dtedOptionsLayout = new QVBoxLayout(dtedOptionsGroup);
    
    // Checkbox'lar her DTED dosyası için
    dtedOptionsLayout->addWidget(new QLabel("Select DTED files to display on map:"));
    
    layout->addWidget(dtedOptionsGroup);
    
    // Connect the browse button
    connect(browseDTEDButton, &QPushButton::clicked, this, &Sidebar::onBrowseDTEDFiles);
    
    layout->addStretch();
}

void Sidebar::createAtmosphereTab()
{
    atmosphereTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(atmosphereTab);
    
    // Weather Condition List Section
    QGroupBox *weatherConditionGroup = new QGroupBox("Weather Condition");
    QVBoxLayout *weatherConditionLayout = new QVBoxLayout(weatherConditionGroup);

    // Calculate Weather (moved from ControlPanel)
    calculateWeatherCheckSB = new QCheckBox("Calculate Weather Conditions");
    calculateWeatherCheckSB->setChecked(false);
    weatherConditionLayout->addWidget(calculateWeatherCheckSB);
    
    weatherConditionList = new QListWidget();
    weatherConditionList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    weatherConditionList->setTextElideMode(Qt::ElideNone);
    weatherConditionList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    weatherConditionList->setResizeMode(QListView::Adjust);
    weatherConditionList->setMaximumHeight(100);
    weatherConditionLayout->addWidget(weatherConditionList);
    
    // Delete button for selected weather condition
    QPushButton *deleteWeatherCondition = new QPushButton("Delete Selected");
    weatherConditionLayout->addWidget(deleteWeatherCondition);
    
    layout->addWidget(weatherConditionGroup);
    
    // Connect the delete button
    connect(deleteWeatherCondition, &QPushButton::clicked, this, &Sidebar::onDeleteWeatherCondition);
    
    // Location Section
    QGroupBox *locationGroup = new QGroupBox("Location");
    QFormLayout *locationLayout = new QFormLayout(locationGroup);
    
    latitudeSpinBox = new QDoubleSpinBox();
    latitudeSpinBox->setRange(-90.0, 90.0);
    latitudeSpinBox->setValue(0.0);
    latitudeSpinBox->setDecimals(6);
    latitudeSpinBox->setSuffix(" Deg");
    locationLayout->addRow("Latitude (N):", latitudeSpinBox);
    
    longitudeSpinBox = new QDoubleSpinBox();
    longitudeSpinBox->setRange(-180.0, 180.0);
    longitudeSpinBox->setValue(0.0);
    longitudeSpinBox->setDecimals(6);
    longitudeSpinBox->setSuffix(" Deg");
    locationLayout->addRow("Longitude (W):", longitudeSpinBox);
    
    layout->addWidget(locationGroup);
    
    // Area Section
    QGroupBox *areaGroup = new QGroupBox("Area");
    QFormLayout *areaLayout = new QFormLayout(areaGroup);
    
    circleRadiusSpinBox = new QDoubleSpinBox();
    circleRadiusSpinBox->setRange(0.1, 1000.0);
    circleRadiusSpinBox->setValue(10.0);
    circleRadiusSpinBox->setDecimals(2);
    circleRadiusSpinBox->setSuffix(" km");
    areaLayout->addRow("Circle Radius:", circleRadiusSpinBox);
    
    layout->addWidget(areaGroup);
    
    // Weather Condition Profile Section
    QGroupBox *weatherProfileGroup = new QGroupBox("Weather Condition Profile");
    QFormLayout *weatherProfileLayout = new QFormLayout(weatherProfileGroup);
    
    conditionTypeComboBox = new QComboBox();
    conditionTypeComboBox->addItem("Rain");
    conditionTypeComboBox->addItem("Fog&Cloud");
    weatherProfileLayout->addRow("Condition:", conditionTypeComboBox);
    
    // Rain specific fields
    rainRateSpinBox = new QDoubleSpinBox();
    rainRateSpinBox->setRange(0.1, 100.0);
    rainRateSpinBox->setValue(5.0);
    rainRateSpinBox->setDecimals(1);
    rainRateSpinBox->setSuffix(" mm/h");
    weatherProfileLayout->addRow("Rain Rate:", rainRateSpinBox);
    
    rainTemperatureSpinBox = new QDoubleSpinBox();
    rainTemperatureSpinBox->setRange(200.0, 350.0);
    rainTemperatureSpinBox->setValue(293.15);
    rainTemperatureSpinBox->setDecimals(2);
    rainTemperatureSpinBox->setSuffix(" K");
    weatherProfileLayout->addRow("Temperature:", rainTemperatureSpinBox);
    
    // Fog&Cloud specific fields
    fogVisibilitySpinBox = new QDoubleSpinBox();
    fogVisibilitySpinBox->setRange(1.0, 10000.0);
    fogVisibilitySpinBox->setValue(1000.0);
    fogVisibilitySpinBox->setDecimals(0);
    fogVisibilitySpinBox->setSuffix(" m");
    weatherProfileLayout->addRow("Fog&Cloud Visibility:", fogVisibilitySpinBox);
    
    fogTemperatureSpinBox = new QDoubleSpinBox();
    fogTemperatureSpinBox->setRange(200.0, 350.0);
    fogTemperatureSpinBox->setValue(293.15);
    fogTemperatureSpinBox->setDecimals(2);
    fogTemperatureSpinBox->setSuffix(" K");
    weatherProfileLayout->addRow("Temperature:", fogTemperatureSpinBox);
    
    layout->addWidget(weatherProfileGroup);
    
    // Add New Condition Button
    QPushButton *addNewCondition = new QPushButton("Add New Condition");
    layout->addWidget(addNewCondition);
    
    // Connect the add button
    connect(addNewCondition, &QPushButton::clicked, this, &Sidebar::onAddWeatherCondition);
    
    layout->addStretch();
}

void Sidebar::createRadarTab()
{
    radarTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(radarTab);
    layout->setContentsMargins(6, 6, 6, 6);
    layout->setSpacing(6);
    
    // Radar sub-tabs
    radarSubTabs = new QTabWidget();
    radarSubTabs->setStyleSheet("QTabBar::tab{height:24px; font-size:11px;} QTabWidget{font-size:11px;}");
    layout->addWidget(radarSubTabs);
    
    // Page 1
    radarPage1 = new QWidget();
    QVBoxLayout *page1Layout = new QVBoxLayout(radarPage1);
    page1Layout->setContentsMargins(8, 8, 8, 8);
    page1Layout->setSpacing(8);
    radarPage1->setStyleSheet(
        "QGroupBox{font-size:11px; margin-top:8px;}"
        "QLabel{font-size:11px;}"
        "QComboBox, QSpinBox, QDoubleSpinBox, QLineEdit, QListWidget, QPushButton{font-size:11px; min-height:20px;}"
        "QCheckBox{font-size:11px;}"
    );

    // Radar Configuration
    radarConfigGroup = new QGroupBox("Radar Configuration");
    QFormLayout *radarCfgLayout = new QFormLayout(radarConfigGroup);
    radarCfgLayout->setLabelAlignment(Qt::AlignRight);
    radarCfgLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    radarCfgLayout->setHorizontalSpacing(8);
    radarCfgLayout->setVerticalSpacing(6);
    radarConfigGroup->setContentsMargins(6, 6, 6, 6);

    radarModeCombo = new QComboBox();
    radarModeCombo->addItems({"Monostatic", "Bistatic Tx", "Bistatic Rx"});
    radarCfgLayout->addRow("Radar Mode:", radarModeCombo);

    centerFreqSpin = new QDoubleSpinBox();
    centerFreqSpin->setRange(0.1, 100.0);
    centerFreqSpin->setValue(3000.0);
    centerFreqSpin->setSuffix(" MHz");
    radarCfgLayout->addRow("Center Frequency:", centerFreqSpin);

    txPeakPowerSpin = new QDoubleSpinBox();
    txPeakPowerSpin->setRange(0.001, 10000.00);
    txPeakPowerSpin->setValue(8000.00);
    txPeakPowerSpin->setSuffix(" W");
    radarCfgLayout->addRow("Tx Peak Power:", txPeakPowerSpin);

    pulseWidthSpin = new QDoubleSpinBox();
    pulseWidthSpin->setRange(0.01, 1000.0);
    pulseWidthSpin->setValue(0.51);
    pulseWidthSpin->setSuffix(" us");
    radarCfgLayout->addRow("Pulse Width:", pulseWidthSpin);

    bandwidthSpin = new QDoubleSpinBox();
    bandwidthSpin->setRange(0.1, 1000.0);
    bandwidthSpin->setValue(5.0);
    bandwidthSpin->setSuffix(" MHz");
    radarCfgLayout->addRow("Bandwidth:", bandwidthSpin);

    page1Layout->addWidget(radarConfigGroup);

    // PRF Section
    prfGroup = new QGroupBox("PRF");
    QGridLayout *prfLayout = new QGridLayout(prfGroup);
    prfLayout->setHorizontalSpacing(8);
    prfLayout->setVerticalSpacing(6);
    prfGroup->setContentsMargins(6, 6, 6, 6);

    hopPrfsRandomlyCheck = new QCheckBox("Hop PRFs Randomly");
    prfLayout->addWidget(hopPrfsRandomlyCheck, 0, 0, 1, 2);

    prfLayout->addWidget(new QLabel("PRF Count:"), 1, 0);
    prfCountCombo = new QComboBox();
    for (int i = 1; i <= 7; ++i) prfCountCombo->addItem(QString::number(i));
    prfLayout->addWidget(prfCountCombo, 1, 1);

    prfLayout->addWidget(new QLabel("Select PRF:"), 2, 0);
    prfIndexCombo = new QComboBox();
    prfLayout->addWidget(prfIndexCombo, 2, 1);

    prfLayout->addWidget(new QLabel("PRF Value:"), 3, 0);
    prfValueSpin = new QDoubleSpinBox();
    prfValueSpin->setRange(1.0, 1000000.0);
    prfValueSpin->setDecimals(2);
    prfValueSpin->setValue(750.00);
    prfValueSpin->setSuffix(" Hz");
    prfLayout->addWidget(prfValueSpin, 3, 1);

    // init PRF state
    prfValues = QVector<double>(1, 750.00);
    prfIndexCombo->clear();
    prfIndexCombo->addItem("1");

    page1Layout->addWidget(prfGroup);

    // Waveform Section
    waveformGroup = new QGroupBox("Waveform");
    QFormLayout *waveLayout = new QFormLayout(waveformGroup);
    waveLayout->setLabelAlignment(Qt::AlignRight);
    waveLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    waveLayout->setHorizontalSpacing(8);
    waveLayout->setVerticalSpacing(6);
    waveformGroup->setContentsMargins(6, 6, 6, 6);

    waveformTypeCombo = new QComboBox();
    waveformTypeCombo->addItems({"Rectangular", "LFM", "Barker", "Frank", "P1", "P2", "P3", "P4", "Zadoff-Chu"});
    waveLayout->addRow("Waveform Type:", waveformTypeCombo);

    barkerLengthCombo = new QComboBox();
    barkerLengthCombo->addItems({"2","3","4","5","7","11","13"});
    waveLayout->addRow("Barker Length:", barkerLengthCombo);

    frankCodeSizeSpin = new QDoubleSpinBox();
    frankCodeSizeSpin->setRange(2.0, 2048.0);
    frankCodeSizeSpin->setValue(4.0);
    waveLayout->addRow("Frank Code Size:", frankCodeSizeSpin);

    pCodeSizeSpin = new QDoubleSpinBox();
    pCodeSizeSpin->setRange(2.0, 2048.0);
    pCodeSizeSpin->setValue(4.0);
    waveLayout->addRow("P Code Size:", pCodeSizeSpin);

    zcRootSpin = new QDoubleSpinBox();
    zcRootSpin->setRange(1.0, 1024.0);
    zcRootSpin->setValue(25.0);
    waveLayout->addRow("Zadoff-Chu Root:", zcRootSpin);

    windowingTypeCombo = new QComboBox();
    windowingTypeCombo->addItems({"No window", "Hamming", "Hanning", "Blackman", "Flat-top"});
    waveLayout->addRow("Windowing Type:", windowingTypeCombo);

    page1Layout->addWidget(waveformGroup);

    // CFAR Section
    cfarGroup = new QGroupBox("CFAR");
    QGridLayout *cfarLayout = new QGridLayout(cfarGroup);
    cfarLayout->setHorizontalSpacing(8);
    cfarLayout->setVerticalSpacing(6);
    cfarGroup->setContentsMargins(6, 6, 6, 6);

    useCFARCheck = new QCheckBox("Use CFAR Process");
    cfarLayout->addWidget(useCFARCheck, 0, 0, 1, 2);

    cfarLayout->addWidget(new QLabel("CFAR Type:"), 1, 0);
    cfarTypeCombo = new QComboBox();
    cfarTypeCombo->addItems({"CA", "SO", "GO", "OS"});
    cfarLayout->addWidget(cfarTypeCombo, 1, 1);

    cfarLayout->addWidget(new QLabel("Number of Training Cells:"), 2, 0);
    numTrainingCellsSpin = new QSpinBox();
    numTrainingCellsSpin->setRange(1, 10000);
    numTrainingCellsSpin->setValue(200);
    cfarLayout->addWidget(numTrainingCellsSpin, 2, 1);

    cfarLayout->addWidget(new QLabel("Number of Guard Cells:"), 3, 0);
    numGuardCellsSpin = new QSpinBox();
    numGuardCellsSpin->setRange(0, 10000);
    numGuardCellsSpin->setValue(2);
    cfarLayout->addWidget(numGuardCellsSpin, 3, 1);

    cfarLayout->addWidget(new QLabel("Rank of OS-CFAR:"), 4, 0);
    osRankSpin = new QSpinBox();
    osRankSpin->setRange(1, 10000);
    osRankSpin->setValue(100);
    cfarLayout->addWidget(osRankSpin, 4, 1);

    cfarLayout->addWidget(new QLabel("Probability false alarm:"), 5, 0);
    pfaSpin = new QDoubleSpinBox();
    pfaSpin->setRange(0.000001, 0.009999);
    pfaSpin->setDecimals(6);
    pfaSpin->setSingleStep(0.000001);
    pfaSpin->setValue(0.000001);
    cfarLayout->addWidget(pfaSpin, 5, 1);

    pfaFormattedLabel = new QLabel();
    pfaFormattedLabel->setTextFormat(Qt::RichText);
    pfaFormattedLabel->setText("1.000000 e<sup>-6</sup>");
    cfarLayout->addWidget(pfaFormattedLabel, 5, 2);
    

    page1Layout->addWidget(cfarGroup);

    // STC Section
    stcGroup = new QGroupBox("STC - Sensitivity Time Control");
    QFormLayout *stcLayout = new QFormLayout(stcGroup);
    stcLayout->setLabelAlignment(Qt::AlignRight);
    stcLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    stcLayout->setHorizontalSpacing(8);
    stcLayout->setVerticalSpacing(6);
    stcGroup->setContentsMargins(6, 6, 6, 6);

    useSTCCheck = new QCheckBox("Use STC Filter");
    stcLayout->addRow(useSTCCheck);

    stcCutoffRangeSpin = new QDoubleSpinBox();
    stcCutoffRangeSpin->setRange(0.0, 100000.0);
    stcCutoffRangeSpin->setValue(50000.00);
    stcCutoffRangeSpin->setSuffix(" m");
    stcLayout->addRow("STC Cutoff Range:", stcCutoffRangeSpin);

    stcFactorSpin = new QDoubleSpinBox();
    stcFactorSpin->setRange(0.0, 10.0);
    stcFactorSpin->setValue(4.0);
    stcLayout->addRow("STC Factor:", stcFactorSpin);

    page1Layout->addWidget(stcGroup);

    // Frequency Agile Section
    freqAgileGroup = new QGroupBox("Frequency Agile");
    QFormLayout *faLayout = new QFormLayout(freqAgileGroup);
    faLayout->setLabelAlignment(Qt::AlignRight);
    faLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    faLayout->setHorizontalSpacing(8);
    faLayout->setVerticalSpacing(6);
    freqAgileGroup->setContentsMargins(6, 6, 6, 6);

    useFreqHopCheck = new QCheckBox("Use Frequency Hopping");
    faLayout->addRow(useFreqHopCheck);

    lowerFreqMHzSpin = new QDoubleSpinBox();
    lowerFreqMHzSpin->setRange(0.1, 100000.0);
    lowerFreqMHzSpin->setValue(2000.00);
    lowerFreqMHzSpin->setSuffix(" MHz");
    faLayout->addRow("Lower Frequency:", lowerFreqMHzSpin);

    upperFreqMHzSpin = new QDoubleSpinBox();
    upperFreqMHzSpin->setRange(0.1, 100000.0);
    upperFreqMHzSpin->setValue(4000.00);
    upperFreqMHzSpin->setSuffix(" MHz");
    faLayout->addRow("Upper Frequency:", upperFreqMHzSpin);

    page1Layout->addWidget(freqAgileGroup);

    page1Layout->addStretch();
    
    // Connections for Radar Page 1
    connect(radarModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Sidebar::onRadarModeChanged);
    connect(prfCountCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Sidebar::onPrfCountChanged);
    connect(prfIndexCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Sidebar::onPrfIndexChanged);
    connect(prfValueSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Sidebar::onPrfValueChanged);
    connect(waveformTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Sidebar::onWaveformTypeChanged);
    connect(pfaSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Sidebar::onPfaValueChanged);

    // Başlangıç görünürlük ayarları
    onRadarModeChanged(radarModeCombo->currentIndex());
    onPrfCountChanged(prfCountCombo->currentIndex());
    onWaveformTypeChanged(waveformTypeCombo->currentIndex());

    // Page 2 (placeholder)
    radarPage2 = new QWidget();
    QVBoxLayout *page2Layout = new QVBoxLayout(radarPage2);
    page2Layout->setContentsMargins(8,8,8,8);
    page2Layout->setSpacing(8);

    radarPage2->setStyleSheet(
        "QGroupBox{font-size:11px; margin-top:8px;}"
        "QLabel{font-size:11px;}"
        "QComboBox, QSpinBox, QDoubleSpinBox, QLineEdit, QPushButton{font-size:11px; min-height:20px;}"
        "QCheckBox{font-size:11px;}"
    );

    // Antenna Configuration
    antennaConfigGroup = new QGroupBox("Antenna Configuration");
    QFormLayout *antLayout = new QFormLayout(antennaConfigGroup);
    antLayout->setLabelAlignment(Qt::AlignRight);
    antLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    antLayout->setHorizontalSpacing(8);
    antLayout->setVerticalSpacing(6);
    antennaConfigGroup->setContentsMargins(6,6,6,6);

    antennaPolarizationCombo = new QComboBox();
    antennaPolarizationCombo->addItems({"Horizontal", "Vertical"});
    antLayout->addRow("Antenna Polarization:", antennaPolarizationCombo);

    fixedGainCheck = new QCheckBox("Fixed Gain Antenna");
    antLayout->addRow(fixedGainCheck);
    
    beamPatternCheck = new QCheckBox("Antenna Beam Pattern File");
    antLayout->addRow(beamPatternCheck);

    fixedGainDbiSpin = new QDoubleSpinBox();
    fixedGainDbiSpin->setRange(-50.0, 80.0);
    fixedGainDbiSpin->setValue(20.0);
    fixedGainDbiSpin->setSuffix(" dBi");
    antLayout->addRow("Fixed Gain (dBi):", fixedGainDbiSpin);

    // Beam pattern file pickers
    QWidget *azRow = new QWidget();
    QHBoxLayout *azRowLayout = new QHBoxLayout(azRow);
    azRowLayout->setContentsMargins(0,0,0,0);
    azPatternLine = new QLineEdit();
    azPatternBrowseBtn = new QPushButton("...");
    azRowLayout->addWidget(azPatternLine);
    azRowLayout->addWidget(azPatternBrowseBtn);
    antLayout->addRow("Azimuth Beam Pattern File:", azRow);

    QWidget *elRow = new QWidget();
    QHBoxLayout *elRowLayout = new QHBoxLayout(elRow);
    elRowLayout->setContentsMargins(0,0,0,0);
    elPatternLine = new QLineEdit();
    elPatternBrowseBtn = new QPushButton("...");
    elRowLayout->addWidget(elPatternLine);
    elRowLayout->addWidget(elPatternBrowseBtn);
    antLayout->addRow("Elevation Beam Pattern File:", elRow);

    // Common fields
    timeDwellSecSpin = new QDoubleSpinBox();
    timeDwellSecSpin->setRange(0.0, 10000.0);
    timeDwellSecSpin->setValue(1.0);
    timeDwellSecSpin->setSuffix(" s");
    antLayout->addRow("Time dwell:", timeDwellSecSpin);

    noiseFigureDbSpin = new QDoubleSpinBox();
    noiseFigureDbSpin->setRange(0.0, 30.0);
    noiseFigureDbSpin->setValue(3.0);
    noiseFigureDbSpin->setSuffix(" dB");
    antLayout->addRow("Noise Figure:", noiseFigureDbSpin);

    effectiveTempKSpin = new QDoubleSpinBox();
    effectiveTempKSpin->setRange(1.0, 10000.0);
    effectiveTempKSpin->setValue(290.0);
    effectiveTempKSpin->setSuffix(" K");
    antLayout->addRow("Effective Temperature:", effectiveTempKSpin);

    totalSystemLossDbSpin = new QDoubleSpinBox();
    totalSystemLossDbSpin->setRange(0.0, 50.0);
    totalSystemLossDbSpin->setValue(2.0);
    totalSystemLossDbSpin->setSuffix(" dB");
    antLayout->addRow("Total System Loss:", totalSystemLossDbSpin);

    radarSnrThresholdDbSpin = new QDoubleSpinBox();
    radarSnrThresholdDbSpin->setRange(-20.0, 40.0);
    radarSnrThresholdDbSpin->setValue(10.0);
    radarSnrThresholdDbSpin->setSuffix(" dB");
    antLayout->addRow("Radar SNR Threshold:", radarSnrThresholdDbSpin);

    timeScanSecSpin = new QDoubleSpinBox();
    timeScanSecSpin->setRange(0.0, 3600.0);
    timeScanSecSpin->setValue(10.0);
    timeScanSecSpin->setSuffix(" s");
    antLayout->addRow("Time scan:", timeScanSecSpin);

    page2Layout->addWidget(antennaConfigGroup);

    // Show configs button
    showRadarConfigsBtn = new QPushButton("Show Radar Configurations");
    page2Layout->addWidget(showRadarConfigsBtn, 0, Qt::AlignRight);

    page2Layout->addStretch();
    
    // Wire up Page 2
    connect(fixedGainCheck, &QCheckBox::toggled, this, &Sidebar::onFixedGainToggled);
    connect(beamPatternCheck, &QCheckBox::toggled, this, &Sidebar::onBeamPatternToggled);
    connect(azPatternBrowseBtn, &QPushButton::clicked, this, &Sidebar::onBrowseAzPattern);
    connect(elPatternBrowseBtn, &QPushButton::clicked, this, &Sidebar::onBrowseElPattern);
    connect(showRadarConfigsBtn, &QPushButton::clicked, this, &Sidebar::onShowRadarConfigs);

    // Başlangıç: checkbox durumlarına göre alanları ayarla
    onFixedGainToggled(false);
    onBeamPatternToggled(false);

    // Alt sekmeleri ekle
    radarSubTabs->addTab(radarPage1, "Page 1");
    radarSubTabs->addTab(radarPage2, "Page 2");
}

void Sidebar::createTargetTab()
{
    targetTab = new QWidget();
    QVBoxLayout *rootLayout = new QVBoxLayout(targetTab);
    rootLayout->setSpacing(10);
    rootLayout->setContentsMargins(8, 8, 8, 8);

    targetTab->setStyleSheet(
        "QGroupBox{font-size:12px; font-weight:bold; margin-top:8px;}"
        "QLabel{font-size:11px;}"
        "QComboBox, QSpinBox, QDoubleSpinBox, QLineEdit, QListWidget, QPushButton{font-size:11px; min-height:24px;}"
        "QCheckBox{font-size:11px;}"
    );

    targetStack = new QStackedWidget(targetTab);
    rootLayout->addWidget(targetStack);

    // Sayfa 1
    targetMainPage = new QWidget();
    QVBoxLayout *mainPageLayout = new QVBoxLayout(targetMainPage);
    mainPageLayout->setSpacing(8);

    QGroupBox *targetListGroup = new QGroupBox("Target List");
    targetListGroup->setFont(QFont("Arial", 10, QFont::Bold));
    QVBoxLayout *targetListLayout = new QVBoxLayout(targetListGroup);

    QHBoxLayout *targetNameLayout = new QHBoxLayout();
    QLabel *targetNameLabel = new QLabel("Target Name:");
    targetNameLabel->setFont(QFont("Arial", 10));
    targetNameEdit = new QLineEdit();
    targetNameEdit->setPlaceholderText("Enter target name");
    targetNameEdit->setFont(QFont("Arial", 10));
    targetNameEdit->setMinimumHeight(22);
    targetNameLayout->addWidget(targetNameLabel);
    targetNameLayout->addWidget(targetNameEdit);
    targetListLayout->addLayout(targetNameLayout);

    targetList = new QListWidget();
    targetList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    targetList->setTextElideMode(Qt::ElideNone);
    targetList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    targetList->setResizeMode(QListView::Adjust);
    targetList->setFont(QFont("Arial", 10));
    targetList->setMaximumHeight(140);
    targetListLayout->addWidget(targetList);

    QHBoxLayout *targetButtonLayout = new QHBoxLayout();
    targetButtonLayout->setSpacing(6);
    addTargetBtn = new QPushButton("Add Target");
    deleteTargetBtn = new QPushButton("Delete Target");
    showWaypointsBtn = new QPushButton("Target's Route");
    showWaypointsBtn->setEnabled(false);

    targetButtonLayout->addWidget(addTargetBtn);
    targetButtonLayout->addWidget(deleteTargetBtn); 
    targetButtonLayout->addWidget(showWaypointsBtn);
    targetListLayout->addLayout(targetButtonLayout);

    mainPageLayout->addWidget(targetListGroup);

    // Initial Position
    QGroupBox *initPosGroup = new QGroupBox("Initial Position");
    initPosGroup->setFont(QFont("Arial", 10, QFont::Bold));
    QFormLayout *initPosLayout = new QFormLayout(initPosGroup);
    initPosLayout->setSpacing(6);
    initPosLayout->setLabelAlignment(Qt::AlignRight);

    initLatSpin = new QDoubleSpinBox();
    initLatSpin->setRange(-90.0, 90.0);
    initLatSpin->setValue(47.93943);
    initLatSpin->setDecimals(6);
    initLatSpin->setSuffix(" Deg");
    initLatSpin->setFont(QFont("Arial", 10));
    initLatSpin->setMinimumHeight(22);
    initPosLayout->addRow("Latitude (N):", initLatSpin);

    initLonSpin = new QDoubleSpinBox();
    initLonSpin->setRange(-180.0, 180.0);
    initLonSpin->setValue(3.68785);
    initLonSpin->setDecimals(6);
    initLonSpin->setSuffix(" Deg");
    initLonSpin->setFont(QFont("Arial", 10));
    initLonSpin->setMinimumHeight(22);
    initPosLayout->addRow("Longitude (W):", initLonSpin);

    initAltSpin = new QDoubleSpinBox();
    initAltSpin->setRange(0.0, 50000.0);
    initAltSpin->setValue(1000.0);
    initAltSpin->setSuffix(" m");
    initAltSpin->setFont(QFont("Arial", 10));
    initAltSpin->setMinimumHeight(22);
    initPosLayout->addRow("Altitude:", initAltSpin);

    initRCSSpin = new QDoubleSpinBox();
    initRCSSpin->setRange(-50.0, 50.0);
    initRCSSpin->setValue(10.0);
    initRCSSpin->setSuffix(" dBsm");
    initRCSSpin->setFont(QFont("Arial", 10));
    initRCSSpin->setMinimumHeight(22);
    initPosLayout->addRow("RCS:", initRCSSpin);

    initVelNSpin = new QDoubleSpinBox();
    initVelNSpin->setRange(-1000.0, 1000.0);
    initVelNSpin->setValue(0.0);
    initVelNSpin->setSuffix(" mps");
    initVelNSpin->setFont(QFont("Arial", 10));
    initVelNSpin->setMinimumHeight(22);
    initPosLayout->addRow("Velocity N:", initVelNSpin);

    initVelESpin = new QDoubleSpinBox();
    initVelESpin->setRange(-1000.0, 1000.0);
    initVelESpin->setValue(0.0);
    initVelESpin->setSuffix(" mps");
    initVelESpin->setFont(QFont("Arial", 10));
    initVelESpin->setMinimumHeight(22);
    initPosLayout->addRow("Velocity E:", initVelESpin);

    initVelDSpin = new QDoubleSpinBox();
    initVelDSpin->setRange(-1000.0, 1000.0);
    initVelDSpin->setValue(0.0);
    initVelDSpin->setSuffix(" mps");
    initVelDSpin->setFont(QFont("Arial", 10));
    initVelDSpin->setMinimumHeight(22);
    initPosLayout->addRow("Velocity D:", initVelDSpin);

    mainPageLayout->addWidget(initPosGroup);
    mainPageLayout->addStretch();

    targetStack->addWidget(targetMainPage);

    // ---------- Sayfa 2: Selected Target Waypoints + Trajectory ----------
    targetWaypointsPage = new QWidget();
    QVBoxLayout *wpPageLayout = new QVBoxLayout(targetWaypointsPage);
    wpPageLayout->setSpacing(8);

    QHBoxLayout *topRow = new QHBoxLayout();
    selectedTargetLabel = new QLabel("Selected Target: None");
    selectedTargetLabel->setFont(QFont("Arial", 10, QFont::Bold));
    backToTargetsBtn = new QPushButton("Back to Targets");
    topRow->addWidget(selectedTargetLabel, 1);
    topRow->addWidget(backToTargetsBtn, 0);
    wpPageLayout->addLayout(topRow);

    // Waypoint List
    QGroupBox *waypointGroup = new QGroupBox("Waypoint List");
    waypointGroup->setFont(QFont("Arial", 10, QFont::Bold));
    QVBoxLayout *waypointLayout = new QVBoxLayout(waypointGroup);
    waypointLayout->setSpacing(6);

    waypointList = new QListWidget();
    waypointList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    waypointList->setTextElideMode(Qt::ElideNone);
    waypointList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    waypointList->setResizeMode(QListView::Adjust);
    waypointList->setFont(QFont("Arial", 10));
    waypointList->setMaximumHeight(180);
    waypointLayout->addWidget(waypointList);

    QHBoxLayout *waypointButtonLayout = new QHBoxLayout();
    waypointButtonLayout->setSpacing(6);
    appendWaypointBtn = new QPushButton("Append");
    deleteWaypointBtn = new QPushButton("Delete");
    appendWaypointBtn->setEnabled(false);
    waypointButtonLayout->addWidget(appendWaypointBtn);
    waypointButtonLayout->addWidget(deleteWaypointBtn);
    waypointLayout->addLayout(waypointButtonLayout);

    wpPageLayout->addWidget(waypointGroup);

    // Trajectory
    QGroupBox *trajectoryGroup = new QGroupBox("Trajectory");
    trajectoryGroup->setFont(QFont("Arial", 10, QFont::Bold));
    QFormLayout *trajectoryLayout = new QFormLayout(trajectoryGroup);
    trajectoryLayout->setSpacing(6);
    trajectoryLayout->setLabelAlignment(Qt::AlignRight);

    trajLatSpin = new QDoubleSpinBox();
    trajLatSpin->setRange(-90.0, 90.0);
    trajLatSpin->setValue(47.93943);
    trajLatSpin->setDecimals(6);
    trajLatSpin->setSuffix(" Deg");
    trajLatSpin->setFont(QFont("Arial", 10));
    trajLatSpin->setMinimumHeight(22);
    trajectoryLayout->addRow("Latitude (N):", trajLatSpin);

    trajLonSpin = new QDoubleSpinBox();
    trajLonSpin->setRange(-180.0, 180.0);
    trajLonSpin->setValue(3.68785);
    trajLonSpin->setDecimals(6);
    trajLonSpin->setSuffix(" Deg");
    trajLonSpin->setFont(QFont("Arial", 10));
    trajLonSpin->setMinimumHeight(22);
    trajectoryLayout->addRow("Longitude (W):", trajLonSpin);

    trajAltSpin = new QDoubleSpinBox();
    trajAltSpin->setRange(0.0, 50000.0);
    trajAltSpin->setValue(1000.0);
    trajAltSpin->setSuffix(" m");
    trajAltSpin->setFont(QFont("Arial", 10));
    trajAltSpin->setMinimumHeight(22);
    trajectoryLayout->addRow("Altitude:", trajAltSpin);

    trajVelNSpin = new QDoubleSpinBox();
    trajVelNSpin->setRange(-1000.0, 1000.0);
    trajVelNSpin->setValue(0.0);
    trajVelNSpin->setSuffix(" mps");
    trajVelNSpin->setFont(QFont("Arial", 10));
    trajVelNSpin->setMinimumHeight(22);
    trajectoryLayout->addRow("Velocity N:", trajVelNSpin);

    trajVelESpin = new QDoubleSpinBox();
    trajVelESpin->setRange(-1000.0, 1000.0);
    trajVelESpin->setValue(0.0);
    trajVelESpin->setSuffix(" mps");
    trajVelESpin->setFont(QFont("Arial", 10));
    trajVelESpin->setMinimumHeight(22);
    trajectoryLayout->addRow("Velocity E:", trajVelESpin);

    trajVelDSpin = new QDoubleSpinBox();
    trajVelDSpin->setRange(-1000.0, 1000.0);
    trajVelDSpin->setValue(0.0);
    trajVelDSpin->setSuffix(" mps");
    trajVelDSpin->setFont(QFont("Arial", 10));
    trajVelDSpin->setMinimumHeight(22);
    trajectoryLayout->addRow("Velocity D:", trajVelDSpin);

    wpPageLayout->addWidget(trajectoryGroup);
    wpPageLayout->addStretch();

    targetStack->addWidget(targetWaypointsPage);

    // Baslangicta ana sayfa
    targetStack->setCurrentWidget(targetMainPage);

    // Connect signals
    connect(addTargetBtn, &QPushButton::clicked, this, &Sidebar::onAddTarget);
    connect(deleteTargetBtn, &QPushButton::clicked, this, &Sidebar::onDeleteTarget);
    connect(showWaypointsBtn, &QPushButton::clicked, this, &Sidebar::onShowWaypointsPage);
    connect(backToTargetsBtn, &QPushButton::clicked, this, &Sidebar::onBackToTargetsPage);
    connect(appendWaypointBtn, &QPushButton::clicked, this, &Sidebar::onAppendWaypoint);
    connect(deleteWaypointBtn, &QPushButton::clicked, this, &Sidebar::onDeleteWaypoint);
    connect(targetList, &QListWidget::itemSelectionChanged, this, &Sidebar::onTargetSelectionChanged);
}

void Sidebar::onShowWaypointsPage()
{
    QListWidgetItem *currentItem = targetList->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "Warning", "Please select a target first.");
        return;
    }
    currentSelectedTarget = currentItem->text();
    selectedTargetLabel->setText("Selected Target: " + currentSelectedTarget);

    // Secilen target'in waypointlerini tabloya yukle
    waypointList->clear();
    if (targets.contains(currentSelectedTarget)) {
        const QStringList &waypoints = targets[currentSelectedTarget].waypoints;
        for (const QString &wp : waypoints) waypointList->addItem(wp);
    }

    appendWaypointBtn->setEnabled(true);
    targetStack->setCurrentWidget(targetWaypointsPage);
}

void Sidebar::onBackToTargetsPage()
{
    targetStack->setCurrentWidget(targetMainPage);
}

void Sidebar::onShowRadarRoutePage()
{
    // Artık kullanılmıyor: Route alt sekmesi üzerinden yönetiliyor
}

void Sidebar::onBackFromRadarRoutePage()
{
    // Artık kullanılmıyor: geri tuşu kaldırıldı
}

void Sidebar::onAppendRadarWaypoint()
{
    RadarWaypoint wpd;
    wpd.lat = radarWpLatSpin->value();
    wpd.lon = radarWpLonSpin->value();
    wpd.alt = radarWpAltSpin->value();
    wpd.velN = radarWpVelNSpin->value();
    wpd.velE = radarWpVelESpin->value();
    wpd.velD = radarWpVelDSpin->value();
    radarWaypointsData.append(wpd);

    QString wp = QString("Lat=%1, Lon=%2, Alt=%3m, VelN=%4, VelE=%5, VelD=%6")
        .arg(wpd.lat, 0, 'f', 6)
        .arg(wpd.lon, 0, 'f', 6)
        .arg(wpd.alt, 0, 'f', 1)
        .arg(wpd.velN, 0, 'f', 1)
        .arg(wpd.velE, 0, 'f', 1)
        .arg(wpd.velD, 0, 'f', 1);
    radarWaypointList->addItem(wp);

    QVector<QPair<double,double>> latLon;
    latLon.push_back({ generalLatSpin->value(), generalLonSpin->value() });
    for (const auto &rw : radarWaypointsData) latLon.push_back({ rw.lat, rw.lon });
    emit radarRouteChanged(latLon);
}

void Sidebar::onDeleteRadarWaypoint()
{
    QListWidgetItem *it = radarWaypointList->currentItem();
    if (!it) return;
    int row = radarWaypointList->row(it);
    radarWaypointsData.removeAt(row);
    delete it;

    QVector<QPair<double,double>> latLon;
    latLon.push_back({ generalLatSpin->value(), generalLonSpin->value() });
    for (const auto &rw : radarWaypointsData) latLon.push_back({ rw.lat, rw.lon });
    emit radarRouteChanged(latLon);
}

void Sidebar::onAddWeatherCondition()
{
    // Weather condition verilerini al
    double lat = latitudeSpinBox->value();
    double lon = longitudeSpinBox->value();
    double radius = circleRadiusSpinBox->value();
    QString conditionType = conditionTypeComboBox->currentText();
    
    // WeatherCondition struct'ını oluştur
    WeatherCondition condition;
    condition.latitude = lat;
    condition.longitude = lon;
    condition.radius = radius;
    condition.type = conditionType;
    
    if (conditionType == "Rain") {
        condition.rainRate = rainRateSpinBox->value();
        condition.rainTemperature = rainTemperatureSpinBox->value();
        condition.fogVisibility = 0.0;
        condition.fogTemperature = 0.0;
    } else if (conditionType == "Fog&Cloud") {
        condition.fogVisibility = fogVisibilitySpinBox->value();
        condition.fogTemperature = fogTemperatureSpinBox->value();
        condition.rainRate = 0.0;
        condition.rainTemperature = 0.0;
    }
    
    QString conditionText;
    if (conditionType == "Rain") {
        conditionText = QString("Rain - Lat: %1, Lon: %2, Radius: %3km, Rate: %4mm/h, Temp: %5K")
                       .arg(lat, 0, 'f', 6)
                       .arg(lon, 0, 'f', 6)
                       .arg(radius, 0, 'f', 2)
                       .arg(condition.rainRate, 0, 'f', 1)
                       .arg(condition.rainTemperature, 0, 'f', 2);
    } else if (conditionType == "Fog&Cloud") {
        conditionText = QString("Fog&Cloud - Lat: %1, Lon: %2, Radius: %3km, Visibility: %4m, Temp: %5K")
                       .arg(lat, 0, 'f', 6)
                       .arg(lon, 0, 'f', 6)
                       .arg(radius, 0, 'f', 2)
                       .arg(condition.fogVisibility, 0, 'f', 0)
                       .arg(condition.fogTemperature, 0, 'f', 2);
    }
    
    // Listeye ekle
    weatherConditionList->addItem(conditionText);
    weatherStore.append(condition);
    
    // Signal emit et
    emit weatherConditionAdded(condition);
    
    // Form'u temizle
    latitudeSpinBox->setValue(0.0);
    longitudeSpinBox->setValue(0.0);
    circleRadiusSpinBox->setValue(10.0);
    conditionTypeComboBox->setCurrentIndex(0);
    rainRateSpinBox->setValue(5.0);
    rainTemperatureSpinBox->setValue(293.15);
    fogVisibilitySpinBox->setValue(1000.0);
    fogTemperatureSpinBox->setValue(293.15);
}

void Sidebar::onDeleteWeatherCondition()
{
    QListWidgetItem *selectedItem = weatherConditionList->currentItem();
    if (selectedItem) {
        int row = weatherConditionList->row(selectedItem);
        if (row >= 0 && row < weatherStore.size()) weatherStore.removeAt(row);
        emit weatherConditionRemoved(row);
        weatherConditionList->takeItem(row);
    }
}

void Sidebar::onBrowseDTEDFiles()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(
        this,
        "Select DTED Files",
        "",
        "DTED Files (*.dt1 *.dt0 *.dt2);;All Files (*)"
    );
    
    if (!fileNames.isEmpty()) {
        for (const QString &fileName : fileNames) {
            QFileInfo fileInfo(fileName);
            QString displayName = fileInfo.fileName();
            QListWidgetItem *item = new QListWidgetItem(displayName);
            item->setData(Qt::UserRole, fileName);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);
            dtedFilesList->addItem(item);
            dtedStore.append(fileName);
        }
        emit dtedFilesAdded(fileNames);
    }
}

void Sidebar::onRadarModeChanged(int index)
{
    // index: 0 Monostatic, 1 Bistatic Tx, 2 Bistatic Rx
    auto setRowVisible = [&](QFormLayout *layout, QWidget *field, bool visible){
        if (!layout || !field) return;
        for (int i = 0; i < layout->rowCount(); ++i) {
            QLayoutItem *labelItem = layout->itemAt(i, QFormLayout::LabelRole);
            QLayoutItem *fieldItem = layout->itemAt(i, QFormLayout::FieldRole);
            if (fieldItem && fieldItem->widget() == field) {
                if (labelItem && labelItem->widget()) labelItem->widget()->setVisible(visible);
                field->setVisible(visible);
                break;
            }
        }
    };

    QFormLayout *cfgLayout = static_cast<QFormLayout*>(radarConfigGroup->layout());
    QFormLayout *waveLayout = static_cast<QFormLayout*>(waveformGroup->layout());

    if (index == 0) { // Monostatic
        // Radar Configuration: tüm alanlar
        setRowVisible(cfgLayout, centerFreqSpin, true);
        setRowVisible(cfgLayout, txPeakPowerSpin, true);
        setRowVisible(cfgLayout, pulseWidthSpin, true);
        setRowVisible(cfgLayout, bandwidthSpin, true);
        radarConfigGroup->setVisible(true);

        // Gruplar
        prfGroup->setVisible(true);
        waveformGroup->setVisible(true);
        cfarGroup->setVisible(true);
        stcGroup->setVisible(true);
        freqAgileGroup->setVisible(true);

        // Waveform alt alan görünürlükleri mevcut seçime göre
        onWaveformTypeChanged(waveformTypeCombo->currentIndex());
    } else if (index == 1) { // Bistatic Tx
        // Radar Configuration: monostatic ile aynı 4 alan
        setRowVisible(cfgLayout, centerFreqSpin, true);
        setRowVisible(cfgLayout, txPeakPowerSpin, true);
        setRowVisible(cfgLayout, pulseWidthSpin, true);
        setRowVisible(cfgLayout, bandwidthSpin, true);
        radarConfigGroup->setVisible(true);

        // Görünecek/gizlenecek gruplar
        prfGroup->setVisible(true);
        waveformGroup->setVisible(true);
        cfarGroup->setVisible(false);
        stcGroup->setVisible(false);
        freqAgileGroup->setVisible(false);

        // Waveform alt alan görünürlükleri mevcut seçime göre
        onWaveformTypeChanged(waveformTypeCombo->currentIndex());
    } else { // Bistatic Rx
        // Radar Configuration: sadece Center Frequency ve Bandwidth
        setRowVisible(cfgLayout, centerFreqSpin, true);
        setRowVisible(cfgLayout, txPeakPowerSpin, false);
        setRowVisible(cfgLayout, pulseWidthSpin, false);
        setRowVisible(cfgLayout, bandwidthSpin, true);
        radarConfigGroup->setVisible(true);

        // Gruplar
        prfGroup->setVisible(false);
        waveformGroup->setVisible(false);
        freqAgileGroup->setVisible(false);

        cfarGroup->setVisible(true);
        stcGroup->setVisible(true);
    }
}

void Sidebar::onPrfCountChanged(int index)
{
    int count = index + 1; // combo 0..6 -> 1..7
    prfValues.resize(count);
    // Yeni slotlara default değer ata
    for (int i = 0; i < count; ++i) {
        if (prfValues[i] <= 0.0) prfValues[i] = 1000.0;
    }
    prfIndexCombo->clear();
    for (int i = 1; i <= count; ++i) prfIndexCombo->addItem(QString::number(i));
    prfIndexCombo->setCurrentIndex(0);
    prfValueSpin->setValue(prfValues[0]);
}

void Sidebar::onPrfIndexChanged(int index)
{
    if (index >= 0 && index < prfValues.size()) {
        prfValueSpin->blockSignals(true);
        prfValueSpin->setValue(prfValues[index]);
        prfValueSpin->blockSignals(false);
    }
}

void Sidebar::onPrfValueChanged(double value)
{
    int idx = prfIndexCombo->currentIndex();
    if (idx >= 0 && idx < prfValues.size()) {
        prfValues[idx] = value;
    }
}

void Sidebar::onWaveformTypeChanged(int index)
{
    // 0 Rect, 1 LFM, 2 Barker, 3 Frank, 4 P1, 5 P2, 6 P3, 7 P4, 8 ZC
    bool showBarker = (index == 2);
    bool showFrank  = (index == 3);
    bool showPcode  = (index >= 4 && index <= 7);
    bool showZC     = (index == 8);

    auto setRowVisible = [&](QFormLayout *layout, QWidget *field, bool visible){
        for (int i = 0; i < layout->rowCount(); ++i) {
            QLayoutItem *labelItem = layout->itemAt(i, QFormLayout::LabelRole);
            QLayoutItem *fieldItem = layout->itemAt(i, QFormLayout::FieldRole);
            if (fieldItem && fieldItem->widget() == field) {
                if (labelItem && labelItem->widget()) labelItem->widget()->setVisible(visible);
                field->setVisible(visible);
                break;
            }
        }
    };

    QFormLayout *waveLayout = static_cast<QFormLayout*>(waveformGroup->layout());
    setRowVisible(waveLayout, barkerLengthCombo, showBarker);
    setRowVisible(waveLayout, frankCodeSizeSpin, showFrank);
    setRowVisible(waveLayout, pCodeSizeSpin, showPcode);
    setRowVisible(waveLayout, zcRootSpin, showZC);
}

void Sidebar::onPfaValueChanged(double value)
{
    // 1.0e-6 biçiminde gösterim
    // value aralığı 1e-6 .. 9.999e-3, kullanıcı girdisi 0.000001 gibi
    // Notasyon: a.bce^-n şeklinde render edelim
    int exponent = 0;
    double mantissa = value;
    while (mantissa < 1.0 && exponent < 12) { mantissa *= 10.0; exponent++; }
    pfaFormattedLabel->setText(QString("%1.e^-%2")
        .arg(QString::number(mantissa, 'f', 6))
        .arg(exponent));
}

void Sidebar::onFixedGainToggled(bool checked)
{
    QFormLayout *form = static_cast<QFormLayout*>(antennaConfigGroup->layout());
    auto setRowVisible = [&](QWidget *field, bool visible){
        if (!form || !field) return;
        for (int i = 0; i < form->rowCount(); ++i) {
            QLayoutItem *labelItem = form->itemAt(i, QFormLayout::LabelRole);
            QLayoutItem *fieldItem = form->itemAt(i, QFormLayout::FieldRole);
            if (fieldItem && fieldItem->widget() == field) {
                if (labelItem && labelItem->widget()) labelItem->widget()->setVisible(visible);
                field->setVisible(visible);
                break;
            }
        }
    };

    QWidget *azRow = azPatternLine ? azPatternLine->parentWidget() : nullptr;
    QWidget *elRow = elPatternLine ? elPatternLine->parentWidget() : nullptr;

    if (checked) {
        // Karşı kutuyu kapat (sinyal döngüsünü engelle)
        beamPatternCheck->blockSignals(true);
        beamPatternCheck->setChecked(false);
        beamPatternCheck->blockSignals(false);
        // Sadece fixed gain satırı görünsün
        setRowVisible(fixedGainDbiSpin, true);
        setRowVisible(azRow, false);
        setRowVisible(elRow, false);
    } else {
        // Fixed gain kapandıysa, eğer beam pattern de kapalıysa tümü kapalı
        if (!beamPatternCheck->isChecked()) {
            setRowVisible(fixedGainDbiSpin, false);
        }
        // Beam pattern açık ise az/el görünürlüğü beam handler tarafından yönetilecek
    }
}

void Sidebar::onBeamPatternToggled(bool checked)
{
    QFormLayout *form = static_cast<QFormLayout*>(antennaConfigGroup->layout());
    auto setRowVisible = [&](QWidget *field, bool visible){
        if (!form || !field) return;
        for (int i = 0; i < form->rowCount(); ++i) {
            QLayoutItem *labelItem = form->itemAt(i, QFormLayout::LabelRole);
            QLayoutItem *fieldItem = form->itemAt(i, QFormLayout::FieldRole);
            if (fieldItem && fieldItem->widget() == field) {
                if (labelItem && labelItem->widget()) labelItem->widget()->setVisible(visible);
                field->setVisible(visible);
                break;
            }
        }
    };

    QWidget *azRow = azPatternLine ? azPatternLine->parentWidget() : nullptr;
    QWidget *elRow = elPatternLine ? elPatternLine->parentWidget() : nullptr;

    if (checked) {
        // Karşı kutuyu kapat (sinyal döngüsünü engelle)
        fixedGainCheck->blockSignals(true);
        fixedGainCheck->setChecked(false);
        fixedGainCheck->blockSignals(false);
        // Sadece az/el dosya satırları görünsün
        setRowVisible(fixedGainDbiSpin, false);
        setRowVisible(azRow, true);
        setRowVisible(elRow, true);
    } else {
        // Kapatıldıysa satırları sakla
        setRowVisible(azRow, false);
        setRowVisible(elRow, false);
        // Fixed gain açık değilse onu da sakla
        if (!fixedGainCheck->isChecked()) {
            setRowVisible(fixedGainDbiSpin, false);
        }
    }
}

void Sidebar::onBrowseAzPattern()
{
    QString path = QFileDialog::getOpenFileName(this, "Select Azimuth Beam Pattern File", "", "Pattern Files (*.csv *.txt);;All Files (*)");
    if (!path.isEmpty()) {
        azPatternLine->setText(path);
    }
}

void Sidebar::onBrowseElPattern()
{
    QString path = QFileDialog::getOpenFileName(this, "Select Elevation Beam Pattern File", "", "Pattern Files (*.csv *.txt);;All Files (*)");
    if (!path.isEmpty()) {
        elPatternLine->setText(path);
    }
}

static QString boolToYesNo(bool v){ return v ? "Yes" : "No"; }

void Sidebar::onShowRadarConfigs()
{
    // Page 1 değerlerini topla
    currentRadarConfig.radarMode = radarModeCombo->currentText();
    currentRadarConfig.centerFreqGHz = centerFreqSpin->isVisible() ? centerFreqSpin->value() : currentRadarConfig.centerFreqGHz;
    currentRadarConfig.txPeakKw = txPeakPowerSpin->isVisible() ? txPeakPowerSpin->value() : 0.0;
    currentRadarConfig.pulseWidthUs = pulseWidthSpin->isVisible() ? pulseWidthSpin->value() : 0.0;
    currentRadarConfig.bandwidthMHz = bandwidthSpin->isVisible() ? bandwidthSpin->value() : currentRadarConfig.bandwidthMHz;

    currentRadarConfig.hopPrfsRandomly = prfGroup->isVisible() ? hopPrfsRandomlyCheck->isChecked() : false;
    currentRadarConfig.prfCount = prfGroup->isVisible() ? (prfCountCombo->currentIndex()+1) : 0;
    currentRadarConfig.prfValues = prfGroup->isVisible() ? prfValues : QVector<double>{};

    if (waveformGroup->isVisible()) {
        currentRadarConfig.waveformType = waveformTypeCombo->currentText();
        currentRadarConfig.barkerLength = barkerLengthCombo->isVisible() ? barkerLengthCombo->currentText().toInt() : 0;
        currentRadarConfig.frankCodeSize = frankCodeSizeSpin->isVisible() ? frankCodeSizeSpin->value() : 0.0;
        currentRadarConfig.pCodeSize = pCodeSizeSpin->isVisible() ? pCodeSizeSpin->value() : 0.0;
        currentRadarConfig.zcRoot = zcRootSpin->isVisible() ? zcRootSpin->value() : 0.0;
        currentRadarConfig.windowingType = windowingTypeCombo->currentText();
    } else {
        currentRadarConfig.waveformType.clear();
        currentRadarConfig.barkerLength = 0;
        currentRadarConfig.frankCodeSize = 0.0;
        currentRadarConfig.pCodeSize = 0.0;
        currentRadarConfig.zcRoot = 0.0;
        currentRadarConfig.windowingType.clear();
    }

    currentRadarConfig.useCFAR = cfarGroup->isVisible() ? useCFARCheck->isChecked() : false;
    currentRadarConfig.cfarType = cfarGroup->isVisible() ? cfarTypeCombo->currentText() : QString();
    currentRadarConfig.numTrainingCells = cfarGroup->isVisible() ? numTrainingCellsSpin->value() : 0;
    currentRadarConfig.numGuardCells = cfarGroup->isVisible() ? numGuardCellsSpin->value() : 0;
    currentRadarConfig.osRank = cfarGroup->isVisible() ? osRankSpin->value() : 0;
    currentRadarConfig.pfa = cfarGroup->isVisible() ? pfaSpin->value() : 0.0;

    currentRadarConfig.useSTC = stcGroup->isVisible() ? useSTCCheck->isChecked() : false;
    currentRadarConfig.stcCutoffRange = stcGroup->isVisible() ? stcCutoffRangeSpin->value() : 0.0;
    currentRadarConfig.stcFactor = stcGroup->isVisible() ? stcFactorSpin->value() : 0.0;

    currentRadarConfig.useFreqHop = freqAgileGroup->isVisible() ? useFreqHopCheck->isChecked() : false;
    currentRadarConfig.lowerFreqMHz = freqAgileGroup->isVisible() ? lowerFreqMHzSpin->value() : 0.0;
    currentRadarConfig.upperFreqMHz = freqAgileGroup->isVisible() ? upperFreqMHzSpin->value() : 0.0;

    // Page 2 değerleri
    currentRadarConfig.polarization = antennaPolarizationCombo->currentText();
    currentRadarConfig.fixedGain = fixedGainCheck->isChecked();
    currentRadarConfig.fixedGainDbi = fixedGainDbiSpin->value();
    currentRadarConfig.beamPattern = beamPatternCheck->isChecked();
    currentRadarConfig.azimuthPatternPath = azPatternLine->text();
    currentRadarConfig.elevationPatternPath = elPatternLine->text();
    currentRadarConfig.timeDwellSec = timeDwellSecSpin->value();
    currentRadarConfig.noiseFigureDb = noiseFigureDbSpin->value();
    currentRadarConfig.effectiveTempK = effectiveTempKSpin->value();
    currentRadarConfig.totalSystemLossDb = totalSystemLossDbSpin->value();
    currentRadarConfig.radarSnrThresholdDb = radarSnrThresholdDbSpin->value();
    currentRadarConfig.timeScanSec = timeScanSecSpin->value();

    // Özet metin
    QString summary;
    summary += QString("Mode: %1\n").arg(currentRadarConfig.radarMode);
    summary += QString("Center Freq: %1 GHz, BW: %2 MHz\n").arg(currentRadarConfig.centerFreqGHz).arg(currentRadarConfig.bandwidthMHz);
    if (txPeakPowerSpin->isVisible()) summary += QString("Tx Peak: %1 kW, PW: %2 us\n").arg(currentRadarConfig.txPeakKw).arg(currentRadarConfig.pulseWidthUs);
    if (prfGroup->isVisible()) {
        summary += QString("PRF Count: %1, Hop Randomly: %2\n").arg(currentRadarConfig.prfCount).arg(boolToYesNo(currentRadarConfig.hopPrfsRandomly));
        for (int i = 0; i < currentRadarConfig.prfValues.size(); ++i) {
            summary += QString("  PRF[%1] = %2 Hz\n").arg(i+1).arg(currentRadarConfig.prfValues[i]);
        }
    }
    if (!currentRadarConfig.waveformType.isEmpty()) {
        summary += QString("Waveform: %1, Window: %2\n").arg(currentRadarConfig.waveformType, currentRadarConfig.windowingType);
        if (barkerLengthCombo->isVisible()) summary += QString("  Barker Len: %1\n").arg(currentRadarConfig.barkerLength);
        if (frankCodeSizeSpin->isVisible()) summary += QString("  Frank Size: %1\n").arg(currentRadarConfig.frankCodeSize);
        if (pCodeSizeSpin->isVisible()) summary += QString("  P Code Size: %1\n").arg(currentRadarConfig.pCodeSize);
        if (zcRootSpin->isVisible()) summary += QString("  ZC Root: %1\n").arg(currentRadarConfig.zcRoot);
    }
    if (cfarGroup->isVisible()) {
        summary += QString("CFAR: %1, T:%2 G:%3 OS-Rank:%4 Pfa:%5\n").arg(currentRadarConfig.cfarType)
                  .arg(currentRadarConfig.numTrainingCells)
                  .arg(currentRadarConfig.numGuardCells)
                  .arg(currentRadarConfig.osRank)
                  .arg(currentRadarConfig.pfa, 0, 'g', 8);
    }
    if (stcGroup->isVisible()) {
        summary += QString("STC: %1, Cutoff:%2 m, Factor:%3\n").arg(boolToYesNo(currentRadarConfig.useSTC))
                  .arg(currentRadarConfig.stcCutoffRange)
                  .arg(currentRadarConfig.stcFactor);
    }
    if (freqAgileGroup->isVisible()) {
        summary += QString("Freq Hop: %1, [%2, %3] MHz\n").arg(boolToYesNo(currentRadarConfig.useFreqHop))
                  .arg(currentRadarConfig.lowerFreqMHz)
                  .arg(currentRadarConfig.upperFreqMHz);
    }
    summary += QString("Polarization: %1\n").arg(currentRadarConfig.polarization);
    summary += QString("Fixed Gain: %1").arg(boolToYesNo(currentRadarConfig.fixedGain));
    if (currentRadarConfig.fixedGain) summary += QString(", %1 dBi").arg(currentRadarConfig.fixedGainDbi);
    summary += "\n";
    summary += QString("Beam Pattern: %1\n").arg(boolToYesNo(currentRadarConfig.beamPattern));
    if (currentRadarConfig.beamPattern) {
        summary += QString("  Az: %1\n  El: %2\n").arg(currentRadarConfig.azimuthPatternPath, currentRadarConfig.elevationPatternPath);
    }
    summary += QString("Time dwell: %1 s, NF: %2 dB, Te: %3 K\n").arg(currentRadarConfig.timeDwellSec)
               .arg(currentRadarConfig.noiseFigureDb)
               .arg(currentRadarConfig.effectiveTempK);
    summary += QString("SysLoss: %1 dB, SNR Th: %2 dB, Time scan: %3 s\n").arg(currentRadarConfig.totalSystemLossDb)
               .arg(currentRadarConfig.radarSnrThresholdDb)
               .arg(currentRadarConfig.timeScanSec);

    QMessageBox::information(this, "Radar Configurations", summary);
}

// Target tab slot functions
// Mevcut target fonksiyonlarınızı bu şekilde değiştirin:

void Sidebar::onAddTarget()
{
    QString targetName = targetNameEdit->text().trimmed();
    if (targetName.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter a target name.");
        return;
    }
    
    if (targets.contains(targetName)) {
        QMessageBox::warning(this, "Warning", "Target name already exists.");
        return;
    }
    
    // Yeni target oluştur
    Target newTarget;
    newTarget.name = targetName;
    newTarget.initLatitude = initLatSpin->value();
    newTarget.initLongitude = initLonSpin->value();
    newTarget.initAltitude = initAltSpin->value();
    newTarget.initRCS = initRCSSpin->value();
    newTarget.initVelocityN = initVelNSpin->value();
    newTarget.initVelocityE = initVelESpin->value();
    newTarget.initVelocityD = initVelDSpin->value();
    
    // Waypoints listesini başlat
    newTarget.waypoints = QStringList();
    
    // Target'ı map'e ekle
    targets[targetName] = newTarget;
    
    // Liste widget'ına ekle
    targetList->addItem(targetName);
    
    // Signal emit et - ControlPanel'e target'ı gönder
    emit targetAdded(newTarget);
    
    // Form'u temizle
    targetNameEdit->clear();
}

void Sidebar::onDeleteTarget()
{
    QListWidgetItem *currentItem = targetList->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "Warning", "Please select a target to delete.");
        return;
    }
    
    QString targetName = currentItem->text();
    
    // Signal emit et - ControlPanel'den target'ı kaldır
    emit targetRemoved(targetName);
    
    // Map'ten kaldır
    targets.remove(targetName);
    
    // Liste widget'ından kaldır
    delete currentItem;
    
    // Eğer silinen target seçiliyse waypoint listesini temizle
    if (currentSelectedTarget == targetName) {
        currentSelectedTarget.clear();
        waypointList->clear();
        selectedTargetLabel->setText("Selected Target: None");
        appendWaypointBtn->setEnabled(false);
        if (showWaypointsBtn) showWaypointsBtn->setEnabled(false);
    }
}

void Sidebar::onTargetSelectionChanged()
{
    QListWidgetItem *currentItem = targetList->currentItem();
    if (!currentItem) {
        currentSelectedTarget.clear();
        waypointList->clear();
        selectedTargetLabel->setText("Selected Target: None");
        appendWaypointBtn->setEnabled(false);
        if (showWaypointsBtn) showWaypointsBtn->setEnabled(false);
        return;
    }
    
    currentSelectedTarget = currentItem->text();
    selectedTargetLabel->setText("Selected Target: " + currentSelectedTarget);
    appendWaypointBtn->setEnabled(true);
    if (showWaypointsBtn) showWaypointsBtn->setEnabled(true);
    
    // Secilen target'in waypoint'lerini tabloya yukle (detay sayfada gosterilecek)
    waypointList->clear();
    if (targets.contains(currentSelectedTarget)) {
        const QStringList &waypoints = targets[currentSelectedTarget].waypoints;
        for (const QString &wp : waypoints) waypointList->addItem(wp);
    }
}

void Sidebar::onAppendWaypoint()
{
    if (currentSelectedTarget.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a target first.");
        return;
    }
    
    // Trajectory değerlerinden waypoint string'i oluştur
    QString waypointStr = QString("WP%1: Lat=%2, Lon=%3, Alt=%4m, VelN=%5, VelE=%6, VelD=%7")
                         .arg(targets[currentSelectedTarget].waypoints.size() + 1)
                         .arg(trajLatSpin->value(), 0, 'f', 6)
                         .arg(trajLonSpin->value(), 0, 'f', 6)
                         .arg(trajAltSpin->value(), 0, 'f', 1)
                         .arg(trajVelNSpin->value(), 0, 'f', 1)
                         .arg(trajVelESpin->value(), 0, 'f', 1)
                         .arg(trajVelDSpin->value(), 0, 'f', 1);
    
    targets[currentSelectedTarget].waypoints.append(waypointStr);

    // Tabloya ekle
    int r = waypointList->count();
    waypointList->addItem(waypointStr);

    // Trajectory publish
    if (!currentSelectedTarget.isEmpty()) {
        QVector<QPair<double,double>> latLon;
        QRegularExpression re("Lat=([+-]?\\d+\\.?\\d*),\\s*Lon=([+-]?\\d+\\.?\\d*)");
        for (const QString &w : targets[currentSelectedTarget].waypoints) {
            auto m = re.match(w);
            if (m.hasMatch()) latLon.push_back({ m.captured(1).toDouble(), m.captured(2).toDouble() });
        }
        emit targetTrajectoryChanged(currentSelectedTarget, latLon);
    }
}

void Sidebar::onDeleteWaypoint()
{
    QListWidgetItem *currentItem = waypointList->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "Warning", "Please select a waypoint to delete.");
        return;
    }
    
    if (currentSelectedTarget.isEmpty()) {
        return;
    }
    
    int row = waypointList->row(currentItem);
    
    if (row >= 0 && row < targets[currentSelectedTarget].waypoints.size()) {
        targets[currentSelectedTarget].waypoints.removeAt(row);
    }
    delete currentItem;
    
    waypointList->clear();
    QStringList &waypoints = targets[currentSelectedTarget].waypoints;
    for (int i = 0; i < waypoints.size(); ++i) {
        QString waypoint = waypoints[i];
        waypoint.replace(QRegularExpression("^WP\\d+:"), QString("WP%1:").arg(i + 1));
        waypoints[i] = waypoint;
        waypointList->addItem(waypoint);
    }
}

void Sidebar::onInitialPositionChanged()
{
    double lat = generalLatSpin->value();
    double lon = generalLonSpin->value();
    double alt = generalAltSpin->value();
    emit initialPositionChanged(lat, lon, alt);
    QVector<QPair<double,double>> latLon;
    latLon.push_back({ lat, lon });
    for (const auto &rw : radarWaypointsData) latLon.push_back({ rw.lat, rw.lon });
    emit radarRouteChanged(latLon);
}

void Sidebar::newRadarProfile()
{
    // snapshot current
    RadarProfile rp;
    snapshotActiveRadarInto(rp);
    s_radarProfiles.append(rp);

    radarProfileCounter++;
    if (activeRadarLabel) activeRadarLabel->setText(QString("Active Radar: #%1").arg(radarProfileCounter));
    if (radarNameEdit) radarNameEdit->setText(QString("Radar %1").arg(radarProfileCounter));
    if (generalLatSpin) generalLatSpin->setValue(47.93943);
    if (generalLonSpin) generalLonSpin->setValue(3.68785);
    if (generalAltSpin) generalAltSpin->setValue(1000.0);
    if (generalRcsSpin) generalRcsSpin->setValue(10.0);
    if (generalVelNSpin) generalVelNSpin->setValue(0.0);
    if (generalVelESpin) generalVelESpin->setValue(0.0);
    if (generalVelDSpin) generalVelDSpin->setValue(0.0);
    radarWaypointsData.clear();
    if (radarWaypointList) radarWaypointList->clear();
}

void Sidebar::snapshotActiveRadarInto(RadarProfile &out) const
{
    out.name = radarName();
    out.initLat = radarInitLat();
    out.initLon = radarInitLon();
    out.initAlt = radarInitAlt();
    out.velN = radarInitVelN();
    out.velE = radarInitVelE();
    out.velD = radarInitVelD();
    out.route = radarWaypointsData;
    out.cfg = currentRadarConfig;
}

QList<Sidebar::RadarProfile> Sidebar::getAllRadarProfiles() const
{
    QList<RadarProfile> list = s_radarProfiles;
    // Include current active as well
    RadarProfile cur; snapshotActiveRadarInto(cur);
    list.append(cur);
    return list;
}

QMap<QString, QVector<RadarRouteWaypoint>> Sidebar::buildAllTargetRoutes() const
{
    QMap<QString, QVector<RadarRouteWaypoint>> map;
    for (auto it = targets.begin(); it != targets.end(); ++it) {
        const Target &t = it.value();
        QVector<RadarRouteWaypoint> r;
        QRegularExpression re("Lat=([+-]?\\d+\\.?\\d*),\\s*Lon=([+-]?\\d+\\.?\\d*)");
        for (const QString &w : t.waypoints) {
            auto m = re.match(w);
            if (m.hasMatch()) {
                RadarRouteWaypoint wp{};
                wp.lat = m.captured(1).toDouble();
                wp.lon = m.captured(2).toDouble();
                wp.alt = t.trajAltitude;
                wp.velN = t.trajVelocityN;
                wp.velE = t.trajVelocityE;
                wp.velD = t.trajVelocityD;
                r.push_back(wp);
            }
        }
        map[it.key()] = r;
    }
    return map;
}

QList<Target> Sidebar::getAllTargets() const
{
    QList<Target> list;
    for (auto it = targets.begin(); it != targets.end(); ++it) list.push_back(it.value());
    return list;
}

void Sidebar::onTabChanged(int index)
{
    Q_UNUSED(index);
}