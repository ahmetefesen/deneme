#include "mainwindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QTime>
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <QSplitter>
#include <QGroupBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , isRunning(false)
{
    setWindowTitle("Radar Map Application");
    setMinimumSize(1200, 800);
    resize(1400, 900);
    
    createMenus();
    createStatusBar();
    setupLayout();
    createConnections();
    
    // Başlangıç initial position'ını göster
    QTimer::singleShot(1000, this, [this]() {
        updateInitialPositionOnMap(47.93943, 3.68785, 1000.0);
    });
    

}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenus()
{
    // File menüsü
    fileMenu = menuBar()->addMenu("&File");
    
    newAction = new QAction("&New", this);
    newAction->setShortcut(QKeySequence::New);
    fileMenu->addAction(newAction);
    
    openAction = new QAction("&Open", this);
    openAction->setShortcut(QKeySequence::Open);
    fileMenu->addAction(openAction);
    
    closeAction = new QAction("&Close", this);
    closeAction->setShortcut(QKeySequence::Close);
    fileMenu->addAction(closeAction);
    
    fileMenu->addSeparator();
    
    saveAction = new QAction("&Save", this);
    saveAction->setShortcut(QKeySequence::Save);
    fileMenu->addAction(saveAction);
    
    saveAsAction = new QAction("Save &As", this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    fileMenu->addAction(saveAsAction);
    
    // Menu menüsü
    menuMenu = menuBar()->addMenu("&Menu");
    
    settingsAction = new QAction("&Settings", this);
    menuMenu->addAction(settingsAction);
    
    // Help menüsü
    helpMenu = menuBar()->addMenu("&Help");
    
    documentationAction = new QAction("&Documentation", this);
    helpMenu->addAction(documentationAction);
    
    aboutAction = new QAction("&About", this);
    helpMenu->addAction(aboutAction);
}

void MainWindow::createStatusBar()
{
    statusLabel = new QLabel("Ready");
    statusBar()->addWidget(statusLabel);
}

void MainWindow::setupLayout()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Splitter
    mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->setOpaqueResize(true);
    mainSplitter->setChildrenCollapsible(true);
    mainLayout->addWidget(mainSplitter);

    // Sol: Sidebar
    sidebar = new Sidebar(this);
    mainSplitter->addWidget(sidebar);

    // Sağ: Harita + ControlPanel konteyneri
    mapContainer = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(mapContainer);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    // Offline harita widget'ını kullan
    mapWidget = new MapWidget(this);
    rightLayout->addWidget(mapWidget, 1);

    controlPanel = new ControlPanel(this);
    rightLayout->addWidget(controlPanel, 0);

    mainSplitter->addWidget(mapContainer);

    // Collapsible ayarlarını widgetlar eklendikten sonra yap
    mainSplitter->setCollapsible(0, true); // Sidebar tamamen kapanabilsin
    mainSplitter->setCollapsible(1, true);

    // Boyut ve esneme: Başlangıçta ilk iki sekme başlığını sığdıracak genişlik
    int sideW = sidebar ? sidebar->preferredWidthForFirstTabs(2) : 300;
    mainSplitter->setSizes({ sideW, qMax(800, width() - sideW) });
    mainSplitter->setStretchFactor(0, 0);
    mainSplitter->setStretchFactor(1, 1);
}

