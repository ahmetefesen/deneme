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
    
    // Timer'ı başlat
    timeTimer = new QTimer(this);
    connect(timeTimer, &QTimer::timeout, this, &MainWindow::updateTime);
    timeTimer->start(1000);
    updateTime();
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

    // Boyut ve esneme
    mainSplitter->setSizes({300, 1100});
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
}

void MainWindow::newFile()
{
    QMessageBox::information(this, "New File", "New file functionality will be implemented.");
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
    QMessageBox::information(this, "Save File", "Save file functionality will be implemented.");
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

void MainWindow::updateTime()
{
    QTime currentTime = QTime::currentTime();
    controlPanel->updateTime(currentTime.toString("hh:mm:ss"));
}

void MainWindow::startSimulation()
{
    isRunning = true;
    statusLabel->setText("Running");
    controlPanel->setStatus("Running");
}

void MainWindow::stopSimulation()
{
    isRunning = false;
    controlPanel->setStatus("Paused");
    statusLabel->setText("Simulation stopped");
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
