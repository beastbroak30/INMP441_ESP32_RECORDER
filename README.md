# INMP441_ESP32_RECORDER

This project implements an audio recording system on a 30-pin ESP32 (e.g., ESP32-WROOM-32) using an INMP441 I2S microphone, an SD card module, and a tactile button. Recordings are saved as WAV files on the SD card with incremental filenames (audio1.wav, audio2.wav, etc.). The system uses a custom library for I2S audio processing and supports configurable sample rates (e.g., 44100 Hz) and bit depths (e.g., 16-bit).
# Find the latest codes and changes in [VERSION_LATEST](https://github.com/beastbroak30/INMP441_ESP32_RECORDER/tree/main/VERSIONS_LATEST) of this repo
## You need the lib always so copy it there on your working folder

Features
--------

*   **Record Audio**: Press a tactile button to start recording, press again to stop. 
*   **Incremental File Naming**: Each recording creates a new file (audioN.wav).
*   **Configurable Audio**: Supports 8-bit or 16-bit WAV files with adjustable sample rates (e.g., 16000 Hz, 44100 Hz).
*   **Gain Boost**: Amplifies the INMP441 microphone signal for better audio quality.
*   **LED Feedback**: LED indicates recording status (ON when recording, OFF when stopped).
*   **Debug Output**: Serial Monitor logs initialization, button presses, and recording details.
    
Hardware Requirements
---------------------

*   **ESP32**: 30-pin module (e.g., ESP32-WROOM-32).
*   **INMP441 I2S Microphone**: For capturing audio. 
*   **MicroSD Card Module**: For storing WAV files (FAT32 formatted SD card)  
*   **Tactile Button**: To start/stop recording.
*   **LED**: Uses the inbuilt LED in GPIO2 (not required) 
*   **Breadboard and Jumper Wires**: For connections.
*   **OLED** : For the latest versions

### Pin Connections
 Component             | ESP32 Pin       |          
|-----------------------|-----------------|
| **SD Card Module**     |                 |
| GND                   | GND             |
| VCC                   | VIN (5V)        |
| MISO                  | D19             |
| MOSI                  | D23             |
| SCK                   | D18             |
| CS                    | D5              |

 Component             | ESP32 Pin       |          
|-----------------------|-----------------|
| **INMP441 Microphone** |                 |
| GND                   | GND             |
| VDD                   | 3.3V            |
| SD                    | D35             |
| SCK                   | D33             |
| WS                    | D22             |
| L/R                   | 3.3V (Right channel) |

 Component             | ESP32 Pin       |  
 |-----------------------|-----------------|
| **Tactile Button**    |                 |
| One leg               | D26             |
| Other leg             | GND             |

Installation
------------

1.  git clone https://github.com/beastbroak30/INMP441_ESP32_RECORDER.git  
2.  **Open in Arduino IDE**:
    *   Open mic_test.ino in the Arduino IDE.   
    *   Ensure lib_audio_recording.ino is in the same sketch folder (as a separate tab).    
3.  **Install ESP32 Core**: 
    *   In Arduino IDE, go to Tools > Board > Boards Manager.   
    *   Search for esp32 and install the latest version by Espressif Systems.
4.  **Select Board**:
    *   Go to Tools > Board > ESP32 Arduino > ESP32 Dev Module.
    *   (optional if no error)Set Partition Scheme to No OTA (2MB APP / 2MB SPIFFS) to avoid "Sketch too large" errors.
5.  **Connect ESP32**:
    *   Connect the ESP32 to your computer via USB.
    *   Select the correct port under Tools > Port.
6.  **Upload Code**:
    *   Click the Upload button in the Arduino IDE.
      
Usage
-----
1.  **Power On**:
    *   Connect the ESP32 to a stable power source (USB or external 5V/3.3V).
2.  **Monitor Output**:
    *   Open the Serial Monitor (Tools > Serial Monitor) at 115200 baud to view debug messages.
3.  **Record Audio**:
    *   Press the tactile button (connected to D26) to start recording. The LED (D2) turns ON.
    *   Speak or make sounds near the INMP441 microphone.
    *   Press the button again to stop recording. The LED turns OFF, and the WAV file is saved.
4.  **Check Files**:
    *   Remove the SD card and insert it into a computer.
    *   Find WAV files (audio1.wav, audio2.wav, etc.) in the root directory.
    *   Play files using software like Audacity or any media player.

