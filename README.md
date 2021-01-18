# KicsiKocsi
###### Projektmunka Mikroelektromechanikai rendszerek (GKLB_INTM020) tárgyból<br />Készítette: Borsodi Zoltán (B7PK8Z), mérnökinformatika, Széchenyi István Egyetem

Egy autó, egy vezeték nélkül csatlakozó távirányítóval, amelyet kézmozdulatokkal lehet vezérelni, elől hátul távolságérzékelővel, hogy ne menjen neki az akadályoknak.

![https://github.com/theqbz/kicsikocsi/blob/master/rajzok/kicsikocsi2.jpg](https://github.com/theqbz/kicsikocsi/blob/master/rajzok/kicsikocsi2.jpg)

Felépítés
====
A projekt két részből áll: az autó illetve a távirányító. Minkét fél egy-egy Arduino fejlesztőeszközre épül. Egymással vezeték nélkül kommunikálnak.

Autó
----
- "központ": Arduino Nano
- "lokátor": Arduino UNO R3
- nRF24L01 (2.4GHz adóvevő modul)
- HC-SR04 ×2 (ultrahangos távolságérzékelő)
- L293DNE (H híd, motor vezérlő)
- SM-S2309S (szervó motor)
- RM-17 (áttételes modell DC motor)
- Energiaforrás: 4×AA (+ 9V a DC motorhoz)

![https://github.com/theqbz/kicsikocsi/blob/master/rajzok/kicsikocsi5.jpg](https://github.com/theqbz/kicsikocsi/blob/master/rajzok/kicsikocsi5.jpg)

Távirányító
----
- Arduino Nano
- nRF24L01 (2.4GHz adóvevő modul)
- MPU6050 (gyorsulásmérő és giroszkóp)
- LED + 220Ohm ellenállás
- nyomógomb + 10kOhm ellenállás
- Energiaforrás: 4×AA

![https://github.com/theqbz/kicsikocsi/blob/master/rajzok/kicsikocsi4.jpg](https://github.com/theqbz/kicsikocsi/blob/master/rajzok/kicsikocsi4.jpg)

Kapcsolási rajzok
---


Autó: ![https://github.com/theqbz/kicsikocsi/blob/master/rajzok/%C3%9AjK%C3%B6zpont_schem.png](https://github.com/theqbz/kicsikocsi/blob/master/rajzok/%C3%9AjK%C3%B6zpont_schem.png)

Távirányító: ![https://github.com/theqbz/kicsikocsi/blob/master/rajzok/T%C3%A1vir%C3%A1ny%C3%ADt%C3%B3_schem.jpg](https://github.com/theqbz/kicsikocsi/blob/master/rajzok/T%C3%A1vir%C3%A1ny%C3%ADt%C3%B3_schem.jpg)

Forráskódok
====
A központi egység két része számára:
- Központ (Arduino Nano): ![KpMotor.ino](KpMotor.ino)
- "Lokátor" (Arduino UNO): ![KpLokator.ino](KpLokator.ino) + ![Lokator.h](Lokator.h)

A távirányító (Arduino Nano): ![taviranyito.ino](taviranyito.ino)
