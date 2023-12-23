#define pinTegangan A5

float faktorTrim = 0.2; //5%-20%
int jumlahSampel = 50;
float maxV = 16.8;  // maksimum tegangan baterai
float minV = 11.4;  // minimum tegangan baterai

int durationBaterai = 10; //interval delay
unsigned long prevMillisBaterai = 0;

void setupBATERAI() {
  pinMode(pinTegangan, INPUT);
}

int cekBaterai() {
  //  Volt = (analogRead(pinTegangan) * 33) / 4095.0;
  unsigned long currentMillis = millis();

  //ngambil data sesuai jumlah sample
  float nilaiSensor[jumlahSampel];
  for (int i = 0; i < jumlahSampel; i++) {
    if (currentMillis - prevMillisBaterai >= durationBaterai) {
      nilaiSensor[i] = analogRead(pinTegangan);

      prevMillisBaterai = currentMillis;
    }
  }

  //proses pengurutan (sorting)
  for (int i = 0; i < jumlahSampel - 1; i++) {
    for (int j = i + 1; j < jumlahSampel; j++) {
      if (nilaiSensor[i] > nilaiSensor[j]) {
        float temp = nilaiSensor[i];
        nilaiSensor[i] = nilaiSensor[j];
        nilaiSensor[j] = temp;
      }
    }
  }

  //rata-rata (trimmed mean)
  float sum = 0;
  for (int i = jumlahSampel * (faktorTrim / 2); i < jumlahSampel * (1 - (faktorTrim / 2)); i++) {
    sum += nilaiSensor[i];
  }
  float trimmedMean = sum / (jumlahSampel * (1 - faktorTrim));
  float voltage = trimmedMean * (3.3 / 4095);  // Konversi nilai ADC ke tegangan (3.3V adalah referensi tegangan, 4095 adalah max. bit ESP32)
  int persenBaterai = map(voltage, minV, maxV, 0, 100);

  return persenBaterai;
}

bool awalBateraiAman() {
  if (cekBaterai() > 95) {
    return true;
  }
  return false;
}

/*
int cekBaterai() {
  if (persenBaterai() > 20){
    return 3;
  }
  if (persenBaterai() <= 20 && persenBaterai() > 15) {
    return 2;
    
    //timedTampilOLED("BATERAI LEMAH!", "Segera Kembalikan HoverBoard di Stasiun Terdekat.", 5000);
    //buzzer nyala nitnit nitnit nitnit
    //kirim alarm baterai lemah ke TB
  }
  else if (persenBaterai() <= 15 && persenBaterai() > 10) {
    return 1;
    //matikan relay
    //buzzer bunyi nitttttt selama 1 detik secara berkala
    //kirim alarm alat mati
    //increment variable alatMati
  } else {
    return 0;
    //matikan buzzer
  }
}
*/
