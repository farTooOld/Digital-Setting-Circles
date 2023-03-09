# Digital-Setting-Circles
Digital Setting Circle for Telescope

UPDATE, 3/9/23: Posted new DSC design using Teensy. App includes catalogs with Real-Time Alt/Dec tracking for Messier, Caldwell and some Bright-Stars.
Video and links are posted on YouTube: https://www.youtube.com/watch?v=BdSfx6Pw-qg

UPDATE, 12/30/17: For those not interested (or skilled in programming) I'm Posting .Hex files of the full code for UNO and Nano.  You'll need to learn how to flash your UNO/Nano (many Youtube vid's to help you). These code use an LCD for display - no Serial or Bluetooth display output.  The .Hex files are for 4:1 Gear Ratio and Signwise (or other 600ppr) encoders. Polaris is 'fixed' at 45.4 deg's. Use the Manual setting feature to set a new value... (or, create your own code based on the code for 'Posting'.

Rev -A-  12/2/17,  Added some code for Bluetooth display of output (can use your smartphone/tablet to see Telescope/encoder position)

Coded in the Arduino IDE but, it does not use the typical 'Setup' and 'Loop'.  Instead, it used 'Main' and 'While' for increased speed.
The code is setup for an Atmel p328 chip or an UNO or NANO board but you can change the pinouts as needed for other boards such as a Mega.

The circuit is posted in Fritzing' project site: http://fritzing.org/projects/telescope-tracker


The 3D printable parts are at: https://www.thingiverse.com/thing:2242951


A YouTube video is at: https://www.youtube.com/watch?v=SezbbfoszEI&feature=youtu.be

