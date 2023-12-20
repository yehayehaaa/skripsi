#include "RFID.h"
#include "OLED.h"
#include "BATERAI.h"
#include "GPS.h"

void setup() {
  Serial.begin(115200);
  setupRFID();
  setupOLED();
  setupBATERAI();
}

void loop() {
  //cek koneksi
  //cek gps
  //if gps valid, led merah nyala & display tempelkan kartu
  if (cekLokasi()){
    if (isInsideGeofence(currentLatitude, currentLongitude)){
      
    }
  }

  if (cekID == 0) {
    tampilOLED("Pengguna :", namaPengguna[nomorPengguna]);
  } else if (cekID == 1) {
    //jalankan HB
    //display nama pengguna
    timedTampilOLED("Hai!", namaPengguna[nomorPengguna], 3000);
  } else if (cekID == 3) {
    //matikan HB
    //display Terimakasih telah menggunakan HB
    timedTampilOLED("Terima Kasih telah menggunakan HoverBoard :)", "~UM", 3000);
  } else if (cekID == 4) {
    //display HB dalam penggunaan orang lain
    timedTampilOLED("HoverBoard dalam penggunaan orang lain.", " ", 3000);
  } else {
    //relay mati/tetep off
    //display ID tidak terdaftar
    timedTampilOLED(rfidData, "Tidak Terdaftar.", 3000);
  }
  

  if (cekBaterai() <= 20 && baterai > 15){
    timedTampilOLED("BATERAI LEMAH!", "Segera Kembalikan HoverBoard di Stasiun Terdekat.", 5000);
    //buzzer nyala nitnit nitnit nitnit
    //kirim alarm baterai lemah ke TB
  }
  else if(cekBaterai() <= 15 && cekBaterai() > 10){
    //matikan relay
    //buzzer bunyi nitttttt selama 1 detik secara berkala
    //kirim alarm alat mati
    //increment variable alatMati
  }else{
    //matikan buzzer
  }
}

