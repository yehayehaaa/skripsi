#include <TinyGPS++.h>

TinyGPSPlus gps;
#define GPS_BAUD 9600   // Baud rate GPS module
#define gpsSerial Serial2

double currentLatitude, currentLongitude;

// Koordinat geofence
const double fenceLatitude[] = { -7.957269, -7.958352, -7.960099, /*mulai ke bawah*/ -7.963182, -7.964584, -7.964333, -7.965480, -7.964663, -7.963995, -7.962792, /*mulai ke kiri atas*/ -7.959988, -7.959668};
const double fenceLongitude[] = {112.617494, 112.619254, 112.621450, /*mulai ke bawah*/ 112.619589, 112.618635, 112.617984, 112.617290, 112.616042, 112.616418, 112.614269, /*mulai ke kiri atas*/ 112.615709, 112.616379};
const int fenceVertices = 12;

void setup() {
  gpsSerial.begin(GPS_BAUD);

  Serial.println("Geofence System Started");
}

bool cekLokasi() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        currentLatitude = -7.965203;
        currentLongitude = 112.616661;

        // Check apakah titik berada di dalam geofence
        //        if (isInsideGeofence(currentLatitude, currentLongitude)) {
        //          Serial.println("Inside Geofence!");
        //        } else {
        //          Serial.println("Outside Geofence!");
        return true;
      }
    }
  }
  return false;
}

bool isInsideGeofence(double lat, double lon) {
  int crossings = 0;

  for (int i = 0; i < (sizeof(fenceLatitude) / sizeof(fenceLatitude[0])); i++) {
    double xi = fenceLatitude[i];
    double yi = fenceLongitude[i];
    double xi1 = fenceLatitude[(i + 1) % fenceVertices];
    double yi1 = fenceLongitude[(i + 1) % fenceVertices];

    if (xi == lat && yi == lon) {
      crossings = 1;
    }
    else {
      // Check apakah titik di atas garis dan segmen garis memotong garis horizontal dari titik
      if ((yi <= lon && yi1 > lon) || (yi1 <= lon && yi > lon)) {
        // Hitung titik potong garis horizontal dari titik P ke garis poligon
        double xIntersection = ((lon - yi) * (xi1 - xi) / (yi1 - yi)) + xi;

        // Jika titik potong di sebelah kanan dari titik P, itu adalah crossing
        if (xIntersection > lat) {
          crossings++;
        }
      }
    }
  }

  // Jika jumlah crossings ganjil, titik berada di dalam geofence
  return crossings % 2 != 0;
}
