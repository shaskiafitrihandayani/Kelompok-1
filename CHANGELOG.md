# Changelog

Seluruh perubahan dan pencapaian penting dalam pengembangan sistem pemantau suhu dan kelembapan udara **Kelompok 1** dicatat pada berkas ini.

---

## [1.0.0] - 2026-11-23 (Finalisasi Repositori & Dokumentasi UTS)

### Ditambahkan
* **Dokumentasi Utama (`README.md`):** Diagram arsitektur aliran data terpadu, tabel pembagian peran anggota, rincian langkah reproduksi instalasi, serta tabel kendala dan solusi (*troubleshooting*).
* **Kode Firmware (`firmware/dht22_mqtt.ino`):** Kode pembacaan sensor DHT22 pada pin `GPIO4` (`D4`) dengan interval pengiriman 10 detik melalui protokol MQTT ke topik `iot/kel1/dht22`.
* **Konfigurasi Server (`rpi/`):** Berkas konfigurasi broker `mosquitto.conf`, agen pengumpul data `telegraf.conf`, dan layanan *systemd* `cloudflared-tunnel.service`.
* **Ekspor Dashboard (`dashboard/grafana-dashboard.json`):** Konfigurasi panel visualisasi *time series*, *gauge* suhu dan kelembapan, kueri Flux, serta aturan peringatan (*alert*).
* **Bukti Pengujian (`docs/`):** Tangkapan layar verifikasi untuk seluruh tahapan pengujian (`tahap1-piconnect-ufw.png` hingga `tahap4-grafana-dashboard.png`).

### Keamanan
* Menstandarkan seluruh kredensial sensitif pada kode dan berkas konfigurasi publik menggunakan *placeholder* (`WIFI_PASSWORD`, `MQTT_PASSWORD`, dan `INFLUXDB_API_TOKEN`) untuk mencegah kebocoran data.

---

## [0.4.0] - 2026-11-20 (Tahap 4: Basis Data InfluxDB & Dashboard Grafana)

### Ditambahkan
* Mengonfigurasi agen **Telegraf** untuk berlangganan topik MQTT `iot/kel1/dht22` dengan format payload JSON dan menyimpannya ke *bucket* `iot` pada **InfluxDB 2.x**.
* Membangun dashboard pemantauan pada **Grafana** (`localhost:3000`) menggunakan kueri Flux serta mengaktifkan aturan peringatan (*alert*) apabila suhu melewati ambang batas yang ditentukan.

---

## [0.3.0] - 2026-11-19 (Tahap 3a & 3b: Broker MQTT & Cloudflare Tunnel)

### Ditambahkan
* Mengaktifkan autentikasi pengguna (`esp32`) pada broker **Eclipse Mosquitto** (`port 1883`) dengan menonaktifkan akses anonim (`allow_anonymous false`).
* Mengonfigurasi **Cloudflare Quick Tunnel** (`cloudflared`) sebagai layanan *systemd* untuk mengekspos layanan Grafana lokal (`http://localhost:3000`) ke alamat HTTPS publik dengan tetap mewajibkan login pengguna.

---

## [0.2.0] - 2026-11-17 (Tahap 2: Integrasi Sensor DHT22 & Firmware ESP32)

### Ditambahkan
* Merangkai sensor **DHT22** ke **ESP32 DevKit** (`VCC` ke `3V3`, `DATA` ke `D4`/`GPIO4`, `GND` ke `GND`).
* Mengunggah sketsa Arduino untuk koneksi WiFi kampus (`paramadina_cikarang`), indikator LED status koneksi, serta publikasi data JSON suhu dan kelembapan secara berkala.

---

## [0.1.0] - 2026-11-05 (Tahap 1: Inisialisasi Raspberry Pi & Keamanan Dasar)

### Ditambahkan
* Instalasi **Raspberry Pi OS (64-bit)** menggunakan Raspberry Pi Imager dengan *hostname* `rpi-kel1`.
* Mengaktifkan layanan *remote management* **Raspberry Pi Connect** untuk pengelolaan terminal dan desktop jarak jauh.
* Mengonfigurasi *firewall* **UFW** agar hanya membuka port SSH dan port MQTT (`1883`) dari jaringan lokal kampus (`192.168.10.0/24`).
