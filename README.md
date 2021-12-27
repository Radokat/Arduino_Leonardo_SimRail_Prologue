# Arduino_Leonardo_SimRail_Prologue
Controlling "SimRail - The Railway Simulator: Prologue" via an Arduino Leonardo.
## Requirements:
* 1 * Arduino Leonardo
* 2 * Potentiometers of your choice.
* 3 * Buttons
* 3 * 10k resistors for the buttons.
## Connections:
Connect a potentiometer to A0 to regulate the power.  
Connect a potentiometer to A1 to regulate the local brake.  
Connect a button to pin 12 for operation the left door.  
Connect a button to pin 11 for operation the AWS.  
Connect a button to pin 10 for operation the right door.  
  
Adapt the potentiometer values in the script corresponding to your potentiometers/liking, mine are for RSA0N12-LIN10K.  
https://www.reichelt.de/schiebepotentiometer-stereo-10-kohm-linear-rsa0n12-lin10k-p73873.html


## Files  
[EN96](Simrail_Leonardo_EN96.ino) (A0 Power / A1 Local brake)
