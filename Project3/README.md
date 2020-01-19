# Embedded operating systems

## Project 3: Ultrasoon
Example file for using hardware with a Xilinx Avnet Minized Board.

### Hardware Connections

```
* VCC - 5V
* GND - Ground 
* CLK is connected to 100MHz Clock

set_property PACKAGE_PIN F14 [get_ports sonar_echo_0]
set_property PACKAGE_PIN F13 [get_ports sonar_trig_0]
set_property IOSTANDARD LVCMOS33 [get_ports sonar_echo_0]
set_property IOSTANDARD LVCMOS33 [get_ports sonar_trig_0]
```

## Author

**Jarni Vanmal** - [JarniV](https://github.com/JarniV)

