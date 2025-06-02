# Docker ve Docker Compose Komutları Rehberi

Bu belge, Docker ve Docker Compose'un en önemli komutlarını, detaylı açıklamalarını, yaygın seçeneklerini ve gerçek dünyadan kullanım örneklerini içerir. Docker, konteyner tabanlı uygulamaları oluşturmak, dağıtmak ve yönetmek için güçlü bir platformdur. Docker Compose ise birden fazla konteyneri koordine etmek için kullanılır. Bu rehber, hem tekil konteyner yönetimi hem de çoklu konteyner uygulamaları için kapsamlı bir referans sunar. Her komut, pratik senaryolar, olası hata durumları ve en iyi uygulamalarla açıklanmıştır.

**Not**: Komutlar, `docker [KOMUT] [SEÇENEKLER] [ARGÜMANLAR]` veya `docker-compose [KOMUT] [SEÇENEKLER] [ARGÜMANLAR]` formatında çalışır. Örnekler, genellikle `bash` kabuğunda çalıştırılır. Tarih: 10 Mayıs 2025.

## İçindekiler

1. [Konteyner Komutları](#1-konteyner-komutları)
   - [docker run](#a-docker-run)
   - [docker exec](#b-docker-exec)
   - [docker ps](#c-docker-ps)
   - [docker stop](#d-docker-stop)
   - [docker start](#e-docker-start)
   - [docker restart](#f-docker-restart)
   - [docker rm](#g-docker-rm)
   - [docker logs](#h-docker-logs)
   - [docker inspect](#i-docker-inspect)
   - [docker cp](#j-docker-cp)
   - [docker commit](#k-docker-commit)
   - [docker pause](#l-docker-pause)
   - [docker unpause](#m-docker-unpause)
2. [İmaj Komutları](#2-imaj-komutları)
   - [docker build](#a-docker-build)
   - [docker images](#b-docker-images)
   - [docker pull](#c-docker-pull)
   - [docker push](#d-docker-push)
   - [docker rmi](#e-docker-rmi)
   - [docker tag](#f-docker-tag)
   - [docker save](#g-docker-save)
   - [docker load](#h-docker-load)
   - [docker history](#i-docker-history)
   - [docker import](#j-docker-import)
3. [Docker Compose Komutları](#3-docker-compose-komutları)
   - [docker-compose up](#a-docker-compose-up)
   - [docker-compose down](#b-docker-compose-down)
   - [docker-compose ps](#c-docker-compose-ps)
   - [docker-compose logs](#d-docker-compose-logs)
   - [docker-compose exec](#e-docker-compose-exec)
   - [docker-compose build](#f-docker-compose-build)
   - [docker-compose config](#g-docker-compose-config)
   - [docker-compose restart](#h-docker-compose-restart)
   - [docker-compose stop](#i-docker-compose-stop)
   - [docker-compose start](#j-docker-compose-start)
   - [docker-compose rm](#k-docker-compose-rm)
   - [docker-compose pause](#l-docker-compose-pause)
   - [docker-compose unpause](#m-docker-compose-unpause)
   - [docker-compose kill](#n-docker-compose-kill)
   - [docker-compose top](#o-docker-compose-top)
4. [Ağ Komutları](#4-ağ-komutları)
   - [docker network ls](#a-docker-network-ls)
   - [docker network create](#b-docker-network-create)
   - [docker network rm](#c-docker-network-rm)
   - [docker network connect](#d-docker-network-connect)
   - [docker network disconnect](#e-docker-network-disconnect)
   - [docker network inspect](#f-docker-network-inspect)
5. [Hacim Komutları](#5-hacim-komutları)
   - [docker volume ls](#a-docker-volume-ls)
   - [docker volume create](#b-docker-volume-create)
   - [docker volume rm](#c-docker-volume-rm)
   - [docker volume inspect](#d-docker-volume-inspect)
6. [Diğer Önemli Komutlar](#6-diğer-önemli-komutlar)
   - [docker info](#a-docker-info)
   - [docker login](#b-docker-login)
   - [docker logout](#c-docker-logout)
   - [docker system prune](#d-docker-system-prune)
   - [docker system df](#e-docker-system-df)
   - [docker version](#f-docker-version)
   - [docker events](#g-docker-events)
   - [docker diff](#h-docker-diff)
7. [Genel İpuçları ve En İyi Uygulamalar](#7-genel-ipuçları-ve-en-iyi-uygulamalar)

---

## 1. Konteyner Komutları

Konteynerler, uygulamaların izole bir ortamda çalışmasını sağlar. Aşağıdaki komutlar, konteynerlerin oluşturulması, çalıştırılması, durdurulması, silinmesi ve diğer yönetim işlemleri için kullanılır.

### a. `docker run`

**Açıklama**: Bir Docker imajından yeni bir konteyner oluşturur ve başlatır. Docker’ın en temel ve çok yönlü komutudur. Konteyner oluşturma, yapılandırma ve çalıştırma işlemlerini tek adımda gerçekleştirir. Geliştirme, test ve üretim ortamlarında uygulamaları başlatmak için kullanılır. İmaj yerel sistemde yoksa, Docker Hub’dan otomatik indirilir.

**Yapısı**:
```bash
docker run [SEÇENEKLER] İMAJ[:ETİKET] [KOMUT] [ARGÜMANLAR...]
```

**Yaygın Seçenekler**:
- `-d` veya `--detach`: Konteyneri arka planda çalıştırır. Aksi takdirde, konteyner ön planda çalışır ve terminali bağlar.
- `--name`: Konteynere bir isim atar (ör. `--name my_app`). İsim verilmezse, Docker rastgele bir isim üretir.
- `-p` veya `--publish`: Host ile konteyner arasında port eşleşmesi yapar (ör. `-p 8080:80`, host’un 8080 portunu konteynerin 80 portuna bağlar).
- `-v` veya `--volume`: Hacim bağlar (ör. `-v /host/path:/container/path`, host’taki bir dizini konteynere bağlar).
- `--rm`: Konteyner durduğunda otomatik olarak silinir (geçici konteynerler için idealdir).
- `-it` veya `--interactive --tty`: İnteraktif terminal başlatır (kabuk veya komut için).
- `--env` veya `-e`: Ortam değişkeni tanımlar (ör. `-e DB_HOST=localhost`).
- `--env-file`: Ortam değişkenlerini bir dosyadan okur (ör. `--env-file .env`).
- `--network`: Konteyneri belirli bir ağa bağlar (ör. `--network my_network` veya `--network host`).
- `--restart`: Konteynerin yeniden başlatma politikasını belirler:
  - `no`: Yeniden başlatma yok (varsayılan).
  - `on-failure[:N]`: Hata durumunda yeniden başlat (N kez).
  - `always`: Her zaman yeniden başlat.
  - `unless-stopped`: Manuel durdurma hariç her zaman yeniden başlat.
- `--memory`: Maksimum bellek sınırını belirler (ör. `--memory 512m`).
- `--cpus`: Kullanılabilir CPU çekirdeklerini sınırlar (ör. `--cpus 1.5`).
- `--cap-add` / `--cap-drop`: Linux yeteneklerini ekler veya kaldırır (ör. `--cap-add NET_ADMIN`).
- `--privileged`: Konteynere genişletilmiş yetkiler verir (güvenlik riski taşır).

**Kullanım Senaryoları**:
- Web sunucusu çalıştırmak (ör. Nginx, Apache).
- Veritabanı konteyneri başlatmak (ör. MySQL, PostgreSQL).
- Geliştirme ortamında interaktif kabuk açmak.
- Geçici test veya CI/CD konteynerleri oluşturmak.
- Mikroservis mimarisinde servisleri dağıtmak.

**Örnekler**:
```bash
docker run -d --name my_nginx -p 8080:80 nginx:latest
```
- Nginx imajından bir konteyner başlatır, host’un 8080 portunu konteynerin 80 portuna bağlar ve arka planda çalıştırır.
```bash
docker run -it --rm ubuntu /bin/bash
```
- Ubuntu imajından interaktif bir kabuk başlatır ve konteyner durduğunda siler.
```bash
docker run --name mysql_db -e MYSQL_ROOT_PASSWORD=secret -v mysql_data:/var/lib/mysql --restart unless-stopped -d mysql:8.0
```
- MySQL konteyneri başlatır, ortam değişkeni tanımlar, hacim bağlar, manuel durdurma hariç yeniden başlatır.
```bash
docker run --network my_network --memory 256m redis:latest
```
- Redis konteynerini özel bir ağda ve 256 MB bellek sınırı ile başlatır.
```bash
docker run --env-file .env -p 3000:3000 my_app:1.0
```
- Ortam değişkenlerini `.env` dosyasından okuyarak bir uygulama başlatır.

**Hata Durumları**:
- **Port çakışması**: Aynı host portu başka bir konteyner tarafından kullanılıyorsa hata verir (`docker ps` ile kontrol edin).
- **İmaj bulunamadı**: İmaj yerel sistemde veya registry’de yoksa, `docker pull` ile indirilmelidir.
- **Hatalı komut**: İmajda belirtilen komut yoksa (ör. `/bin/bash` yoksa), konteyner başlar ve hemen durur.

**Notlar**:
- `docker run`, `docker create` + `docker start` işlemlerinin birleşimidir.
- `-it` genellikle kabuk gibi interaktif komutlar için, `-d` servis tabanlı uygulamalar için kullanılır.
- `--privileged` güvenlik riski taşır; yalnızca gerekliyse kullanın.
- Büyük kaynak kullanımı için `--memory` ve `--cpus` ile sınırlar belirleyin.
- Yeniden başlatma politikaları, üretim ortamlarında kesintisiz çalışma için kritiktir.

---

### b. `docker exec`

**Açıklama**: Çalışan bir konteynerin içinde yeni bir komut çalıştırır. Hata ayıklama, dosya sistemi kontrolü, veritabanı sorguları veya interaktif kabuk açma gibi işlemler için kullanılır. Konteynerin mevcut süreçlerine müdahale etmeden ek komutlar yürütür.

**Yapısı**:
```bash
docker exec [SEÇENEKLER] KONTEYNER KOMUT [ARGÜMANLAR...]
```

**Yaygın Seçenekler**:
- `-it` veya `--interactive --tty`: İnteraktif terminal başlatır (kabuk için gereklidir).
- `-d` veya `--detach`: Komutu arka planda çalıştırır.
- `--user`: Komutu belirli bir kullanıcıyla çalıştırır (ör. `--user root`).
- `--env` veya `-e`: Komut için ortam değişkeni tanımlar (ör. `-e DEBUG=1`).
- `--workdir`: Komutun çalışacağı dizini belirtir (ör. `--workdir /app`).
- `--privileged`: Komuta genişletilmiş yetkiler verir (nadiren kullanılır).

**Kullanım Senaryoları**:
- Konteyner içinde kabuk açarak dosya sistemini veya yapılandırmaları kontrol etmek.
- Veritabanında SQL sorguları çalıştırmak (ör. MySQL, PostgreSQL).
- Log veya hata dosyalarını incelemek.
- Konteyner içinde betik veya yönetim komutları yürütmek.
- Çalışan bir servisin durumunu izlemek (ör. `top`, `ps`).

**Örnekler**:
```bash
docker exec -it my_container /bin/bash
```
- `my_container` içinde interaktif bir bash kabuğu açar.
```bash
docker exec my_container ls -la /app
```
- Konteynerin `/app` dizinindeki dosyaları listeler.
```bash
docker exec -d my_container touch /tmp/test.txt
```
- Konteynerde arka planda bir dosya oluşturur.
```bash
docker exec --user mysql mysql_container mysql -uroot -psecret -e "SHOW DATABASES;"
```
- MySQL konteynerinde bir SQL sorgusu çalıştırır.
```bash
docker exec -e DEBUG=1 my_container python script.py
```
- Ortam değişkeni tanımlayarak bir betik çalıştırır.

**Hata Durumları**:
- **Konteyner çalışmıyor**: Konteyner `running` durumunda değilse hata verir (`docker ps` ile kontrol edin).
- **Komut bulunamadı**: Örneğin, `bash` yoksa `/bin/sh` deneyin veya imajın içeriğini kontrol edin.
- **Erişim reddedildi**: Yanlış kullanıcıyla çalıştırılıyorsa `--user` ile doğru kullanıcıyı belirtin.

**Notlar**:
- Hafif imajlarda (ör. Alpine), `bash` yerine `/bin/sh` bulunabilir.
- `-it` olmadan interaktif komutlar (ör. `bash`, `top`) düzgün çalışmaz.
- Güvenlik için, `--user` ile en düşük yetkili kullanıcıyı tercih edin.
- `docker exec`, konteynerin ana sürecini değiştirmez; yalnızca ek bir süreç başlatır.
- Uzun süren komutlar için `-d` kullanılabilir, ancak dikkatli olun.

---

### c. `docker ps`

**Açıklama**: Çalışan konteynerları listeler. Konteynerlerin ID’si, isimleri, durumları, port eşleşmeleri ve kullanılan imajlar gibi bilgileri gösterir. Sistemdeki konteyner aktivitelerini izlemek için temel bir komuttur.

**Yapısı**:
```bash
docker ps [SEÇENEKLER]
```

**Yaygın Seçenekler**:
- `-a` veya `--all`: Tüm konteynerları (çalışan ve durmuş) listeler.
- `-q` veya `--quiet`: Sadece konteyner ID’lerini gösterir.
- `--format`: Çıktıyı özelleştirir (ör. `--format "{{.ID}} {{.Names}}"`).
- `-n N`: Son N konteyneri listeler.
- `--filter`: Konteynerleri filtreler (ör. `--filter "status=running"`, `--filter "name=my_app"`).
- `-s` veya `--size`: Konteynerlerin disk kullanımını gösterir.
- `-l` veya `--latest`: En son oluşturulan konteyneri gösterir.

**Kullanım Senaryoları**:
- Çalışan konteynerleri ve port eşleşmelerini kontrol etmek.
- Durmuş konteynerleri temizlemeden önce listelemek.
- Otomasyon betiklerinde konteyner ID’lerini toplamak.
- Port çakışmalarını veya kaynak kullanımını izlemek.
- Hata ayıklama için konteyner durumlarını analiz etmek.

**Örnekler**:
```bash
docker ps
```
- Şu anda çalışan konteynerları listeler.
```bash
docker ps -a
```
- Tüm konteynerları (durmuş olanlar dahil) listeler.
```bash
docker ps -q
```
- Sadece çalışan konteyner ID’lerini gösterir.
```bash
docker ps --format "table {{.ID}}\t{{.Names}}\t{{.Ports}}\t{{.Status}}"
```
- Konteyner ID’si, isim, portlar ve durumu tablo formatında gösterir.
```bash
docker ps -a --filter "status=exited" --filter "name=my_app"
```
- `my_app` adında ve durmuş olan konteynerleri listeler.
```bash
docker ps -s
```
- Konteynerlerin disk kullanımını gösterir.

**Hata Durumları**:
- **Boş çıktı**: Çalışan konteyner yoksa, `docker ps` boş döner; `-a` ile durmuş konteynerleri kontrol edin.
- **Filtre hatası**: Yanlış filtre sözdizimi hata verebilir (ör. `--filter "status=invalid"`).

**Notlar**:
- Çıktı, konteyner ID’si, imaj, komut, durum, portlar ve isim gibi bilgileri içerir.
- `-q` ile alınan ID’ler, diğer komutlarla (ör. `docker stop`, `docker rm`) kullanılabilir.
- Büyük sistemlerde `--filter` ile hedefli listeleme yapmak performans kazandırır.
- Konteyner durumu: `running`, `exited`, `paused`, `restarting` vb. olabilir.

---

### d. `docker stop`

**Açıklama**: Çalışan bir konteyneri durdurur. Konteynere bir SIGTERM sinyali gönderir ve düzgün bir şekilde kapanmasını sağlar. Konteyner durur, ancak silinmez ve yeniden başlatılabilir.

**Yapısı**:
```bash
docker stop [SEÇENEKLER] KONTEYNER [KONTEYNER...]
```

**Yaygın Seçenekler**:
- `-t` veya `--time`: Konteynerin durması için beklenecek süreyi saniye cinsinden belirler (varsayılan: 10 saniye). Süre dolarsa SIGKILL gönderilir.

**Kullanım Senaryoları**:
- Konteynerleri geçici olarak durdurmak (ör. bakım veya test sırasında).
- Sistem kaynaklarını serbest bırakmak.
- Konteyner silmeden önce durdurmak (gerektiğinde).
- Üretim ortamında servis güncellemeleri öncesi durdurma.

**Örnekler**:
```bash
docker stop my_container
```
- `my_container`’ı durdurur.
```bash
docker stop $(docker ps -q)
```
- Tüm çalışan konteynerları durdurur.
```bash
docker stop -t 5 my_container
```
- `my_container`’ı 5 saniye içinde durdurur, aksi takdirde zorla kapatır.

**Hata Durumları**:
- **Konteyner çalışmıyor**: Zaten durmuş bir konteyner için hata vermez, ancak işlem yapmaz.
- **Zaman aşımı**: Konteyner SIGTERM’e yanıt vermezse, `-t` süresi sonunda SIGKILL gönderilir.

**Notlar**:
- Durdurulan konteynerler `docker ps -a` ile listelenir ve `docker start` ile yeniden başlatılabilir.
- SIGTERM, konteynerin düzgün kapanmasını sağlar; ancak bazı uygulamalar bunu desteklemeyebilir.
- Çalışan bir konteyneri silmek için önce durdurulmalı veya `docker rm -f` kullanılmalıdır.
- Üretimde, ani kesintileri önlemek için `-t` ile uygun bir süre belirleyin.

---

### e. `docker start`

**Açıklama**: Durdurulmuş bir konteyneri yeniden başlatır. Konteynerin mevcut yapılandırmasını (portlar, hacimler, ortam değişkenleri) korur ve yeniden çalıştırır.

**Yapısı**:
```bash
docker start [SEÇENEKLER] KONTEYNER [KONTEYNER...]
```

**Yaygın Seçenekler**:
- `-i` veya `--interactive`: İnteraktif modda başlatır (konteyner çıktısını terminale bağlar).
- `-a` veya `--attach`: Konteynerin standart çıktı ve hata akışlarını terminale bağlar.

**Kullanım Senaryoları**:
- Daha önce durdurulmuş bir konteyneri yeniden çalıştırmak.
- Test veya bakım sonrası konteynerleri geri getirmek.
- Üretimde servisleri yeniden başlatmak.

**Örnekler**:
```bash
docker start my_container
```
- `my_container`’ı yeniden başlatır.
```bash
docker start -i my_container
```
- Konteyneri interaktif modda başlatır.
```bash
docker start $(docker ps -aq --filter "status=exited")
```
- Tüm durmuş konteynerları başlatır.

**Hata Durumları**:
- **Konteyner mevcut değil**: Konteyner silinmişse hata verir (`docker ps -a` ile kontrol edin).
- **Zaten çalışıyor**: Çalışan bir konteyner için hata vermez, ancak işlem yapmaz.

**Notlar**:
- `docker start`, konteyneri sıfırdan oluşturmaz; mevcut konteyneri kullanır.
- İnteraktif konteynerler için `-i` veya `-a` gerekebilir.
- Konteynerin önceki yapılandırması (ör. portlar, hacimler) korunur.

---

### f. `docker restart`

**Açıklama**: Çalışan bir konteyneri durdurur ve yeniden başlatır. `docker stop` + `docker start` kombinasyonuna eşdeğerdir. Servis kesintisini en aza indirmek veya yapılandırmayı yenilemek için kullanılır.

**Yapısı**:
```bash
docker restart [SEÇENEKLER] KONTEYNER [KONTEYNER...]
```

**Yaygın Seçenekler**:
- `-t` veya `--time`: Durdurma için beklenecek süreyi saniye cinsinden belirler (varsayılan: 10 saniye).

**Kullanım Senaryoları**:
- Konteynerin yapılandırmasını veya durumunu yenilemek.
- Hafıza veya performans sorunlarını çözmek.
- Servis güncellemeleri veya yapılandırma değişiklikleri sonrası yeniden başlatma.

**Örnekler**:
```bash
docker restart my_container
```
- `my_container`’ı yeniden başlatır.
```bash
docker restart -t 5 my_container
```
- 5 saniye içinde durdurup yeniden başlatır.
```bash
docker restart $(docker ps -q)
```
- Tüm çalışan konteynerları yeniden başlatır.

**Hata Durumları**:
- **Konteyner çalışmıyor**: Konteyner durmuşsa yalnızca başlatılır.
- **Zaman aşımı**: Konteyner SIGTERM’e yanıt vermezse, `-t` süresi sonunda SIGKILL gönderilir.

**Notlar**:
- Yeniden başlatma, konteynerin durumunu (hacimler, ağlar) korur.
- Üretimde, kesintileri en aza indirmek için `-t` ile uygun bir süre belirleyin.
- Sürekli yeniden başlatma döngüleri, `--restart` politikasıyla önlenebilir.

---

### g. `docker rm`

**Açıklama**: Bir veya daha fazla konteyneri siler. Durmuş konteynerlerin temizlenmesi için kullanılır. Çalışan konteynerler silinmeden önce durdurulmalıdır, aksi takdirde `-f` gereklidir.

**Yapısı**:
```bash
docker rm [SEÇENEKLER] KONTEYNER [KONTEYNER...]
```

**Yaygın Seçenekler**:
- `-f` veya `--force`: Çalışan konteyneri zorla siler (önce SIGKILL gönderir).
- `-v` veya `--volumes`: Konteynerle ilişkili anonim hacimleri siler.
- `-l` veya `--link`: Konteyner bağlantılarını kaldırır (eski bir özellik, nadiren kullanılır).

**Kullanım Senaryoları**:
- Artık kullanılmayan konteynerleri temizlemek.
- Disk alanını serbest bırakmak.
- Test veya geçici konteynerleri kaldırmak.
- Yeni bir konteyner başlatmadan önce eski sürümü silmek.

**Örnekler**:
```bash
docker rm my_container
```
- `my_container`’ı siler.
```bash
docker rm -f $(docker ps -aq)
```
- Tüm konteynerları (çalışan ve durmuş) zorla siler.
```bash
docker rm -v my_container
```
- `my_container`’ı ve ilişkili anonim hacimleri siler.
```bash
docker rm $(docker ps -aq --filter "status=exited")
```
- Tüm durmuş konteynerları siler.

**Hata Durumları**:
- **Konteyner çalışıyor**: Çalışan bir konteyner için `-f` kullanılmazsa hata verir.
- **Konteyner mevcut değil**: Silinmiş bir konteyner için hata verir.

**Notlar**:
- Hacimlerin silinmesi (`-v`) veri kaybına neden olabilir; dikkatli olun.
- `docker system prune` ile birlikte kullanılarak kapsamlı temizlik yapılabilir.
- Silme işlemi geri alınamaz; önemli konteynerleri yedekleyin (ör. `docker commit`).
- Compose projelerinde, `docker-compose rm` tercih edilir.

---

### h. `docker logs`

**Açıklama**: Bir konteynerin standart çıktı (STDOUT) ve hata (STDERR) loglarını gösterir. Hata ayıklama, sorun giderme ve konteyner aktivitelerini izlemek için kullanılır. Loglar, konteynerin çalıştığı süre boyunca birikir.

**Yapısı**:
```bash
docker logs [SEÇENEKLER] KONTEYNER
```

**Yaygın Seçenekler**:
- `-f` veya `--follow`: Logları gerçek zamanlı takip eder.
- `--tail N`: Son N satır logu gösterir.
- `-t` veya `--timestamps`: Loglara zaman damgaları ekler.
- `--since`: Belirli bir zamandan sonraki logları gösterir (ör. `--since "2023-10-01"`).
- `--until`: Belirli bir zamana kadarki logları gösterir (ör. `--until "2023-10-02"`).
- `--details`: Ek log detaylarını gösterir (nadiren kullanılır).

**Kullanım Senaryoları**:
- Uygulama hatalarını teşhis etmek (ör. hata mesajları).
- Servis başlatma loglarını kontrol etmek (ör. web sunucusu, veritabanı).
- Gerçek zamanlı izleme (ör. API istekleri, veritabanı işlemleri).
- CI/CD pipeline’larında log toplamak.

**Örnekler**:
```bash
docker logs my_container
```
- `my_container`’ın tüm loglarını gösterir.
```bash
docker logs -f my_container
```
- Logları canlı olarak takip eder.
```bash
docker logs --tail 10 -t my_container
```
- Son 10 satır logu zaman damgalarıyla gösterir.
```bash
docker logs --since "2023-10-01T10:00:00" my_container
```
- 1 Ekim 2023 sabah 10’dan sonraki logları gösterir.

**Hata Durumları**:
- **Konteyner mevcut değil**: Konteyner silinmişse hata verir.
- **Log bulunamadı**: Konteyner log üretmiyorsa (ör. sessiz bir süreç) boş döner.

**Notlar**:
- Loglar, disk alanı tüketebilir; düzenli temizlik için `docker system prune` veya harici log yönetimi (ör. ELK Stack) kullanılabilir.
- `-f` ile canlı izleme, terminali bağlar; CTRL+C ile çıkılır.
- Zaman damgaları (`-t`), kronolojik analiz için faydalıdır.
- Compose projelerinde, `docker-compose logs` birden fazla servisin loglarını birleştirir.

---

### i. `docker inspect`

**Açıklama**: Bir konteyner, imaj, ağ veya hacmin detaylı bilgilerini JSON formatında gösterir. Yapılandırma, ağ ayarları, ortam değişkenleri, hacim bağlamaları ve diğer meta verileri analiz etmek için kullanılır.

**Yapısı**:
```bash
docker inspect [SEÇENEKLER] NESNE [NESNE...]
```

**Yaygın Seçenekler**:
- `-f` veya `--format`: Çıktıyı özelleştirir (ör. `-f '{{.NetworkSettings.IPAddress}}'`).
- `--type`: Nesne türünü belirtir (ör. `--type container`, `--type image`).
- `-s` veya `--size`: Konteyner için disk kullanımını gösterir.

**Kullanım Senaryoları**:
- Konteynerin IP adresini, port eşleşmelerini veya ortam değişkenlerini öğrenmek.
- Hacim bağlamalarını veya ağ yapılandırmasını kontrol etmek.
- Hata ayıklama için konteyner veya imaj meta verilerini incelemek.
- Otomasyon betiklerinde belirli bilgileri almak.

**Örnekler**:
```bash
docker inspect my_container
```
- `my_container`’ın tüm yapılandırma detaylarını (JSON) gösterir.
```bash
docker inspect -f '{{.NetworkSettings.IPAddress}}' my_container
```
- Konteynerin IP adresini gösterir.
```bash
docker inspect --type image nginx:latest
```
- `nginx:latest` imajının detaylarını gösterir.
```bash
docker inspect -f '{{.Mounts}}' my_container
```
- Konteynerin hacim bağlamalarını listeler.

**Hata Durumları**:
- **Nesne mevcut değil**: Konteyner, imaj veya ağ bulunamazsa hata verir.
- **Hatalı format**: `-f` ile yanlış bir alan belirtilirse hata verir (JSON yapısını kontrol edin).

**Notlar**:
- Çıktı JSON formatındadır ve oldukça ayrıntılıdır.
- `-f` ile belirli alanlar (ör. IP, durum, hacimler) kolayca alınabilir.
- Otomasyonlarda sıkça kullanılır (ör. IP adresini bir betiğe aktarmak).
- Büyük sistemlerde, hedefli veri almak için `-f` önerilir.

---

### j. `docker cp`

**Açıklama**: Host ile konteyner arasında dosya veya dizin kopyalar. Konteynere dosya aktarmak veya konteynerden dosya almak için kullanılır. Hem çalışan hem de durmuş konteynerlerle çalışır.

**Yapısı**:
```bash
docker cp [SEÇENEKLER] KAYNAK HEDEF
```

**Yaygın Seçenekler**:
- `--archive`: Dosyaların meta verilerini (izinler, zaman damgaları) korur.
- `--follow-link`: Sembolik bağlantıları takip eder.
- `-L`: Sembolik bağlantıları çözer.

**Kullanım Senaryoları**:
- Konteynere yapılandırma dosyası veya betik kopyalamak.
- Konteynerden log, veritabanı yedekleri veya hata dosyalarını almak.
- Hata ayıklama için dosya transferi.
- Geliştirme sırasında hızlı dosya güncellemeleri.

**Örnekler**:
```bash
docker cp my_file.txt my_container:/app/
```
- `my_file.txt` dosyasını konteynerin `/app/` dizinine kopyalar.
```bash
docker cp my_container:/app/log.txt ./log.txt
```
- Konteynerin `/app/log.txt` dosyasını host’a kopyalar.
```bash
docker cp -a ./config/ my_container:/etc/config/
```
- `config` dizinini meta verileriyle kopyalar.
```bash
docker cp my_container:/data/backup.sql backup.sql
```
- Konteynerden bir SQL yedeği alır.

**Hata Durumları**:
- **Hedef yol mevcut değil**: Konteynerde hedef dizin yoksa hata verir; önce dizini oluşturun.
- **İzin reddedildi**: Konteyner veya host’ta izin yoksa, `--user` veya dosya izinlerini kontrol edin.
- **Konteyner mevcut değil**: Konteyner silinmişse hata verir.

**Notlar**:
- Büyük dosya transferleri için hacimler veya bind mount’lar daha verimlidir.
- Konteyner çalışmasa bile `docker cp` çalışır.
- Sembolik bağlantılarla çalışırken `-L` veya `--follow-link` dikkatli kullanılmalıdır.
- Güvenlik için, kopyalanan dosyaların izinlerini kontrol edin.

---

### k. `docker commit`

**Açıklama**: Çalışan bir konteynerin mevcut durumundan yeni bir imaj oluşturur. Konteynerde yapılan değişiklikleri (yeni paketler, dosyalar, yapılandırmalar) kalıcı bir imaja kaydetmek için kullanılır. Ancak, üretimde genellikle `Dockerfile` tercih edilir.

**Yapısı**:
```bash
docker commit [SEÇENEKLER] KONTEYNER [DEPO[:ETİKET]]
```

**Yaygın Seçenekler**:
- `-a` veya `--author`: İmajın yazarını belirtir (ör. `-a "John Doe <john@example.com>"`).
- `-m` veya `--message`: İmaj için commit mesajı ekler (ör. `-m "Added custom config"`).
- `-p` veya `--pause`: Commit sırasında konteyneri duraklatır (varsayılan: true).
- `-c` veya `--change`: İmaj oluştururken Dockerfile talimatları uygular (ör. `-c "ENV DEBUG=true"`).

**Kullanım Senaryoları**:
- Konteynerde yapılan değişiklikleri hızlıca bir imaja kaydetmek.
- Hızlı prototip veya test imajları oluşturmak.
- Mevcut bir konteynerin özelleştirilmiş bir kopyasını üretmek.
- Geliştirme sırasında geçici imajlar oluşturmak.

**Örnekler**:
```bash
docker commit my_container my_new_image:1.0
```
- `my_container`’dan `my_new_image:1.0` adında bir imaj oluşturur.
```bash
docker commit -m "Added custom config" -a "Dev Team" my_container custom_image:latest
```
- Commit mesajı ve yazar bilgisiyle imaj oluşturur.
```bash
docker commit -c "ENV APP_ENV=prod" my_container prod_image:1.0
```
- Ortam değişkeni ekleyerek imaj oluşturur.

**Hata Durumları**:
- **Konteyner mevcut değil**: Konteyner silinmişse hata verir.
- **Hatalı talimat**: `-c` ile geçersiz bir Dockerfile talimatı belirtilirse hata verir.

**Notlar**:
- Üretimde `Dockerfile` kullanımı daha izlenebilir ve tekrarlanabilir; `docker commit` nadiren tercih edilir.
- Oluşturulan imaj, `docker images` ile listelenir.
- Değişikliklerin detaylarını görmek için `docker diff` kullanılabilir.
- Büyük değişiklikler imaj boyutunu artırabilir; optimize edilmiş imajlar için dikkatli olun.

---

### l. `docker pause`

**Açıklama**: Çalışan bir konteynerin tüm süreçlerini duraklatır (CGROUP freezer ile). Konteyner geçici olarak askıya alınır, ancak durdurulmaz ve kaynakları serbest bırakmaz.

**Yapısı**:
```bash
docker pause [SEÇENEKLER] KONTEYNER [KONTEYNER...]
```

**Kullanım Senaryoları**:
- Konteyneri geçici olarak askıya almak (ör. bakım veya hata ayıklama sırasında).
- CPU ve bellek kullanımını anlık olarak durdurmak.
- Konteyner süreçlerini analiz etmek.

**Örnekler**:
```bash
docker pause my_container
```
- `my_container`’ı duraklatır.
```bash
docker pause $(docker ps -q)
```
- Tüm çalışan konteynerları duraklatır.

**Hata Durumları**:
- **Konteyner çalışmıyor**: Konteyner `running` durumunda değilse hata verir.
- **Zaten duraklatılmış**: Duraklatılmış bir konteyner için hata vermez, ancak işlem yapmaz.

**Notlar**:
- Duraklatılan konteynerler `docker ps`’de `paused` durumunda görünür.
- `docker unpause` ile konteyner devam ettirilir.
- Duraklatma, kaynakları serbest bırakmaz; yalnızca süreçleri askıya alır.
- Nadiren kullanılan bir komut; genellikle `docker stop` tercih edilir.

---

### m. `docker unpause`

**Açıklama**: Duraklatılmış bir konteynerin süreçlerini devam ettirir. `docker pause` ile askıya alınan konteynerleri yeniden çalışır hale getirir.

**Yapısı**:
```bash
docker unpause [SEÇENEKLER] KONTEYNER [KONTEYNER...]
```

**Kullanım Senaryoları**:
- Duraklatılmış konteynerleri yeniden başlatmak.
- Bakım veya analiz sonrası konteyneri devam ettirmek.

**Örnekler**:
```bash
docker unpause my_container
```
- `my_container`’ı devam ettirir.
```bash
docker unpause $(docker ps -q --filter "status=paused")
```
- Tüm duraklatılmış konteynerları devam ettirir.

**Hata Durumları**:
- **Konteyner duraklatılmamış**: Duraklatılmamış bir konteyner için hata vermez, ancak işlem yapmaz.
- **Konteyner mevcut değil**: Konteyner silinmişse hata verir.

**Notlar**:
- Konteyner, duraklatıldığı durumdan devam eder; yapılandırma değişmez.
- `docker ps`’de durum `running` olarak güncellenir.
- Nadiren kullanılan bir komut; genellikle `docker start` ile karıştırılmaz.

---

## 2. İmaj Komutları

İmajlar, konteynerlerin oluşturulduğu şablonlardır. Aşağıdaki komutlar, imajların oluşturulması, yönetilmesi, paylaşılması ve dışa/içe aktarılması için kullanılır.

### a. `docker build`

**Açıklama**: Bir `Dockerfile`’dan yeni bir imaj oluşturur. Uygulama kodunu, bağımlılıkları ve yapılandırmaları paketlemek için kullanılır. İmaj oluşturma süreci, Dockerfile’daki talimatları adım adım yürütür ve katmanlı bir yapı oluşturur.

**Yapısı**:
```bash
docker build [SEÇENEKLER] YOL | URL
```

**Yaygın Seçenekler**:
- `-t` veya `--tag`: İmaj için isim ve etiket belirler (ör. `-t my_app:1.0`).
- `-f` veya `--file`: Dockerfile’ın yolunu belirtir (varsayılan: `./Dockerfile`).
- `--no-cache`: Önbellek kullanmadan inşa eder (eski bağımlılıkları önler).
- `--build-arg`: Yapım sırasında değişken tanımlar (ör. `--build-arg VERSION=1.0`).
- `--target`: Çok aşamalı yapılandırmalarda hedef aşamayı belirtir (ör. `--target production`).
- `--quiet` veya `-q`: Sadece imaj ID’sini gösterir.
- `--pull`: İmajın temel imajlarını her zaman registry’den çeker.
- `--compress`: Yapım bağlamını sıkıştırır (ağ üzerinden gönderim için).
- `--label`: İmaj için meta veri etiketleri ekler (ör. `--label version=1.0`).

**Kullanım Senaryoları**:
- Uygulama imajları oluşturmak (ör. Node.js, Python, Java uygulamaları).
- Özelleştirilmiş imajlar üretmek (ör. ek paketler, yapılandırmalar).
- CI/CD pipeline’larında otomatik imaj oluşturma.
- Çok aşamalı yapılarla optimize edilmiş imajlar oluşturma.

**Örnekler**:
```bash
docker build -t my_app:1.0 .
```
- Geçerli dizindeki `Dockerfile`’dan `my_app:1.0` imajını oluşturur.
```bash
docker build -t my_app:dev --no-cache -f Dockerfile.dev .
```
- Özel bir Dockerfile kullanarak önbelleksiz bir imaj oluşturur.
```bash
docker build -t my_app:prod --build-arg ENV=production --target prod .
```
- Çok aşamalı bir yapıda `prod` aşamasını hedefler ve ortam değişkeni tanımlar.
```bash
docker build -t my_app:latest --pull .
```
- Temel imajları güncelleyerek imaj oluşturur.

**Hata Durumları**:
- **Dockerfile hatası**: Sözdizimi veya komut hataları yapımı durdurur; hata mesajını inceleyin.
- **Bağlam bulunamadı**: Belirtilen dizin veya URL geçersizse hata verir.
- **Disk alanı yetersiz**: Büyük imajlar için yeterli alan olmalıdır.

**Notlar**:
- `.` (nokta), Dockerfile’ın bulunduğu dizini ifade eder; uzak bir Git deposu da kullanılabilir (ör. `github.com/user/repo`).
- Önbellek, yapım süresini hızlandırır; `--no-cache` ile eski bağımlılıklar önlenir.
- Çok aşamalı yapılar (multi-stage builds), imaj boyutunu küçültmek için idealdir.
- Büyük imajlar disk alanı tüketebilir; optimize edilmiş imajlar için hafif temel imajlar (ör. `alpine`) kullanın.
- `docker-compose build` ile entegre çalışabilir.

---

### b. `docker images`

**Açıklama**: Yerel sistemdeki Docker imajlarını listeler. İmaj isimleri, etiketler, ID’ler, oluşturulma tarihleri ve boyutları gibi bilgileri gösterir.

**Yapısı**:
```bash
docker images [SEÇENEKLER] [DEPO[:ETİKET]]
```

**Yaygın Seçenekler**:
- `-a` veya `--all`: Tüm imajları (ara katmanlar dahil) listeler.
- `-q` veya `--quiet`: Sadece imaj ID’lerini gösterir.
- `--filter`: İmajları filtreler (ör. `--filter "dangling=true"`, `--filter "reference=nginx"`).
- `--format`: Çıktıyı özelleştirir (ör. `--format "{{.Repository}}:{{.Tag}}"`).
- `--no-trunc`: Kısaltılmış ID’leri tam olarak gösterir.

**Kullanım Senaryoları**:
- Yerel imajları kontrol etmek ve yönetmek.
- Kullanılmayan (dangling) imajları temizlemeden önce listelemek.
- Otomasyon betiklerinde imaj ID’lerini toplamak.
- İmaj boyutlarını analiz etmek.

**Örnekler**:
```bash
docker images
```
- Tüm imajları listeler.
```bash
docker images nginx
```
- Sadece `nginx` imajlarını listeler.
```bash
docker images -q --filter dangling=true
```
- Kullanılmayan (dangling) imajların ID’lerini listeler.
```bash
docker images --format "table {{.Repository}}\t{{.Tag}}\t{{.Size}}"
```
- İmajları tablo formatında (depo, etiket, boyut) gösterir.
```bash
docker images --no-trunc
```
- Tam imaj ID’lerini gösterir.

**Hata Durumları**:
- **Filtre hatası**: Yanlış filtre sözdizimi hata verebilir (ör. `--filter "invalid=value"`).
- **Boş çıktı**: Yerel sistemde imaj yoksa boş döner.

**Notlar**:
- Dangling imajlar, etiketi olmayan ve kullanılmayan imajlardır (ör. eski yapılar).
- `-q` ile alınan ID’ler, `docker rmi` gibi komutlarla kullanılabilir.
- Büyük imajlar disk alanını tüketebilir; düzenli temizlik için `docker system prune` kullanın.
- Aynı imajın birden fazla etiketi olabilir; bu, disk alanını artırmaz.

---

### c. `docker pull`

**Açıklama**: Bir imajı Docker Hub veya başka bir registry’den yerel sisteme indirir. Konteyner çalıştırmadan önce gerekli imajları almak için kullanılır.

**Yapısı**:
```bash
docker pull [SEÇENEKLER] İMAJ[:ETİKET]
```

**Yaygın Seçenekler**:
- `--platform`: Belirli bir platform için imaj çeker (ör. `linux/arm64`, `linux/amd64`).
- `--all-tags`: Depodaki tüm etiketleri indirir.
- `--disable-content-trust`: İçerik güvenliğini devre dışı bırakır.

**Kullanım Senaryoları**:
- Resmi imajları indirmek (ör. `nginx`, `mysql`, `redis`).
- Özel registry’lerden imaj çekmek (ör. AWS ECR, GitLab Registry).
- CI/CD pipeline’larında imajları güncellemek.
- Çevrimdışı kullanım için imajları önceden indirmek.

**Örnekler**:
```bash
docker pull nginx:latest
```
- Docker Hub’dan `nginx` imajının `latest` etiketli sürümünü indirir.
```bash
docker pull myuser/my_app:1.0
```
- Özel bir registry’den imaj indirir.
```bash
docker pull --platform linux/arm64 redis:latest
```
- ARM64 platformu için `redis` imajını indirir.
```bash
docker pull --all-tags nginx
```
- `nginx` deposundaki tüm etiketleri indirir.

**Hata Durumları**:
- **İmaj bulunamadı**: Registry’de imaj veya etiket yoksa hata verir.
- **Erişim reddedildi**: Özel registry’ler için `docker login` gerekebilir.
- **Ağ hatası**: İnternet bağlantısı yoksa hata verir.

**Notlar**:
- İmaj zaten yerel sistemde varsa, yalnızca güncellenmesi gereken katmanlar indirilir.
- `--platform` ile platform uyumluluğu sağlanır (ör. M1 Mac için ARM imajları).
- Büyük imajlar indirme süresini artırabilir; hızlı bir bağlantı önerilir.
- `docker run`, imajı otomatik çeker; ancak `docker pull` önceden indirme için kullanışlıdır.

---

### d. `docker push`

**Açıklama**: Yerel bir imajı bir registry’ye (ör. Docker Hub, AWS ECR, GitLab Registry) yükler. İmajları paylaşmak veya dağıtmak için kullanılır.

**Yapısı**:
```bash
docker push [SEÇENEKLER] İMAJ[:ETİKET]
```

**Yaygın Seçenekler**:
- `--disable-content-trust`: İçerik güvenliğini devre dışı bırakır.

**Kullanım Senaryoları**:
- CI/CD pipeline’larında imajları registry’ye yüklemek.
- Ekip üyeleriyle veya diğer sistemlerle imaj paylaşmak.
- Üretim ortamına imaj dağıtmak.
- Açık kaynak projeler için imajları Docker Hub’a yüklemek.

**Örnekler**:
```bash
docker push myuser/my_app:1.0
```
- `myuser/my_app:1.0` imajını Docker Hub’a yükler.
```bash
docker push myregistry.example.com/my_app:latest
```
- Özel bir registry’ye imaj yükler.

**Hata Durumları**:
- **Giriş yapılmadı**: `docker login` olmadan hata verir.
- **İzin reddedildi**: Registry’de yazma izni yoksa hata verir.
- **İmaj mevcut değil**: Yerel sistemde imaj yoksa hata verir.

**Notlar**:
- Önce `docker login` ile registry’ye giriş yapılmalıdır.
- İmajın etiketi registry formatına uygun olmalıdır (ör. `kullanıcı/depo:etiket`).
- Büyük imajlar yükleme süresini artırabilir; optimize edilmiş imajlar kullanın.
- Güvenlik için, imajların içeriği kontrol edilmeli ve güvenilir registry’ler tercih edilmelidir.

---

### e. `docker rmi`

**Açıklama**: Bir veya daha fazla imajı yerel sistemden siler. Kullanılmayan veya eski imajları temizleyerek disk alanını serbest bırakmak için kullanılır.

**Yapısı**:
```bash
docker rmi [SEÇENEKLER] İMAJ [İMAJ...]
```

**Yaygın Seçenekler**:
- `-f` veya `--force`: İmajı zorla siler (bağlı konteynerler olsa bile).
- `--no-prune`: İlişkili ara katmanları silmez.

**Kullanım Senaryoları**:
- Kullanılmayan veya dangling imajları temizlemek.
- Eski imaj sürümlerini kaldırmak.
- Disk alanını optimize etmek.
- Yeni imajlar oluşturmadan önce eski imajları silmek.

**Örnekler**:
```bash
docker rmi nginx:latest
```
- `nginx:latest` imajını siler.
```bash
docker rmi -f $(docker images -q)
```
- Tüm imajları siler (tekrar eden ID’ler hariç).
```bash
docker rmi $(docker images -q --filter dangling=true)
```
- Kullanılmayan (dangling) imajları siler.
```bash
docker rmi my_app:1.0 my_app:2.0
```
- Belirli imajları siler.

**Hata Durumları**:
- **İmaj kullanımda**: İmaj bir konteynere bağlıysa silinemez; önce konteyneri silin veya `-f` kullanın.
- **İmaj mevcut değil**: İmaj zaten silinmişse hata verir.

**Notlar**:
- Dangling imajlar, etiketi olmayan ve kullanılmayan imajlardır; `docker images --filter dangling=true` ile listelenir.
- `docker system prune` ile birlikte kullanılarak kapsamlı temizlik yapılabilir.
- Silme işlemi geri alınamaz; önemli imajları `docker save` ile yedekleyin.
- Aynı imajın birden fazla etiketi varsa, sadece belirtilen etiket silinir.

---

### f. `docker tag`

**Açıklama**: Bir imaja yeni bir etiket ekler veya mevcut bir imajı yeniden adlandırır. İmajları registry’ye yüklemeden önce uygun formata getirmek veya sürümleri organize etmek için kullanılır.

**Yapısı**:
```bash
docker tag KAYNAK_İMAJ[:ETİKET] HEDEF_İMAJ[:ETİKET]
```

**Kullanım Senaryoları**:
- İmajları registry formatına uygun hale getirmek (ör. `kullanıcı/depo:etiket`).
- Aynı imaj için birden fazla etiket oluşturmak (ör. `latest` ve `1.0`).
- İmaj sürümlerini yönetmek.
- Eski bir imajı yeni bir isimle yeniden kullanmak.

**Örnekler**:
```bash
docker tag my_app:1.0 myuser/my_app:1.0
```
- `my_app:1.0` imajını `myuser/my_app:1.0` olarak etiketler.
```bash
docker tag nginx:latest nginx:backup
```
- `nginx:latest` imajına `backup` etiketi ekler.
```bash
docker tag my_app:1.0 my_app:latest
```
- `my_app:1.0`’ı `latest` olarak etiketler.

**Hata Durumları**:
- **Kaynak imaj mevcut değil**: Yerel sistemde imaj yoksa hata verir.
- **Geçersiz etiket**: Registry formatına uymayan etiketler sorun çıkarabilir (ör. `docker push` sırasında).

**Notlar**:
- Etiketleme, imajın kopyasını oluşturmaz; sadece yeni bir referans ekler.
- `docker push` öncesi etiketleme sıkça kullanılır.
- Yanlış etiketler karmaşaya neden olabilir; etiketleme stratejisi belirleyin.
- Aynı imajın birden fazla etiketi, `docker images`’da ayrı satırlar olarak görünür.

---

### g. `docker save`

**Açıklama**: Bir veya daha fazla imajı bir tar arşivine dışa aktarır. İmajları çevrimdışı paylaşmak, yedeklemek veya farklı sistemlere taşımak için kullanılır.

**Yapısı**:
```bash
docker save [SEÇENEKLER] -o DOSYA İMAJ [İMAJ...]
```

**Yaygın Seçenekler**:
- `-o` veya `--output`: Çıktı dosyasını belirtir.

**Kullanım Senaryoları**:
- İmajları internet bağlantısı olmayan bir sisteme aktarmak.
- İmaj yedekleri oluşturmak.
- CI/CD pipeline’larında imajları arşivlemek.
- İmajları farklı Docker sürümlerine taşımak.

**Örnekler**:
```bash
docker save -o my_app.tar my_app:1.0
```
- `my_app:1.0` imajını `my_app.tar` dosyasına kaydeder.
```bash
docker save -o all_images.tar nginx redis
```
- Birden fazla imajı tek bir arşive kaydeder.
```bash
docker save my_app:1.0 > my_app.tar
```
- Çıktıyı boru hattıyla bir dosyaya yönlendirir.

**Hata Durumları**:
- **İmaj mevcut değil**: Yerel sistemde imaj yoksa hata verir.
- **Disk alanı yetersiz**: Büyük imajlar için yeterli alan olmalıdır.

**Notlar**:
- Oluşturulan tar dosyası büyük olabilir; sıkıştırma (ör. `gzip`) önerilir.
- İmajlar, `docker load` ile geri yüklenebilir.
- Registry erişimi olmayan durumlarda veya yedekleme için kullanışlıdır.
- Aynı imajın birden fazla etiketi varsa, tüm etiketler dışa aktarılır.

---

### h. `docker load`

**Açıklama**: Bir tar arşivinden imajları yerel sisteme yükler. `docker save` ile dışa aktarılan imajları geri yüklemek için kullanılır.

**Yapısı**:
```bash
docker load [SEÇENEKLER] -i DOSYA
```

**Yaygın Seçenekler**:
- `-i` veya `--input`: Giriş dosyasını belirtir.
- `-q` veya `--quiet`: Yükleme sırasında minimum çıktı üretir.

**Kullanım Senaryoları**:
- Çevrimdışı imajları sisteme yüklemek.
- Yedeklenmiş imajları geri getirmek.
- İmajları farklı makineler arasında taşımak.
- Registry erişimi olmayan ortamlarda imaj dağıtımı.

**Örnekler**:
```bash
docker load -i my_app.tar
```
- `my_app.tar` dosyasından imajları yükler.
```bash
cat my_app.tar | docker load
```
- Tar dosyasını boru hattıyla yükler.
```bash
docker load -q -i all_images.tar
```
- Minimum çıktı ile imajları yükler.

**Hata Durumları**:
- **Geçersiz dosya**: Tar dosyası bozuk veya yanlış formatta ise hata verir.
- **Disk alanı yetersiz**: Yükleme için yeterli alan olmalıdır.

**Notlar**:
- Yüklenen imajlar, `docker images` ile listelenir.
- Dosyanın doğru formatta (tar) olduğundan emin olun.
- Büyük imajlar yükleme süresini artırabilir; hızlı bir disk önerilir.
- Aynı imajın mevcut olması durumunda üzerine yazılmaz; yeni bir ID oluşturulabilir.

---

### i. `docker history`

**Açıklama**: Bir imajın oluşturma geçmişini gösterir. İmajın katmanlarını, her katmanın komutlarını ve boyutlarını listeler. İmajın nasıl oluşturulduğunu anlamak için kullanılır.

**Yapısı**:
```bash
docker history [SEÇENEKLER] İMAJ[:ETİKET]
```

**Yaygın Seçenekler**:
- `--no-trunc`: Kısaltılmış bilgileri tam olarak gösterir.
- `-H` veya `--human-readable`: Boyutları insan-okunabilir formatta gösterir.
- `--format`: Çıktıyı özelleştirir.
- `-q` veya `--quiet`: Sadece katman ID’lerini gösterir.

**Kullanım Senaryoları**:
- İmajın hangi komutlarla oluşturulduğunu analiz etmek.
- İmaj boyutunu optimize etmek için gereksiz katmanları tespit etmek.
- Güvenlik denetimi için imaj içeriğini incelemek.
- Hata ayıklama veya imaj oluşturma sürecini anlamak.

**Örnekler**:
```bash
docker history nginx:latest
```
- `nginx:latest` imajının geçmişini gösterir.
```bash
docker history --no-trunc my_app:1.0
```
- Tam komutlarla geçmişini listeler.
```bash
docker history -H my_app:1.0
```
- Boyutları insan-okunabilir formatta gösterir.

**Hata Durumları**:
- **İmaj mevcut değil**: Yerel sistemde imaj yoksa hata verir.
- **Geçersiz format**: `--format` ile yanlış bir alan belirtilirse hata verir.

**Notlar**:
- Her satır, bir katmanı ve ilgili Dockerfile komutunu temsil eder.
- Büyük katmanlar imaj boyutunu artırır; optimize edilmiş imajlar için dikkatli olun.
- Güvenlik için, bilinmeyen imajların geçmişi incelenmelidir.
- `docker inspect` ile birlikte kullanılarak daha fazla bilgi alınabilir.

---

### j. `docker import`

**Açıklama**: Bir tar arşivinden yeni bir imaj oluşturur. Genellikle bir dosya sistemini veya mevcut bir konteyner anlık görüntüsünü imaja dönüştürmek için kullanılır.

**Yapısı**:
```bash
docker import [SEÇENEKLER] DOSYA | URL [DEPO[:ETİKET]]
```

**Yaygın Seçenekler**:
- `-c` veya `--change`: İmaj oluştururken Dockerfile talimatları uygular (ör. `-c "CMD [\"nginx\", \"-g\", \"daemon off;\"]"`).
- `-m` veya `--message`: İmaj için bir açıklama ekler.

**Kullanım Senaryoları**:
- Bir dosya sistemi tar dosyasından imaj oluşturmak.
- Eski konteyner anlık görüntülerini imaja dönüştürmek.
- Özelleştirilmiş minimal imajlar üretmek.

**Örnekler**:
```bash
docker import my_filesystem.tar my_image:1.0
```
- `my_filesystem.tar` dosyasından `my_image:1.0` imajını oluşturur.
```bash
docker import -c "CMD [\"bash\"]" http://example.com/rootfs.tar my_image:latest
```
- URL’den bir imaj oluşturur ve giriş noktası tanımlar.
```bash
docker import -m "Imported base filesystem" my_filesystem.tar base_image:1.0
```
- Açıklama ekleyerek imaj oluşturur.

**Hata Durumları**:
- **Geçersiz dosya**: Tar dosyası bozuk veya yanlış formatta ise hata verir.
- **Hatalı talimat**: `-c` ile geçersiz bir talimat belirtilirse hata verir.

**Notlar**:
- `docker import`, genellikle minimal veya özelleştirilmiş imajlar için kullanılır; `docker build` daha yaygındır.
- Oluşturulan imajlar, `docker images` ile listelenir.
- İmajın işlevselliği, tar dosyasının içeriğine bağlıdır.
- Güvenlik için, bilinmeyen kaynaklardan gelen tar dosyaları incelenmelidir.

---

## 3. Docker Compose Komutları

Docker Compose, birden fazla konteyneri tanımlamak, yapılandırmak ve yönetmek için kullanılan bir araçtır. `docker-compose.yml` dosyası, servisler (konteynerlar), ağlar ve hacimler gibi bileşenleri tanımlar. Aşağıdaki komutlar, Compose projelerini (birden fazla konteynerden oluşan uygulamaları) çalıştırmak ve yönetmek için kullanılır.

### a. `docker-compose up`

**Açıklama**: `docker-compose.yml` dosyasındaki tüm servisleri (konteynerları) oluşturur, ağları ve hacimleri yapılandırır ve başlatır. Compose projesinin tamamını çalıştırmak için temel komuttur. Geliştirme, test ve üretim ortamlarında çoklu konteyner uygulamalarını başlatmak için kullanılır.

**Yapısı**:
```bash
docker-compose up [SEÇENEKLER] [SERVİS...]
```

**Yaygın Seçenekler**:
- `-d` veya `--detach`: Arka planda çalıştırır (detached mode).
- `--build`: Servis imajlarını yeniden inşa eder (Dockerfile değiştiyse).
- `--force-recreate`: Konteynerleri zorla yeniden oluşturur (mevcut konteynerleri siler).
- `--no-build`: İmajları inşa etmeden mevcut imajları kullanır.
- `--scale SERVİS=N`: Bir servisten N kopya başlatır (ör. `--scale web=3`).
- `-f` veya `--file`: Özel bir Compose dosyası belirtir (ör. `-f docker-compose.prod.yml`).
- `--no-deps`: Bağımlı servisleri başlatmaz (yalnızca belirtilen servisleri çalıştırır).
- `--abort-on-container-exit`: Bir konteyner durursa tüm projeyi durdurur.
- `--timeout`: Konteynerlerin durması için beklenecek süreyi saniye cinsinden belirler.
- `--remove-orphans`: Compose dosyasında tanımlı olmayan servisleri siler.
- `--no-start`: Konteynerleri oluşturur ama başlatmaz.
- `--renew-anon-volumes`: Anonim hacimleri yeniden oluşturur.
- `--exit-code-from SERVİS`: Belirtilen servisin çıkış kodunu döndürür.

**Kullanım Senaryoları**:
- Web uygulamalarını başlatmak (frontend, backend, veritabanı, önbellek).
- Geliştirme ortamlarını hızlıca kurmak.
- Üretimde çoklu konteyner uygulamalarını dağıtmak.
- Test ortamlarını geçici olarak çalıştırmak.
- Mikroservis mimarisini yerel olarak simüle etmek.

**Örnekler**:
```bash
docker-compose up -d
```
- Tüm servisleri arka planda başlatır.
```bash
docker-compose up --build web
```
- `web` servisini yeniden inşa ederek başlatır.
```bash
docker-compose up -f docker-compose.prod.yml --scale web=3 --remove-orphans
```
- Özel bir Compose dosyasından projeyi başlatır, `web` servisini 3 kopya çalıştırır ve tanımlı olmayan servisleri siler.
```bash
docker-compose up --no-build --force-recreate
```
- İmajları inşa etmeden konteynerleri zorla yeniden oluşturur.
```bash
docker-compose up --abort-on-container-exit --exit-code-from web
```
- `web` servisi durursa projeyi durdurur ve çıkış kodunu döndürür.

**Hata Durumları**:
- **Port çakışması**: Aynı host portu başka bir konteyner tarafından kullanılıyorsa hata verir (`docker ps` ile kontrol edin).
- **Hatalı Compose dosyası**: Sözdizimi veya yapılandırma hataları başlatmayı durdurur; `docker-compose config` ile doğrulayın.
- **İmaj bulunamadı**: Gerekli imajlar yerel sistemde veya registry’de yoksa hata verir.
- **Bağımlılık hatası**: `depends_on` ile tanımlı servisler başlatılamazsa hata verebilir.

**Notlar**:
- Varsayılan dosya `docker-compose.yml`’dir; birden fazla dosya için `-f` kullanılır (ör. `-f docker-compose.yml -f docker-compose.override.yml`).
- `--build` olmadan mevcut imajlar kullanılır; bu, süreci hızlandırır.
- `--scale` ile birden fazla konteyner başlatıldığında port çakışmalarına dikkat edin (ör. farklı portlar tanımlayın).
- Çalışan konteynerler, `docker-compose ps` ile kontrol edilir.
- `--remove-orphans`, eski veya tanımlı olmayan servisleri temizlemek için kritiktir.
- Üretimde, `-d` ve `--restart` politikaları ile kesintisiz çalışma sağlanır.

---

### b. `docker-compose down`

**Açıklama**: Compose projesindeki tüm servisleri (konteynerları), ağları ve varsayılan hacimleri durdurur ve siler. Projenin tamamını temizlemek için kullanılır. Kalıcı hacimler, `-v` olmadan korunur.

**Yapısı**:
```bash
docker-compose down [SEÇENEKLER]
```

**Yaygın Seçenekler**:
- `-v` veya `--volumes`: İlişkili hacimleri siler (kalıcı veriler kaybolur).
- `--rmi TYPE`: İlgili imajları siler:
  - `all`: Tüm imajları siler.
  - `local`: Yerel olarak inşa edilmiş imajları siler.
- `--remove-orphans`: Compose dosyasında tanımlı olmayan servisleri (orphans) siler.
- `-t` veya `--timeout`: Konteynerlerin durması için beklenecek süreyi saniye cinsinden belirler.

**Kullanım Senaryoları**:
- Geliştirme ortamını tamamen temizlemek.
- Üretimde eski projeleri veya sürümleri kaldırmak.
- Disk alanını serbest bırakmak.
- Yeni bir proje başlatmadan önce eski kaynakları temizlemek.

**Örnekler**:
```bash
docker-compose down
```
- Tüm servisleri, ağları ve varsayılan hacimleri siler.
```bash
docker-compose down -v
```
- Hacimleri de siler (veri kaybına dikkat!).
```bash
docker-compose down --remove-orphans
```
- Tanımlı olmayan servisleri de siler.
```bash
docker-compose down --rmi all --volumes
```
- Tüm imajları ve hacimleri siler.
```bash
docker-compose down -t 5
```
- Konteynerleri 5 saniye içinde durdurur ve siler.

**Hata Durumları**:
- **Hacim kullanımda**: Hacim başka bir konteyner tarafından kullanılıyorsa `-v` ile silinemez.
- **İmaj silme hatası**: İmaj başka bir konteyner veya proje tarafından kullanılıyorsa `--rmi` hata verebilir.

**Notlar**:
- Kalıcı hacimler `-v` olmadan silinmez; veri kaybını önlemek için dikkatli olun.
- `--remove-orphans`, Compose dosyasından kaldırılan veya geçici servislerin konteynerlarını temizler.
- `docker-compose down` sonrası proje yeniden başlatılabilir (`docker-compose up`).
- Üretimde, hacim silme (`-v`) dikkatli kullanılmalıdır; veri yedekleri alın.
- `docker system prune` ile birlikte kullanılarak daha kapsamlı temizlik yapılabilir.

---

### c. `docker-compose ps`

**Açıklama**: Compose projesindeki konteynerların durumunu listeler. Çalışan ve durmuş konteynerlerin detaylarını (servis adı, konteyner ID’si, durum, portlar) gösterir.

**Yapısı**:
```bash
docker-compose ps [SEÇENEKLER] [SERVİS...]
```

**Yaygın Seçenekler**:
- `-q` veya `--quiet`: Sadece konteyner ID’lerini gösterir.
- `--services`: Sadece servis isimlerini listeler.
- `--filter`: Konteynerleri filtreler (ör. `--filter status=running`).
- `-a` veya `--all`: Tüm konteynerları (durmuş olanlar dahil) listeler.

**Kullanım Senaryoları**:
- Projedeki konteynerlerin durumunu ve port eşleşmelerini kontrol etmek.
- Hangi servislerin çalıştığını veya durduğunu doğrulamak.
- Otomasyon betiklerinde konteyner ID’lerini toplamak.
- Hata ayıklama için proje durumunu analiz etmek.

**Örnekler**:
```bash
docker-compose ps
```
- Çalışan servislerin durumunu gösterir.
```bash
docker-compose ps -a
```
- Tüm servisleri (durmuş olanlar dahil) listeler.
```bash
docker-compose ps -q
```
- Sadece konteyner ID’lerini gösterir.
```bash
docker-compose ps --services
```
- Projedeki servis isimlerini listeler.
```bash
docker-compose ps --filter status=running
```
- Sadece çalışan servisleri listeler.

**Hata Durumları**:
- **Proje mevcut değil**: Compose dosyası veya proje kaynakları yoksa boş döner.
- **Filtre hatası**: Yanlış filtre sözdizimi hata verebilir.

**Notlar**:
- `docker ps`’ye benzer, ancak sadece Compose projesiyle sınırlıdır.
- Çıktı, servis adı, konteyner ID’si, durum, portlar ve komut gibi bilgileri içerir.
- `-q` ile alınan ID’ler, diğer komutlarla (ör. `docker-compose rm`) kullanılabilir.
- Proje durumunu hızlıca görmek için sıkça kullanılır.

---

### d. `docker-compose logs`

**Açıklama**: Compose projesindeki servislerin loglarını gösterir. Her servisin standart çıktı ve hata loglarını birleştirir ve renk kodlarıyla ayrıştırır. Hata ayıklama ve izleme için kullanılır.

**Yapısı**:
```bash
docker-compose logs [SEÇENEKLER] [SERVİS...]
```

**Yaygın Seçenekler**:
- `-f` veya `--follow`: Logları gerçek zamanlı takip eder.
- `--tail N`: Son N satır logu gösterir.
- `-t` veya `--timestamps`: Loglara zaman damgaları ekler.
- `--no-color`: Renkli çıktıyı devre dışı bırakır.
- `--no-log-prefix`: Servis adlarını log satırlarından kaldırır.

**Kullanım Senaryoları**:
- Çoklu konteyner uygulamalarında hata ayıklama.
- Servis başlatma, çalışma veya hata loglarını izlemek.
- Gerçek zamanlı izleme (ör. API istekleri, veritabanı işlemleri).
- CI/CD pipeline’larında log toplamak.

**Örnekler**:
```bash
docker-compose logs web
```
- `web` servisinin loglarını gösterir.
```bash
docker-compose logs -f
```
- Tüm servislerin loglarını canlı takip eder.
```bash
docker-compose logs --tail 10 -t
```
- Son 10 satır logu zaman damgalarıyla gösterir.
```bash
docker-compose logs --no-color --no-log-prefix
```
- Renksiz ve önek olmadan logları gösterir.

**Hata Durumları**:
- **Servis mevcut değil**: Belirtilen servis yoksa hata verir.
- **Log bulunamadı**: Servis log üretmiyorsa (ör. sessiz bir süreç) boş döner.

**Notlar**:
- Loglar, servisler arasında renk kodlarıyla ayrılır; `--no-color` ile düz metin alınır.
- `-f` ile canlı izleme, terminali bağlar; CTRL+C ile çıkılır.
- Büyük projelerde `--tail` ile logları sınırlamak performans kazandırır.
- Log yönetimi için harici araçlar (ör. ELK Stack, Fluentd) kullanılabilir.

---

### e. `docker-compose exec`

**Açıklama**: Compose projesindeki bir servisin konteynerinde komut çalıştırır. Konteyner içinde hata ayıklama, yönetim veya interaktif işlemler için kullanılır.

**Yapısı**:
```bash
docker-compose exec [SEÇENEKLER] SERVİS KOMUT [ARGÜMANLAR...]
```

**Yaygın Seçenekler**:
- `-it` veya `--interactive --tty`: İnteraktif terminal başlatır.
- `--user`: Komutu belirli bir kullanıcıyla çalıştırır (ör. `--user root`).
- `--workdir`: Komutun çalışacağı dizini belirtir (ör. `--workdir /app`).
- `--env` veya `-e`: Komut için ortam değişkeni tanımlar.
- `--index`: Çoklu konteyner durumunda belirli bir kopyayı hedefler (ör. `--index 2`).

**Kullanım Senaryoları**:
- Servis konteynerinde kabuk açarak dosya sistemi veya yapılandırmaları kontrol etmek.
- Veritabanı sorguları çalıştırmak (ör. MySQL, PostgreSQL istemcisi).
- Servis içinde betik veya yönetim komutları yürütmek.
- Çalışan bir servisin durumunu izlemek (ör. `top`, `ps`).

**Örnekler**:
```bash
docker-compose exec web /bin/bash
```
- `web` servisinin konteynerinde bir bash kabuğu açar.
```bash
docker-compose exec db mysql -uroot -