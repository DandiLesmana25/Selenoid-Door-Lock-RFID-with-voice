/*
 * PROJECT: RFID Smart Door Lock with Voice (DFPlayer Mini)
 * HARDWARE: Arduino Uno, RC522, Relay, Solenoid 12V, DFPlayer Mini + Speaker
 */

#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// --- Definisi Pin RFID & Relay ---
#define RST_PIN   9
#define SS_PIN    10
#define PIN_RELAY 2

// --- Definisi Pin DFPlayer Mini (Menggantikan Buzzer) ---
// Catatan: Pin 10 & 11 dipakai RFID (SPI), jadi DFPlayer memakai Pin 3 & 4
#define DF_RX_PIN 3  // Hubungkan ke pin TX DFPlayer
#define DF_TX_PIN 4  // Hubungkan ke pin RX DFPlayer (melalui resistor 1k ohm)

// Buat objek RFID & DFPlayer
MFRC522 mfrc522(SS_PIN, RST_PIN);
SoftwareSerial mySerial(DF_RX_PIN, DF_TX_PIN);
DFRobotDFPlayerMini myDFPlayer;

// --- SESUAIKAN UID KARTU KITA ---
byte kartuTerdaftar[] = {0x93, 0x4F, 0xE7, 0x00}; // Kartu RFID Hansel

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  SPI.begin();        // Inisialisasi bus SPI
  mfrc522.PCD_Init(); // Inisialisasi modul RFID RC522

  pinMode(PIN_RELAY, OUTPUT);
  
  // Setelan awal: Pintu terkunci (Relay OFF - Active LOW)
  digitalWrite(PIN_RELAY, HIGH); 

  // Berikan waktu DFPlayer Mini untuk menyala sempurna
  delay(1500);
  Serial.println(F("Memulai DFPlayer Mini..."));

  if (!myDFPlayer.begin(mySerial)) {
    Serial.println(F("DFPlayer Mini gagal diinisialisasi atau MicroSD tidak terbaca!"));
    // Sistem tetap bisa lanjut meskipun audio error, atau gunakan while(true); jika ingin berhenti
  } else {
    Serial.println(F("DFPlayer siap bekerja."));
    myDFPlayer.volume(30); // Volume maksimal (0 - 30)
  }

  Serial.println(F("-----------------------------------"));
  Serial.println(F("Sistem Smart Door Lock Aktif!"));
  Serial.println(F("Silakan tempelkan kartu Anda..."));
}

void loop() {
  // 1. Cek apakah ada kartu baru yang ditempelkan
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // 2. Baca nomor seri (UID) kartu tersebut
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // 3. Tampilkan UID Kartu ke Serial Monitor
  Serial.print(F("UID Kartu Terdeteksi:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // 4. Proses Verifikasi
  bool aksesDiberikan = true;
  
  if (mfrc522.uid.size == 4) { 
    for (byte i = 0; i < 4; i++) {
      if (mfrc522.uid.uidByte[i] != kartuTerdaftar[i]) {
        aksesDiberikan = false;
        break;
      }
    }
  } else {
    aksesDiberikan = false; // Tolak jika format ukuran UID berbeda
  }

  // 5. Eksekusi Berdasarkan Hasil Verifikasi
  if (aksesDiberikan) {
    Serial.println(F("Akses Diterima! Pintu Terbuka."));
    bukaPintu();
  } else {
    Serial.println(F("Akses Ditolak! Kartu Tidak Dikenal."));
    tolakAkses();
  }

  // Hentikan pembacaan kartu agar tidak terbaca berulang-ulang
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

// ==========================================
// FUNGSI BUKA PINTU & TOLAK AKSES (DENGAN SUARA MP3)
// ==========================================
void bukaPintu() {
  // Putar suara Akses Diterima (file: mp3/0001.mp3)
  myDFPlayer.play(1);

  // Buka pengunci pintu (Relay ON)
  digitalWrite(PIN_RELAY, LOW); 
  
  // Tahan pintu terbuka selama 5 detik
  delay(5000); 
  
  // Kunci kembali pintu (Relay OFF)
  digitalWrite(PIN_RELAY, HIGH);
  Serial.println(F("Pintu Kembali Terkunci."));
}

void tolakAkses() {
  // Putar suara Akses Ditolak (file: mp3/0002.mp3)
  myDFPlayer.play(2);
  
  // Pintu dipastikan tetap terkunci
  digitalWrite(PIN_RELAY, HIGH); 

  // Beri jeda 2.5 detik agar suara selesai diputar sebelum sensor membaca ulang
  delay(2500);
}