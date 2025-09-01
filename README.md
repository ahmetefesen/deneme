# Radar Map Application

Bu Qt6 tabanlı masaüstü uygulaması, radar simülasyonu ve görselleştirme için geliştirilmiştir.

## Özellikler

### Harita Görselleştirme
- MapLibre tabanlı harita (MapTiler Basic Dark stili – internet gerekir)
- Zoom ve pan kontrolleri
- Koordinat gösterimi (WGS84)

### Radar ve Target Simülasyonu
- Hz’e göre zamanlama (deltaTime = 1/Hz)
- ENU hız bileşenleri ile Euler integrasyon
- Waypoint’e varınca o waypoint’in hızlarını devralma, son WP’den sonra son hızla devam
- Target rotası istenirse polyline olarak çizdirilebilir (Control Panel → Show Targets Traj)
- Radar işareti: kırmızı dört köşeli yıldız; Target: mavi nokta

### Atmosphere (Weather)
- Yağmur ve Sis/Cloud koşulları
- Konum (lat/lon), yarıçap, parametreler
- “Calculate Weather Conditions” seçeneği (ileride eklenecek DLL için bağlam hazırlanmıştır)

### Terrain (DTED)
- DTED dosyalarını ekleme ve footprint gösterimi (GDAL ile bbox)

---

## Kurulum ve Derleme (macOS)

### Gereksinimler
- Qt6 (Core, Widgets, Network, WebEngineWidgets)
- CMake 3.16+
- C++17 uyumlu derleyici
- (DTED için) GDAL

### Derleme (macOS)
```bash
mkdir build
cd build
cmake ..
make -j4
./RadarMapApplication
```

Not: Harita çevrimiçi stil yükler, internet gerekir.

---

## Windows’ta Kurulum ve Çalıştırma (Qt6 + WebEngine + GDAL)
Bu bölüm, Windows’ta daha önce Qt veya GDAL kurmamış birinin bile adım adım uygulamayı çalıştırabilmesi için hazırlanmıştır.

### 1) Gerekli Yazılımlar
- Visual Studio 2022 (Desktop development with C++)
- CMake 3.20+ (veya cmake eklentili VS)
- Qt 6.x (WebEngine dahil, msvc x64)
- GDAL (runtime + dev)

### 2) Qt Kurulumu
- Qt Online Installer ile 6.x sürümünü kurun.
  - Bileşenler: “MSVC 2019/2022 64-bit” + “Qt WebEngine”.
- Örnek kurulum yolu: `C:\Qt\6.7.2\msvc2022_64`
- Not: `windeployqt.exe` bu dizinin `bin` klasöründedir.

### 3) GDAL Kurulumu
- Önerilen: OSGeo4W Network Installer → Advanced Install
  - `gdal`, `proj`, `geos` paketlerini seçin (bin + dev)
  - Örnek kurulum: `C:\OSGeo4W`
- Alternatif: GISInternals veya vcpkg (ör. `vcpkg install gdal:x64-windows`)

### 4) Derleme (CMake)
CMake’e Qt ve GDAL konumlarını gösterin.

```bat
REM x64 yapılandırmasıyla VS 2022
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 ^
  -DQt6_DIR=C:\Qt\6.7.2\msvc2022_64\lib\cmake\Qt6 ^
  -DGDAL_DIR=C:\OSGeo4W\share\cmake\gdal

cmake --build build --config Release
```

- `Qt6_DIR` yolu Qt kurulumunuzdaki `lib\cmake\Qt6` klasörünü göstermelidir.
- `GDAL_DIR` yolu OSGeo4W’de genelde `C:\OSGeo4W\share\cmake\gdal` ya da `C:\OSGeo4W\lib\cmake\gdal` olur.

### 5) Çalıştırılabilir Paketleme (Qt WebEngine + GDAL)
Qt DLL’leri ve WebEngine kaynaklarının exe yanına taşınması gerekir.

```bat
REM Qt DLL ve WebEngine kaynaklarını kopyala
C:\Qt\6.7.2\msvc2022_64\bin\windeployqt.exe --release --webengine ^
  build\Release\RadarMapApplication.exe
```

GDAL runtime dosyalarını ekleyin:
- `C:\OSGeo4W\bin\gdal*.dll` dosyalarını exe’nin bulunduğu klasöre kopyalayın.
- `C:\OSGeo4W\share\gdal` klasörünü exe yanına `gdal-data` olarak kopyalayın (veya `gdal` klasörüyle birlikte).
- Alternatif: `C:\OSGeo4W\bin` dizinini `PATH` ortam değişkenine ekleyin.

### 6) Çalıştırma
- `build\Release\RadarMapApplication.exe`
- İlk çalıştırmada Windows SmartScreen uyarısı çıkarsa “More info → Run anyway”.
- Harita çevrimiçi stil yükler, internet gerekir.

### 7) Sorun Giderme (Windows)
- “Qt WebEngine Process not found” → `windeployqt --webengine` çalıştırıldığından emin olun.
- “gdalXXX.dll bulunamadı” → OSGeo4W `bin`’deki dll’leri exe yanına kopyalayın veya `PATH`’e ekleyin.
- “Qt/GDAL bulunamadı” → `Qt6_DIR` ve `GDAL_DIR` yollarını doğru gösterin; VS yapılandırması `x64` olmalı.

---

## Kullanım

### Simülasyonu Başlatma
1. Sidebar → General → Initial: Radar başlangıç konumu ve hızlarını girin.
2. General → Route: Radar waypoint ve hızlarını girin.
3. Target sekmesi: Target ekleyin, initial ve waypoint/trajectory değerlerini girin.
4. Control Panel: Hz değerini seçin, isterseniz “Show Targets Traj” ve “Calculate Weather Conditions” işaretleyin.
5. Start: Sidebar gizlenir, simülasyon başlar. Stop: Sidebar geri gelir; Save aktif olur.

### Kaydetme
- Stop’tan sonra File → Save: Zaman damgalı bir JSON dosyası oluşturulur.
- JSON içeriği: Hz, görünürlük/hesaplama bayrakları, Radar(lar) initial+route, Targets initial/trajectory/waypoints, Weather, Terrain (DTED yolları).

---

## Teknik Detaylar

### Simulation Engine
- QTimer tabanlı döngü
- Hz’e göre deltaTime hesabı
- ENU→ECEF→Geodetic dönüşümleri, Euler integrasyon

### Target Movement (özet)
```cpp
// ENU hızları ile konum güncelleme (deltaTime = 1/Hz)
double dE = velE * dt;    // east
double dN = velN * dt;    // north
double dU = -velD * dt;   // up (D aşağı)
// ENU offset → ECEF → Geodetic dönüşümleri uygulanır
```

### Signal-Slot Architecture
- MainWindow, Sidebar, ControlPanel, MapWidget arasında Qt sinyal/slot akışı
- Start’ta Sidebar → ControlPanel otomatik besleme (Radar/Targets)
- İsteğe bağlı polyline çizimi (targets)

---

## Geliştirici Notları
- Qt WebEngine çevrimiçi harita stili (MapTiler) yükler – internet gerekir.
- GDAL entegrasyonu DTED bbox/footprint içindir; kaldırmak isterseniz CMake’e opsiyonel bayrak eklenebilir (WITH_GDAL).

---

## Lisans
Bu proje açık kaynak kodludur.

## Katkıda Bulunma
1. Fork yapın
2. Feature branch oluşturun
3. Commit yapın
4. Pull request gönderin
