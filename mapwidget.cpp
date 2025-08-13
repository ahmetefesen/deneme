#include "mapwidget.h"
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QUrl>
#include <QTimer>
#include <QDebug>
#include <QApplication>
#include <QDir>

MapWidget::MapWidget(QWidget *parent)
    : QWidget(parent)
    , centerLat(47.93943)
    , centerLon(3.68785)
    , zoomLevel(2)
    , mapStyleUrl("https://api.maptiler.com/maps/01988441-8731-71a1-a587-039aeb5bfab4/style.json")
    , mapKey("4GG7QQD4WbEvrdaLs9UX")
    , showWeatherConditions(false)
    , showDTEDAreas(false)
{
    setupWebView();
    loadMap();
}

MapWidget::~MapWidget()
{
}

void MapWidget::setupWebView()
{
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    
    // WebEngineView oluştur
    webView = new QWebEngineView(this);
    webPage = new QWebEnginePage(webView);
    webView->setPage(webPage);
    
    // WebEngine ayarları
    webPage->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    webPage->settings()->setAttribute(QWebEngineSettings::AllowRunningInsecureContent, true);
    webPage->settings()->setAttribute(QWebEngineSettings::AllowGeolocationOnInsecureOrigins, true);
    // Fullscreen kontrolü QWebEngine içinde her zaman desteklenmediği için JS tarafında koşullu ekleyeceğiz
    
    layout->addWidget(webView);
    
    // Bağlantılar
    connect(webView, &QWebEngineView::loadFinished, this, &MapWidget::onLoadFinished);
    connect(webView, &QWebEngineView::loadProgress, this, &MapWidget::onLoadProgress);
}

void MapWidget::loadMap()
{
    QString html = generateMapHTML();
    webView->setHtml(html);
}

QString MapWidget::generateMapHTML()
{
    QString html = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Radar Map - Offline Tile System</title>
    <!-- MapLibre GL JS (jsDelivr CDN) -->
    <script src="https://cdn.jsdelivr.net/npm/maplibre-gl@3.6.2/dist/maplibre-gl.min.js"></script>
    <link href="https://cdn.jsdelivr.net/npm/maplibre-gl@3.6.2/dist/maplibre-gl.min.css" rel="stylesheet" />
    <style>
        html, body { margin: 0; padding: 0; height: 100%; }
        #map { position: absolute; top: 0; bottom: 0; width: 100%; }
        .offline-indicator {
            position: absolute;
            top: 10px;
            right: 10px;
            background: rgba(0,0,0,0.7);
            color: white;
            padding: 5px 10px;
            border-radius: 5px;
            font-size: 12px;
            z-index: 1000;
        }
        .coordinate-display {
            position: absolute;
            bottom: 10px;
            left: 10px;
            background: rgba(0,0,0,0.7);
            color: white;
            padding: 5px 10px;
            border-radius: 5px;
            font-size: 12px;
            z-index: 1000;
        }
    </style>
</head>
<body>
    <div id='map'></div>
    <div class="offline-indicator">Offline Tile System</div>
    <div class="coordinate-display" id="coords">Lat: 0, Lon: 0</div>
    <script>
        const map = new maplibregl.Map({
            container: 'map',
            style: {
                'version': 8,
                'sources': {
                    'osm': {
                        'type': 'raster',
                        'tiles': [
                            'https://tile.openstreetmap.org/{z}/{x}/{y}.png'
                        ],
                        'tileSize': 256,
                        'attribution': '© OpenStreetMap contributors'
                    }
                },
                'layers': [
                    {
                        'id': 'osm-tiles',
                        'type': 'raster',
                        'source': 'osm',
                        'minzoom': 0,
                        'maxzoom': 19
                    }
                ]
            },
            center: [)";
    
    html += QString::number(centerLon) + ", " + QString::number(centerLat) + "]";
    html += R"(,
            zoom: )" + QString::number(zoomLevel) + R"(
        });
        
        map.on('load', function() {
            console.log('Map loaded successfully');
            )";
    
    for (int i = 0; i < markers.size(); ++i) {
        html += QString("map.addSource('marker-%1', {\n").arg(i);
        html += "    'type': 'geojson,\n";
        html += "    'data': {\n";
        html += "        'type': 'Feature',\n";
        html += "        'geometry': {\n";
        html += "            'type': 'Point',\n";
        html += QString("            'coordinates': [%1, %2]\n").arg(markers[i].second).arg(markers[i].first);
        html += "        },\n";
        html += QString("        'properties': {\n            'title': '%1'\n        }\n").arg(markerTitles[i]);
        html += "    }\n";
        html += "});\n";
        
        html += QString("map.addLayer({\n");
        html += QString("    'id': 'marker-layer-%1',\n").arg(i);
        html += QString("    'type': 'circle',\n");
        html += QString("    'source': 'marker-%1',\n").arg(i);
        html += "    'paint': {\n";
        html += "        'circle-radius': 8,\n";
        html += "        'circle-color': '#ff0000',\n";
        html += "        'circle-stroke-width': 2,\n";
        html += "        'circle-stroke-color': '#ffffff'\n";
        html += "    }\n";
        html += "});\n";
    }
    
    html += R"(
        });
        
        // Koordinat gösterimi
        map.on('mousemove', function(e) {
            const coords = e.lngLat;
            document.getElementById('coords').innerHTML = 
                'Lat: ' + coords.lat.toFixed(6) + ', Lon: ' + coords.lng.toFixed(6);
        });
        
        // Tile bilgisi gösterimi
        map.on('sourcedata', function(e) {
            if (e.sourceId === 'osm' && e.isSourceLoaded) {
                const tiles = map.getStyle().sources.osm.tiles;
                console.log('Loaded tiles:', tiles);
            }
        });
        
        // Zoom kontrolleri
        map.addControl(new maplibregl.NavigationControl(), 'top-right');
        
        // Fullscreen, sadece tarayıcı destekliyorsa ekle
        try {
            const fsEnabled = document.fullscreenEnabled || document.webkitFullscreenEnabled || false;
            if (fsEnabled && maplibregl.FullscreenControl) {
                map.addControl(new maplibregl.FullscreenControl());
            }
        } catch (e) {
            console.warn('Fullscreen not supported:', e);
        }
        
        // Harita tıklama olayı
        map.on('click', function(e) {
            const coordinates = e.lngLat;
            console.log('Clicked at:', coordinates);
        });
    </script>
