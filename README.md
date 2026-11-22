# Kelompok 1 - Pemantau Suhu dan Kelembapan

## Anggota dan peran

| Nama | NIM | Peran |
| :--- | :--- | :--- |
| Naflah Keumala Zahwa | 125103003 | Peran A: Network dan RPI Mengerjakan **Tahap 1** |
| Dimas Saputra | 125103085 | Peran B: Sensor dan Firmware Mengerjakan **Tahap 2** |
| Luthfi Muhammad | 125103001 | Peran C: Platform dan Akses Internet engerjakan **Tahap 3a&b** |
| Shaskia Fitri Handayani | 125103059 | Peran D: Data dan Dashboard Mengerjakan **Tahap 4** |

## Arsitektur

```
+-----------------------+      WiFi 2,4 GHz      +-----------------------------------------------------------------------+
|     ESP32 + DHT22     | ---------------------> |                      Raspberry Pi (rpi-kel1)                          |
| (Interval 10 detik)   |       MQTT :1883       |                                                                       |
+-----------------------+                        |  +-------------+     +----------+     +------------+     +---------+  |
                                                 |  |  Mosquitto  | --> | Telegraf | --> | InfluxDB 2 | --> | Grafana |  |
                                                 |  |   (:1883)   |     |  (MQTT)  |     |  (:8086)   |     | (:3000) |  |
                                                 |  +-------------+     +----------+     +------------+     +---------+  |
                                                 +----------------------------------------------------------------+------+
                                                                                                                  |
+-----------------------+     HTTPS (Publik)     +------------------------+            Localhost :3000            |
|    Browser Publik     | <--------------------- |   Cloudflare Tunnel    | <-------------------------------------+
|     (Wajib Login)     |                        |   (Layanan systemd)    |
+-----------------------+                        +------------------------+

*Jalur Manajemen Terpisah (Bukan Jalur Data):
[Mahasiswa] <---------> [Raspberry Pi Connect] <---------> [Terminal & Desktop rpi-kel1]
```

Sistem pemantau suhu dan kelembapan udara ini bekerja dalam satu rantai data terpadu yang dimulai dari sensor DHT22 pada mikrokontroler ESP32 DevKit (`GPIO4`) yang membaca kondisi ruangan setiap 10 detik dan mengirimkan pesan berformat JSON melalui WiFi 2,4 GHz ke broker MQTT Eclipse Mosquitto (port `1883` lokal) pada Raspberry Pi (`rpi-kel1`, IP `192.168.10.86`) di jaringan kampus. Agen Telegraf kemudian mengambil data dari topik `iot/kel1/dht22` tersebut dan menyimpannya ke dalam bucket `iot` pada basis data deret waktu InfluxDB 2.x (port `8086` lokal). Selanjutnya, Grafana (port `3000`) mengambil data menggunakan kueri Flux untuk menampilkan grafik *time series*, indikator *gauge*, serta mengelola aturan peringatan (*alert*) suhu, di mana hanya layanan Grafana yang dibuka ke Internet publik secara aman melalui Cloudflare Tunnel sementara pengelolaan server dilakukan terpisah menggunakan Raspberry Pi Connect.

## Hasil uji per tahap
` 
| Tahap | Bukti | Tanggal |
| :--- | :--- | :--- |
| Tahap 1: RPI ke Internet (`rpi-kel1` terhubung Pi Connect, SSH key & `ufw` aktif) | `docs/tahap1-piconnect-ufw.png` | 2026-11-05 |
| Tahap 2: Sensor ke RPI (DHT22 terbaca stabil di Serial Monitor tanpa `nan`) | `docs/tahap2-serial-monitor.png` | 2026-11-17 |
| Tahap 3a: Broker MQTT (`mosquitto_sub -t 'iot/#'` menerima pesan dengan autentikasi) | `docs/tahap3a-mosquitto-sub.png` | 2026-11-19 |
| Tahap 3b: Akses Internet (URL publik Cloudflare Tunnel terbuka dari jaringan seluler) | `docs/tahap3b-akses-publik.png` | 2026-11-19 |
| Tahap 4: Basisdata dan dashboard (data tersimpan di InfluxDB, tampil di Grafana & *alert* aktif) | `docs/tahap4-grafana-dashboard.png` | 2026-11-20 |

## Akses publik

* **Alamat dashboard:** `https://others-reality-displaying-show.trycloudflare.com` *(alamat quick tunnel berubah apabila layanan dimulai ulang)*
* **Jalur tunnel yang dipakai:** Cloudflare Tunnel (`cloudflared` dijalankan sebagai layanan `systemd` ke `http://localhost:3000`)
* **Video demo:** [ ]

