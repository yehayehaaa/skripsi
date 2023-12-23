#include "RFID.h"
#include "OLED.h"
#include "BATERAI.h"
#include "GPS.h"

#define Aktif true
bool statHB = !Aktif;

#define Nyala HIGH
int pinRelay = 4;
int pinBuzzer = 5;

void setup() {
  Serial.begin(115200);
  setupRFID();
  setupOLED();
  setupBATERAI();
  setupGPS();

  pinMode(pinRelay, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);

  HBmati();
}

void loop() {
  //cek koneksi
  //cek gps
  //if gps valid, led merah nyala & display tempelkan kartu
  if (statHB == !Aktif) {
    if (awalBateraiAman()) {
      if (lokasiAktif()) {
        //send data gps ke TB
        if (isInsideGeofence()) {
          //send data AMAN ke TB
          if (cekID()) {
            timedTampilOLED("Hai!", namaPengguna[nomorPengguna], 3000);
            HBaktif();
            if (statHB == Aktif) {
              tampilOLED("Pengguna :", namaPengguna[nomorPengguna]);
            }

            //kirim data ke TB "HB aktif"
          } else {
            timedTampilOLED(rfidData, "Tidak Terdaftar.", 3000);
          }
        } else {
          //kirim sinyal ke TB "Outside geofence"
          //pake millis untuk mematikan HB dalam 10 detik
          //oled di dalam if millis(), matiin dalam else millis()

          display.clearDisplay();
          tampilOLED("Hoverboard Berada di Luar Wilayah", "Harap Kembali ke Wilayah UM");
        }
      } else {
        tampilOLED("Sedang Menyiapkan Sistem", "Harap Menunggu :)");
      }
    } else {
      display.clearDisplay();
      tampilOLED("Harap Isi Daya Hoverboard.", "Baterai Belum Siap :(");
    }
  }
  else {
    if (lokasiAktif()) {
      //send data gps ke TB
      if (isInsideGeofence()) {
        //send data AMAN ke TB

        if (cekMatiRFID()) {
          HBmati();
          timedTampilOLED("Terima Kasih", namaPengguna[nomorPengguna], 3000);
          // nomorPengguna = -1; (gausah kyknya)
          // send data to TB alat mati
        }
        if (cekBaterai() > 20) {
          display.clearDisplay();
          tampilOLED("Pengguna :", namaPengguna[nomorPengguna]);
        } else if (cekBaterai() <= 20 && cekBaterai() > 15) {
          display.clearDisplay();
          tampilOLED("BATERAI LEMAH!", "Segera Kembalikan HoverBoard di Stasiun Terdekat.");
          //buzzer nyala nitnit nitnit nitnit
          //kirim alarm baterai lemah ke TB
        } else if (cekBaterai() <= 15 && cekBaterai() > 10) {
          HBmati();
          //buzzer bunyi nitttttt selama 1 detik secara berkala
          //kirim alarm alat mati
          //increment variable alatMati
        } else {
          //matikan buzzer
        }
      } else {
        display.clearDisplay();
        tampilOLED("Hoverboard Berada di Luar Wilayah UM", "Harap Kembali dalam 1 Menit");

        unsigned long currentMillisGPS = millis();
        unsigned long prevMillisGPS;
        if (currentMillisGPS - prevMillisGPS < 60000 ) { //1 menit
          HBmati();

          if (!isInsideGeofence()) {
            prevMillisGPS = currentMillisGPS;
          }
        }
      }
    } else {
      display.clearDisplay();
      tampilOLED("Sistem Kehilangan Sinyal GPS", "Harap berpindah ke Ruang Terbuka");
      //send data to TB sinyal ilang
    }
  }
}

void HBaktif() {
  statHB = Aktif;
  digitalWrite(pinRelay, Nyala);
}

void HBmati() {
  statHB = !Aktif;
  digitalWrite(pinRelay, Nyala);
}

void buzzWarn() {
  unsigned long currentMillisBuzz = millis();
  unsigned long prevMillisBuzz;
  int interval = 500;
  if (currentMillisBuzz - prevMillisBuzz < interval || ((currentMillisBuzz - prevMillisBuzz >= interval * 2) && (currentMillisBuzz - prevMillisBuzz < interval * 3)))) { //1 menit
    digitalWrite(pinBuzzer, Nyala);
  } else {
    digitalWrite(pinBuzzer, !Nyala);
  }

  if (currentMillisBuzz - prevMillisBuzz >= interval * 4) {
  prevMillisBuzz = currentMillisBuzz;
}
}

void buzzDanger() {

}


/*
  if (cekBaterai() <= 20 && baterai > 15) {
    timedTampilOLED("BATERAI LEMAH!", "Segera Kembalikan HoverBoard di Stasiun Terdekat.", 5000);
    //buzzer nyala nitnit nitnit nitnit
    //kirim alarm baterai lemah ke TB
  }
  else if (cekBaterai() <= 15 && cekBaterai() > 10) {
    //matikan relay
    //buzzer bunyi nitttttt selama 1 detik secara berkala
    //kirim alarm alat mati
    //increment variable alatMati
  } else {
    //matikan buzzer
  }


  void cekPengguna() {
  if (cekID() == 1) {
    //jalankan HB
    //display nama pengguna
    statHB = Aktif;
    digitalWrite(pinRelay, statHB);

    timedTampilOLED("Hai!", namaPengguna[nomorPengguna], 3000);
    tampilOLED("Pengguna :", namaPengguna[nomorPengguna]);
  } else if (cekID() == 3) {
    //matikan HB
    //display Terimakasih telah menggunakan HB
    timedTampilOLED("Terima Kasih telah menggunakan HoverBoard :)", "~UM", 3000);
  } else if (cekID() == 4) {
    //display HB dalam penggunaan orang lain
    timedTampilOLED("HoverBoard dalam penggunaan orang lain.", " ", 3000);
  } else {
    //relay mati/tetep off
    //display ID tidak terdaftar
    timedTampilOLED(rfidData, "Tidak Terdaftar.", 3000);
  }
  }
*/
