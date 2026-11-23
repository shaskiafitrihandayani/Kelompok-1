# Kelompok 1 - Pemantau Suhu dan Kelembapan

## Anggota dan peran

| Nama | NIM | Peran |
| :--- | :--- | :--- |
| Naflah Keumala Zahwa | 125103003 | Peran A: Network dan RPI - Mengerjakan **Tahap 1** |
| Dimas Saputra | 125103085 | Peran B: Sensor dan Firmware - Mengerjakan **Tahap 2** (Koordinator) |
| Luthfi Muhammad | 125103001 | Peran C: Platform dan Akses Internet - Mengerjakan **Tahap 3a & 3b** |
| Shaskia Fitri Handayani | 125103059 | Peran D: Data dan Dashboard - Mengerjakan **Tahap 4** |

## Arsitektur

```text
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
Sistem pemantau suhu dan kelembapan udara ini bekerja dalam satu rantai data terpadu yang dimulai dari sensor DHT22 pada mikrokontroler ESP32 (GPIO4) yang membaca kondisi ruangan dan mengirimkan pesan berformat JSON melalui WiFi 2,4 GHz ke broker MQTT Eclipse Mosquitto (port 1883 lokal) pada Raspberry Pi (rpi-kel1, IP 192.168.10.86) di jaringan kampus. Agen Telegraf kemudian mengambil data dari topik iot/kel1/dht22 tersebut dan menyimpannya ke dalam bucket iot pada basis data deret waktu InfluxDB 2.x (port 8086 lokal). Selanjutnya, Grafana (port 3000) mengambil data menggunakan kueri Flux untuk menampilkan grafik time series, indikator gauge, serta mengelola aturan peringatan (alert) suhu, di mana hanya layanan Grafana yang dibuka ke Internet publik secara aman melalui Cloudflare Tunnel sementara pengelolaan server dilakukan terpisah menggunakan Raspberry Pi Connect.

## Hasil uji per tahap

| Tahap | Bukti | Tanggal |
| :--- | :--- | :--- |
| **Tahap 1: RPI ke Internet** (`rpi-kel1` terhubung Pi Connect, konfigurasi IP, `ufw` aktif, dan uji `ping` berhasil) | • [`01-pi-connect-aktif.jpg`](docs/tahap1_rpi_piconnect/01-pi-connect-aktif.jpg)<br>• [`02-ip-address.png`](docs/tahap1_rpi_piconnect/02-ip-address.png)<br>• [`03-ufw-status.png`](docs/tahap1_rpi_piconnect/03-ufw-status.png)<br>• [`04-ping-internet.png`](docs/tahap1_rpi_piconnect/04-ping-internet.png) | 2026-11-15 |
| **Tahap 2: Sensor ke RPI** (Rangkaian fisik DHT22 pada ESP32 & pembacaan stabil di Serial Monitor tanpa `nan`) | • [`01-rangkaian-esp32-dht22.jpeg`](docs/tahap2-sensor-esp32/01-rangkaian-esp32-dht22.jpeg)<br>• [`02-serial-monitor-ok.png`](docs/tahap2-sensor-esp32/02-serial-monitor-ok.png) | 2026-11-17 |
| **Tahap 3a: Broker MQTT** (`mosquitto_sub -t 'iot/#'` menerima pesan JSON dari ESP32 dengan autentikasi user & password) | • [`01-mosquitto-sub-terima-data.jpeg`](docs/tahap3a-broker-mqtt/01-mosquitto-sub-terima-data.jpeg) | 2026-11-19 |
| **Tahap 3b: Akses Internet** (URL publik Cloudflare Tunnel terbuka dari jaringan data seluler & wajib login) | • [`01-akses-publik-seluler.jpeg`](docs/tahap3b-tunnel/01-akses-publik-seluler.jpeg) | 2026-11-19 |
| **Tahap 4: Basisdata dan dashboard** (Data tersimpan di InfluxDB, tampil di Grafana, & aturan *Alert Suhu Tinggi* berstatus *Firing*) | • [`01-grafana-dashboard-aktif.png`](docs/tahap4-grafana-dashboard/01-grafana-dashboard-aktif.png)<br>• [`02-grafana-alert-dan-kueri-influxdb.png`](docs/tahap4-grafana-dashboard/02-grafana-alert-dan-kueri-influxdb.png) | 2026-11-19 |

## Akses publik

- **Alamat dashboard:** [https://others-reality-displaying-show.trycloudflare.com](https://others-reality-displaying-show.trycloudflare.com)
- **Jalur tunnel yang dipakai:** Cloudflare Tunnel (`cloudflared` dijalankan sebagai layanan `systemd` ke `http://localhost:3000`)
- **Video demo:** `[drive]`

## Kendala dan solusi

| Gejala | Penyebab | Perbaikan |
| :--- | :--- | :--- |
| Port COM ESP32 tidak muncul saat pertama dihubungkan ke laptop. | Driver USB-to-UART untuk chip serial CP2102 belum terpasang. | Menginstal CP210x Universal Windows Driver dari Silicon Labs dan memilih port COM yang terdeteksi. |
| ESP32 gagal terhubung ke broker MQTT (`rc=-2` atau `rc=5` di Serial Monitor). | Mosquitto hanya mendengarkan `localhost` secara bawaan (`listener 1883` belum diatur untuk jaringan lokal) atau kredensial user/password MQTT tidak cocok dengan `/etc/mosquitto/passwd`. | Menambahkan `listener 1883` dan `allow_anonymous false` pada konfigurasi Mosquitto, lalu menyamakan user dan password akun `esp32` di firmware dengan `/etc/mosquitto/passwd`. |
| Data MQTT sudah masuk ke `mosquitto_sub`, tetapi belum muncul di bucket `iot` InfluxDB. | Agen Telegraf belum dimuat ulang (*restart*) setelah konfigurasi `telegraf.conf` diubah atau token API InfluxDB salah tempel. | Memperbarui *token* InfluxDB pada `/etc/telegraf/telegraf.conf` dan menjalankan `sudo systemctl restart telegraf`. |
| Alamat URL publik `trycloudflare.com` berubah setelah Raspberry Pi dimatikan/di-*restart*. | Layanan Cloudflare Quick Tunnel menghasilkan subdomain acak baru setiap kali layanan `cloudflared` dimulai ulang. | Mengecek URL terbaru melalui log `sudo journalctl -u cloudflared-tunnel` pada Raspberry Pi Connect dan memperbarui tautan akses di dokumentasi. |

## Cara menjalankan ulang

1. **Perbarui Raspberry Pi OS (64-bit), aktifkan SSH key, dan atur keamanan firewall UFW:**
   ```bash
   sudo apt update && sudo apt full-upgrade -y
   sudo hostnamectl set-hostname rpi-kel1
   sudo apt install -y ufw mosquitto mosquitto-clients

   # Konfigurasi kunci SSH (daftarkan public key dari perangkat klien)
   mkdir -p ~/.ssh && chmod 700 ~/.ssh
   echo "ISI_PUBLIC_KEY_ED25519" >> ~/.ssh/authorized_keys && chmod 600 ~/.ssh/authorized_keys

   # Aktivasi firewall UFW (hanya membuka akses SSH dan port MQTT jaringan lokal)
   sudo ufw allow ssh
   sudo ufw allow from 192.168.10.0/24 to any port 1883
   sudo ufw enable
