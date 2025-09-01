#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QUrl>
#include <QTimer>
#include <QList>
#include <QVector>
#include <QPair>

// Weather condition struct
struct WeatherCondition {
    double latitude;
    double longitude;
    double radius; // km
    QString type; // "Rain" or "Fog&Cloud"
    double rainRate; // mm/h
    double rainTemperature; // K
    double fogVisibility; // m
    double fogTemperature; // K
};

// DTED file struct
struct DTEDFile {
    QString fileName;
    QString filePath;
    double latitude;
    double longitude;
    double radius; // km
    QString level; // DTED level
    double resolution; // terrain resolution
    bool isVisible; // visibility flag
};

// Target struct
struct Target {
    QString name;
    // Initial Position
    double initLatitude;
    double initLongitude;
    double initAltitude; // m
    double initRCS; // dBsm
    double initVelocityN; // mps
    double initVelocityE; // mps
    double initVelocityD; // mps
    // Trajectory
    double trajLatitude;
    double trajLongitude;
    double trajAltitude; // m
    double trajVelocityN; // mps
    double trajVelocityE; // mps
    double trajVelocityD; // mps
    // Waypoints
    QStringList waypoints;
};

// Radar route waypoint (for simulation)
struct RadarRouteWaypoint {
    double lat{0.0};
    double lon{0.0};
    double alt{0.0};
    double velN{0.0};
    double velE{0.0};
    double velD{0.0};
};

Q_DECLARE_METATYPE(RadarRouteWaypoint)

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = nullptr);
    ~MapWidget();

    void loadMap();
    void setZoom(int zoom);
    void setCenter(double lat, double lon);
    void addMarker(double lat, double lon, const QString &title = "");
    void clearMarkers();
    void addWeatherCondition(const WeatherCondition &condition);
    void clearWeatherConditions();
    void removeWeatherCondition(int index);
    void addDTEDFile(const DTEDFile &dtedFile);
    void clearDTEDFiles();
    void setShowDTEDAreas(bool show);
    void setShowWeatherConditions(bool show);
    void updateLegend();
    // Geodezik alan ve DTED footprint
    void drawGeodesicCircle(double lat, double lon, double radiusKm,
                            const QString &id,
                            const QString &fillColor = "#0066cc40",
                            const QString &lineColor = "#0066cc");
    void drawDTEDFootprint(double minLon, double minLat, double maxLon, double maxLat,
                           const QString &id,
                           const QString &fillColor = "#8B451340",
                           const QString &lineColor = "#654321");
    // Trajectory polyline
    void drawTrajectory(const QString &targetName, const QVector<QPair<double,double>> &latLon);
    void clearTrajectories();
    
    // Target fonksiyonları
    void addTarget(const Target &target);
    void removeTarget(const QString &targetName);
    void updateTargetPosition(const QString &targetName, double lat, double lon, double alt);
    void clearTargets();
    
    // Initial Position fonksiyonları (General tab'dan)
    void addInitialPosition(double lat, double lon, double alt, const QString &name = "Initial Position");
    void updateInitialPosition(double lat, double lon, double alt);
    void clearInitialPosition();

    // Multi-radar markers
    void addRadar(const QString &radarName, double lat, double lon, double alt);
    void updateRadar(const QString &radarName, double lat, double lon, double alt);
    void removeRadar(const QString &radarName);

private slots:
    void onLoadFinished(bool success);
    void onLoadProgress(int progress);

private:
    void setupWebView();
    void createMapHTML();
    QString generateMapHTML();
    QString generateWeatherConditionsJS();
    QString generateLegendHTML();
    void drawWeatherCondition(const WeatherCondition &condition);
    void drawDTEDArea(const DTEDFile &dtedFile);

    QWebEngineView *webView;
    QWebEnginePage *webPage;
    QVBoxLayout *layout;
    
    // Harita ayarları
    double centerLat;
    double centerLon;
    int zoomLevel;
    QString mapStyleUrl;
    QString mapKey;
    
    // Marker'lar için
    QList<QPair<double, double>> markers;
    QList<QString> markerTitles;
    
    // Weather conditions için
    QList<WeatherCondition> weatherConditions;
    bool showWeatherConditions;
    
    // DTED files için
    QList<DTEDFile> dtedFiles;
    bool showDTEDAreas;
};

#endif // MAPWIDGET_H