</body>
</html>
)";
    
    return html;
}

void MapWidget::setZoom(int zoom)
{
    zoomLevel = zoom;
    if (webView && webView->page()) {
        QString script = QString("map.setZoom(%1);").arg(zoom);
        webView->page()->runJavaScript(script);
    }
}

void MapWidget::setCenter(double lat, double lon)
{
    centerLat = lat;
    centerLon = lon;
    if (webView && webView->page()) {
        QString script = QString("map.setCenter([%1, %2]);").arg(lon).arg(lat);
        webView->page()->runJavaScript(script);
    }
}

void MapWidget::addMarker(double lat, double lon, const QString &title)
{
    markers.append(qMakePair(lat, lon));
    markerTitles.append(title);
    
    if (webView && webView->page()) {
        QString script = QString(
            "map.addSource('marker-%1', {"
            "    'type': 'geojson',"
            "    'data': {"
            "        'type': 'Feature',"
            "        'geometry': {"
            "            'type': 'Point',"
            "            'coordinates': [%2, %3]"
            "        },"
            "        'properties': {"
            "            'title': '%4'"
            "        }"
            "    }"
            "});"
            "map.addLayer({"
            "    'id': 'marker-layer-%1',"
            "    'type': 'circle',"
            "    'source': 'marker-%1',"
            "    'paint': {"
            "        'circle-radius': 8,"
            "        'circle-color': '#ff0000',"
            "        'circle-stroke-width': 2,"
            "        'circle-stroke-color': '#ffffff'"
            "    }"
            "});"
        ).arg(markers.size() - 1).arg(lon).arg(lat).arg(title);
        
        webView->page()->runJavaScript(script);
    }
}

