# Goggles-X-VR
In case anyone wants to (for some reason like I did) use a monoscopic displayed device for VR through SteamVR & OpenTrack

Software:
OpenTrack, Steam, Arduino IDE

Hardware used:
- RPI Pico/XIAO SAMD21 microcontroller
- MPU6500 
  - Although any MPU IMU (or ones listed in code) should work you might need to run code to figure out exactly what chip you have and change the code to your chip model
      - Found in FastIMU example code library "IMUIdentifier"

Board library:
- [Seeed microcrontroller library for SAMD21](https://wiki.seeedstudio.com/Seeeduino-XIAO/#software)
    - Or you can click on File > Preference, and fill Additional Boards Manager URLs with the url -> ```https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json```

## Dependencies
This project relies on the following software/libraries:

- ```MadgWick```: get through Arduino IDE libraries manager – Lib for Quaternon functions
- ```FastIMU```: also through lib manager – For IMU support
- [OpenTrack](https://github.com/opentrack/opentrack) – Program for tracking user's head rotation
- [SteamVR](https://store.steampowered.com/steamvr/) – VR runtime
- [Arduino IDE](https://www.arduino.cc/en/software/) – Likely installed but I'll mention it anyway

*Full disclosure this is a modified version of example code from the "FastIMU" lib but with OpenTrack Integration
