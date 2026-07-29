# Laser communication
A Free Space Optical communication system is a line of sight wireless technology that transmits high speed data using focused light beams through open air or outer space rather than physical fiber optic cables.

# Results
<img src="https://github.com/MansoorAlmaleki/Laser-communication/blob/77c3bf23cfd28e5b7b9846a931bacf16516aa4bd/misc/IMG_6317.PNG" width="1000" alt="Project Results">

<img src="https://github.com/MansoorAlmaleki/Laser-communication/blob/3d0af95a7da16938e62e3ca2814fd5331e37cc7a/misc/BitmapImage.png" width="500" alt="Transmitted image">

## Required materials
* microcontroller (Esp32 or faster is recommended)
* photodiode input module (photodiode LM393 module used)
* laser diode

## Wiring
For the transmitter part the laser is not connected to the microcontrollers output directly, this would either fry the output pin or would be too weak to drive
so a Low side NPN transistor switch is used for driving the laser diode where the current limited [laser diode](http://hiletgo.com/ProductDetail/2010704.html) sits between the supply+ and collector input.

On the receiver end, the photodiode module has a fast stock response time but not fast enough for this system so it must be modified.
  
## License
[MIT](https://github.com/MansoorAlmaleki/Laser-communication/blob/f93b1685b51eef905da93d90d014dedf51ae5748/LICENSE)

