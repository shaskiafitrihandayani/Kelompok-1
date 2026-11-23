# Log Kontribusi Anggota — Kelompok 1

Dokumen ini mencatat pembagian peran, rincian pekerjaan teknis, serta rekam jejak kontribusi masing-masing anggota pada repositori GitHub proyek **Sistem Pemantau Suhu dan Kelembapan Udara Berbasis IoT**.

---

## 1. Ringkasan Peran & Tanggung Jawab

| Peran | Nama Anggota | Fokus Utama | Berkas & Direktori Tanggung Jawab |
| :--- | :--- | :--- | :--- |
| **Peran A** | **Naflah** | *Network & Raspberry Pi Setup* | `docs/tahap1-piconnect-ufw.png`, `kontribusi.md` |
| **Peran B** | **Dimas** | *Sensor Wiring & Firmware ESP32* | `firmware/dht22_mqtt.ino`, `docs/tahap2-serial-monitor.png` |
| **Peran C** | **Luthfi** | *MQTT Broker, Telegraf & Cloudflare Tunnel* | `rpi/mosquitto.conf`, `rpi/telegraf.conf`, `rpi/cloudflared-tunnel.service`, `docs/tahap3a-mosquitto-sub.png`, `docs/tahap3b-akses-publik.png` |
| **Peran D** | **Shaskia** | *InfluxDB, Grafana Dashboard & Dokumentasi Utama* | `dashboard/grafana-dashboard.json`, `docs/tahap4-grafana-dashboard.png`, `README.md`, `CHANGELOG.md` |

---

## 2. Rincian Kontribusi per Anggota

### A. Naflah — *Network & Raspberry Pi*
* Melakukan instalasi **Raspberry Pi OS (64-bit)** pada kartu microSD menggunakan Raspberry Pi Imager dengan *hostname* `rpi-kel1`.
* Mengaktifkan layanan *remote management* **Raspberry Pi Connect** agar perangkat server di kampus dapat dikelola secara jarak jauh.
* Mengonfigurasi aturan *firewall* **UFW** untuk membatasi akses jaringan hanya pada port SSH dan port MQTT (`1883`) dari segmen jaringan lokal kampus (`192.168.10.0/24`).
* Mengunggah bukti tangkapan layar verifikasi Tahap 1 (`docs/tahap1-piconnect-ufw.png`) serta menyusun log audit pada `kontribusi.md`.

### B. Dimas — *Sensor & Firmware ESP32*
* Merangkai modul sensor **DHT22** ke mikrokontroler **ESP32 DevKit** (`VCC` ke `3V3`, `DATA` ke `D4`/`GPIO4`, dan `GND` ke `GND`).
* Mengembangkan dan mengunggah program `firmware/dht22_mqtt.ino` menggunakan Arduino IDE untuk membaca data suhu dan kelembapan setiap 10 detik, mengatur indikator LED status koneksi, serta memublikasikan *payload* JSON ke topik `iot/kel1/dht22`.
* Menguji stabilitas pembacaan sensor pada Serial Monitor tanpa nilai `nan` dan mengunggah bukti tangkapan layar Tahap 2 (`docs/tahap2-serial-monitor.png`).

### C. Luthfi — *Platform Broker & Akses Internet*
* Menginstal dan mengonfigurasi broker **Eclipse Mosquitto** (`rpi/mosquitto.conf`) dengan autentikasi pengguna (`esp32`) dan menonaktifkan akses anonim.
* Mengonfigurasi agen **Telegraf** (`rpi/telegraf.conf`) untuk berlangganan topik MQTT dan meneruskan data ke basis data InfluxDB 2.x.
* Menyiapkan layanan *systemd* **Cloudflare Quick Tunnel** (`rpi/cloudflared-tunnel.service`) untuk mengekspos layanan Grafana lokal (`http://localhost:3000`) ke alamat HTTPS publik.
* Mengunggah bukti pengujian langganan broker MQTT (`docs/tahap3a-mosquitto-sub.png`) dan pengujian akses tautan publik melalui jaringan seluler (`docs/tahap3b-akses-publik.png`).

### D. Shaskia — *Basis Data, Dashboard & Dokumentasi Utama*
* Menyiapkan organisasi `paramadina` dan *bucket* `iot` pada **InfluxDB 2.x** serta menghasilkan token API untuk integrasi dengan Telegraf dan Grafana.
* Merancang visualisasi pemantauan pada **Grafana** (panel *Time Series*, indikator *Gauge* suhu dan kelembapan, serta aturan peringatan/*alert*) dan mengekspornya ke `dashboard/grafana-dashboard.json`.
* Mengunggah bukti tangkapan layar dashboard aktif (`docs/tahap4-grafana-dashboard.png`).
* Menyusun diagram arsitektur, panduan reproduksi sistem, tabel kendala dan solusi pada `README.md`, serta riwayat rilis pada `CHANGELOG.md`.

---

## 3. Audit Keamanan & Kebersihan Repositori

Sebelum finalisasi repositori publik, seluruh anggota telah melakukan pemeriksaan bersama untuk memastikan:
1. **Tidak ada kredensial asli yang tersimpan di GitHub:** Seluruh kata sandi WiFi, kata sandi broker MQTT, dan token API InfluxDB pada berkas kode maupun konfigurasi telah diganti menggunakan *placeholder* standar (`<WIFI_PASSWORD>`, `<MQTT_PASSWORD>`, dan `<INFLUXDB_API_TOKEN>`).
2. **Penamaan berkas konsisten:** Seluruh tautan gambar pada tabel pengujian di `README.md` merujuk tepat ke berkas yang tersedia di dalam direktori `docs/`.
3. **Riwayat *commit* terdistribusi:** Setiap anggota melakukan *commit* langsung menggunakan akun GitHub masing-masing sesuai pembagian direktori tanggung jawabnya.
