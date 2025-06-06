# GTU-C312 CPU Simulator Usage Guide

Bu simülatör, GTU-C312 CPU mimarisi için yazılmış operating system ve thread'leri çalıştırmak için tasarlanmıştır.

## Derleme

```bash
make simulator
```

Bu komut `bin/simulator` executable dosyasını oluşturacaktır.

## Kullanım

```bash
./bin/simulator <filename> [-D debug_mode] [-max max_instructions]
```

### Parametreler

- `<filename>`: Çalıştırılacak assembly program dosyası (örn: `programs/os.asm`)
- `-D debug_mode`: Debug modu (0-3 arası, varsayılan: 0)
- `-max max_instructions`: Maksimum instruction sayısı (varsayılan: 50000)

### Debug Modları

#### Debug Mode 0 (Varsayılan)
CPU durduğunda bellek içeriğinin tamamını stderr'e yazdırır.

```bash
./bin/simulator programs/os.asm -D 0
```

#### Debug Mode 1
Her instruction execution'dan sonra tüm bellek içeriğini stderr'e yazdırır.

```bash
./bin/simulator programs/os.asm -D 1
```

**Uyarı:** Bu mod çok fazla çıktı üretir. Küçük programlar için uygundur.

#### Debug Mode 2
Her instruction execution'dan sonra tüm bellek içeriğini stderr'e yazdırır ve devam etmek için klavyeden bir tuşa basılmasını bekler.

```bash
./bin/simulator programs/os.asm -D 2
```

**Kullanım:** Step-by-step debugging için idealdir. Her instruction'dan sonra ENTER tuşuna basarak ilerleyebilirsiniz.

#### Debug Mode 3
Her context switch ve system call'dan sonra thread table'ının içeriğini stderr'e yazdırır.

```bash
./bin/simulator programs/os.asm -D 3
```

Bu mod thread management ve scheduling'i debug etmek için en uygunudur.

## Örnek Kullanımlar

### 1. Normal Çalıştırma
```bash
./bin/simulator programs/os.asm
```

### 2. Thread Table Monitoring
```bash
./bin/simulator programs/os.asm -D 3
```

### 3. Step-by-step Debugging
```bash
./bin/simulator programs/os.asm -D 2 -max 100
```

### 4. Bellek İçeriği Monitoring
```bash
./bin/simulator programs/os.asm -D 1 -max 50 2> memory_trace.txt
```

## Çıktı Formatları

### Normal Çıktı (stdout)
- Genel bilgiler ve execution summary
- Thread'lerin SYSCALL PRN çıktıları
- CPU durumu ve istatistikler

### Debug Çıktısı (stderr)
- Bellek dump'ları (Debug Mode 0, 1, 2)
- Thread table içerikleri (Debug Mode 3)
- Context switch bildirimleri

### Thread Table Format (Debug Mode 3)
```
Thread Table Base Address: 220
Format: [ID] [State] [PC] [SP] [DataBase] [InstrBase] [IC] [WakeupCount]
States: 0=READY, 1=RUNNING, 2=BLOCKED, 3=TERMINATED

Thread 0: [0] [1:RUNNING] [1000] [1999] [20] [1000] [0] [0]
Thread 1: [1] [0:READY] [3000] [3999] [2000] [3000] [0] [0]
Thread 2: [2] [0:READY] [5000] [5999] [4000] [5000] [0] [0]
Thread 3: [3] [0:READY] [7000] [7999] [6000] [7000] [0] [0]
Thread 4: [4] [0:READY] [9000] [9999] [8000] [9000] [0] [0]
```

## Test Programları

Proje ile birlikte gelen test programları:

1. **programs/os.asm**: Ana OS ve 4 user thread (sorting, searching, multiplication, idle)
2. **tests/test_source/sort_test.asm**: Sadece bubble sort thread'i
3. **tests/test_source/linear_search.asm**: Sadece linear search thread'i
4. **tests/test_source/mult_by_addition.asm**: Sadece multiplication thread'i

## Performans Notları

- **Debug Mode 0**: En hızlı, sadece sonunda dump
- **Debug Mode 1**: Çok yavaş, her instruction için full memory dump
- **Debug Mode 2**: En yavaş, kullanıcı etkileşimi gerekli
- **Debug Mode 3**: Orta hızlı, sadece gerekli anlarda thread table

## Sorun Giderme

### Program çok uzun sürüyor
```bash
./bin/simulator programs/os.asm -max 10000
```

### Bellek hatası
Debug mode 1 veya 2 ile bellek durumunu kontrol edin.

### Thread problemi
Debug mode 3 ile thread state'lerini takip edin.

### Infinite loop şüphesi
Düşük max instruction limit ile test edin:
```bash
./bin/simulator programs/os.asm -max 1000
```
