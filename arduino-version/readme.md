# mi:muz:can (Arduino Version)

![mimuz-can logo](../mimuz-can.png)

# 1. Arduio UNO Version

![can attacker Arduino UNO Version](can_uno.jpg)

Arduino UNOでも作ってみました。

## Dependencies

Arduino UNOでUSB-MIDIを扱うために、[Arduino MIDI Library](http://playground.arduino.cc/Main/MIDILibrary)と、@morecat_labさん作[mocoLUFA](https://github.com/kuwatay)を利用させて頂いています。

## Version

v1.0 (2015.08.30)

## Schematics

![can attacker Arduino UNO version](canattacker_uno.png)

## parts

- Arduino UNO Rev.3
- R1: 1kΩ
- R2: 470Ω
- R3: 1kΩ
- R4: 470KΩ
- Q1/Q2: [2SK4017](http://akizukidenshi.com/catalog/g/gI-07597/)
- D1/D2: 1N4007
- A1/A2: [5V Push Solenoid　ROB-11015](https://www.sparkfun.com/products/11015)

## Firmware

[Firmware](./can2_uno/can2_uno.ino)

----

# 2. Arduino Leonardo Version

![can attacker Arduino Leonardo Version](can_leonardo.jpg)

Arduino Leonardoでも作ってみました！！！！

## Dependencies

Arduino LeonardoをUSB-MIDIに対応させるため、[rkistner/arcore](https://github.com/rkistner/arcore)を利用させて頂きました！

利用方法は、[こちら](http://qiita.com/tadfmac/items/d685dae3e46d570386c3)を参照ください。

## Version

v1.0 (2015.11.25)

## Schematics

配線はArduino UNOバージョンとほぼ同じです。
ただし、下記のように変更しています。

|Arduino UNO Version|Arduino Leonardo Versionでの変更点|
|:----|:---:|
|PIN8|PIN2|
|PIN9|PIN3|

※スケッチがそうなっているというだけで、基本的にお好きなデジタル端子に繋げば大丈夫です。

## parts

- Arduino Leonardo
- R1: 100kΩ (変更してみました)
- R2: 470Ω
- R3: 100kΩ (変更してみました)
- R4: 470KΩ
- Q1/Q2: [IRLU3410PBF](http://akizukidenshi.com/catalog/g/gI-06025/) (2SK4017がどっか行ったので)
- D1/D2: 1N4007
- A1/A2: [5V Push Solenoid　ROB-11015](https://www.sparkfun.com/products/11015)

## Firmware

[Firmware](./can4_leonardo/can4_leonardo.ino)

# Licenses

<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="クリエイティブ・コモンズ・ライセンス" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/88x31.png" /></a><br />この 作品 は <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">クリエイティブ・コモンズ 表示 4.0 国際 ライセンスの下に提供されています。</a>






