// basic use modul df player
// mini project system penghitung pengunjung indomaret

    
#include <SoftwareSerial.h>        //ini tidak perlu install linrary karena sudah ada di bawaan board arduino
#include <DFRobotDFPlayerMini.h> 

// Definisi pin
const int IR_PIN = 5;          // Pin OUT sensor IR
const int DF_RX_PIN = 10;       // Terhubung ke pin TX DFPlayer
const int DF_TX_PIN = 11;       // Terhubung ke pin RX DFPlayer (lewat resistor 1k)

// Setup SoftwareSerial & DFPlayer
SoftwareSerial mySerial(DF_RX_PIN, DF_TX_PIN);
DFRobotDFPlayerMini myDFPlayer;

// Variabel status pendeteksi tepi sinyal
int statusSebelumnya;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  // Pin IR diatur sebagai INPUT
  pinMode(IR_PIN, INPUT);


// Berikan waktu modul DFPlayer untuk menyala sempurna
  delay(1500);
  
  Serial.println(F("Memulai DFPlayer Mini..."));

  // Inisialisasi DFPlayer Mini
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println(F("DFPlayer Mini gagal diinisialisasi atau MicroSD tidak terbaca!"));
    while (true); // Berhenti jika gagal
  }

  Serial.println(F("DFPlayer siap bekerja."));
  myDFPlayer.volume(30); // Atur volume (skala 0 - 30)
}

void loop() {
  int statusSekarang = digitalRead(IR_PIN);
  // Serial.println(statusSekarang);

  // Deteksi ketika status berubah dari TIDAK ADA objek (HIGH) ke ADA objek (LOW)
  if (statusSekarang == LOW && statusSebelumnya == HIGH) {
    Serial.println(F("Pengunjung terdeteksi! Memutar: Welcome..."));
    
    // Putar file mp3/0001.mp3
    myDFPlayer.play(1);

    // Jeda penundaan (cooldown) agar suara tidak berulang-ulang saat pengunjung masih berdiri di depan sensor
    delay(3000); // 3 detik
  }

  statusSebelumnya = statusSekarang;
}