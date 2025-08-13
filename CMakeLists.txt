cmake_minimum_required(VERSION 3.16)
project(RadarMapApplication VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6'yı bul
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network WebEngineWidgets)

# Offline harita desteği için gerekli ayarlar
# Marble entegrasyonu için hazırlık (şimdilik devre dışı)
# set(MARBLE_APP_PATH "/Applications/Marble.app")
# set(MARBLE_LIB_PATH "${MARBLE_APP_PATH}/Contents/MacOS/lib")
# set(MARBLE_INCLUDE_PATH "${MARBLE_APP_PATH}/Contents/Headers")

# Kaynak dosyaları
set(SOURCES
    main.cpp
    mainwindow.cpp
    mapwidget.cpp
    sidebar.cpp
    controlpanel.cpp
)

set(HEADERS
    mainwindow.h
    mapwidget.h
    sidebar.h
    controlpanel.h
    
)

# Qt uygulaması oluştur
qt_add_executable(RadarMapApplication ${SOURCES} ${HEADERS})

# Qt modüllerini bağla
target_link_libraries(RadarMapApplication PRIVATE 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::Network 
    Qt6::WebEngineWidgets
)

# MOC, UIC ve RCC için
set_target_properties(RadarMapApplication PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# Derleme ayarları
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(RadarMapApplication PRIVATE DEBUG)
endif()
