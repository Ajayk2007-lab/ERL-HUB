#ifndef ERL_HUB_GPS_BEACON_H
#define ERL_HUB_GPS_BEACON_H

#include <string>
#include <iostream>
#include <cmath>

namespace ReliefOps {
namespace Logistics {

/**
 * @brief Inline utility to calculate Euclidean/Haversine approximate distance between coordinates.
 * Demonstrates Unit I Inline Functions.
 */
inline double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    // Approximate conversion factor: 1 degree latitude ~ 111 km
    double dLat = (lat2 - lat1) * 111.0;
    double dLon = (lon2 - lon1) * 111.0 * std::cos((lat1 + lat2) * 0.00872665); // lat in radians/2
    return std::sqrt(dLat * dLat + dLon * dLon);
}

/**
 * @brief Represents an embedded GPS tracking beacon.
 * Demonstrates Unit III Containership (Composition).
 */
class GPSBeacon {
private:
    std::string beaconId;
    double latitude;
    double longitude;
    std::string currentLocationName;
    bool signalActive;

public:
    GPSBeacon(const std::string& id = "GPS-DEFAULT", 
              double lat = 30.3165, 
              double lon = 78.0322, 
              const std::string& locName = "Central Hub Dehradun");

    void updateCoordinates(double lat, double lon, const std::string& locName);
    void setSignalStatus(bool active);

    // Getters
    std::string getBeaconId() const;
    double getLatitude() const;
    double getLongitude() const;
    std::string getCurrentLocationName() const;
    bool isSignalActive() const;

    void display() const;
};

} // namespace Logistics
} // namespace ReliefOps

#endif // ERL_HUB_GPS_BEACON_H
