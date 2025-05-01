 # Akıllı Araç Güvenlik Sistemi

## 📝 Proje Hakkında

Bu proje, Arduino Mega 2560 tabanlı bir akıllı araç güvenlik sisteminin Proteus simülasyon ortamında tasarlanması ve uygulanmasını kapsamaktadır. Sistem, araç güvenliğini artırmak amacıyla bir dizi sensör ve kontrol mekanizması kullanarak çeşitli güvenlik ve konfor fonksiyonlarını gerçekleştirmektedir.

## 🚀 Özellikler

- **Emniyet Kemeri Kontrolü**: Emniyet kemeri takılı değilse kırmızı LED yanar, buzzer öter ve LCD'de uyarı mesajı gösterilir
- **Kapı Durumu Kontrolü**: Kapı açıkken RGB LED (kırmızı) yanar, motor çalışmaz ve LCD'de "Kapi Acik" uyarısı gösterilir
- **Sıcaklık Kontrolü**: LM35 ile ölçülen sıcaklık 25°C'yi aştığında klima fanı (DC motor) otomatik çalışır
- **Otomatik Farlar**: LDR sensörü ile ortam ışığı seviyesi düşükse mavi LED (farlar) otomatik yanar
- **Yakıt Seviyesi İzleme**: Yakıt seviyesi %10'un altına düştüğünde sarı LED yanar; %0'a ulaştığında motor durur
- **LCD Bilgilendirme**: Tüm uyarılar ve sistem durumu 16x2 I2C LCD ekran üzerinden kullanıcıya bildirilir

## 🛠️ Teknolojiler

Bu projede aşağıdaki teknolojiler kullanılmıştır:

- **Donanım**: 
  - Arduino Mega 2560
  - LM016L LCD ekran
  - LM35 sıcaklık sensörü
  - LDR ışık sensörü
  - RV1 potansiyometresi
  - L293D motor sürücüsü
  - DC motorlar
  - LED'ler (Kırmızı, Mavi, Sarı, RGB)
  - Buzzer
  - Butonlar ve anahtarlar

- **Yazılım**:
  - Arduino IDE 2.3.2
  - Proteus 8.13 (Simülasyon)
  - C++ programlama dili

## ⚙️ Kurulum

1. Proteus 8.13'ü yükleyin
2. Arduino IDE 2.3.2'yi yükleyin
3. Gerekli kütüphaneleri Arduino IDE'ye ekleyin:
   ```bash
   LiquidCrystal_I2C
   ```
4. Arduino Mega 2560 V2 kütüphanesini The Engineering Projects'ten indirin ve Proteus'a ekleyin
5. Proteus devre şemasını açın
6. Arduino kodunu derleyin ve yükleyin
7. Simülasyonu başlatın

## 📸 Devre Şeması

![Ekran görüntüsü 2025-04-24 151342](https://github.com/user-attachments/assets/c093c1a0-1833-4289-8a83-2b2929f6e353)


Bu proje akademik bir çalışma olarak Kocaeli Üniversitesi Programlama Laboratuvarı-2 dersi kapsamında geliştirilmiştir.
