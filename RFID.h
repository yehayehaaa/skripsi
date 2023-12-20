#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5  // Pin CS dari modul RFID RC522
#define RST_PIN 14 // Pin RST dari modul RFID RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); // Buat objek MFRC522

String dataPengguna[] = {"123456", "234567", "345678", "456789", "567890"}; // Data pegawai yang diinputkan secara manual
String namaPengguna[] = {"Rizki Chandra", "Muhammad Andhika", "Dimas Derajat", "Admin HB UM 1", "Admin HB UM 2"};
String rfidData;
int nomorPengguna;
bool statHB = false;

void setupRFID() {
  SPI.begin(); // Mulai komunikasi SPI
  mfrc522.PCD_Init(); // Inisialisasi modul RFID
}

int cekID() {
  // Periksa apakah ada kartu RFID yang terdeteksi
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    rfidData = ""; // Simpan data RFID

    // Baca data RFID
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      rfidData += (mfrc522.uid.uidByte[i]);
    }

    for (int i = 0; i < (sizeof(dataPengguna) / sizeof(dataPengguna[0])); i++) {
      if (statHB == false) {
        if (rfidData == dataPengguna[i]) {
          statHB = true;
          nomorPengguna = i;
          return 1; // Kesesuaian ditemukan
        } else {
          return 2; //gada yg cocok
        }
      }
      else {
        if (rfidData == dataPengguna[nomorPengguna]) {
          //relay off
          statHB = false;
          nomorPengguna = -1;
          return 3; // 2 kali tap id yang sama
        } else {
          Serial.println("HB dalam penggunaan ID lain");
          return 4; // tap pada sedang penggunaan
        }
      }
      return 0; // Kesesuaian tidak ditemukan

      mfrc522.PICC_HaltA(); // Hentikan komunikasi dengan kartu
    }
  }
}
