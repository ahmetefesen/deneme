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
#include "geo.h"
#include <gdal_priv.h>
#include <cmath> // For std::sin, std::cos, std::asin, std::atan2

// Vincenty (Direct) - WGS84 elipsoidi üzerinde ileri problem çözümü
// Girdi: lat1, lon1 (deg), başlangıç azimut (deg), s (metre)
// Çıktı: lat2, lon2 (deg)
static bool vincentyDirect(double lat1Deg, double lon1Deg, double alpha1Deg, double sMeters,
                           double &lat2Deg, double &lon2Deg)
{
    const double a = Geo::a;
    const double f = Geo::f;
    const double b = a * (1.0 - f);

    const double phi1 = lat1Deg * Geo::deg2rad;
    const double L1 = lon1Deg * Geo::deg2rad;
    const double alpha1 = alpha1Deg * Geo::deg2rad;

    const double sinAlpha1 = std::sin(alpha1);
    const double cosAlpha1 = std::cos(alpha1);

    const double tanU1 = (1.0 - f) * std::tan(phi1);
    const double U1 = std::atan(tanU1);
    const double sinU1 = std::sin(U1);
    const double cosU1 = std::cos(U1);

    const double sinAlpha = cosU1 * sinAlpha1;
    const double sin2Alpha = sinAlpha * sinAlpha;
    const double cos2Alpha = 1.0 - sin2Alpha;

    const double uSq = cos2Alpha * (a*a - b*b) / (b*b);
    const double A = 1.0 + (uSq/16384.0) * (4096.0 + uSq * (-768.0 + uSq * (320.0 - 175.0*uSq)));
    const double B = (uSq/1024.0) * (256.0 + uSq * (-128.0 + uSq * (74.0 - 47.0*uSq)));

    double sigma = sMeters / (b * A);
    double sigmaP = 2.0 * M_PI;
    double cos2SigmaM = 0.0;
    double sinSigma = 0.0;
    double cosSigma = 0.0;

    int iter = 0;
    while (std::fabs(sigma - sigmaP) > 1e-12 && ++iter < 200) {
        cos2SigmaM = std::cos(2.0*U1 + sigma);
        sinSigma = std::sin(sigma);
        cosSigma = std::cos(sigma);
        const double deltaSigma = B * sinSigma * (cos2SigmaM + (B/4.0) * (cosSigma * (-1.0 + 2.0 * cos2SigmaM * cos2SigmaM)
                                          - (B/6.0) * cos2SigmaM * (-3.0 + 4.0 * sinSigma * sinSigma) * (-3.0 + 4.0 * cos2SigmaM * cos2SigmaM)));
        sigmaP = sigma;
        sigma = sMeters / (b * A) + deltaSigma;
    }

    if (iter >= 200) return false;

    const double tmp = sinU1 * sinSigma - cosU1 * cosSigma * cosAlpha1;
    const double phi2 = std::atan2(sinU1 * cosSigma + cosU1 * sinSigma * cosAlpha1,
                                   (1.0 - f) * std::sqrt(sin2Alpha + tmp * tmp));
    const double lambda = std::atan2(sinSigma * sinAlpha1,
                                     cosU1 * cosSigma - sinU1 * sinSigma * cosAlpha1);
    const double C = (f/16.0) * cos2Alpha * (4.0 + f * (4.0 - 3.0 * cos2Alpha));
    const double L = lambda - (1.0 - C) * f * sinAlpha * (sigma + C * sinSigma * (cos2SigmaM + C * cosSigma * (-1.0 + 2.0 * cos2SigmaM * cos2SigmaM)));

    lat2Deg = phi2 * Geo::rad2deg;
    lon2Deg = Geo::wrapLon((L1 + L) * Geo::rad2deg);
    return true;
}

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
        function __wrapLon(lon){ while(lon>180) lon-=360; while(lon<-180) lon+=360; return lon; }
        const map = new maplibregl.Map({
            container: 'map',
            style: 'https://api.maptiler.com/maps/01988441-8731-71a1-a587-039aeb5bfab4/style.json?key=4GG7QQD4WbEvrdaLs9UX',
            renderWorldCopies: true,
            center: [)";
    
    html += QString::number(centerLon) + ", " + QString::number(centerLat) + "]";
    html += R"(,
            zoom: )" + QString::number(zoomLevel) + R"(
        });
    
        
        map.on('load', function() {
            console.log('Map loaded successfully');
            window.__mapReady = true;
            )";

    for (int i = 0; i < markers.size(); ++i) {
        html += QString("map.addSource('marker-%1', {\n").arg(i);
        html += "    'type': 'geojson',\n";
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
        
        // Koordinat gösterimi - WGS84 formatında (lon sarmalı)
        map.on('mousemove', function(e) {
            const lat = e.lngLat.lat;
            const lon = __wrapLon(e.lngLat.lng);
            document.getElementById('coords').innerHTML = 
                'Lat: ' + lat.toFixed(6) + ', Lon: ' + lon.toFixed(6) + ' (WGS84)';
        });
        
        // Tile bilgisi gösterimi
        // (MapTiler stilinde özel kaynak adları olabilir; genel log'u kapatalım)
        // map.on('sourcedata', function(e) {
        //     console.log('sourcedata event:', e.sourceId);
        // });
        
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
            const lat = e.lngLat.lat;
            const lon = __wrapLon(e.lngLat.lng);
            console.log('Clicked at:', {lat, lon});
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
        webView->page()->runJavaScript(
            QString("(function(){ if(window.__mapReady && typeof map!=='undefined'){ %1 } else { setTimeout(()=>{ %1 }, 150);} })();").arg(script)
        );
    }
}