void MainWindow::createConnections()
{
    // Menü bağlantıları
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(closeAction, &QAction::triggered, this, &MainWindow::closeFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::showSettings);
    connect(documentationAction, &QAction::triggered, this, &MainWindow::showDocumentation);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    
    // Control panel bağlantıları
    connect(controlPanel, &ControlPanel::startClicked, this, &MainWindow::startSimulation);
    connect(controlPanel, &ControlPanel::stopClicked, this, &MainWindow::stopSimulation);
    
    // Weather condition bağlantıları
    connect(controlPanel, &ControlPanel::showWeatherConditionsChanged, mapWidget, &MapWidget::setShowWeatherConditions);
    connect(sidebar, &Sidebar::weatherConditionAdded, this, &MainWindow::addWeatherCondition);
    connect(sidebar, &Sidebar::weatherConditionRemoved, this, &MainWindow::removeWeatherCondition);
    
    // DTED files bağlantıları
    connect(controlPanel, &ControlPanel::showDTEDAreasChanged, mapWidget, &MapWidget::setShowDTEDAreas);
    connect(sidebar, &Sidebar::dtedFilesAdded, this, &MainWindow::addDTEDFiles);
    
    // Target bağlantıları
    connect(sidebar, &Sidebar::targetAdded, this, &MainWindow::addTargetToSimulation);
    connect(sidebar, &Sidebar::targetRemoved, this, &MainWindow::removeTargetFromSimulation);
    connect(sidebar, &Sidebar::targetTrajectoryChanged, this, [this](const QString &name, const QVector<QPair<double,double>> &latLon){
        if (controlPanel) {
            // Checkbox açıksa çiz
            // showTargetsTrajChanged sinyali MapWidget temizlemeyi de yönetiyor
            // Burada sadece istenirse çizgiyi güncelliyoruz
            mapWidget->drawTrajectory(name, latLon);
        }
    });
    connect(controlPanel, &ControlPanel::showTargetsTrajChanged, this, &MainWindow::onShowTargetsTraj);
    
    // Target pozisyon güncellemelerini MapWidget'a ilet
    connect(controlPanel, &ControlPanel::targetPositionUpdated, this, &MainWindow::updateTargetPositionOnMap);

    // Radar pozisyon güncellemesini initial marker ile gösterelim
    connect(controlPanel, &ControlPanel::radarPositionUpdated, this, [this](double lat, double lon, double alt){
        mapWidget->updateInitialPosition(lat, lon, alt);
        // Çoklu radar için isimlendirilmiş marker da güncellenebilir (tek profil için başlangıçta ekleme yapılabilir)
        if (sidebar) mapWidget->updateRadar(sidebar->radarName(), lat, lon, alt);
    });
    connect(controlPanel, &ControlPanel::namedRadarPositionUpdated, this, [this](const QString &name, double lat, double lon, double alt){
        mapWidget->updateRadar(name, lat, lon, alt);
    });

    // Initial position değişikliklerini MapWidget'a ilet
    connect(sidebar, &Sidebar::initialPositionChanged, this, &MainWindow::updateInitialPositionOnMap);
}

void MainWindow::newFile()
{
    if (sidebar) sidebar->newRadarProfile();
    QMessageBox::information(this, "New Radar", "New radar profile created.");
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "All Files (*)");
    if (!fileName.isEmpty()) {
        QMessageBox::information(this, "Open File", "Opening file: " + fileName);
    }
}

void MainWindow::closeFile()
{
    QMessageBox::information(this, "Close File", "Close file functionality will be implemented.");
}

