# Changelog - Kelompok 1

Catatan kronologis perubahan konfigurasi sistem, kode mikrokontroler, serta kelengkapan dokumentasi berdasarkan peran masing-masing anggota (siapa, kapan, dan apa yang dikerjakan).

---

## Seluruh Anggota (Naflah, Dimas, Luthfi, Shaskia) - 2026-11-23 (Audit Silang & Finalisasi Laporan)
- **Audit Lintas Peran:** Melakukan pengujian ulang setiap tahapan oleh anggota peran berikutnya untuk memastikan seluruh panduan dapat dijalankan kembali tanpa kendala.
- **Pembersihan Kredensial & Dokumentasi:** Menyensor seluruh password WiFi, password MQTT, dan token API dari kode program maupun tangkapan layar, melengkapi bukti uji tiap tahap ke folder `docs/`, serta merampungkan `README.md`, `CHANGELOG.md`, dan `kontribusi.md`.

---

## Peran D: Shaskia Fitri Handayani - 2026-11-20 (Basis Data InfluxDB & Dashboard Grafana)
- **Penyimpanan Deret Waktu (`rpi/telegraf.conf`):** Menyiapkan organisasi `paramadina` serta *bucket* `iot` pada **InfluxDB 2.x** (port lokal `8086`), lalu menghubungkan agen **Telegraf** (`mqtt_consumer`) untuk mencatat data dari topik MQTT ke basis data.
- **Visualisasi & Aturan Peringatan (`dashboard/grafana-dashboard.json`):** Merancang panel grafik *Time Series* dan dua indikator *Gauge* pada dashboard Grafana menggunakan kueri Flux, serta mengaktifkan aturan **`Alert Suhu Tinggi`**.

---

## Peran C: Luthfi Muhammad - 2026-11-18 (Layanan Broker MQTT & Akses Publik Tunnel)
- **Konfigurasi Broker Pesan (`rpi/mosquitto.conf`):** Mengaktifkan layanan **Eclipse Mosquitto** pada port lokal `1883`, mematikan akses anonim (`allow_anonymous false`), serta membuat akun kredensial khusus `esp32`.
- **Uji Aliran Topik:** Memverifikasi penerimaan muatan data JSON pada topik `iot/kel1/dht22` menggunakan perintah `mosquitto_sub` di Raspberry Pi.
- **Penerbitan Akses Publik (`rpi/cloudflared-tunnel.service`):** Menyiapkan jalur **Cloudflare Tunnel** yang dijalankan sebagai layanan latar belakang (`systemd`) untuk meneruskan antarmuka Grafana (`localhost:3000`) ke Internet dan mengujinya lewat jaringan data seluler.

---

## Peran B: Dimas Saputra - 2026-11-16 (Perakitan Sensor DHT22 & Kode Program ESP32)
- **Pemasangan Perangkat Keras:** Menghubungkan pin modul sensor **DHT22** (`VCC` ke `3V3`, `DATA` ke `GPIO4`, dan `GND` ke `GND`) pada papan **ESP32 Dev Module**.
- **Pengembangan Firmware:** Menulis dan mengunggah program pembacaan suhu dan kelembapan udara setiap 10 detik melalui koneksi WiFi `2,4 GHz` menggunakan pustaka `DHT sensor library` dan `PubSubClient`.
- **Verifikasi Serial:** Memastikan pembacaan nilai suhu (`°C`) dan kelembapan (`%`) pada Serial Monitor berjalan stabil tanpa muncul galat `nan`.

---

## Peran A: Naflah Keumala Zahwa - 2026-11-12 (Persiapan Awal Raspberry Pi & Pengamanan Server)
- **Setup Sistem Operasi & Identitas Perangkat:** Memasang **Raspberry Pi OS (64-bit)** pada perangkat kelompok dengan nama host `rpi-kel1` pada jaringan lokal kampus (`192.168.10.86`).
- **Manajemen Jarak Jauh:** Menghubungkan perangkat ke layanan **Raspberry Pi Connect** agar administrasi terminal maupun desktop dapat dilakukan secara jarak jauh tanpa layar monitor fisik.
- **Pengamanan Dasar & Firewall:** Mengatur autentikasi berbasis *SSH key* dan mengaktifkan *firewall* **UFW** (`ufw enable`) guna membatasi akses lalu lintas jaringan hanya pada port lokal yang diperlukan.