void MapWidget::setCenter(double lat, double lon)
{
    centerLat = lat;
    centerLon = Geo::wrapLon(lon);
    if (webView && webView->page()) {
        QString script = QString("map.setCenter([%1, %2]);").arg(centerLon).arg(centerLat);
        webView->page()->runJavaScript(
            QString("(function(){ if(window.__mapReady && typeof map!=='undefined'){ %1 } else { setTimeout(()=>{ %1 }, 150);} })();").arg(script)
        );
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
        
        webView->page()->runJavaScript(
            QString("(function(){ if(window.__mapReady && typeof map!=='undefined'){ %1 } else { setTimeout(()=>{ %1 }, 150);} })();").arg(script)
        );
    }
}

void MapWidget::clearMarkers()
{
    markers.clear();
    markerTitles.clear();
    
    if (webView && webView->page()) {
        webView->page()->runJavaScript(
            "(function tryClean(){ if(window.__mapReady && typeof map!=='undefined'){ const layers = map.getStyle().layers; layers.forEach(layer => { if (layer.id.startsWith('marker-layer-')) { try{ map.removeLayer(layer.id);}catch(e){} } }); const sources = Object.keys(map.getStyle().sources); sources.forEach(src => { if (src.startsWith('marker-')) { try{ map.removeSource(src);}catch(e){} } }); } else { setTimeout(tryClean, 150);} })();"
        );
    }
}

