# SPIKO: Sistem Pakan Ikan Otomatis & Monitoring Kualitas Air Cerdas

SPIKO adalah sistem IoT cerdas berbasis **Arduino Mega 2560 (ATMega2560 @ 16 MHz)** yang dirancang untuk otomatisasi pemberian pakan ikan serta pemantauan kualitas air secara *real-time*. Sistem ini mengintegrasikan sensor fisik suhu (DS18B20) dan keasaman air (pH Analog) dengan kontrol aktuator servo melalui dasbor web interaktif berbasis WebSocket.

## 🛠️ Arsitektur Komunikasi Sistem
Sistem ini menggunakan arsitektur komunikasi data *full-duplex* tiga arah yang menjamin transmisi data tanpa hambatan (*non-blocking*):

1. **Lapisan Perangkat Keras (Firmware C++):**
   Arduino Mega membaca data sensor secara berkala menggunakan logika `millis()` dan mengirimkannya dalam format JSON melalui port serial `Serial1` (Pin 18/19). Lapisan ini juga mendengarkan instruksi eksekusi pakan dari web.
   
2. **Jembatan Jaringan (Node.js Bridge):**
   Aplikasi `bridge.js` bertindak sebagai *multi-protocol gateway*. Ia membuka **Raw TCP Server** di port `8080` untuk menangkap data dari simulator Wokwi, menggunakan mekanisme penampung (*Buffer*) untuk mencegah fragmentasi data JSON, kemudian meneruskannya melalui **WebSocket Server** di port `8081` menuju browser.

3. **Antarmuka Pengguna (Frontend Web):**
   Dasbor berbasis HTML, Tailwind CSS, dan Chart.js yang terhubung langsung ke WebSocket port `8081`. Berguna untuk visualisasi grafik tren kualitas air dan memicu intervensi pakan manual secara instan.

---

## 📌 Spesifikasi Pemetaan Pin Hardware (Wiring Map)
Berdasarkan konfigurasi pada proyek, berikut adalah pemetaan pin aktual pada Arduino Mega 2560:

| Komponen Perangkat Keras | Jenis Pin | Pin Arduino Mega | Deskripsi Fungsi |
| :--- | :--- | :--- | :--- |
| **Wokwi Serial Bridge** | Serial Async | `Pin 18 (TX1)` | Mengirimkan data JSON sensor ke RX Jembatan TCP |
| **Wokwi Serial Bridge** | Serial Async | `Pin 19 (RX1)` | Menerima perintah (`FEED_MANUAL`) dari TX Jembatan |
| **Servo Motor Pakan** | PWM Digital | `Pin 9` | Mengendalikan katup pembuka pakan (0° - 90°) |
| **Sensor Suhu DS18B20** | OneWire | `Pin 10` | Mengirim data suhu air dengan protokol 1-Wire |
| **Sensor pH Analog** | Analog Input | `Pin A0` | Membaca tegangan analog linear tingkat keasaman air |
| **DS3231 RTC Module** | I2C | `SDA / SCL` | Sinkronisasi waktu aktual berbasis jam sistem (*Real-Time Clock*) |

---

## 📋 Prasyarat Sistem
Sebelum menjalankan aplikasi, pastikan lingkungan pengembangan Anda telah dilengkapi dengan alat berikut:
- **Visual Studio Code (VS Code)**
- Ekstensi **PlatformIO IDE** & **Wokwi for VS Code**
- **Node.js** (Versi 16 ke atas direkomendasikan)
- Peramban Web Modern (Google Chrome atau Microsoft Edge)

---

## 🚀 Panduan Pengoperasian Langkah Demi Langkah
### Langkah 1: Kompilasi Firmware Perangkat Keras (PlatformIO)
1. Buka folder proyek utama Anda menggunakan VS Code.
2. Pastikan konfigurasi dependensi pada file `platformio.ini` sudah lengkap seperti berikut:
   ```ini
   [env:megaatmega2560]
   platform = atmelavr
   board = megaatmega2560
   framework = arduino
   lib_deps =
       arduino-libraries/Servo @ ^1.2.1
       SPI
       Wire
       adafruit/Adafruit BusIO @ ^1.16.1
       milesburton/DallasTemperature @ ^3.11.0
       paulstoffregen/OneWire @ ^2.3.7
       adafruit/RTClib @ ^2.1.1
3. Tekan tombol PlatformIO: Clean pada Project Tasks untuk membersihkan sisa cache kompilasi lama yang mungkin korup.
4. Klik tombol PlatformIO: Build (ikon centang di status bar bawah VS Code) untuk mengompilasi kode biner firmware.hex. Pastikan proses berakhir dengan status SUCCESS.


└── index.html            # Dasbor Pemantauan Web Interaktif (Tailwind CSS)
### Langkah 2: Eksekusi Jembatan Gateway (Node.js)
1. Buka terminal baru terintegrasi di dalam VS Code Anda.
2. Jalankan skrip jembatan komunikasi dengan mengetik perintah berikut:
   
node bridge.js

3. Pastikan terminal memunculkan log inisialisasi:
[*] SISTEM SIAP: Menunggu Wokwi di port 8080 & Web di port 8081
(Biarkan jendela terminal ini tetap terbuka dan menyala selama sistem beroperasi).

### Langkah 3: Menjalankan Simulasi Perangkat Keras (Wokwi)
1. Buka file konfigurasi tata letak perangkat keras diagram.json.
2. Klik tombol Start Simulation (ikon Play berwarna hijau) yang muncul di area editor visual Wokwi VS Code.
3. Periksa kembali terminal bridge.js yang Anda jalankan pada Langkah 2. Jika koneksi berhasil, akan muncul pesan jabat tangan (handshake):
[*] BERHASIL: Wokwi VS Code Terhubung!

### Langkah 4: Membuka Dasbor Monitoring (Frontend Web)
1. Cari file index.html di dalam direktori proyek Anda.
2. Buka file index.html tersebut dengan klik ganda atau klik kanan dan pilih peramban web pilihan Anda (Google Chrome atau Microsoft Edge sangat direkomendasikan).
3. Dasbor akan langsung terhubung secara otomatis ke gerbang port 8081 melalui protokol WebSocket.
4. Indikator pada sidebar kiri yang semula merah bertuliskan "Bridge Offline" akan seketika berubah menjadi hijau bertuliskan "Arduino Terhubung", menandakan visualisasi grafik dan kontrol pakan manual siap digunakan.

## Struktur Directory Project Pada Platform.Io
 ```
SPIKO-Project/
├── .pio/                 # Berisi cache kompilasi PlatformIO (Abaikan)
├── include/              # Folder untuk file header tambahan (.h)
├── src/
│   └── main.cpp          # Firmware Utama C++ (Logika Arduino Mega)
├── diagram.json          # Konfigurasi Pinout & Kabel Simulator Wokwi
├── platformio.ini        # Manajer Dependensi Library PlatformIO
├── bridge.js             # Gateway Server TCP-to-WebSocket (Node.js)
└── index.html            # Dasbor Pemantauan Web Interaktif (Tailwind CSS)
 ```
