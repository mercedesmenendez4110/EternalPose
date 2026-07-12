#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b2638"

#define HOME_LAT -24.7859   // AJUSTÁ con tus coordenadas reales
#define HOME_LON -65.4117

class BLEManager {
public:
    static void init();
    static float getBearingToHome();
    static void setCoordinates(float lat, float lon);
    static bool hasFix();
private:
    static volatile float phoneLat;
    static volatile float phoneLon;
    static volatile bool gotFix;
    static float calcularBearing(float lat1, float lon1, float lat2, float lon2);
};