void MapWidget::onLoadFinished(bool success)
{
    if (success) {
        qDebug() << "Map loaded successfully";
        // Hover popup init (only once)
        if (webView && webView->page()) {
            QString js =
                "(function initHover(){\n"
                " if (window.__hoverInited) return;\n"
                " function safe(){ if(!(window.__mapReady && typeof map!=='undefined')){ setTimeout(safe,150); return;}\n"
                "  try{ const hoverPopup = new maplibregl.Popup({ closeButton:false, closeOnClick:false });\n"
                "   map.on('mousemove', function(e){ try{ const feats = map.queryRenderedFeatures(e.point); const tf = feats.find(f=>f.layer && f.layer.id && f.layer.id.indexOf('target-layer-')===0); if(tf && tf.properties && tf.properties.name){ hoverPopup.setLngLat(e.lngLat).setText(tf.properties.name).addTo(map); } else { try{ hoverPopup.remove(); }catch(err){} } }catch(err){} });\n"
                "   window.__hoverInited = true; }catch(err){ console.warn('hover init failed', err);} }\n"
                " safe();\n"
                "})();";
            webView->page()->runJavaScript(js);
        }
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
            "(function tryClean(){ if(window.__mapReady && typeof map!=='undefined'){ const layers = map.getStyle().layers; layers.forEach(layer => { if (layer.id.startsWith('weather-')) { try{ map.removeLayer(layer.id);}catch(e){} } }); const sources = Object.keys(map.getStyle().sources); sources.forEach(src => { if (src.startsWith('weather-')) { try{ map.removeSource(src);}catch(e){} } }); } else { setTimeout(tryClean, 150);} })();"
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
    // Geodezik halka üret ve polygon olarak çiz
    QString color = (condition.type == "Rain") ? "#0066cc" : "#666666";
    QString fillColor = (condition.type == "Rain") ? "#0066cc40" : "#66666640";
    const QString id = QString("weather-%1").arg(weatherConditions.size() - 1);
    drawGeodesicCircle(condition.latitude, condition.longitude, condition.radius, id, fillColor, color);
}

void MapWidget::updateLegend()
{
    if (!webView || !webView->page()) return;
    
    QString legendHTML = generateLegendHTML();
    QString script = QString(
        "var legend = document.getElementById('legend');"
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
    QString legend = "<h4>Map Legend</h4>";
    legend += "<div style='margin: 5px 0;'><span style=\"display:inline-block;width: 0;height: 0;border-left: 10px solid transparent;border-right: 10px solid transparent;border-bottom: 18px solid #ff0000;margin-right:8px;transform: rotate(45deg);\"></span>Radar (Star)</div>";
    legend += "<div style='margin: 5px 0;'><span style='display: inline-block; width: 20px; height: 20px; background: rgba(0,102,255,0.6); border-radius: 50%; margin-right: 8px; border:1px solid #ffffff;'></span>Targets</div>";
    legend += "<div style='margin: 5px 0;'><span style='display: inline-block; width: 20px; height: 20px; background: #0066cc; border-radius: 50%; margin-right: 8px;'></span>Rain</div>";
    legend += "<div style='margin: 5px 0;'><span style='display: inline-block; width: 20px; height: 20px; background: #666666; border-radius: 50%; margin-right: 8px;'></span>Fog&Cloud</div>";
    legend += "<div style='margin: 5px 0;'><span style='display: inline-block; width: 20px; height: 20px; background: #8B4513; border-radius: 50%; margin-right: 8px;'></span>DTED Areas</div>";
    legend += "<div style='margin: 10px 0; font-size: 10px; color: #666;'>Coordinates: WGS84</div>";
    return legend;
}

void MapWidget::addDTEDFile(const DTEDFile &dtedFile)
{
    dtedFiles.append(dtedFile);
    // GDAL ile bbox oku
    double minLon=0, minLat=0, maxLon=0, maxLat=0;
    bool ok = false;
    {
        GDALAllRegister();
        GDALDataset *ds = (GDALDataset*) GDALOpen(dtedFile.filePath.toUtf8().constData(), GA_ReadOnly);
        if (ds) {
            double gt[6];
            if (ds->GetGeoTransform(gt) == CE_None) {
                int w = ds->GetRasterXSize();
                int h = ds->GetRasterYSize();
                // Rotasyonsuz varsayım (DTED için tipik):
                // lon = gt[0] + x*gt[1], lat = gt[3] + y*gt[5]
                minLon = gt[0];
                maxLon = gt[0] + gt[1] * w;
                maxLat = gt[3];
                minLat = gt[3] + gt[5] * h; // gt[5] negatif
                ok = true;
            }
            GDALClose(ds);
        }
    }
    if (ok) {
        const QString id = QString("dted-%1").arg(dtedFiles.size() - 1);
        drawDTEDFootprint(Geo::wrapLon(minLon), minLat, Geo::wrapLon(maxLon), maxLat, id);
    } else if (showDTEDAreas) {
        // geri-düşüş: eski noktasal gösterim
        drawDTEDArea(dtedFile);
    }
}

void MapWidget::clearDTEDFiles()
{
    dtedFiles.clear();
    if (webView && webView->page()) {
        webView->page()->runJavaScript(
            "(function(){"
            "const layers = map.getStyle().layers;"
            "layers.forEach(layer => { if (layer.id.startsWith('dted-')) { map.removeLayer(layer.id); } });"
            "const sources = Object.keys(map.getStyle().sources);"
            "sources.forEach(src => { if (src.startsWith('dted-')) { map.removeSource(src); } });"
            "})();"
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
    
    // DTED dosyası için bbox bilgisi yoksa: merkez + 1x1 derece varsayımsal kutu (geçici)
    double minLon = Geo::wrapLon(dtedFile.longitude);
    double minLat = dtedFile.latitude;
    double maxLon = Geo::wrapLon(minLon + 1.0);
    double maxLat = minLat + 1.0;
    const QString id = QString("dted-%1").arg(dtedFiles.size() - 1);
    drawDTEDFootprint(minLon, minLat, maxLon, maxLat, id);
}

void MapWidget::drawGeodesicCircle(double lat, double lon, double radiusKm,
                            const QString &id,
                            const QString &fillColor,
                            const QString &lineColor)
{
    if (!webView || !webView->page()) return;

    QString coords = "[";
    const int segments = 360; // elipsoidal örnekleme, yüksek doğruluk
    const double sMeters = radiusKm * 1000.0;
    for (int i = 0; i <= segments; ++i) {
        const double az = (360.0 * i) / segments; // derece
        double plat, plon;
        if (!vincentyDirect(lat, lon, az, sMeters, plat, plon)) continue;
        coords += QString("[%1,%2],").arg(plon, 0, 'f', 6).arg(plat, 0, 'f', 6);
    }
    coords.chop(1);
    coords += "]";

    QString js = QString(
        "(function(){"
        "try{ if(map.getLayer('%1-fill')) map.removeLayer('%1-fill'); }catch(e){}"
        "try{ if(map.getLayer('%1-line')) map.removeLayer('%1-line'); }catch(e){}"
        "try{ if(map.getSource('%1')) map.removeSource('%1'); }catch(e){}"
        "map.addSource('%1',{'type':'geojson','data':{"
        "'type':'Feature','geometry':{'type':'Polygon','coordinates':[%2]}}});"
        "map.addLayer({'id':'%1-fill','type':'fill','source':'%1','paint':{"
        "'fill-color':'%3','fill-opacity':0.35}});"
        "map.addLayer({'id':'%1-line','type':'line','source':'%1','paint':{"
        "'line-color':'%4','line-width':2}});"
        "})();"
    ).arg(id, coords, fillColor, lineColor);
    webView->page()->runJavaScript(wrapWithMapReady(js));
}

void MapWidget::drawDTEDFootprint(double minLon, double minLat, double maxLon, double maxLat,
                           const QString &id,
                           const QString &fillColor,
                           const QString &lineColor)
{
    if (!webView || !webView->page()) return;
    // [ [minLon,minLat], [maxLon,minLat], [maxLon,maxLat], [minLon,maxLat], [minLon,minLat] ]
    QString coords = QString("[[%1,%2],[%3,%2],[%3,%4],[%1,%4],[%1,%2]]")
        .arg(minLon, 0, 'f', 6)
        .arg(minLat, 0, 'f', 6)
        .arg(maxLon, 0, 'f', 6)
        .arg(maxLat, 0, 'f', 6);

    QString js = QString(
        "(function(){"
        "try{ if(map.getLayer('%1-fill')) map.removeLayer('%1-fill'); }catch(e){}"
        "try{ if(map.getLayer('%1-line')) map.removeLayer('%1-line'); }catch(e){}"
        "try{ if(map.getSource('%1')) map.removeSource('%1'); }catch(e){}"
        "map.addSource('%1',{'type':'geojson','data':{"
        "'type':'Feature','geometry':{'type':'Polygon','coordinates':[%2]}}});"
        "map.addLayer({'id':'%1-fill','type':'fill','source':'%1','paint':{"
        "'fill-color':'%3','fill-opacity':0.25}});"
        "map.addLayer({'id':'%1-line','type':'line','source':'%1','paint':{"
        "'line-color':'%4','line-width':2}});"
        "})();"
    ).arg(id, coords, fillColor, lineColor);
    webView->page()->runJavaScript(wrapWithMapReady(js));
}

void MapWidget::drawTrajectory(const QString &targetName, const QVector<QPair<double,double>> &latLon)
{
    if (!webView || !webView->page() || latLon.size() < 2) return;
    QString coords = "[";
    for (const auto &p : latLon) {
        coords += QString("[%1,%2],").arg(Geo::wrapLon(p.second), 0, 'f', 6).arg(p.first, 0, 'f', 6);
    }
    coords.chop(1);
    coords += "]";
    QString id = "traj-" + targetName;
    QString js = QString(
        "if (map.getSource('%1')) { try{ map.removeLayer('%1-line'); }catch(e){} map.removeSource('%1'); }"
        "map.addSource('%1',{'type':'geojson','data':{"
        "'type':'Feature','geometry':{'type':'LineString','coordinates':%2}}});"
        "map.addLayer({'id':'%1-line','type':'line','source':'%1','paint':{"
        "'line-color':'#00aaff','line-width':2}});"
    ).arg(id, coords);
    webView->page()->runJavaScript(js);
}

void MapWidget::clearTrajectories()
{
    if (!webView || !webView->page()) return;
    webView->page()->runJavaScript(
        "(function(){"
        "const layers = map.getStyle().layers;"
        "layers.forEach(l=>{ if(l.id && l.id.startsWith('traj-')) { try{ map.removeLayer(l.id); }catch(e){} } });"
        "const srcs = Object.keys(map.getStyle().sources);"
        "srcs.forEach(s=>{ if(s.startsWith('traj-')) { try{ map.removeSource(s); }catch(e){} } });"
        "})();"
    );
}

// Target fonksiyonları
void MapWidget::addTarget(const Target &target)
{
    if (!webView || !webView->page()) return;
    
    // WGS84 koordinatları - MapLibre GL JS [longitude, latitude] formatı bekler
    double wgs84Lon = Geo::wrapLon(target.initLongitude);
    double wgs84Lat = target.initLatitude;
    
    QString script = QString(
        "(function(){"
        "try{ if(map.getLayer('target-layer-%1')) map.removeLayer('target-layer-%1'); }catch(e){}"
        "try{ if(map.getSource('target-%1')) map.removeSource('target-%1'); }catch(e){}"
        "map.addSource('target-%1', {"
        "    'type': 'geojson',"
        "    'data': {"
        "        'type': 'Feature',"
        "        'geometry': {"
        "            'type': 'Point',"
        "            'coordinates': [%2, %3]"
        "        },"
        "        'properties': {"
        "            'name': '%4',"
        "            'altitude': %5,"
        "            'type': 'target'"
        "        }"
        "    }"
        "});"
        "map.addLayer({"
        "    'id': 'target-layer-%1',"
        "    'type': 'circle',"
        "    'source': 'target-%1',"
        "    'paint': {"
        "        'circle-radius': 8,"
        "        'circle-color': '#0066ff',"
        "        'circle-opacity': 0.6,"
        "        'circle-stroke-width': 1,"
        "        'circle-stroke-color': '#ffffff'"
        "    }"
        "});"
        "})();"
    ).arg(target.name)
     .arg(wgs84Lon, 0, 'f', 6)
     .arg(wgs84Lat, 0, 'f', 6)
     .arg(target.name)
     .arg(target.initAltitude);
    
    webView->page()->runJavaScript(script);
}

void MapWidget::removeTarget(const QString &targetName)
{
    if (!webView || !webView->page()) return;
    
    QString script = QString(
        "if (map.getLayer('target-layer-%1')) {"
        "    map.removeLayer('target-layer-%1');"
        "}"
        "if (map.getSource('target-%1')) {"
        "    map.removeSource('target-%1');"
        "}"
    ).arg(targetName);
    
    webView->page()->runJavaScript(script);
}

void MapWidget::updateTargetPosition(const QString &targetName, double lat, double lon, double alt)
{
    if (!webView || !webView->page()) return;
    
    // WGS84 koordinatları - MapLibre GL JS [longitude, latitude] formatı bekler
    double wgs84Lon = Geo::wrapLon(lon);
    double wgs84Lat = lat;
    
    QString script = QString(
        "(function(){"
        "var src = map.getSource('target-%1');"
        "if (!src) {"
        "    try{ if(map.getLayer('target-layer-%1')) map.removeLayer('target-layer-%1'); }catch(e){}"
        "    try{ if(map.getSource('target-%1')) map.removeSource('target-%1'); }catch(e){}"
        "    map.addSource('target-%1', {"
        "        'type': 'geojson',"
        "        'data': {"
        "            'type': 'Feature',"
        "            'geometry': { 'type': 'Point', 'coordinates': [%2, %3] },"
        "            'properties': { 'name': '%1', 'altitude': %4, 'type': 'target' }"
        "        }"
        "    });"
        "    map.addLayer({ 'id': 'target-layer-%1', 'type': 'circle', 'source': 'target-%1', 'paint': {"
        "        'circle-radius': 8, 'circle-color': '#0066ff', 'circle-opacity': 0.9, 'circle-stroke-width': 2, 'circle-stroke-color': '#ffffff' } });"
        "    src = map.getSource('target-%1');"
        "}"
        "if (src) {"
        "    src.setData({ 'type': 'Feature', 'geometry': { 'type': 'Point', 'coordinates': [%2, %3] }, 'properties': { 'name': '%1', 'altitude': %4, 'type': 'target' } });"
        "}"
        "})();"
    ).arg(targetName)
     .arg(wgs84Lon, 0, 'f', 6)
     .arg(wgs84Lat, 0, 'f', 6)
     .arg(alt);
    
    webView->page()->runJavaScript(script);
}

void MapWidget::clearTargets()
{
    if (!webView || !webView->page()) return;
    
    webView->page()->runJavaScript(
        "(function(){"
        "const layers = map.getStyle().layers;"
        "layers.forEach(layer => { if (layer.id.startsWith('target-layer-') || layer.id.startsWith('target-label-')) { map.removeLayer(layer.id); } });"
        "const sources = Object.keys(map.getStyle().sources);"
        "sources.forEach(src => { if (src.startsWith('target-')) { map.removeSource(src); } });"
        "})();"
    );
}

// Initial Position fonksiyonları
void MapWidget::addInitialPosition(double lat, double lon, double alt, const QString &name)
{
    if (!webView || !webView->page()) return;
    
    // WGS84 koordinatları - MapLibre GL JS [longitude, latitude] formatı bekler
    double wgs84Lon = Geo::wrapLon(lon);
    double wgs84Lat = lat;
    
    QString script = QString(
        "map.addSource('initial-pos', {"
        "    'type': 'geojson',"
        "    'data': {"
        "        'type': 'Feature',"
        "        'geometry': {"
        "            'type': 'Point',"
        "            'coordinates': [%1, %2]"
        "        },"
        "        'properties': {"
        "            'name': '%3',"
        "            'altitude': %4,"
        "            'type': 'initial_position'"
        "        }"
        "    }"
        "});"
        "// Red four-pointed star as a symbol"
        "map.addLayer({"
        "    'id': 'initial-pos-layer',"
        "    'type': 'symbol',"
        "    'source': 'initial-pos',"
        "    'layout': {"
        "        'text-field': '\u2726'," // ✦
        "        'text-font': ['Open Sans Regular', 'Arial Unicode MS Regular'],"
        "        'text-size': 20,"
        "        'text-anchor': 'center'"
        "    },"
        "    'paint': {"
        "        'text-color': '#ff0000',"
        "        'text-halo-color': '#ffffff',"
        "        'text-halo-width': 1"
        "    }"
        "});"
        "// Label for initial position name"
        "map.addLayer({"
        "    'id': 'initial-pos-label',"
        "    'type': 'symbol',"
        "    'source': 'initial-pos',"
        "    'layout': {"
        "        'text-field': '%3',"
        "        'text-font': ['Open Sans Semibold', 'Arial Unicode MS Bold'],"
        "        'text-offset': [0, -2],"
        "        'text-anchor': 'bottom',"
        "        'text-size': 12"
        "    },"
        "    'paint': {"
        "        'text-color': '#ffffff',"
        "        'text-halo-color': '#ff0000',"
        "        'text-halo-width': 1"
        "    }"
        "});"
    ).arg(wgs84Lon, 0, 'f', 6)
     .arg(wgs84Lat, 0, 'f', 6)
     .arg(name)
     .arg(alt);
    
    webView->page()->runJavaScript(script);
}

void MapWidget::updateInitialPosition(double lat, double lon, double alt)
{
    if (!webView || !webView->page()) return;
    
    // WGS84 koordinatları - MapLibre GL JS [longitude, latitude] formatı bekler
    double wgs84Lon = lon;
    double wgs84Lat = lat;
    
    QString script = QString(
        "(function(){"
        "var source = map.getSource('initial-pos');"
        "if (source) {"
        "    source.setData({"
        "        'type': 'Feature',"
        "        'geometry': { 'type': 'Point', 'coordinates': [%1, %2] },"
        "        'properties': { 'name': 'Initial Position', 'altitude': %3, 'type': 'initial_position' }"
        "    });"
        "}"
        "})();"
    ).arg(wgs84Lon, 0, 'f', 6)
     .arg(wgs84Lat, 0, 'f', 6)
     .arg(alt);
    
    webView->page()->runJavaScript(wrapWithMapReady(script));
}

void MapWidget::clearInitialPosition()
{
    if (!webView || !webView->page()) return;
    
    webView->page()->runJavaScript(
        "if (map.getLayer('initial-pos-label')) {"
        "    map.removeLayer('initial-pos-label');"
        "}"
        "if (map.getLayer('initial-pos-layer')) {"
        "    map.removeLayer('initial-pos-layer');"
        "}"
        "if (map.getSource('initial-pos')) {"
        "    map.removeSource('initial-pos');"
        "}"
    );
}

void MapWidget::addRadar(const QString &radarName, double lat, double lon, double alt)
{
    if (!webView || !webView->page()) return;
    double wgs84Lon = Geo::wrapLon(lon);
    double wgs84Lat = lat;
    QString src = QString("radar-%1").arg(radarName);
    QString layer = QString("radar-layer-%1").arg(radarName);
    QString js = QString(
        "(function(){"
        "try{ if(map.getLayer('%1')) map.removeLayer('%1'); }catch(e){}"
        "try{ if(map.getSource('%2')) map.removeSource('%2'); }catch(e){}"
        "map.addSource('%2', { 'type':'geojson','data':{ 'type':'Feature','geometry':{ 'type':'Point','coordinates':[%3,%4]}, 'properties':{ 'name':'%5','alt':%6 }}});"
        "map.addLayer({ 'id':'%1', 'type':'symbol', 'source':'%2', 'layout':{ 'text-field':'\\u2726', 'text-font':['Open Sans Regular','Arial Unicode MS Regular'], 'text-size': 20, 'text-anchor':'center' }, 'paint':{ 'text-color':'#ff0000', 'text-halo-color':'#ffffff', 'text-halo-width':1 }});"
        "})();"
    ).arg(layer, src)
     .arg(wgs84Lon, 0, 'f', 6)
     .arg(wgs84Lat, 0, 'f', 6)
     .arg(radarName)
     .arg(alt);
    webView->page()->runJavaScript(wrapWithMapReady(js));
}

void MapWidget::updateRadar(const QString &radarName, double lat, double lon, double alt)
{
    if (!webView || !webView->page()) return;
    double wgs84Lon = Geo::wrapLon(lon);
    double wgs84Lat = lat;
    QString src = QString("radar-%1").arg(radarName);
    QString layer = QString("radar-layer-%1").arg(radarName);
    QString js = QString(
        "(function(){"
        "var s = map.getSource('%1');"
        "if (!s) {"
        "    try{ if(map.getLayer('%2')) map.removeLayer('%2'); }catch(e){}"
        "    try{ if(map.getSource('%1')) map.removeSource('%1'); }catch(e){}"
        "    map.addSource('%1', { 'type':'geojson','data':{ 'type':'Feature','geometry':{ 'type':'Point','coordinates':[%3,%4]}, 'properties':{ 'name':'%1','alt':%5 }}});"
        "    map.addLayer({ 'id':'%2', 'type':'symbol', 'source':'%1', 'layout':{ 'text-field':'\\u2726', 'text-font':['Open Sans Regular','Arial Unicode MS Regular'], 'text-size': 20, 'text-anchor':'center' }, 'paint':{ 'text-color':'#ff0000', 'text-halo-color':'#ffffff', 'text-halo-width':1 }});"
        "    s = map.getSource('%1');"
        "}"
        "if (s) { s.setData({ 'type':'Feature','geometry':{ 'type':'Point','coordinates':[%3,%4]}, 'properties':{ 'name':'%1','alt':%5 }}); }"
        "})();"
    ).arg(src, layer)
     .arg(wgs84Lon, 0, 'f', 6)
     .arg(wgs84Lat, 0, 'f', 6)
     .arg(alt);
    webView->page()->runJavaScript(wrapWithMapReady(js));
}

void MapWidget::removeRadar(const QString &radarName)
{
    if (!webView || !webView->page()) return;
    QString src = QString("radar-%1").arg(radarName);
    QString layer = QString("radar-layer-%1").arg(radarName);
    QString js = QString(
        "try{ if(map.getLayer('%1')) map.removeLayer('%1'); }catch(e){}"
        "try{ if(map.getSource('%2')) map.removeSource('%2'); }catch(e){}"
    ).arg(layer, src);
    webView->page()->runJavaScript(js);
}

QString MapWidget::wrapWithMapReady(const QString &body) const
{
    return QString(
        "(function runWhenReady(){ if (window.__mapReady && typeof map!=='undefined') { try{ %1 }catch(e){ console.warn(e); } } else { setTimeout(runWhenReady, 100); } })();"
    ).arg(body);
}

void MapWidget::clearRadars()
{
    if (!webView || !webView->page()) return;
    QString js =
        "(function tryClean(){ if(window.__mapReady && typeof map!=='undefined'){ const layers = map.getStyle().layers; layers.forEach(l=>{ if(l.id && l.id.startsWith('radar-layer-')){ try{ map.removeLayer(l.id);}catch(e){} } }); const srcs = Object.keys(map.getStyle().sources); srcs.forEach(s=>{ if(s.startsWith('radar-')){ try{ map.removeSource(s);}catch(e){} } }); } else { setTimeout(tryClean, 150);} })();";
    webView->page()->runJavaScript(js);
}
