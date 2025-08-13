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
#include "mapwidget.h"

class Sidebar : public QWidget
{
    Q_OBJECT

public:
    explicit Sidebar(QWidget *parent = nullptr);
    ~Sidebar();

signals:
    void weatherConditionAdded(const WeatherCondition &condition);
    void weatherConditionRemoved(int index);
    void dtedFilesAdded(const QStringList &fileNames);

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
    void onAddTarget();
    void onDeleteTarget();
    void onAppendWaypoint();
    void onDeleteWaypoint();

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
    QWidget *targetTab;
    
    // Radar tab için sub-tab'lar
    QTabWidget *radarSubTabs;
    QWidget *radarPage1;
    QWidget *radarPage2;
    
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
    
    // Terrain tab için değişkenler
    QListWidget *dtedFilesList;
    QComboBox *dtedLevelComboBox;
    QDoubleSpinBox *terrainResolutionSpinBox;

    // Radar Page 1 (Monostatic) bileşenleri ve durum
    QGroupBox *radarConfigGroup;
    QComboBox *radarModeCombo;
    QDoubleSpinBox *centerFreqSpin;         // GHz
    QDoubleSpinBox *txPeakPowerSpin;        // kW
    QDoubleSpinBox *pulseWidthSpin;         // us
    QDoubleSpinBox *bandwidthSpin;          // MHz

    // PRF Section
    QGroupBox *prfGroup;
    QCheckBox *hopPrfsRandomlyCheck;
    QComboBox *prfCountCombo;               // 1..7
    QComboBox *prfIndexCombo;               // 1..N
    QDoubleSpinBox *prfValueSpin;           // Hz
    QVector<double> prfValues;              // size up to 7

    // Waveform Section
    QGroupBox *waveformGroup;
    QComboBox *waveformTypeCombo;           // Rect, LFM, Barker, Frank, P1..P4, ZC
    QComboBox *barkerLengthCombo;           // visible only for Barker
    QDoubleSpinBox *frankCodeSizeSpin;      // visible only for Frank
    QDoubleSpinBox *pCodeSizeSpin;          // visible for P1..P4
    QDoubleSpinBox *zcRootSpin;             // visible for Zadoff-Chu
    QComboBox *windowingTypeCombo;

    // CFAR Section
    QGroupBox *cfarGroup;
    QCheckBox *useCFARCheck;
    QComboBox *cfarTypeCombo;               // CA, SO, GO, OS
    QSpinBox *numTrainingCellsSpin;
    QSpinBox *numGuardCellsSpin;
    QSpinBox *osRankSpin;
    QDoubleSpinBox *pfaSpin;
    QLabel *pfaFormattedLabel;

    // STC Section
    QGroupBox *stcGroup;
    QCheckBox *useSTCCheck;
    QDoubleSpinBox *stcCutoffRangeSpin;     // m
    QDoubleSpinBox *stcFactorSpin;

    // Frequency Agile Section
    QGroupBox *freqAgileGroup;
    QCheckBox *useFreqHopCheck;
    QDoubleSpinBox *lowerFreqMHzSpin;       // MHz
    QDoubleSpinBox *upperFreqMHzSpin;       // MHz

    // Radar Page 2 (Antenna Configuration)
    QGroupBox *antennaConfigGroup;
    QComboBox *antennaPolarizationCombo;    // Horizontal, Vertical
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
    QListWidget *targetList;
    QLineEdit *targetNameEdit;
    QDoubleSpinBox *initLatSpin;
    QDoubleSpinBox *initLonSpin;
    QDoubleSpinBox *initAltSpin;
    QDoubleSpinBox *initRCSSpin;
    QDoubleSpinBox *initVelNSpin;
    QDoubleSpinBox *initVelESpin;
    QDoubleSpinBox *initVelDSpin;
    QDoubleSpinBox *trajLatSpin;
    QDoubleSpinBox *trajLonSpin;
    QDoubleSpinBox *trajAltSpin;
    QDoubleSpinBox *trajVelNSpin;
    QDoubleSpinBox *trajVelESpin;
    QDoubleSpinBox *trajVelDSpin;
    QListWidget *waypointList;
    QPushButton *addTargetBtn;
    QPushButton *deleteTargetBtn;
    QPushButton *appendWaypointBtn;
    QPushButton *deleteWaypointBtn;

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
};

#endif // SIDEBAR_H
