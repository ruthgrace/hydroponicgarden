# hydroponic garden
firmware for hydroponic garden that never went into production

## Development setup
You will need to set up your IDE, code downloading tool, and hardware.

### IDE

The microcontroller (MCU) used for the project is STC90C58AD, which is a 8051 type microcontroller. To set up your IDE, download Keil C51 development kit: https://www.keil.com/download/product/ . The software that you open on your computer will be called Keil uVision5.

### Code downloading tool

Download the first STC thing here: http://www.stcmicro.com/rjxz.html . I'm using V6.80 and it keeps telling me to update, but I think it should work fine with V6.86, which is the latest.

Follow the first 90 seconds of this video to figure out how to add the STC microcontrollers to Keil: https://www.youtube.com/watch?v=nHCjFw2TS4M .

Make sure the STC-ISP settings look like this:

![STC-ISP software settings](https://github.com/ruthgrace/hydroponicgarden/blob/master/stc-isp-settings.PNG)

### Development workflow

Open the `test.uvproj` file using Keil uVision5.

Make your changes in the code files.

Go to `Target > Build Target` or `Target > Rebuild all project files` to compile. Any errors will show in the bottom of the IDE. If there aren't any errors, this will produce a `test.hex` file.

Open STC-ISP. Navigate to the `Code Buffer` tab. Click `Open Code File` and select your `test.hex`. Click `Download/Program`.

The hardware should start buzzing. Press the power button for 1 second (until it beeps three times) to reset the MCU and load the new firmware.

### Hardware

The hardware I have has 2 PCBs, one is for the top of the hydroponic garden and includes the LCD display, buttons, and a big black relay hanging off some wires that represent the LEDs. There is a long thick cord connecting that to a second PCB for the bottom of the garden, which has the float switches and lights which represent the pumps (they turn on when the firmware tells the pump to turn on). There is also a power supply.

The LCD powers on or off if you press the power button for 3 seconds.

### Troubleshooting development setup

Always double check all electronic connections.

STC-ISP says the COM port is not available: try plugging the USB into a different port on your computer. For me, only the USB on the left side closest to the back of my laptop works.

STC-ISP downloading or checking MCU makes the correct buzzing noise on the hardware, but the code does not download: Make sure that the power supply is unplugged from the board. Sometimes the conflicting power can prevent download. You can plug it back in after the code is downnloaded.

If you are getting a lot of weird errors when compiling the code, you may have to replace your reg52.h that comes with Keil with the one that STC recommends.
