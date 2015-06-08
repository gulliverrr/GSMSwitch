# GSMSwitch
An Arduino based GSM controller that uses up to 4 relays to switch on, off or restart stuff with a simple SMS.
You only need to modify the "MyMaster" variable to your own number as once the command sent is executed, Arduino calls back this number for a few seconds as a form of confirmation.

Hardware required:
- An Arduino with at least 1 serial port (additional port can be used for debugging). I used Leonardo here
- GSM Modem with serial port accepting AT commands like SIEMENS TC35. It only costs 14GBP on eBay
- A relay module
- A 5V 2A power supply or similar

Apart from the modem initialisation commands the rest of the code has enough comments to follow.

Allowed commands:
1 on
1 off
1 restart
2 on
2 off
2 restart
3 on
3 off
3 restart
4 on
4 off
4 restart
all on
all off
all restart

restart command turns off the respective relay, waits 10sec and turns it back on. Its previous state is not taken into consideration.

Contact me if you need more info!
