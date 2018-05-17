# Перехватчик HTTP-трафика
Выводит ключи и значения в передаваемых http-пакетах.

Файл `sniffer` является полной версией перехватчика, декодироющий строки в человекочитаемый формат, `sniffer_no_decode` не декодирует строки и оставлен для сравнения.

---
Системные требования:
- net-libs/libpcap  (GNU/Libux)

Компиляция:
```bash
g++ -std=c++11 -lpcap main.cpp -o sniffer
```
или
```bash
g++ -std=c++11 -lpcap main_no_decode.cpp -o sniffer-no-decode
```

Запуск:
```bash
sudo ./sniffer
```
или
```bash
sudo ./sniffer_no_decode
```
