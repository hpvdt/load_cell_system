# Load Cell System

System to read up to six load cells and broadcast the data over USB to a host computer that would display and record it. Designed originally for testing wings for our wing profiles for lift and drag.

Hardware designed in KiCad, programmed using Arduino. Code depends on the HX711 library by [bodge](https://github.com/bogde/HX711).

The interface for the host computer is located in our [interfaces repository](https://github.com/hpvdt/interfaces) as `wing_tester`, and is based on Processing 3.
