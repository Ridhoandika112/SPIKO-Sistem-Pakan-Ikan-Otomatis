##  Komponen Hardware SPIKO

Sistem **SPIKO** menggunakan beberapa komponen utama yang saling terintegrasi untuk melakukan monitoring kualitas air dan pemberian pakan secara otomatis. Mikrokontroler utama yang digunakan adalah **Arduino Mega 2560**, dengan sensor suhu, sensor pH, RTC, dan motor servo sebagai aktuator.

### Daftar Komponen

| No | Komponen                   | Fungsi                                                                      |
| -- | -------------------------- | --------------------------------------------------------------------------- |
| 1  | **Arduino Mega 2560**      | Mikrokontroler utama untuk memproses data sensor dan mengendalikan aktuator |
| 2  | **DS18B20**                | Sensor suhu air                                                             |
| 3  | **DS1307 RTC Module**      | Modul waktu nyata untuk penjadwalan pemberian pakan                         |
| 4  | **Analog pH Meter Module** | Sensor monitoring pH air                                                    |
| 5  | **Potensiometer**          | Pengganti sensor pH pada simulasi                                           |
| 6  | **Motor Servo**            | Aktuator untuk membuka dan menutup wadah pakan                              |

---

##  Mapping Pinout ATmega2560

Berikut adalah konfigurasi pin yang digunakan pada sistem SPIKO:

| Pin Arduino Mega 2560 | Port ATmega2560 | Terhubung ke                  | Keterangan                              |
| --------------------- | --------------- | ----------------------------- | --------------------------------------- |
| **A0**                | **PF0**         | SIG Sensor pH / Potensiometer | Input analog untuk membaca nilai pH air |
| **22**                | **PA0**         | DQ Sensor DS18B20             | Jalur data sensor suhu air              |
| **20**                | **PD1**         | SDA Modul RTC DS1307          | Jalur data komunikasi I2C               |
| **21**                | **PD0**         | SCL Modul RTC DS1307          | Jalur clock komunikasi I2C              |
| **9**                 | **PH6**         | PWM Motor Servo               | Sinyal PWM untuk mengatur gerakan servo |

---

##  Koneksi Hardware

###  Sensor Suhu DS18B20

Sensor DS18B20 digunakan untuk membaca suhu air secara digital. Pin **DQ** sensor dihubungkan ke pin **22 (PA0)** pada Arduino Mega 2560.

```text
DS18B20 DQ  → Arduino Mega Pin 22 (PA0)
VCC         → 5V
GND         → GND
```

---

### Sensor pH / Potensiometer

Pada simulasi, sensor pH digantikan menggunakan potensiometer. Output potensiometer dihubungkan ke pin analog **A0 (PF0)**.

```text
SIG Sensor pH / Potensiometer → Arduino Mega Pin A0 (PF0)
VCC                            → 5V
GND                            → GND
```

---

###  Modul RTC DS1307

Modul RTC DS1307 digunakan untuk menyimpan dan membaca waktu secara real-time. Komunikasi dilakukan menggunakan protokol **I2C**.

```text
SDA RTC DS1307 → Arduino Mega Pin 20 (PD1)
SCL RTC DS1307 → Arduino Mega Pin 21 (PD0)
VCC            → 5V
GND            → GND
```

---

### Motor Servo

Motor servo digunakan sebagai aktuator untuk membuka dan menutup mekanisme pemberian pakan. Sinyal kontrol servo diberikan melalui pin **9 (PH6)**.

```text
Signal Servo → Arduino Mega Pin 9 (PH6)
VCC          → 5V
GND          → GND
```

---

## Ringkasan Sistem

Sistem SPIKO bekerja dengan membaca parameter suhu dan pH air, kemudian menggunakan modul RTC DS1307 sebagai acuan waktu untuk proses pemberian pakan otomatis. Data dari sensor diproses oleh Arduino Mega 2560, sedangkan motor servo berfungsi sebagai aktuator untuk membuka dan menutup tempat pakan sesuai jadwal yang telah ditentukan.

Dengan konfigurasi ini, sistem dapat digunakan sebagai dasar pengembangan **monitoring dan pemberian pakan ikan otomatis** berbasis mikrokontroler.
