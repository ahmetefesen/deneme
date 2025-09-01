#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
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
#include <QVector>
#include <QStackedWidget>
#include "mapwidget.h"

class Sidebar : public QWidget
{
    Q_OBJECT

public:
    explicit Sidebar(QWidget *parent = nullptr);
    ~Sidebar();

    // Public getters to feed ControlPanel at Start
    double radarInitLat() const { return generalLatSpin ? generalLatSpin->value() : 0.0; }
    double radarInitLon() const { return generalLonSpin ? generalLonSpin->value() : 0.0; }
    double radarInitAlt() const { return generalAltSpin ? generalAltSpin->value() : 0.0; }
    double radarInitVelN() const { return generalVelNSpin ? generalVelNSpin->value() : 0.0; }
    double radarInitVelE() const { return generalVelESpin ? generalVelESpin->value() : 0.0; }
    double radarInitVelD() const { return generalVelDSpin ? generalVelDSpin->value() : 0.0; }
    QString radarName() const { return radarNameEdit ? radarNameEdit->text() : QString("Radar"); }
    QVector<RadarRouteWaypoint> radarRouteWaypoints() const {
        QVector<RadarRouteWaypoint> v;
        v.reserve(radarWaypointsData.size());
        for (const auto &rw : radarWaypointsData) {
            RadarRouteWaypoint w{rw.lat, rw.lon, rw.alt, rw.velN, rw.velE, rw.velD};
            v.push_back(w);
        }
        return v;
    }
    QMap<QString, QVector<RadarRouteWaypoint>> buildAllTargetRoutes() const;
    QList<Target> getAllTargets() const;
    auto getRadarAdvancedConfig() const { return currentRadarConfig; }
    void newRadarProfile();

    // Başlangıçta ilk N sekmenin başlık genişliğine göre tercih edilen genişlik
    int preferredWidthForFirstTabs(int count) const;

signals:
    void weatherConditionAdded(const WeatherCondition &condition);
    void weatherConditionRemoved(int index);
    void dtedFilesAdded(const QStringList &fileNames);
    void targetAdded(const Target &target);
    void targetRemoved(const QString &targetName);
    void initialPositionChanged(double lat, double lon, double alt);
    void targetTrajectoryChanged(const QString &targetName, const QVector<QPair<double,double>> &latLon);
    void radarRouteChanged(const QVector<QPair<double,double>> &latLon);

private slots:
    void onTabChanged(int index);
    void onAddWeatherCondition();
    void onDeleteWeatherCondition();
    void onBrowseDTEDFiles();
    
    // Radar Page 1 slots
    void onRadarModeChanged(int index);
    void onPrfCountChanged(int index);
    void onPrfIndexChanged(int index);
    void onPrfValueChanged(double value);
    void onWaveformTypeChanged(int index);
    void onPfaValueChanged(double value);
    
    // Radar Page 2 slots
    void onFixedGainToggled(bool checked);
    void onBeamPatternToggled(bool checked);
    void onBrowseAzPattern();
    void onBrowseElPattern();
    void onShowRadarConfigs();
    
    // Target tab slots
    void onTargetSelectionChanged();  
    void onAddTarget();
    void onDeleteTarget();
    void onAppendWaypoint();
    void onDeleteWaypoint();
    void onShowWaypointsPage();
    void onBackToTargetsPage();
    
    // General tab slots
    void onInitialPositionChanged();
    // Radar route (General tab)
    void onShowRadarRoutePage();
    void onBackFromRadarRoutePage();
    void onAppendRadarWaypoint();
    void onDeleteRadarWaypoint();
    

private:
    void setupUI();
    void createGeneralTab();
    void createAdvancedPropertiesTab();
    void createTerrainTab();
    void createAtmosphereTab();
    void createRadarTab();
    void createTargetTab();

    QTabWidget *tabWidget;
    
    // Tab'lar
    QWidget *generalTab;
    QWidget *advancedPropertiesTab;
    QWidget *terrainTab;
    QWidget *atmosphereTab;
    QWidget *radarTab;
    // Radar tab alt sekmeleri
    QTabWidget *radarSubTabs;
    QWidget *radarPage1;
    QWidget *radarPage2;
    QWidget *targetTab;
    
