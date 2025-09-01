#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QTime>
#include "mapwidget.h"
#include "sidebar.h"
#include "controlpanel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newFile();
    void openFile();
    void closeFile();
    void saveFile();
    void saveAsFile();
    void showSettings();
    void showDocumentation();
    void showAbout();
    void startSimulation();
    void stopSimulation();
    void addWeatherCondition(const WeatherCondition &condition);
    void removeWeatherCondition(int index);
    void addDTEDFiles(const QStringList &fileNames);
    void addTargetToSimulation(const Target &target);
    void removeTargetFromSimulation(const QString &targetName);
    void updateTargetPositionOnMap(const QString &targetName, double lat, double lon, double alt);
    void updateInitialPositionOnMap(double lat, double lon, double alt);
    void onTargetTrajectoryChanged(const QString &targetName, const QVector<QPair<double,double>> &latLon);
    void onShowTargetsTraj(bool enabled);
    void onRadarRouteChanged(const QVector<QPair<double,double>> &latLon);

private:
    void createMenus();
    void createStatusBar();
    void setupLayout();
    void createConnections();

    // UI bileşenleri
    QSplitter *mainSplitter;
    Sidebar *sidebar;
    QWidget *mapContainer;   // sağ panel için konteyner
    MapWidget *mapWidget;
    ControlPanel *controlPanel;
    
    // Menü öğeleri
    QMenu *fileMenu;
    QMenu *menuMenu;
    QMenu *helpMenu;
    
    // Aksiyonlar
    QAction *newAction;
    QAction *openAction;
    QAction *closeAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *settingsAction;
    QAction *documentationAction;
    QAction *aboutAction;
    
    // Durum
    QLabel *statusLabel;
    bool isRunning;
};

#endif // MAINWINDOW_H
