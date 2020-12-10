# KicsiKocsi
###### Projektmunka Mikroelektromechanikai rendszerek (GKLB_INTM020) tárgyból<br />Készítette: Borsodi Zoltán (B7PK8Z), mérnökinformatika, Széchenyi István Egyetem

Egy autó, egy vezeték nélkül csatlakozó távirányítóval, amelyet kézmozdulatokkal lehet vezérelni, elől hátul távolságérzékelővel, hogy ne menjen neki az akadályoknak.

Felépítés
====
A projekt két részből áll: az autó illetve a távirányító. Minkét fél egy-egy Arduino fejlesztőeszközre épül. Egymással vezeték nélkül kommunikálnak.

Autó
----
- Arduino UNO Wifi Rev2
- nRF24L01 (2.4GHz adóvevő modul)
- HC-SR04 ×2 (ultrahangos távolságérzékelő)
- L293DNE (H híd, motor vezérlő)
- SM-S2309S (szervó motor)
- RM-17 (áttételes modell DC motor)
- Energiaforrás: 4×AA (+ 9V a DC motorhoz)

Távirányító
----
- Arduino Nano
- nRF24L01 (2.4GHz adóvevő modul)
- MPU6050 (gyorsulásmérő és giroszkóp)
- LED + 220Ohm ellenállás
- nyomógomb + 10kOhm ellenállás
- Energiaforrás: 4×AA