void MainWindow::saveFile()
{
    if (!saveAction || !saveAction->isEnabled()) {
        QMessageBox::warning(this, "Not Allowed", "Simulation has not been stopped yet. Save is disabled until after first stop.");
        return;
    }

    const QString ts = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    QString defaultName = QString("scenario_%1.json").arg(ts);
    QString fileName = QFileDialog::getSaveFileName(this, "Save Scenario", defaultName, "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QJsonObject root;
    root["hz"] = controlPanel ? controlPanel->hz() : 10;
    root["showTargetsTraj"] = controlPanel ? controlPanel->showTargetsTrajEnabled() : false;
    root["calculateWeather"] = sidebar ? sidebar->sidebarCalculateWeatherEnabled() : false;

    // Radar initial
    QJsonObject radarInit;
    radarInit["name"] = sidebar ? sidebar->radarName() : "Radar";
    radarInit["lat"] = sidebar ? sidebar->radarInitLat() : 0.0;
    radarInit["lon"] = sidebar ? sidebar->radarInitLon() : 0.0;
    radarInit["alt"] = sidebar ? sidebar->radarInitAlt() : 0.0;
    radarInit["velN"] = sidebar ? sidebar->radarInitVelN() : 0.0;
    radarInit["velE"] = sidebar ? sidebar->radarInitVelE() : 0.0;
    radarInit["velD"] = sidebar ? sidebar->radarInitVelD() : 0.0;
    root["radarInitial"] = radarInit;

    // Radar route
    QJsonArray radarRouteArr;
    if (sidebar) {
        auto r = sidebar->radarRouteWaypoints();
        for (const auto &wp : r) {
            QJsonObject w;
            w["lat"] = wp.lat; w["lon"] = wp.lon; w["alt"] = wp.alt;
            w["velN"] = wp.velN; w["velE"] = wp.velE; w["velD"] = wp.velD;
            radarRouteArr.append(w);
        }
    }
    root["radarRoute"] = radarRouteArr;

    // Targets full
    QJsonArray targetsArr;
    if (sidebar) {
        auto tgts = sidebar->getAllTargets();
        auto routes = sidebar->buildAllTargetRoutes();
        for (const auto &t : tgts) {
            QJsonObject to;
            to["name"] = t.name;
            QJsonObject init;
            init["lat"] = t.initLatitude; init["lon"] = t.initLongitude; init["alt"] = t.initAltitude;
            init["rcs"] = t.initRCS; init["velN"] = t.initVelocityN; init["velE"] = t.initVelocityE; init["velD"] = t.initVelocityD;
            to["initial"] = init;
            QJsonObject traj;
            traj["lat"] = t.trajLatitude; traj["lon"] = t.trajLongitude; traj["alt"] = t.trajAltitude;
            traj["velN"] = t.trajVelocityN; traj["velE"] = t.trajVelocityE; traj["velD"] = t.trajVelocityD;
            to["trajectory"] = traj;
            QJsonArray wps;
            if (routes.contains(t.name)) {
                for (const auto &wp : routes[t.name]) {
                    QJsonObject w; w["lat"] = wp.lat; w["lon"] = wp.lon; w["alt"] = wp.alt; w["velN"] = wp.velN; w["velE"] = wp.velE; w["velD"] = wp.velD; wps.append(w);
                }
            }
            to["waypoints"] = wps;
            targetsArr.append(to);
        }
    }
    root["targets"] = targetsArr;

    // Weather
    QJsonArray weatherArr;
    if (sidebar) {
        for (const auto &w : sidebar->getWeatherConditions()) {
            QJsonObject wo;
            wo["lat"] = w.latitude; wo["lon"] = w.longitude; wo["radiusKm"] = w.radius; wo["type"] = w.type;
            wo["rainRate"] = w.rainRate; wo["rainTemp"] = w.rainTemperature;
            wo["fogVisibility"] = w.fogVisibility; wo["fogTemp"] = w.fogTemperature;
            weatherArr.append(wo);
        }
    }
    root["weather"] = weatherArr;

    // Terrain (DTED paths)
    QJsonArray terrainArr;
    if (sidebar) {
        for (const auto &p : sidebar->getDTEDFiles()) terrainArr.append(p);
    }
    root["terrain"] = terrainArr;

    // Multi-radar profiles
    QJsonArray radarsArr;
    if (sidebar) {
        auto profiles = sidebar->getAllRadarProfiles();
        for (const auto &rp : profiles) {
            QJsonObject ro; ro["name"] = rp.name; ro["lat"] = rp.initLat; ro["lon"] = rp.initLon; ro["alt"] = rp.initAlt; ro["velN"] = rp.velN; ro["velE"] = rp.velE; ro["velD"] = rp.velD;
            QJsonArray rr; for (const auto &wp : rp.route) { QJsonObject w; w["lat"] = wp.lat; w["lon"] = wp.lon; w["alt"] = wp.alt; w["velN"] = wp.velN; w["velE"] = wp.velE; w["velD"] = wp.velD; rr.append(w);} ro["route"] = rr;
            radarsArr.append(ro);
        }
    }
    root["radars"] = radarsArr;

    QJsonDocument doc(root);
    QFile f(fileName);
    if (f.open(QIODevice::WriteOnly)) {
        f.write(doc.toJson(QJsonDocument::Indented));
        f.close();
        QMessageBox::information(this, "Saved", QString("Scenario saved to: %1").arg(fileName));
    } else {
        QMessageBox::warning(this, "Error", "Failed to save file.");
    }
}

void MainWindow::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save As", "", "All Files (*)");
    if (!fileName.isEmpty()) {
        QMessageBox::information(this, "Save As", "Saving as: " + fileName);
    }
}

void MainWindow::showSettings()
{
    QMessageBox::information(this, "Settings", "Settings dialog will be implemented.");
}

void MainWindow::showDocumentation()
{
    QMessageBox::information(this, "Documentation", "Documentation will be implemented.");
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, "About", 
        "Radar Map Application\n\n"
        "Version 1.0.0\n"
        "A Qt-based desktop application for radar simulation and visualization.\n\n"
        "Features:\n"
        "- Interactive map with Mercator projection\n"
        "- Radar simulation capabilities\n"
        "- Dark theme interface\n"
        "- Resizable layout");
}



