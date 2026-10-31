# 🔐 Selenoid RFID Door Lock with LCD & DFPlayer Mini

Sistem kunci pintu otomatis pintar berbasis **Arduino Uno** yang dilengkapi dengan autentikasi **RFID (RC522)**, notifikasi suara interaktif via **DFPlayer Mini**, layar status **LCD 16x2 I2C**, dan pengunci mekanis **Solenoid Door Lock**.
---

## 📌 Fitur Utama

- **Autentikasi Kartu RFID:** Akses cepat menggunakan kartu/tag RFID (13.56 MHz).
- **Audio Feedback Interaktif:** Memberikan respons suara MP3 (misal: *"Access Granted"*, *"Access Denied"*, *"Welcome"*).
- **Layar Status Visual:** Menampilkan instruksi tap dan status akses secara real-time pada LCD 16x2 (I2C).
- **Kontrol Solenoid Door Lock:** Membuka kunci pintu secara otomatis saat akses valid dan menguncinya kembali setelah jeda waktu tertentu.

---

## 🛠️ Komponen yang Dibutuhkan

| Komponen | Jumlah | Keterangan |
| :--- | :---: | :--- |
| **Arduino Uno R3 / Arduino uno r4 wifi ** | 1 | Mikrokontroler utama |
| **RFID Reader RC522** | 1 | Pembaca kartu/tag RFID 13.56 MHz |
| **DFPlayer Mini** | 1 | Modul pemutar audio MP3 |
| **MicroSD Card** | 1 | Format FAT32 (berisi file audio MP3) |
| **Speaker Mini (8Ω 0.5W - 3W)** | 1 | Output suara |
| **LCD 16x2 dengan Modul I2C** | 1 | Display status sistem |
| **Solenoid Door Lock (12V)** | 1 | Mekanisme pengunci pintu |
| **Modul Relay 1-Channel (5V)** | 1 | Saklar pengontrol solenoid |
| **Resistor 1kΩ** | 1 | Perlindungan jalur RX DFPlayer Mini |
| **Power Supply 12V DC & Step Down (opsional)** | 1 | Sumber daya solenoid & mikrokontroler |
| **Breadboard & Kabel Jumper** | Secukupnya | Kabel penghubung rangkaian |

---

## 📐 Skema Rangkaian & Pinout

### 1. RFID RC522 $\rightarrow$ Arduino Uno
> ⚠️ **Catatan:** Pin `VCC` RFID RC522 wajib disambungkan ke **3.3V** (bukan 5V).

| Pin RC522 | Pin Arduino Uno |
| :--- | :--- |
| **SDA (SS)** | Pin 10 |
| **SCK** | Pin 13 |
| **MOSI** | Pin 11 |
| **MISO** | Pin 12 |
| **IRQ** | *Tidak terhubung* |
| **GND** | GND |
| **RST** | Pin 9 |
| **3.3V** | 3.3V |

---

### 2. DFPlayer Mini $\rightarrow$ Arduino Uno

| Pin DFPlayer | Pin Arduino Uno | Keterangan |
| :--- | :--- | :--- |
| **VCC** | 5V | Sumber daya modul |
| **GND** | GND | Ground |
| **RX** | Pin 4 (`DF_TX_PIN` SoftwareSerial) | Pasang resistor 1kΩ seri di jalur ini |
| **TX** | Pin 3 (`DF_RX_PIN` SoftwareSerial) | Jalur komunikasi data serial |
| **SPK_1 / SPK_2** | Speaker (+) & (-) | Terminal speaker langsung |

---

### 3. LCD 16x2 I2C $\rightarrow$ Arduino Uno

| Pin LCD I2C | Pin Arduino Uno |
| :--- | :--- |
| **VCC** | 5V |
| **GND** | GND |
| **SDA** | A4 (atau pin SDA bawaan) |
| **SCL** | A5 (atau pin SCL bawaan) |

---

### 4. Relay & Solenoid Lock

| Pin Relay | Pin Arduino Uno / Sumber Daya |
| :--- | :--- |
| **VCC** | 5V |
| **GND** | GND |
| **IN** | Pin 2 (`PIN_RELAY`) |
| **COM** | +12V DC Power Supply |
| **NO** | Kabel (+) Solenoid Lock |
| *GND 12V* | Terhubung ke (-) Solenoid Lock & Common GND |

---

## 📂 Struktur File Audio MicroSD

Format kartu MicroSD menggunakan **FAT32**. Buat folder bernama `mp3` di root direktori MicroSD dan simpan audio dengan format penamaan numerik 4 digit:

```text
SD Card Root/
└── mp3/
    ├── 0001.mp3   <-- Suara "Access Granted / Welcome"
    └── 0002.mp3   <-- Suara "Access Denied / Try Again"
```

---

## 💻 Library yang Diperlukan

Pasang library berikut melalui **Arduino Library Manager** (`Sketch > Include Library > Manage Libraries...`):

1. **MFRC522** oleh *GithubCommunity*
2. **DFRobotDFPlayerMini** oleh *DFRobot*
3. **LiquidCrystal_I2C** oleh *Frank de Brabander*
4. **SoftwareSerial** *(Bawaan Arduino IDE)*

---

## 🚀 Cara Menjalankan Proyek

1. **Rakit Komponen:** Hubungkan semua modul sesuai diagram pinout di atas.
2. **Siapkan MicroSD:** Salin file MP3 ke dalam folder `mp3` pada MicroSD dan masukkan ke DFPlayer Mini.
3. **Cari UID Kartu:**
   - Gunakan contoh sketch bawaan library `MFRC522 -> DumpInfo` untuk membaca UID kartu Anda via Serial Monitor.
   - Ganti isi array `kartuTerdaftar` pada `sketch_main.ino` dengan UID kartu Anda.
4. **Upload Sketch:** Sambungkan Arduino ke PC dan lakukan compile & upload sketch.
5. **Uji Coba:**
   - Dekatkan kartu yang terdaftar $\rightarrow$ LCD menampilkan *Access Granted*, audio menyala, dan relay membuka kunci.
   - Dekatkan kartu asing $\rightarrow$ LCD menampilkan *Access Denied* dan audio peringatan menyala.

---