void MapWidget::clearMarkers()
{
    markers.clear();
    markerTitles.clear();
    
    if (webView && webView->page()) {
        webView->page()->runJavaScript(
            "const layers = map.getStyle().layers;"
            "layers.forEach(layer => {"
            "    if (layer.id.startsWith('marker-layer-')) {"
            "        map.removeLayer(layer.id);"
            "    }"
            "});"
            "const sources = Object.keys(map.getStyle().sources);"
            "sources.forEach(source => {"
            "    if (source.startsWith('marker-')) {"
            "        map.removeSource(source);"
            "    }"
            "});"
        );
    }
}

void MapWidget::onLoadFinished(bool success)
{
    if (success) {
        qDebug() << "Map loaded successfully";
    } else {
        qDebug() << "Failed to load map";
    }
}

void MapWidget::onLoadProgress(int progress)
{
    qDebug() << "Map loading progress:" << progress << "%";
}

void MapWidget::addWeatherCondition(const WeatherCondition &condition)
{
    weatherConditions.append(condition);
    if (showWeatherConditions) {
        drawWeatherCondition(condition);
    }
}

void MapWidget::clearWeatherConditions()
{
    weatherConditions.clear();
    if (webView && webView->page()) {
        webView->page()->runJavaScript(
            "const layers = map.getStyle().layers;"
            "layers.forEach(layer => {"
            "    if (layer.id.startsWith('weather-')) {"
            "        map.removeLayer(layer.id);"
            "    }"
            "});"
            "const sources = Object.keys(map.getStyle().sources);"
            "sources.forEach(source => {"
            "    if (source.startsWith('weather-')) {"
            "        map.removeSource(source);"
            "    }"
            "});"
        );
    }
}

void MapWidget::removeWeatherCondition(int index)
{
    if (index >= 0 && index < weatherConditions.size()) {
        weatherConditions.removeAt(index);
        
        // Haritadan da kaldır
        if (webView && webView->page()) {
            QString script = QString(
                "if (map.getLayer('weather-circle-%1')) {"
                "    map.removeLayer('weather-circle-%1');"
                "}"
                "if (map.getSource('weather-%1')) {"
                "    map.removeSource('weather-%1');"
                "}"
            ).arg(index);
            
            webView->page()->runJavaScript(script);
        }
        
        // Kalan weather condition'ları yeniden çiz
        if (showWeatherConditions) {
            clearWeatherConditions();
            for (const WeatherCondition &condition : weatherConditions) {
                drawWeatherCondition(condition);
            }
        }
    }
}

void MapWidget::setShowWeatherConditions(bool show)
{
    showWeatherConditions = show;
    if (show) {
        // Tüm weather condition'ları çiz
        for (const WeatherCondition &condition : weatherConditions) {
            drawWeatherCondition(condition);
        }
    } else {
        // Tüm weather condition'ları kaldır
        clearWeatherConditions();
    }
}

void MapWidget::drawWeatherCondition(const WeatherCondition &condition)
{
    if (!webView || !webView->page()) return;
    
    QString color = (condition.type == "Rain") ? "#0066cc" : "#666666";
    QString fillColor = (condition.type == "Rain") ? "#0066cc40" : "#66666640";
    
    QString script = QString(
        "map.addSource('weather-%1', {"
        "    'type': 'geojson',"
        "    'data': {"
        "        'type': 'Feature',"
        "        'geometry': {"
        "            'type': 'Point',"
        "            'coordinates': [%2, %3]"
        "        },"
        "        'properties': {"
        "            'type': '%4',"
        "            'radius': %5"
        "        }"
        "    }"
        "});"
        "map.addLayer({"
        "    'id': 'weather-circle-%1',"
        "    'type': 'circle',"
        "    'source': 'weather-%1',"
        "    'paint': {"
        "        'circle-radius': %6,"
        "        'circle-color': '%7',"
        "        'circle-opacity': 0.3,"
        "        'circle-stroke-width': 2,"
        "        'circle-stroke-color': '%8'"
        "    }"
        "});"
    ).arg(weatherConditions.size() - 1)
     .arg(condition.longitude)
     .arg(condition.latitude)
     .arg(condition.type)
     .arg(condition.radius)
     .arg(condition.radius * 1000) // km to meters
     .arg(fillColor)
     .arg(color);
    
    webView->page()->runJavaScript(script);
}

