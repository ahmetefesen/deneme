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
};

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
