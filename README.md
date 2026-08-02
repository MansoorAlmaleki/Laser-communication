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
For the transmitter part the laser diode should not be connected directly to the microcontroller output, as this would be too weak to drive the laser diode
so instead a low side NPN transistor switch is used for driving the [laser diode](http://hiletgo.com/ProductDetail/2010704.html).

On the receiver end, a photodiode module is used, it has a fast response time but not fast enough for this project so it must be modified.

<img src="https://github.com/MansoorAlmaleki/Laser-communication/blob/79f8d85cfc322a5ca72872cd71649728eb993798/misc/IMG_6289.GIF" width="500" alt="photodiode schematic">

*photodiode LM393 module schematic (ACTIVE LOW)*

To modify the photodiode module for a faster response time, capacitor C2 is removed to eliminate the unwanted RC low pass filter it forms with resistor R1, which would otherwise smooth out sharp signal rises and slow circuit performance. Additionally, adding a 330 ohm pull up resistor between the power supply and the analog pin (AO) places it in parallel with the onboard 10k resistor, dropping the high side resistance to approximately 320 ohm. By both removing the capacitive filtering and drastically lowering the resistance, this dual modification significantly decreases the RC time constant, enabling the module to detect rapid signal changes almost instantaneously.

This is done to the digital out (DO) pin too as the parasitic capacitance of the circuit in combination with a huge resistor amplifies the RC time constant.
Adding a 1k pull up between supply and digital out(DO) gives a parallel resistance of approximately 980 ohms which decreases the RC time constant by a factor of 10, But be aware that the LM393 IC relies on an Open collector output meaning that when the comparator node is set to off it pulls down the pull up resistor and begins sinking current. For safety the sinking current should be below 20mA, any more current will cause damage to the IC.

<img src="https://github.com/MansoorAlmaleki/Laser-communication/blob/0b873e963d6a9386ca1faef40aa2716e99392269/misc/ModifiedPhotodiode.jpeg" width="500" alt="Modifed photodiode board">

## Receiver calibration

```c++
uint32 SignalWidth = 30;//microseconds, an oscilloscope is used for calibrating the receiver side pulse width
```
Adjust this value if needed, although the default value is fine for most cases.
On the transmitter side open the code file at "SignalReference" and change the value of SIGNAL to the desired output pin

```c++
#define SIGNAL 18
```
connect the signal pin to the low side npn input with a current limiting resistor of around 1k, and the laser should begin flashing 
rapidly.
Aim the laser at the photodiode module and hook up an oscilloscope to the digital output (DO) of the photodiode module.

On the screen pulses should appear and it is normal to see that the signal width is shorter or longer than the set signal width, sometimes a poorly calibrated photodiode might be set to a too low/high sensitivity that its either always on or off and this is normal just turn the Blue potentiometer with a screwdriver in both directions until the signal has a width of the set signal width.


## Creating a bitmap image
Gimp image editing software used.

Import your image into GIMP, use the Crop Tool with a locked ⁠1:1⁠ aspect ratio to make it a square, optionally set the color mode to Grayscale, and scale the resolution to 128 pixels or less on each axis while keeping the proportions locked. Next, export the file as a ⁠.ppm⁠ into the ⁠ImageToBinary/InputImage/⁠ directory, making sure to select ASCII format (PPM Version 3) when prompted.Rename the exported ⁠.ppm⁠ file to ⁠.txt⁠, open it, and delete the GIMP comment on row 2 (e.g., ⁠# CREATED BY GIMP⁠). Finally, run ⁠main.exe⁠ (or compile ⁠main.cpp⁠) in ⁠ImageToBinary/InputImage/⁠, type in your ⁠.txt⁠ file's name when prompted in the console, and the program will generate a decimal file containing the average pixel color values inside the ⁠Output/⁠ directory.

## Importing the Decimal file into the Transmitter program

in transmitter program code set the ImageSize variable to the size of the included image (Not total pixels)

```c++
const uint8_t ImageSize = 128;
```
*line 25*

copy all of the components that is inside the file existing in the folder directory “ImageToBinary/Output/“ and
paste the data inside the the array

```c++
const uint8_t ImageData[ImageSize*ImageSize] = {};
```
*line 28*

## Receiving data
(while the transmitter is sending data be sure to not disrupt the signal as there is no error correction in this version of the code)

On the receiver side when the transmission ends data will be printed to the Serial buffer which should then be copied and pasted inside the file in the folder directory “DecimalToImage/Input/“.

The ImageResolution variable must match your image size. 

```c++
int ImageResolution = 128;
```
*line 11*

Run the main.exe program or compile the main.cpp code. After completion a bitmap image will be saved to the output folder.

## PWM waveform & Binary protocol
<img src="https://github.com/MansoorAlmaleki/Laser-communication/blob/457ed8cad1ebfdebb9a437ee194c57475b112a88/misc/PWMWaveform.jpeg" width="300" alt="PWM waveform">



## License
[MIT](https://github.com/MansoorAlmaleki/Laser-communication/blob/f93b1685b51eef905da93d90d014dedf51ae5748/LICENSE)

