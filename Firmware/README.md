## 📥 Tutorial Download dan Membuka Project SPIKO di VSCode

Project **SPIKO** juga disediakan dalam bentuk file **SPIKO.zip**. File ini berisi folder project **PlatformIO** yang dapat dibuka menggunakan **Visual Studio Code**.

### 1. Download File SPIKO.zip

Download file **SPIKO.zip** yang sudah disediakan pada repository GitHub.

Caranya:

1. Klik file **SPIKO.zip**.
2. Tekan tombol **Download** atau **View raw** untuk mengunduh file.
3. Simpan file **SPIKO.zip** ke folder yang mudah ditemukan, misalnya **Downloads** atau **Documents**.

---

### 2. Ekstrak File ZIP

Setelah file berhasil diunduh, ekstrak file **SPIKO.zip** terlebih dahulu.

Caranya:

1. Klik kanan pada file **SPIKO.zip**.
2. Pilih **Extract All...** atau **Ekstrak Semua**.
3. Pilih lokasi penyimpanan folder hasil ekstrak.
4. Klik **Extract**.
5. Setelah selesai, akan muncul folder project SPIKO.

---

### 3. Buka Project di Visual Studio Code

Setelah folder project berhasil diekstrak, buka project tersebut menggunakan **Visual Studio Code**.

Caranya:

1. Buka aplikasi **Visual Studio Code**.
2. Pilih menu **File**.
3. Klik **Open Folder...**.
4. Pilih folder hasil ekstrak dari **SPIKO.zip**.
5. Klik **Select Folder**.

Pastikan folder yang dibuka adalah folder project yang di dalamnya terdapat file:

```text
platformio.ini
src/
include/
lib/
```

File **platformio.ini** menandakan bahwa folder tersebut adalah project **PlatformIO**.

---

### 4. Pastikan Extension PlatformIO Sudah Terpasang

Agar project dapat dijalankan, pastikan extension **PlatformIO IDE** sudah terpasang di VSCode.

Caranya:

1. Buka menu **Extensions** di VSCode.
2. Cari **PlatformIO IDE**.
3. Klik **Install** jika belum terpasang.
4. Tunggu hingga proses instalasi selesai.
5. Restart VSCode jika diperlukan.

---

### 5. Buka dan Jalankan Project

Setelah project terbuka di VSCode, PlatformIO akan membaca konfigurasi dari file **platformio.ini** secara otomatis.

Langkah selanjutnya:

1. Tunggu hingga PlatformIO selesai memuat project.
2. Buka file program utama pada folder:

```text
src/main.cpp
```

3. Klik ikon **Build** untuk melakukan kompilasi program.
4. Jika ingin menjalankan simulasi Wokwi, gunakan file konfigurasi Wokwi yang sudah tersedia di dalam folder project.

---
