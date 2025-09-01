#pragma once
#include <cmath>
#include <vector>
#include <utility>

namespace Geo {
    static constexpr double pi = 3.14159265358979323846;
    static constexpr double a  = 6378137.0;               // WGS84 semi-major axis (m)
    static constexpr double f  = 1.0 / 298.257223563;     // flattening
    static constexpr double e2 = f * (2.0 - f);           // eccentricity^2
    static constexpr double deg2rad = pi / 180.0;
    static constexpr double rad2deg = 180.0 / pi;

    inline double wrapLon(double lonDeg) {
        while (lonDeg > 180.0) lonDeg -= 360.0;
        while (lonDeg < -180.0) lonDeg += 360.0;
        return lonDeg;
    }

    // Geodetic (deg,m) -> ECEF (m)
    inline void geodeticToECEF(double latDeg, double lonDeg, double h,
                               double& X, double& Y, double& Z)
    {
        const double lat = latDeg * deg2rad;
        const double lon = lonDeg * deg2rad;
        const double s = std::sin(lat), c = std::cos(lat);
        const double N = a / std::sqrt(1.0 - e2 * s*s);
        X = (N + h) * c * std::cos(lon);
        Y = (N + h) * c * std::sin(lon);
        Z = (N * (1.0 - e2) + h) * s;
    }

    // ECEF (m) -> Geodetic (deg,m) (Bowring approx.)
    inline void ecefToGeodetic(double X, double Y, double Z,
                               double& latDeg, double& lonDeg, double& h)
    {
        const double b = a * (1.0 - f);
        const double ep2 = (a*a - b*b) / (b*b);
        const double p = std::sqrt(X*X + Y*Y);
        lonDeg = std::atan2(Y, X) * rad2deg;

        const double th = std::atan2(a * Z, b * p);
        const double s = std::sin(th), c = std::cos(th);
        const double lat = std::atan2(Z + ep2 * b * s*s*s, p - e2 * a * c*c*c);

        const double sinLat = std::sin(lat);
        const double N = a / std::sqrt(1.0 - e2 * sinLat*sinLat);
        h = p / std::cos(lat) - N;

        latDeg = lat * rad2deg;
        lonDeg = wrapLon(lonDeg);
    }

    // ECEF -> ENU (m)
    inline void ecefToENU(double X, double Y, double Z,
                          double lat0Deg, double lon0Deg, double h0,
                          double& e, double& n, double& u)
    {
        double X0, Y0, Z0;
        geodeticToECEF(lat0Deg, lon0Deg, h0, X0, Y0, Z0);
        const double lat = lat0Deg * deg2rad, lon = lon0Deg * deg2rad;
        const double sl = std::sin(lat), cl = std::cos(lat);
        const double slon = std::sin(lon), clon = std::cos(lon);
        const double dX = X - X0, dY = Y - Y0, dZ = Z - Z0;
        e = -slon * dX + clon * dY;
        n = -sl * clon * dX - sl * slon * dY + cl * dZ;
        u =  cl * clon * dX + cl * slon * dY + sl * dZ;
    }

    // ENU (m) offset -> ECEF (m) absolute
    inline void enuToECEF(double e, double n, double u,
                          double lat0Deg, double lon0Deg, double h0,
                          double& X, double& Y, double& Z)
    {
        double X0, Y0, Z0;
        geodeticToECEF(lat0Deg, lon0Deg, h0, X0, Y0, Z0);
        const double lat = lat0Deg * deg2rad, lon = lon0Deg * deg2rad;
        const double sl = std::sin(lat), cl = std::cos(lat);
        const double slon = std::sin(lon), clon = std::cos(lon);

        const double dX = -slon * e - sl * clon * n + cl * clon * u;
        const double dY =  clon * e - sl * slon * n + cl * slon * u;
        const double dZ =                cl * n    + sl * u;

        X = X0 + dX; Y = Y0 + dY; Z = Z0 + dZ;
    }

    // ENU tabanlı yaklaşık geodezik halka (küçük/orta yarıçaplar için uygun)
    inline std::vector<std::pair<double,double>> geodesicRing_ENU(
        double lat0Deg, double lon0Deg, double alt0Meters,
        double radiusMeters, int numSegments = 128)
    {
        std::vector<std::pair<double,double>> ring;
        ring.reserve(static_cast<size_t>(numSegments) + 1);
        for (int i = 0; i < numSegments; ++i) {
            double theta = 2.0 * pi * static_cast<double>(i) / static_cast<double>(numSegments);
            double e = radiusMeters * std::cos(theta);
            double n = radiusMeters * std::sin(theta);
            double X, Y, Z;
            enuToECEF(e, n, 0.0, lat0Deg, lon0Deg, alt0Meters, X, Y, Z);
            double plat, plon, ph;
            ecefToGeodetic(X, Y, Z, plat, plon, ph);
            plon = wrapLon(plon);
            ring.emplace_back(std::make_pair(plon, plat)); // GeoJSON: [lon, lat]
        }
        // Kapatma noktası
        ring.push_back(ring.front());
        return ring;
    }
}


