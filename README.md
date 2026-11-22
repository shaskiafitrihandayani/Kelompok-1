# Kelompok 1 - Pemantau Suhu dan Kelembapan

## Anggota dan peran

| Nama | NIM | Peran |
| :--- | :--- | :--- |
| Naflah Keumala Zahwa | 125103003 | Peran A: Network dan RPI Mengerjakan **Tahap 1** (setup OS Raspberry Pi `rpi-kel1`, SSH key, firewall `ufw`, koneksi Raspberry Pi Connect, serta koordinasi repositori). |
| Dimas Saputra | 125103085 | Peran B: Sensor dan Firmware Mengerjakan **Tahap 2** (instalasi lingkungan Arduino IDE & driver CP2102, perakitan ESP32 + DHT22 pada breadboard, dan pengiriman data via MQTT). |
| Luthfi Muhammad | 125103001 | Peran C: Platform dan Akses Internet engerjakan **Tahap 3a** (konfigurasi broker MQTT Mosquitto & autentikasi user) dan **Tahap 3b** (konfigurasi Cloudflare Tunnel `systemd` untuk akses publik). |
| Shaskia Fitri Handayani | 125103059 | Peran D: Data dan Dashboard Mengerjakan **Tahap 4** (konfigurasi Telegraf dan basis data deret waktu InfluxDB 2.x, pembuatan panel visualisasi Grafana, serta pengaturan *alert* suhu). |

## Arsitektur

diagram:

Sistem pemantau suhu dan kelembapan udara ini bekerja dalam satu rantai data terpadu yang dimulai dari sensor DHT22 pada mikrokontroler ESP32 DevKit (`GPIO4` / `GPIO32`) yang membaca kondisi ruangan setiap 10 detik dan mengirimkan pesan berformat JSON melalui WiFi 2,4 GHz ke broker MQTT Eclipse Mosquitto (port `1883` lokal) pada Raspberry Pi (`rpi-kel1`, IP `192.168.10.86`) di jaringan kampus. Agen Telegraf kemudian mengambil data dari topik `iot/kel1/dht22` tersebut dan menyimpannya ke dalam bucket `iot` pada basis data deret waktu InfluxDB 2.x (port `8086` lokal). Selanjutnya, Grafana (port `3000`) mengambil data menggunakan kueri Flux untuk menampilkan grafik *time series*, indikator *gauge*, serta mengelola aturan peringatan (*alert*) suhu, di mana hanya layanan Grafana yang dibuka ke Internet publik secara aman melalui Cloudflare Tunnel sementara pengelolaan server dilakukan terpisah menggunakan Raspberry Pi Connect.

## Hasil uji per tahap

| Tahap | Bukti | Tanggal |
| :--- | :--- | :--- |
| Tahap 1: RPI ke Internet (`rpi-kel1` terhubung Pi Connect, SSH key & `ufw` aktif) | `docs/tahap1-piconnect-ufw.png` | 2026-11- |
| Tahap 2: Sensor ke RPI (DHT22 terbaca stabil di Serial Monitor tanpa `nan`) | `docs/tahap2-serial-monitor.png` | 2026-11- |
| Tahap 3a: Broker MQTT (`mosquitto_sub -t 'iot/#'` menerima pesan dengan autentikasi) | `docs/tahap3a-mosquitto-sub.png` | 2026-11- |
| Tahap 3b: Akses Internet (URL publik Cloudflare Tunnel terbuka dari jaringan seluler) | `docs/tahap3b-akses-publik.png` | 2026-11- |
| Tahap 4: Basisdata dan dashboard (data tersimpan di InfluxDB, tampil di Grafana & *alert* aktif) | `docs/tahap4-grafana-dashboard.png` | 2026-11- |

## Akses publik

* **Alamat dashboard:** `https://others-reality-displaying-show.trycloudflare.com` *(alamat quick tunnel berubah apabila layanan dimulai ulang)*
* **Jalur tunnel yang dipakai:** Cloudflare Tunnel (`cloudflared` dijalankan sebagai layanan `systemd` ke `http://localhost:3000`)
* **Video demo:** [ ]

## Kendala dan solusi

| Gejala | Penyebab | Perbaikan |
| :--- | :--- | :--- |
| Port COM ESP32 tidak muncul saat pertama dihubungkan ke laptop Windows. | Driver USB-to-UART untuk chip serial CP2102 belum terpasang. | Menginstal *CP210x Universal Windows Driver* dari Silicon Labs dan memilih port COM yang terdeteksi. |
| Proses *upload* kode di Arduino IDE tertahan pada tulisan `Connecting.......`. | ESP32 belum masuk ke mode *flashing* secara otomatis. | Menekan dan menahan tombol fisik `BOOT` selama ±2 detik saat proses unggah dimulai hingga *writing* berjalan. |
| Dashboard Grafana menampilkan status *No data* ketika dibuka dari jaringan rumah. | Perangkat ESP32 tidak berada di jaringan WiFi kampus (`paramadina_cikarang`), sehingga tidak ada data baru dalam 15 menit terakhir. | Mengubah *time range* di pojok kanan atas Grafana ke rentang waktu pengujian sebelumnya (misalnya *Last 7 days*) untuk melihat riwayat data di InfluxDB. |

## Cara menjalankan ulang

1. **Perbarui Raspberry Pi OS (64-bit) dan atur keamanan dasar:**
   ```bash
   sudo apt update && sudo apt full-upgrade -y
   sudo hostnamectl set-hostname rpi-kel1
   sudo apt install -y ufw mosquitto mosquitto-clients
   sudo ufw allow ssh && sudo ufw allow from 192.168.10.0/24 to any port 1883 && sudo ufw enable