void MapWidget::updateLegend()
{
    if (!webView || !webView->page()) return;
    
    QString legendHTML = generateLegendHTML();
    QString script = QString(
        "let legend = document.getElementById('legend');"
        "if (!legend) {"
        "    legend = document.createElement('div');"
        "    legend.id = 'legend';"
        "    legend.style.cssText = 'position: absolute; bottom: 20px; right: 20px; background: rgba(255,255,255,0.9); padding: 10px; border-radius: 5px; font-family: Arial, sans-serif; font-size: 12px;';"
        "    document.body.appendChild(legend);"
        "}"
        "legend.innerHTML = `%1`;"
    ).arg(legendHTML);
    
    webView->page()->runJavaScript(script);
}

QString MapWidget::generateLegendHTML()
{
    QString legend = "<h4>Legend</h4>";
    legend += "<div style='margin: 5px 0;'><span style='display: inline-block; width: 20px; height: 20px; background: #0066cc; border-radius: 50%; margin-right: 8px;'></span>Rain</div>";
    legend += "<div style='margin: 5px 0;'><span style='display: inline-block; width: 20px; height: 20px; background: #666666; border-radius: 50%; margin-right: 8px;'></span>Fog&Cloud</div>";
    legend += "<div style='margin: 5px 0;'><span style='display: inline-block; width: 20px; height: 20px; background: #8B4513; border-radius: 50%; margin-right: 8px;'></span>DTED Areas</div>";
    return legend;
}

void MapWidget::addDTEDFile(const DTEDFile &dtedFile)
{
    dtedFiles.append(dtedFile);
    if (showDTEDAreas) {
        drawDTEDArea(dtedFile);
    }
}

void MapWidget::clearDTEDFiles()
{
    dtedFiles.clear();
    if (webView && webView->page()) {
        webView->page()->runJavaScript(
            "const layers = map.getStyle().layers;"
            "layers.forEach(layer => {"
            "    if (layer.id.startsWith('dted-')) {"
            "        map.removeLayer(layer.id);"
            "    }"
            "});"
            "const sources = Object.keys(map.getStyle().sources);"
            "sources.forEach(source => {"
            "    if (source.startsWith('dted-')) {"
            "        map.removeSource(source);"
            "    }"
            "});"
        );
    }
}

void MapWidget::setShowDTEDAreas(bool show)
{
    showDTEDAreas = show;
    if (show) {
        // Tüm DTED alanlarını çiz
        for (const DTEDFile &dtedFile : dtedFiles) {
            if (dtedFile.isVisible) {
                drawDTEDArea(dtedFile);
            }
        }
    } else {
        // Tüm DTED alanlarını kaldır
        clearDTEDFiles();
    }
}

void MapWidget::drawDTEDArea(const DTEDFile &dtedFile)
{
    if (!webView || !webView->page()) return;
    
    QString script = QString(
        "map.addSource('dted-%1', {"
        "    'type': 'geojson',"
        "    'data': {"
        "        'type': 'Feature',"
        "        'geometry': {"
        "            'type': 'Point',"
        "            'coordinates': [%2, %3]"
        "        },"
        "        'properties': {"
        "            'name': '%4',"
        "            'level': '%5',"
        "            'resolution': %6"
        "        }"
        "    }"
        "});"
        "map.addLayer({"
        "    'id': 'dted-area-%1',"
        "    'type': 'circle',"
        "    'source': 'dted-%1',"
        "    'paint': {"
        "        'circle-radius': 50,"
        "        'circle-color': '#8B4513',"
        "        'circle-opacity': 0.6,"
        "        'circle-stroke-width': 2,"
        "        'circle-stroke-color': '#654321'"
        "    }"
        "});"
    ).arg(dtedFiles.size() - 1)
     .arg(dtedFile.longitude)
     .arg(dtedFile.latitude)
     .arg(dtedFile.fileName)
     .arg(dtedFile.level)
     .arg(dtedFile.resolution);
    
    webView->page()->runJavaScript(script);
}
