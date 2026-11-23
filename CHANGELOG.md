# Changelog - Kelompok 1

Catatan perubahan konfigurasi sistem, firmware, dan dokumentasi oleh masing-masing peran (siapa, kapan, dan apa yang diubah).

---

### 2026-11-15 - Peran A (Naflah Keumala Zahwa)
- Mengatur *hostname* Raspberry Pi menjadi `rpi-kel1` dan memperbarui sistem operasi Raspberry Pi OS (64-bit).
- Mengonfigurasi autentikasi kunci SSH (`~/.ssh/authorized_keys`) untuk keamanan akses jarak jauh.
- Mengaktifkan *firewall* UFW (`sudo ufw enable`) dengan hanya membuka akses SSH serta port lokal untuk layanan yang dibutuhkan.
- Memastikan koneksi Internet Raspberry Pi stabil dan terhubung aktif melalui Raspberry Pi Connect.

### 2026-11-17 - Peran B (Dimas Saputra)
- Merangkai sensor suhu dan kelembapan DHT22 ke pin `GPIO4`, `3V3`, dan `GND` pada board ESP32.
- Menulis dan mengunggah firmware pembacaan sensor menggunakan pustaka `DHT sensor library` dan `PubSubClient` dengan interval pengiriman 10 detik.
- Memverifikasi hasil pembacaan suhu (`°C`) dan kelembapan (`%`) pada Serial Monitor hingga stabil tanpa galat `nan`.

### 2026-11-19 - Peran C (Luthfi Muhammad)
- Menginstal broker MQTT Eclipse Mosquitto di Raspberry Pi, menonaktifkan akses anonim (`allow_anonymous false`), dan membuat kredensial khusus akun `esp32`.
- Membuka *listener* MQTT port `1883` khusus jaringan lokal dan menguji penerimaan data JSON pada topik `iot/kel1/dht22` via `mosquitto_sub`.
- Menyiapkan akses publik aman ke Grafana (port `3000`) menggunakan Cloudflare Tunnel (`cloudflared`) yang berjalan sebagai layanan `systemd`.

### 2026-11-19 - Peran D (Shaskia Fitri Handayani)
- Menginstal dan menjalankan InfluxDB 2.x serta membuat organisasi `paramadina`, *bucket* `iot`, dan token API khusus Telegraf.
- Mengonfigurasi Telegraf (`mqtt_consumer`) untuk meneruskan payload JSON dari broker MQTT ke InfluxDB 2.x.
- Membangun dashboard Grafana (panel *Time Series* dan *Gauge*), mengatur aturan peringatan **`Alert Suhu Tinggi`**, serta mengekspor konfigurasi ke `dashboard/grafana-dashboard.json`.

### 2026-11-24 - Seluruh Anggota (Peran A, B, C, D)
- Melakukan audit silang antarperan untuk menguji ulang setiap tahapan berdasarkan dokumentasi.
- Menyensor seluruh kredensial sensitif (password WiFi, password MQTT, dan token API) pada kode maupun tangkapan layar.
- Melengkapi seluruh bukti uji tiap tahap ke folder `docs/` serta menyusun `README.md`, `CHANGELOG.md`, dan `kontribusi.md`.
