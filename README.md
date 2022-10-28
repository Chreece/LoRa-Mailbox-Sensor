# LoRa-Mailbox-Sensor
For LoRaHub

Sends LoRa messages to the Hub to create an occupancy sensor when the mailbox door opens (switch or reed-switch triggered), the battery percentage for LiPo 3.7V (approximation) and a button to reset the occupancy sensor (after getting the mail).
Powers up, sends the messages and pulls it's own power off.

# Features:
- Latching Circuit: https://randomnerdtutorials.com/latching-power-switch-circuit-auto-power-off-circuit-esp32-esp8266-arduino/
 0 power consuption when mailbox door is closed
