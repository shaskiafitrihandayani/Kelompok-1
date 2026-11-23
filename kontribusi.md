# Kontribusi Anggota - Kelompok 1

Catatan pembagian peran, rincian pengerjaan, dan bukti kontribusi masing-masing anggota di repositori GitHub proyek Sistem Pemantau Suhu dan Kelembapan Udara.

---

## 1. Pembagian Peran dan Tanggung Jawab

| Peran | Nama | NIM | Fokus Pekerjaan | File dan Folder Tanggung Jawab |
| :--- | :--- | :--- | :--- | :--- |
| **Peran A** | Naflah Keumala Zahwa | 125103003 | Network dan Setup Raspberry Pi | [`docs/tahap1_rpi_piconnect/`](docs/tahap1_rpi_piconnect), [`kontribusi.md`](kontribusi.md) |
| **Peran B** | Dimas Saputra | 125103085 | Rangkaian Sensor dan Firmware ESP32 | [`firmware/dht22_mqtt.ino`](firmware/dht22_mqtt.ino), [`docs/tahap2-sensor-esp32/`](docs/tahap2-sensor-esp32) |
| **Peran C** | Luthfi Muhammad | 125103001 | Broker MQTT dan Cloudflare Tunnel | [`rpi/mosquitto.conf`](rpi/mosquitto.conf), [`rpi/cloudflared-tunnel.service`](rpi/cloudflared-tunnel.service), [`docs/tahap3a-broker-mqtt/`](docs/tahap3a-broker-mqtt), [`docs/tahap3b-tunnel/`](docs/tahap3b-tunnel) |
| **Peran D** | Shaskia Fitri Handayani | 125103059 | InfluxDB, Telegraf, Grafana, dan Dokumentasi | [`rpi/telegraf.conf`](rpi/telegraf.conf), [`dashboard/grafana-dashboard.json`](dashboard/grafana-dashboard.json), [`docs/tahap4-grafana-dashboard/`](docs/tahap4-grafana-dashboard), [`README.md`](README.md), [`CHANGELOG.md`](CHANGELOG.md) |

---

## 2. Rincian Pengerjaan per Anggota

### Peran A - Naflah Keumala Zahwa (125103003)
- Menginstal Raspberry Pi OS (64-bit) ke microSD memakai Raspberry Pi Imager dan mengatur hostname menjadi `rpi-kel1` (IP lokal `192.168.10.86`).
- Menghubungkan perangkat ke Raspberry Pi Connect supaya terminal dan desktop bisa diakses jarak jauh tanpa monitor.
- Mengatur autentikasi SSH key dan mengaktifkan firewall UFW agar hanya port SSH dan port MQTT (`1883`) dari jaringan lokal kampus (`192.168.10.0/24`) yang terbuka.
- Mengunggah bukti screenshot Tahap 1 ([`01-pi-connect-aktif.jpg`](docs/tahap1_rpi_piconnect/01-pi-connect-aktif.jpg), [`02-ip-address.png`](docs/tahap1_rpi_piconnect/02-ip-address.png), [`03-ufw-status.png`](docs/tahap1_rpi_piconnect/03-ufw-status.png), [`04-ping-internet.png`](docs/tahap1_rpi_piconnect/04-ping-internet.png)) dan menulis catatan di [`kontribusi.md`](kontribusi.md).

### Peran B - Dimas Saputra (125103085)
- Merangkai sensor DHT22 ke board ESP32 Dev Module (`VCC` ke `3V3`, `DATA` ke `D4`/`GPIO4`, dan `GND` ke `GND`).
- Membuat dan mengunggah kode [`firmware/dht22_mqtt.ino`](firmware/dht22_mqtt.ino) lewat Arduino IDE untuk membaca suhu dan kelembapan tiap 10 detik, menyalakan indikator LED koneksi, dan mengirim data JSON ke topik `iot/kel1/dht22`.
- Mengecek hasil pembacaan sensor di Serial Monitor sampai stabil tanpa nilai `nan`, lalu mengunggah bukti Tahap 2 ([`01-rangkaian-esp32-dht22.jpeg`](docs/tahap2-sensor-esp32/01-rangkaian-esp32-dht22.jpeg) dan [`02-serial-monitor-ok.png`](docs/tahap2-sensor-esp32/02-serial-monitor-ok.png)).

### Peran C - Luthfi Muhammad (125103001)
- Menginstal dan mengatur broker Eclipse Mosquitto ([`rpi/mosquitto.conf`](rpi/mosquitto.conf)) dengan login akun `esp32` serta mematikan akses anonim (`allow_anonymous false`).
- Menjalankan Cloudflare Quick Tunnel sebagai layanan systemd ([`rpi/cloudflared-tunnel.service`](rpi/cloudflared-tunnel.service)) supaya halaman Grafana lokal (`http://localhost:3000`) bisa dibuka lewat HTTPS publik.
- Mengunggah bukti penerimaan data MQTT di terminal ([`01-mosquitto-sub-terima-data.jpeg`](docs/tahap3a-broker-mqtt/01-mosquitto-sub-terima-data.jpeg)) dan bukti akses link publik memakai data seluler ([`01-akses-publik-seluler.jpeg`](docs/tahap3b-tunnel/01-akses-publik-seluler.jpeg)).

### Peran D - Shaskia Fitri Handayani (125103059)
- Membuat organisasi `paramadina` dan bucket `iot` di InfluxDB 2.x sekaligus membuat API Token untuk penyimpanan data.
- Mengatur Telegraf ([`rpi/telegraf.conf`](rpi/telegraf.conf)) untuk mengambil data dari topik MQTT `iot/kel1/dht22` dan menyimpannya ke InfluxDB 2.x.
- Membuat tampilan dashboard Grafana (panel Time Series, Gauge suhu dan kelembapan, serta aturan **Alert Suhu Tinggi**) lalu mengekspornya ke [`dashboard/grafana-dashboard.json`](dashboard/grafana-dashboard.json).
- Mengunggah screenshot dashboard aktif dan aturan alert ([`01-grafana-dashboard-aktif.png`](docs/tahap4-grafana-dashboard/01-grafana-dashboard-aktif.png) dan [`02-grafana-alert-dan-kueri-influxdb.png`](docs/tahap4-grafana-dashboard/02-grafana-alert-dan-kueri-influxdb.png)).
- Menyusun diagram arsitektur, langkah instalasi ulang, tabel kendala di [`README.md`](README.md), dan riwayat pengerjaan di [`CHANGELOG.md`](CHANGELOG.md).

---

## 3. Pengecekan Akhir Repositori

Sebelum dikumpulkan, seluruh anggota sudah mengecek ulang isi repositori untuk memastikan:
1. Semua password WiFi, password MQTT, dan API Token InfluxDB di kode maupun file konfigurasi sudah diganti menjadi placeholder (`"WIFI_PASSWORD"`, `"MQTT_PASSWORD"`, dan `"INFLUXDB_API_TOKEN"`).
2. Semua tautan gambar di [`README.md`](README.md) dan [`kontribusi.md`](kontribusi.md) sudah mengarah ke file yang sesuai di folder [`docs/`](docs).
