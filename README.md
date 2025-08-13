# Radar Map Application

Qt6 tabanlı masaüstü uygulaması - Radar simülasyonu ve görselleştirme için interaktif harita uygulaması.

## Özellikler

- İnteraktif harita (OpenStreetMap tile sistemi - Offline çalışır)
- Dark theme tasarım
- Yeniden boyutlandırılabilir layout
- Radar simülasyonu
- Çoklu tab desteği
- Kontrol paneli
- Offline harita desteği
- Weather condition overlay'leri
- DTED file desteği
- Gerçek zamanlı koordinat gösterimi
- Tile sistemi ile yüksek performans

## Gereksinimler

- Qt6 (Core, Widgets, Network, WebEngineWidgets)
- CMake 3.16+
- C++17

## Kurulum

### Uygulama Derleme:
```bash
mkdir build
cd build
cmake ..
make
./RadarMapApplication
```

### Offline Tile Sistemi

Uygulama artık OpenStreetMap tile sistemi ile çalışır:

- **Koordinat Gösterimi**: Mouse ile harita üzerinde gezinirken gerçek zamanlı koordinat gösterimi
- **Tile Sistemi**: Yüksek performanslı tile tabanlı harita sistemi
- **Offline Mod**: Uygulama tamamen offline çalışır, sadece harita tile'ları için internet gerekir
- **Zoom/Pan**: Tam zoom ve pan desteği
- **Simülasyon Hazır**: Koordinat tabanlı simülasyonlar için optimize edilmiş

### Marble Entegrasyonu (Gelecek Sürümler)

Marble kütüphanesi entegrasyonu için hazırlık yapılmıştır. Gelecek sürümlerde tam offline harita desteği eklenecektir.

## Kullanım

Harita üzerinde zoom, pan yapabilirsiniz. Sidebar'da farklı ayarlar yapabilirsiniz.
