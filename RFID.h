#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5  // Pin CS dari modul RFID RC522
#define RST_PIN 14 // Pin RST dari modul RFID RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); // Buat objek MFRC522

String dataPegawai[5] = {"1623118332", "17814010732", "992324245", "11513421945", "991459545"}; // Data pegawai yang diinputkan secara manual
String listPengguna[5] = {"Rafli Amirul", "Thoriq Ekananda", "Ruhil Bilqis", "Ayaturahman", "Rizki Chandra"};
bool statHB;
#define LED_BUILTIN 2
int noPengguna, j;

void setup() {
  Serial.begin(115200);
  SPI.begin(); // Mulai komunikasi SPI
  mfrc522.PCD_Init(); // Inisialisasi modul RFID
  pinMode(LED_BUILTIN, OUTPUT);
  statHB = false;
  //  noPengguna = 99;
}

void loop() {
  // Periksa apakah ada kartu RFID yang terdeteksi
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String rfidData = ""; // Simpan data RFID
    String Pengguna = "";

    // Baca data RFID
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      rfidData += String(mfrc522.uid.uidByte[i] );
    }

    // Periksa kesesuaian dengan data pegawai
    if (statHB == false) {
      if (checkAttendance(rfidData)) {
        statHB = true;
        noPengguna = j;
        Pengguna = listPengguna[noPengguna];
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.print("Hai ");
        Serial.println(Pengguna);
      } else {
        Serial.println("Kartu tidak terdaftar.");
      }
    } else {
      if (checkAttendance(rfidData) && noPengguna == j) {
        Serial.print("Terima kasih");
        Serial.println(Pengguna); 
        statHB = false;
        digitalWrite(LED_BUILTIN, LOW);      
      } else {
        Serial.println("Alat Dalam Penggunaan");
      }
    }
    mfrc522.PICC_HaltA(); // Hentikan komunikasi dengan kartu
  }
}

// Fungsi untuk memeriksa kesesuaian data RFID dengan data pegawai
bool checkAttendance(String rfid) {
  for (int i = 0; i < sizeof(dataPegawai) / sizeof(dataPegawai[0]); i++) {
    if (rfid == dataPegawai[i]) {
      j = i;
      return true; // Kesesuaian ditemukan
    }
  }
  return false; // Kesesuaian tidak ditemukan
}