    // Atmosphere tab için değişkenler
    QListWidget *weatherConditionList;
    QDoubleSpinBox *latitudeSpinBox;
    QDoubleSpinBox *longitudeSpinBox;
    QDoubleSpinBox *circleRadiusSpinBox;
    QComboBox *conditionTypeComboBox;
    QDoubleSpinBox *rainRateSpinBox;
    QDoubleSpinBox *rainTemperatureSpinBox;
    QDoubleSpinBox *fogVisibilitySpinBox;
    QDoubleSpinBox *fogTemperatureSpinBox;
    QCheckBox *calculateWeatherCheckSB;
    
    // Terrain tab için değişkenler
    QListWidget *dtedFilesList;
    QComboBox *dtedLevelComboBox;
    QDoubleSpinBox *terrainResolutionSpinBox;

    // Radar Page 1 (Monostatic) bileşenleri ve durum
    QGroupBox *radarConfigGroup;
    QComboBox *radarModeCombo;        
    QDoubleSpinBox *centerFreqSpin;   
    QDoubleSpinBox *txPeakPowerSpin;  
    QDoubleSpinBox *pulseWidthSpin;   
    QDoubleSpinBox *bandwidthSpin;    

    // PRF Section
    QGroupBox *prfGroup;
    QCheckBox *hopPrfsRandomlyCheck;
    QComboBox *prfCountCombo;         
    QComboBox *prfIndexCombo;         
    QDoubleSpinBox *prfValueSpin;     
    QVector<double> prfValues;        

    // Waveform Section
    QGroupBox *waveformGroup;
    QComboBox *waveformTypeCombo;     
    QComboBox *barkerLengthCombo;     
    QDoubleSpinBox *frankCodeSizeSpin;
    QDoubleSpinBox *pCodeSizeSpin;    
    QDoubleSpinBox *zcRootSpin;       
    QComboBox *windowingTypeCombo;

    // CFAR Section
    QGroupBox *cfarGroup;
    QCheckBox *useCFARCheck;
    QComboBox *cfarTypeCombo;         
    QSpinBox *numTrainingCellsSpin;
    QSpinBox *numGuardCellsSpin;
    QSpinBox *osRankSpin;
    QDoubleSpinBox *pfaSpin;
    QLabel *pfaFormattedLabel;

    // STC Section
    QGroupBox *stcGroup;
    QCheckBox *useSTCCheck;
    QDoubleSpinBox *stcCutoffRangeSpin;     
    QDoubleSpinBox *stcFactorSpin;

    // Frequency Agile Section
    QGroupBox *freqAgileGroup;
    QCheckBox *useFreqHopCheck;
    QDoubleSpinBox *lowerFreqMHzSpin;       
    QDoubleSpinBox *upperFreqMHzSpin;       

    // Radar Page 2 (Antenna Configuration)
    QGroupBox *antennaConfigGroup;
    QComboBox *antennaPolarizationCombo;    
    QCheckBox *fixedGainCheck;
    QCheckBox *beamPatternCheck;
    QDoubleSpinBox *fixedGainDbiSpin;
    QLineEdit *azPatternLine;
    QPushButton *azPatternBrowseBtn;
    QLineEdit *elPatternLine;
    QPushButton *elPatternBrowseBtn;
    QDoubleSpinBox *timeDwellSecSpin;
    QDoubleSpinBox *noiseFigureDbSpin;
    QDoubleSpinBox *effectiveTempKSpin;
    QDoubleSpinBox *totalSystemLossDbSpin;
    QDoubleSpinBox *radarSnrThresholdDbSpin;
    QDoubleSpinBox *timeScanSecSpin;
    QPushButton *showRadarConfigsBtn;

    // Target tab için değişkenler
    // Stacked pages
    QStackedWidget *targetStack;
    QWidget *targetMainPage;
    QWidget *targetWaypointsPage;
    QPushButton *showWaypointsBtn;
    QPushButton *backToTargetsBtn;

    // Target List
    QListWidget *targetList;
    QLineEdit *targetNameEdit;
    QPushButton *addTargetBtn;
    QPushButton *deleteTargetBtn;

    // Initial Position
    QDoubleSpinBox *initLatSpin;
    QDoubleSpinBox *initLonSpin;
    QDoubleSpinBox *initAltSpin;
    QDoubleSpinBox *initRCSSpin;
    QDoubleSpinBox *initVelNSpin;
    QDoubleSpinBox *initVelESpin;
    QDoubleSpinBox *initVelDSpin;

