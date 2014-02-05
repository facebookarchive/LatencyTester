Latency Tester
==============

![Latency Tester](https://www.oculusvr.com/wp-content/themes/oculus/img/oculus-latency-tester.png)

Firmware, Schematics, and Mechanicals for the Oculus Latency Tester.

https://www.oculusvr.com/order/latency-tester/

Firmware
--------

The Latency Tester firmware uses the CooCox CoIDE for development along with 
the gcc-arm-embedded toolchain.

The ST-Link/V2 is the cheapest way to load firmware onto a Latency Tester
board, though any SWD compatible programmer should work.  You can follow these steps
to set up a build environment and load firmware onto the board:

1. Install ST-Link drivers if you haven't: http://www.st.com/web/en/catalog/tools/PF258167
2. Install CoCenter: http://www.coocox.org/CooCox_CoIDE.htm
3. Through CoCenter, install CooCox and CoFlash
4. Install gcc-arm-embedded: https://launchpad.net/gcc-arm-embedded/+download
5. Set up gcc-arm-embedded in CooCox: http://www.coocox.org/CoIDE/Compiler_Settings.html
6. Clone our LatencyTester repository to C:\CooCox\CoIDE\workspace
7. Open the LatencyTester.coproj
8. Build
9. Plug the ST-Link into the board's 10 pin SWD header.
10. Plug in USB to power the board.
12. Download code to Flash 
13. Enjoy!

Note that the USB Product ID is set to 0x0101, which matches the production
version of the Latency Tester.  If you modify the firmware in a way that
changes the USB interface, please change the Product ID to 0x0100.

Mechanical
----------

The Mechanical folder contains STLs of a version of the Latency Tester housing
that is printable on low cost 3D printers as well as STEP files of the original
injection molded housing.

Schematics
----------

The Schematics folder contains an image of the schematic, as well as Eagle CAD files
of the schematic and board layout.

You can also order PCBs directly from OSH Park:

* Main PCB: http://oshpark.com/shared_projects/SHWtp0zm
* Sensor PCB: http://oshpark.com/shared_projects/nyOrdIp9