void MainWindow::startSimulation()
{
    isRunning = true;
    statusLabel->setText("Running");
    controlPanel->setStatus("Running");
    if (controlPanel) controlPanel->setRunningUI(true);

    if (sidebar && controlPanel) {
        controlPanel->setRadarInitialKinematics(
            sidebar->radarInitLat(),
            sidebar->radarInitLon(),
            sidebar->radarInitAlt(),
            sidebar->radarInitVelN(),
            sidebar->radarInitVelE(),
            sidebar->radarInitVelD()
        );
        controlPanel->setRadarRoute(sidebar->radarRouteWaypoints());

        mapWidget->addRadar(sidebar->radarName(), sidebar->radarInitLat(), sidebar->radarInitLon(), sidebar->radarInitAlt());

        auto profiles = sidebar->getAllRadarProfiles();
        for (const auto &rp : profiles) {
            controlPanel->addRadarProfile(rp.name, rp.initLat, rp.initLon, rp.initAlt, rp.velN, rp.velE, rp.velD,
                                          [&](){ QVector<RadarRouteWaypoint> v; for(const auto &x: rp.route){ RadarRouteWaypoint w{ x.lat,x.lon,x.alt,x.velN,x.velE,x.velD}; v.push_back(w);} return v;}());
            mapWidget->addRadar(rp.name, rp.initLat, rp.initLon, rp.initAlt);
        }

        auto targets = sidebar->getAllTargets();
        for (const auto &t : targets) {
            addTargetToSimulation(t);
        }
        auto routes = sidebar->buildAllTargetRoutes();
        for (auto it = routes.begin(); it != routes.end(); ++it) {
            controlPanel->setTargetRoute(it.key(), it.value());
            if (controlPanel->showTargetsTrajEnabled()) {
                QVector<QPair<double,double>> latLon;
                for (const auto &tar : targets) {
                    if (tar.name == it.key()) {
                        latLon.push_back({ tar.initLatitude, tar.initLongitude });
                        for (const auto &wp : it.value()) latLon.push_back({ wp.lat, wp.lon });
                        break;
                    }
                }
                mapWidget->drawTrajectory(it.key(), latLon);
            }
        }
    }

    if (mainSplitter && sidebar) sidebar->hide();
    if (saveAction) saveAction->setEnabled(false);
}

void MainWindow::stopSimulation()
{
    isRunning = false;
    controlPanel->setStatus("Paused");
    statusLabel->setText("Simulation stopped");
    if (controlPanel) controlPanel->setRunningUI(false);

    if (mainSplitter && sidebar) sidebar->show();
    if (saveAction) saveAction->setEnabled(true);
}

void MainWindow::addWeatherCondition(const WeatherCondition &condition)
{
    mapWidget->addWeatherCondition(condition);
    mapWidget->updateLegend();
}

void MainWindow::removeWeatherCondition(int index)
{
    mapWidget->removeWeatherCondition(index);
}

void MainWindow::addDTEDFiles(const QStringList &fileNames)
{
    for (const QString &fileName : fileNames) {
        DTEDFile dtedFile;
        dtedFile.fileName = QFileInfo(fileName).fileName();
        dtedFile.filePath = fileName;
        dtedFile.isVisible = true;
        dtedFile.latitude = 0.0; // Varsayılan değerler
        dtedFile.longitude = 0.0;
        dtedFile.resolution = 30.0;
        dtedFile.level = "DTED-1";
        
        mapWidget->addDTEDFile(dtedFile);
    }
    mapWidget->updateLegend();
}

void MainWindow::addTargetToSimulation(const Target &target)
{
    // ControlPanel'e target'ı ekle
    controlPanel->addTarget(target);
    
    // MapWidget'a da target'ı ekle
    mapWidget->addTarget(target);
}

void MainWindow::removeTargetFromSimulation(const QString &targetName)
{
    // ControlPanel'den target'ı kaldır
    controlPanel->removeTarget(targetName);
    
    // MapWidget'dan da target'ı kaldır
    mapWidget->removeTarget(targetName);
}

void MainWindow::updateTargetPositionOnMap(const QString &targetName, double lat, double lon, double alt)
{
    // MapWidget'da target pozisyonunu güncelle
    mapWidget->updateTargetPosition(targetName, lat, lon, alt);
}

void MainWindow::updateInitialPositionOnMap(double lat, double lon, double alt)
{
    // İlk kez ekleniyor mu kontrol et
    static bool initialPositionExists = false;
    
    if (!initialPositionExists) {
        // İlk kez ekle
        mapWidget->addInitialPosition(lat, lon, alt, "Initial Position");
        initialPositionExists = true;
    } else {
        // Mevcut pozisyonu güncelle
        mapWidget->updateInitialPosition(lat, lon, alt);
    }
}

void MainWindow::onTargetTrajectoryChanged(const QString &targetName, const QVector<QPair<double,double>> &latLon)
{
    Q_UNUSED(targetName);
    // ControlPanel tarafındaki toggle durumunu bilmiyoruz; basitçe çizmeyi deneyelim
    mapWidget->drawTrajectory(targetName, latLon);
}

void MainWindow::onShowTargetsTraj(bool enabled)
{
    if (!enabled) {
        mapWidget->clearTrajectories();
    }
}

void MainWindow::onRadarRouteChanged(const QVector<QPair<double,double>> &latLon)
{
    // Radar rotasını özel bir id ile çizelim
    mapWidget->drawTrajectory("radar-route", latLon);
}
