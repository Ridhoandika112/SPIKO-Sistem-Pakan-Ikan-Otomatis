
#  Sistem Pakan Ikan Otomatis Di Aquarium Berbasis IoT
<img width="968" height="1005" alt="image" src="https://github.com/user-attachments/assets/f44cf915-afe7-4649-a831-3c361a58234f" />

<div align="center">
</div>



# Deskripsi Proyek
Proyek ini memanfaatkan mikrokontroler Arduino Mega (ATMega2560) sebagai platform utama, yang terintegrasi dengan beberapa sensor dan aktuator, yaitu sensor suhu air DS18B20, sensor pH, motor servo sebagai mekanisme pembuka pakan, serta modul RTC DS3231 untuk pengaturan dan penjadwalan waktu yang presisi. Sistem ini bertujuan untuk mengotomatisasi proses pemberian pakan ikan sesuai jadwal yang telah ditentukan. Data parameter kualitas air (suhu dan pH) akan dikirimkan secara kontinu ke sebuah dashboard web yang dapat diakses dari jarak jauh melalui internet. Selain fitur otomatisasi, sistem juga menyediakan fasilitas kontrol manual melalui aplikasi mobile atau web, sehingga pengguna tetap dapat mengintervensi pemberian pakan secara langsung sesuai kebutuhan.

---

# Tujuan Proyek
Membangun sistem otomatisasi pemberian pakan ikan berbasis mikrokontroler Arduino Mega yang dapat bekerja secara terjadwal, memantau kualitas air secara real-time, serta memungkinkan pengendalian dan pemantauan jarak jauh melalui dashboard web.



---
## Support By

> * Dosen Pengampu: Akhmad Hendriawan ST., MT. (NIP. 197501272002121003)
> * Mata Kuliah: Mikrokontroller
> * Program Studi: D4 Teknik Elektronika 
> * Politeknik Elektronika Negeri Surabaya


#  Anggota Tim
<img width="1920" height="1080" alt="1" src="https://github.com/user-attachments/assets/4a8b55aa-15fd-4e6f-b804-b5ae9a5f73eb" />


| NRP | Nama | Jobdesk | Akun |
|-----|------|---------|------|
| 2124600030 | Aidan Pasha Rabani | Project Manager | [Aidan031](https://github.com/Aidan031) |
| 2124600034 | M Arya Cendekia S| Hardware Specialist | [maryacs212434](https://github.com/maryacs212434) |
| 2124600058 | M Reihan Maulana | Programmer | [reiihaaannn](https://github.com/reiihaaannn) |
| 2124600043 | Sabrina Keisha Maharani | UI/UX Designer | [sabrinakeisha](https://github.com/sabrinakeisha) |
| 2124600051 | M Ridho Andika | 3D Designer | [Ridhoandika112](https://github.com/Ridhoandika112) |
| 2124600035 | Setyo Wahyu Nur F | Hardware Specialist |[bopengfirmansyah-sketch](https://github.com/bopengfirmansyah-sketch)|


---

### Fitur Utama

| Fitur | Kegunaan |
| :---: | :---: |
|**IoT Web Dashboard** | Antarmuka pengguna responsif untuk pemantauan kualitas air dan status perangkat secara *real-time*. |
|**Penjadwalan Presisi** | Pemberian pakan otomatis yang diatur secara presisi menggunakan modul RTC DS3231. |
|**Sensor Suhu DS18B20** | Pembacaan suhu air kolam secara akurat (protokol *One-Wire*) untuk memantau kelayakan habitat. |
|**Sensor pH Analog** | Deteksi tingkat keasaman atau kebasaan air secara berkesinambungan untuk mencegah anomali. |
|**Dual Control Mode** | Mendukung eksekusi pemberian pakan secara otomatis sesuai jadwal maupun *override* kontrol manual. |
|**Arduino Mega 2560** | Pusat komputasi sistem yang tangguh dengan kapasitas I/O berlimpah untuk kelancaran multi-sensor. |
|**Aktuator Presisi** | Menggunakan Motor Servo untuk mekanisme katup pakan dengan pengaturan porsi (Kecil, Sedang, Besar). |


---


### Daftar Komponen & Spesifikasi Perangkat Keras

| Komponen | Spesifikasi / Konfigurasi Pin | Fungsi Utama pada Sistem |
| :---: | :---: | :---: |
|**Arduino Mega 2560** | ATmega2560, 5V, 16 MHz | Mikrokontroler utama sebagai pusat pemrosesan logika kontrol, pembacaan data sensor, dan eksekusi aktuator. |
| **Motor Servo** | *PWM Output* (Pin Digital 9) | Bertindak sebagai mekanik pembuka dan penutup katup wadah pakan sesuai porsi/durasi yang diinstruksikan. |
|**Modul RTC (Real-Time Clock)** | DS1307 / DS3231 (I²C: SDA Pin 20, SCL Pin 21) | Menyediakan data waktu nyata (jam, kalender) yang presisi agar jadwal pakan    otomatis tidak terpengaruh saat sistem *restart*. |
| **Sensor Suhu DS18B20** | *One-Wire Digital* (Pin Digital 10) | Memantau perubahan suhu air kolam secara presisi dan mengirimkan data secara digital ke mikrokontroler. |
|**Sensor pH Air** | *Analog Input* (Pin Analog A0) | Mendeteksi tingkat keasaman/kebasaan air. *(Catatan: Pada diagram simulasi, modul ini diwakili oleh komponen Potensiometer)*. |

---

## PPT ##
<a href="https://canva.link/strf7c127qnlvye" target="_blank">
  <img src="https://img.shields.io/badge/PPT%20CANVA-0000FF?style=for-the-badge&logo=canva&logoColor=white" alt="PPT Canva">
</a>

## UI-UX ##
<a href="https://www.figma.com/make/QbkKy7pj1Dl7t2FqqGLqkM/Automated-Fish-Feeding-System?t=4AijnWL1JbrCFFVg-20&fullscreen=1" target="_blank">
  <img src="https://img.shields.io/badge/UI/UX%20DESAIN-0000FF?style=for-the-badge&logo=figma&logoColor=white" alt="UI/UX Desain">
</a>

## SIMULASI WOKWI ##
<a href="https://wokwi.com/projects/465644996192840705" target="_blank">
  <img src="https://img.shields.io/badge/SIMULASI%20WOKWI-0000FF?style=for-the-badge&logo=arduino&logoColor=white" alt="Simulasi Wokwi">
</a>

## MINDMAP ##
![image_alt](https://github.com/Ridhoandika112/SPIKO-Sistem-Pakan-Ikan-Otomatis/blob/main/ASSET/mindmap.png)

## FISHBONE ##
![image_alt](https://github.com/Ridhoandika112/SPIKO-Sistem-Pakan-Ikan-Otomatis/blob/main/ASSET/fishbone.png)

## VIDIO IKLAN ##
<a href="https://github.com/Ridhoandika112/SPIKO-Sistem-Pakan-Ikan-Otomatis/blob/9bb35ed6f565db23a5436418f983b1fbc057d9f7/ASSET/vidio%20iklan%20github.mp4" target="_blank">
  <img src="https://img.shields.io/badge/VIDIO IKLAN-0000FF?style=for-the-badge&logo=figma&logoColor=white" alt="Fishbone">
</a>
