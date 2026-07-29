# Laser communication
A Free Space Optical communication system is a line of sight wireless technology that transmits high speed data using focused light beams through open air or outer space rather than physical fiber optic cables.

# Results
<img src="https://github.com/MansoorAlmaleki/Laser-communication/blob/77c3bf23cfd28e5b7b9846a931bacf16516aa4bd/misc/IMG_6317.PNG" width="1000" alt="Project Results">

<img src="https://github.com/MansoorAlmaleki/Laser-communication/blob/3d0af95a7da16938e62e3ca2814fd5331e37cc7a/misc/BitmapImage.png" width="500" alt="Transmitted image">

*Received image*

## Required materials
* microcontroller (Esp32 or faster is recommended)
* photodiode input module (photodiode LM393 module used)
* laser diode

## Wiring
For the transmitter part the laser is not connected to the microcontrollers output directly, this would either fry the output pin or would be too weak to drive
so a Low side NPN transistor switch is used for driving the laser diode where the current limited [laser diode](http://hiletgo.com/ProductDetail/2010704.html) sits between the supply and collector input.

On the receiver end, the photodiode module has a fast stock response time but not fast enough for this system so it must be modified.
<img src="https://github.com/MansoorAlmaleki/Laser-communication/blob/79f8d85cfc322a5ca72872cd71649728eb993798/misc/IMG_6289.GIF" width="500" alt="Transmitted image">

*photodiode LM393 module schematic*

To modify the photodiode module Capacitor(C2) is removed as it causes an unwanted low pass filter in combination with Resistor(R1), essentially its an RC circuit
so a sharp rise signal will be smoothed by the RC filter slowing down the response time of the circuit, to combat it we remove capacitor C2 on the circuit board, and adding a 330 ohm pull up resistor between supply and analog pin (AO) reduces the total resistance of the photodiode high side resistor branch thus lowering the RC time constant. When a 330 ohm resistor is placed between supply and analog pin (AO) the circuit changes to a 10k resistor in parallel to a 330 ohm resistor which gives us approximately 320 ohms of parallel resistance and this change hugely changes the RC time constant for a faster response time

This is done to the digital out (DO) pin too as the parasitic capacitance of the traces in combination with a huge resistor amplifies the RC time constant.
Adding a 1k pull up between supply and digital out(DO) gives a parallel resistance of approximately 980 which decreases the RC time constant by a factor of 10, But be aware that the LM393 IC relies on an Open collector output meaning that when the comparator node is set to off it pulls down the pull up resistor and begins sinking current, and for safety the sinking current should be below 20mA, any more current will cause damage to the IC
  
## License
[MIT](https://github.com/MansoorAlmaleki/Laser-communication/blob/f93b1685b51eef905da93d90d014dedf51ae5748/LICENSE)