    // Trajectory + Waypoints (detay sayfasi)
    QDoubleSpinBox *trajLatSpin;
    QDoubleSpinBox *trajLonSpin;
    QDoubleSpinBox *trajAltSpin;
    QDoubleSpinBox *trajVelNSpin;
    QDoubleSpinBox *trajVelESpin;
    QDoubleSpinBox *trajVelDSpin;
    QListWidget *waypointList;
    QPushButton *appendWaypointBtn;
    QPushButton *deleteWaypointBtn;

    QLabel *selectedTargetLabel;
    
    // Radar konfigürasyon saklama
    struct RadarConfig {
        QString radarMode;
        double centerFreqGHz = 0.0;
        double txPeakKw = 0.0;
        double pulseWidthUs = 0.0;
        double bandwidthMHz = 0.0;
        bool hopPrfsRandomly = false;
        int prfCount = 1;
        QVector<double> prfValues;
        QString waveformType;
        int barkerLength = 0;
        double frankCodeSize = 0.0;
        double pCodeSize = 0.0;
        double zcRoot = 0.0;
        QString windowingType;
        bool useCFAR = false;
        QString cfarType;
        int numTrainingCells = 0;
        int numGuardCells = 0;
        int osRank = 0;
        double pfa = 0.0;
        bool useSTC = false;
        double stcCutoffRange = 0.0;
        double stcFactor = 0.0;
        bool useFreqHop = false;
        double lowerFreqMHz = 0.0;
        double upperFreqMHz = 0.0;
        // Page 2
        QString polarization;
        bool fixedGain = false;
        double fixedGainDbi = 0.0;
        bool beamPattern = false;
        QString azimuthPatternPath;
        QString elevationPatternPath;
        double timeDwellSec = 0.0;
        double noiseFigureDb = 0.0;
        double effectiveTempK = 0.0;
        double totalSystemLossDb = 0.0;
        double radarSnrThresholdDb = 0.0;
        double timeScanSec = 0.0;
    } currentRadarConfig;
    
    QMap<QString, Target> targets;
    QString currentSelectedTarget;
    
    // General tab için değişkenler
    QDoubleSpinBox *generalLatSpin;
    QDoubleSpinBox *generalLonSpin;
    QDoubleSpinBox *generalAltSpin;
    QDoubleSpinBox *generalRcsSpin;
    QDoubleSpinBox *generalVelNSpin;
    QDoubleSpinBox *generalVelESpin;
    QDoubleSpinBox *generalVelDSpin;
    // General tab - alt sekmeler (Initial & Route)
    QTabWidget *generalSubTabs;
    QWidget *generalInitialTab;
    QWidget *generalRouteTab;
    QLabel *activeRadarLabel;
    QLineEdit *radarNameEdit;
    int radarProfileCounter{1};
    QPushButton *openRadarRouteBtn;
    QPushButton *radarBackBtn;
    QListWidget *radarWaypointList;
    QPushButton *radarAppendBtn;
    QPushButton *radarDeleteBtn;
    QDoubleSpinBox *radarWpLatSpin;
    QDoubleSpinBox *radarWpLonSpin;
    QDoubleSpinBox *radarWpAltSpin;
    QDoubleSpinBox *radarWpVelNSpin;
    QDoubleSpinBox *radarWpVelESpin;
    QDoubleSpinBox *radarWpVelDSpin;
    QStringList radarWaypoints;

    struct RadarWaypoint {
        double lat = 0.0;
        double lon = 0.0;
        double alt = 0.0;
        double velN = 0.0;
        double velE = 0.0;
        double velD = 0.0;
    };
    QVector<RadarWaypoint> radarWaypointsData;

    // Stored weather and terrain for saving
    QList<WeatherCondition> weatherStore;
    QStringList dtedStore;
public:
    QList<WeatherCondition> getWeatherConditions() const { return weatherStore; }
    QStringList getDTEDFiles() const { return dtedStore; }
    bool sidebarCalculateWeatherEnabled() const { return calculateWeatherCheckSB ? calculateWeatherCheckSB->isChecked() : false; }

    struct RadarProfile {
        QString name;
        double initLat{0.0};
        double initLon{0.0};
        double initAlt{0.0};
        double velN{0.0};
        double velE{0.0};
        double velD{0.0};
        QVector<RadarWaypoint> route;
        RadarConfig cfg;
    };

    QList<RadarProfile> getAllRadarProfiles() const;
    void snapshotActiveRadarInto(RadarProfile &out) const;
};


#endif // SIDEBAR_H