### Example Serial Output
```bash
18:26:30.875 -> [DEBUG] Starting setup...
18:26:30.875 -> [DEBUG] Pins initialized: LED (2), Button (26)
18:26:30.875 -> 
18:26:30.875 -> ak esp32 INMP441
18:26:30.875 -> [DEBUG] Initializing SD card...
18:26:31.014 -> [DEBUG] SD Card initialized successfully
18:26:31.014 -> [DEBUG] Initializing I2S recording...
18:26:31.014 -> [DEBUG] I2S recording initialized
18:26:31.047 -> [DEBUG] Setup complete. Waiting for button press to start/stop recording...
18:26:35.630 -> [DEBUG] Button pressed!
18:26:35.690 -> [DEBUG] Starting recording to /audio3.wav...
18:26:35.690 -> 
18:26:35.690 -> > No AUDIO file found
18:26:35.713 -> > WAV Header generated, Audio Recording started ... 
18:26:35.713 -> [DEBUG] Recording started successfully
18:26:35.747 -> > I2S Rx Samples [Original, 16bit signed]:    1024	1024	1792	1792	896	896	992	992	96	96	0	0	-928	-928	-896	-896	
18:26:35.779 -> > I2S Rx Samples [Original, 16bit signed]:    128	128	480	480	-128	-128	-256	-256	-768	-768	-672	-672	-1312	-1312	-1568	-1568	
18:26:35.855 -> > I2S Rx Samples [Original, 16bit signed]:    0	0	128	128	384	384	864	864	992	992	896	896	992	992	480	480	
18:26:35.941 -> > I2S Rx Samples [Original, 16bit signed]:    128	128	-256	-256	736	736	864	864	1248	1248	1248	1248	864	864	480	480	
18:26:36.014 -> > I2S Rx Samples [Original, 16bit signed]:    1024	1024	896	896	896	896	1152	1152	480	480	128	128	224	224	-32	-32	
18:26:46.980 -> [DEBUG] Button pressed!
18:26:46.980 -> [DEBUG] Stopping recording...
18:26:46.980 -> > ... Done. Audio Recording finished.
```
Configuration
-------------

The library (lib_audio_recording.ino) allows customization:
*   **Sample Rate**: Adjust SAMPLE\_RATE (e.g., 16000, 44100 Hz) for audio quality vs. file size.    
*   **Bit Depth**: Set BITS\_PER\_SAMPLE (8 or 16) for file size vs. fidelity.    
*   **Gain Boost**: Modify GAIN\_BOOSTER\_I2S (1–64) to amplify the microphone signal.    
*   **I2S Channel**: Change I2S\_SLOT\_MASK (I2S\_STD\_SLOT\_RIGHT or I2S\_STD\_SLOT\_LEFT) based on L/R pin wiring.
     
Future Work
-----------
*   **File Management**: Add options to delete or rename WAV files via additional buttons or Serial commands.
*   **Recording Modes**: Implement timed recordings or continuous recording with auto-split into multiple files.
*   **OLED Display**: Integrate a display to show recording status, file number, and duration.
*  **Into a product**       
 
---------------
*   **No Audio in WAV Files**:
    *   Verify INMP441 wiring (especially L/R to 3.3V for right channel).
    *   Check SAMPLE\_RATE and GAIN\_BOOSTER\_I2S in lib\_audio\_recording.ino
    *   Test the microphone with a different ESP32 or I2S setup.
        
*   **Button Not Responding**:    
    *   Ensure the tactile button is connected to D26 and GND.
    *   Verify INPUT\_PULLUP works; 
        
*   **SD Card Errors**:
    *   Ensure the SD card is FAT32 formatted and not write-protected.
    *   Check SD module wiring (CS: D5, MISO: D19, MOSI: D23, SCK: D18).
        
*   **Serial Output Issues**:
   *   Confirm the Serial Monitor is set to 115200 baud.   
   *   Check USB connection and port selection.
        

Contributing
------------

Contributions are welcome! Please:
1.  Fork the repository.
2.  Create a feature branch (git checkout -b feature/your-feature).
3.  Commit changes (git commit -m 'Add your feature').
4.  Push to the branch (git push origin feature/your-feature).
    
5.  Open a pull request.
    

License
-------

This project is licensed under something