## Kendala dan solusi

| Gejala | Penyebab | Perbaikan |
| :--- | :--- | :--- |
| Port COM ESP32 tidak muncul saat pertama dihubungkan ke laptop. | Driver USB-to-UART untuk chip serial CP2102 belum terpasang. | Menginstal CP210x Universal Windows Driver dari Silicon Labs dan memilih port COM yang terdeteksi. |
| ESP32 gagal terhubung ke broker MQTT (`rc=-2` atau `rc=5` di Serial Monitor). | Port `1883` masih terblokir *firewall* `ufw` pada Raspberry Pi atau kredensial akun MQTT tidak cocok dengan `/etc/mosquitto/passwd`. | Membuka akses port `1883` untuk jaringan lokal (`sudo ufw allow from 192.168.10.0/24 to any port 1883`) dan menyamakan kredensial akun `esp32`. |
| Data MQTT sudah masuk ke `mosquitto_sub`, tetapi belum muncul di bucket `iot` InfluxDB. | Agen Telegraf belum dimuat ulang (*restart*) setelah konfigurasi `telegraf.conf` diubah atau token API InfluxDB salah tempel. | Memperbarui *token* InfluxDB pada `/etc/telegraf/telegraf.conf` dan menjalankan `sudo systemctl restart telegraf`. |
| Alamat URL publik `trycloudflare.com` berubah setelah Raspberry Pi dimatikan/di-*restart*. | Layanan Cloudflare Quick Tunnel menghasilkan subdomain acak baru setiap kali layanan `cloudflared` dimulai ulang. | Mengecek URL terbaru melalui log `sudo journalctl -u cloudflared-tunnel` pada Raspberry Pi Connect dan memperbarui tautan akses di dokumentasi. |

## Cara menjalankan ulang

1. **Perbarui Raspberry Pi OS (64-bit) dan atur keamanan dasar:**
   ```bash
   sudo apt update && sudo apt full-upgrade -y
   sudo hostnamectl set-hostname rpi-kel1
   sudo apt install -y ufw mosquitto mosquitto-clients
   sudo ufw allow ssh && sudo ufw allow from 192.168.10.0/24 to any port 1883 && sudo ufw enable
   ```

2. **Konfigurasi autentikasi broker MQTT Mosquitto:**
   ```bash
   sudo mosquitto_passwd -c /etc/mosquitto/passwd esp32
   sudo cp rpi/mosquitto.conf /etc/mosquitto/conf.d/iot.conf
   sudo systemctl enable --now mosquitto
   ```

3. **Instal dan jalankan basis data InfluxDB 2.x serta Telegraf:**
   ```bash
   sudo apt install -y influxdb2 telegraf
   sudo systemctl enable --now influxdb
   # Buat organisasi "paramadina", bucket "iot", dan salin API Token melalui http://localhost:8086
   sudo cp rpi/telegraf.conf /etc/telegraf/telegraf.conf
   # Masukkan <MQTT_PASSWORD> dan <INFLUXDB_API_TOKEN> pada /etc/telegraf/telegraf.conf
   sudo systemctl enable --now telegraf
   ```

4. **Instal Grafana dan impor konfigurasi dashboard:**
   ```bash
   sudo apt install -y grafana
   sudo systemctl enable --now grafana-server
   # Buka http://localhost:3000, tambahkan data source InfluxDB (Flux), lalu impor dashboard/grafana-dashboard.json
   ```

5. **Aktifkan akses publik melalui Cloudflare Tunnel:**
   ```bash
   sudo apt install -y cloudflared
   sudo cp rpi/cloudflared-tunnel.service /etc/systemd/system/cloudflared-tunnel.service
   sudo systemctl daemon-reload
   sudo systemctl enable --now cloudflared-tunnel
   ```

6. **Unggah firmware sensor ke ESP32 DevKit:**
   * Buka `firmware/dht22_mqtt.ino` menggunakan Arduino IDE (pasang pustaka `DHT sensor library` dan `PubSubClient`).
   * Isi kredensial `WIFI_PASSWORD` dan `MQTT_PASSWORD`, pilih board **DOIT ESP32 DEVKIT V1**, lalu klik **Upload**.
