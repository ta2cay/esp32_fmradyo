# 📻 ESP32/ESP8266 Dijital FM Radyo Projesi (OLED Ekranlı & Hafızalı)

![Project Banner](https://img.shields.io/badge/Project-DIY%20FM%20Radio-blue?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Completed-success?style=for-the-badge)
![Language](https://img.shields.io/badge/Language-C%2B%2B%20%2F%20Arduino-orange?style=for-the-badge)

Bu proje, **ESP32 (veya ESP8266)** mikrodenetleyicisi kullanılarak geliştirilmiş, gelişmiş özelliklere sahip bir FM Radyo alıcısıdır. Standart radyo özelliklerinin yanı sıra ortam sıcaklığı/nem göstergesi, frekans kilitleme ve favori kanal hafızası gibi özellikler içerir.

## 📺 Proje Tanıtım Videosu

Projeyi çalışırken görmek ve adım adım yapım aşamalarını izlemek için YouTube videoma göz atın:

[![YouTube Video](https://img.youtube.com/vi/62vIEDWaQRM/0.jpg)](https://www.youtube.com/watch?v=62vIEDWaQRM)

> **Tıklayın ve İzleyin:** [ESP32 İLE HAFIZALI VE DİJİTAL EKRANLI FM RADYO NASIL YAPILIR?](https://www.youtube.com/watch?v=62vIEDWaQRM)

## 🔥 Özellikler

* **RDA5807M Modülü:** Yüksek hassasiyetli FM radyo alıcısı.
* **OLED Ekran (SSD1306):** Frekans, Radyo İstasyon Adı, Saat, Sıcaklık ve Nem bilgilerini gösterir.
* **Kanal Hafızası:** Buton ile favori kanallar arasında hızlı geçiş (EEPROM/Hafıza desteği).
* **Manuel Arama:** Potansiyometre ile hassas frekans ayarı.
* **Frekans Kilitleme (AFC):** Ayarlanan frekansta sabit kalma, kaymayı önleme.
* **Ortam Sensörü:** DHT11 sensörü ile anlık sıcaklık ve nem takibi.
* **Sinyal Göstergesi:** Ekranın altında sinyal gücüne göre değişen bar grafiği.

## 🛠️ Gerekli Malzemeler

| Bileşen | Açıklama |
| :--- | :--- |
| **Mikrodenetleyici** | ESP32 veya ESP8266 (NodeMCU) |
| **Radyo Modülü** | RDA5807M |
| **Ekran** | 0.96" I2C OLED (SSD1306) |
| **Sensör** | DHT11 (Sıcaklık ve Nem) |
| **Kontrol** | 10K Potansiyometre & Push Buton |
| **Ses Çıkışı** | Hoparlör veya Kulaklık Jakı |
| **Diğer** | Breadboard, Jumper Kablolar, 3.7V Pil veya USB Güç |

## 🔌 Bağlantı Şeması (Pinout)

Videoda kullanılan bağlantı yapısı aşağıdaki gibidir. *(Kullandığınız karta göre pinleri kod içerisinden kontrol ediniz)*

* **RDA5807M & OLED (I2C):**
    * `SDA` -> `D2` (veya ESP32 varsayılan SDA)
    * `SCL` -> `D1` (veya ESP32 varsayılan SCL)
* **Potansiyometre:**
    * `Orta Bacak` -> `A0` (Analog Giriş)
* **DHT11 Sensör:**
    * `Data` -> `D3`
* **Buton (Favori Kanal):**
    * `Pin` -> `D5`

## 💻 Kurulum ve Kütüphaneler

Bu projeyi derlemek için Arduino IDE kullanabilirsiniz. Aşağıdaki kütüphanelerin yüklü olduğundan emin olun:

1.  `Adafruit_SSD1306` (Ekran için)
2.  `Adafruit_GFX` (Grafik işlemleri için)
3.  `DHT sensor library` (Sıcaklık sensörü için)
4.  `RDA5807` (Radyo modülü için uygun kütüphane)

**Nasıl Yüklenir?**
1.  Repo'daki `.ino` dosyasını indirin.
2.  Arduino IDE ile açın.
3.  Kartınızı ve Portunuzu seçin.
4.  Yükle butonuna basın.

---

*Proje sahibi: [AMATÖR RADYOCULUK VE SWL REHBERİ](https://www.youtube.com/@AMATORRADYOCULUKVESWLREHBERI)*
