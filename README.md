[Ru](/README-RU.md)

| Supported Targets |
| ESP32S3 |
| ----------------- |

# MULTIPLE INTEGRATED LASER ENGAGEMENT SYSTEM (MILES)
# COMMUNICATION CODE (MCC)
# ESP32S3 Transmitter & Receiver

## Basic Parameters
- Transmitter uses 1 RMT channel
- Receiver uses ESP32S3 CAM module in 16 channel mode
## Interface is defined in include/mcc_rx_tx.h 
## Transmitter and receiver parameters are defined in private_include/mcc_enc_dec.h
- Receiver requires 1 external unconnected GPIO and one LEDC channel
## Usage example main/main.c
- Initializes RMT for transmitter
- Initializes receiver
- Transmitter sends packet - > 2 * MCC word / 1600 µs pause / 4 * MCC word / 5 sec pause
- The receiver receives data from the queue and sends it to console

## Example tested on ESP-IDF 5.3.1, should work on 5.2.X and above.