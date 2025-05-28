# Automated-Barista
 A fully Autonomous robot arm that serves a coffee from A to Z
 This project is Arduino based

Main components:
 Arduino Uno
 Servo driver 16 channels
 Mechnical design & 3d printed parts for robot and containers
 CNC Base
 Jumper wires
 Powwer supply
 Relay Modules
 Water pumps
 LCD 20 * 4
 Push buttons & switches
 Dispensers
 Mixer

System Sequence:
1.	Start Trigger:
The process begins when the user presses the push button. This signals the robot to move to its rest position.
2.	User Input via LCD:
The user selects the desired number of sugar scoops through the LCD interface.
3.	Cup Pickup:
The robot arm moves to the cup holder, picks up the cup, and holds it throughout the process.
4.	Coffee Dispensing:
While holding the cup, the robot moves to the coffee machine and dispenses coffee into the cup.
5.	Sugar Addition:
If sugar is requested, the robot moves to the sugar dispenser and adds the selected number of scoops into the cup while holding it.
6.	Water & Milk Dispensing:
The robot then moves to the water & milk dispensers and fills the cup with the required amount.
7.	Mixing:
The robot activates the mixer to blend the coffee, sugar, and water thoroughly.
8.	Return to Rest:
After completing the process, the robot places the cup back (if required) and returns to its rest position, ready for the next operation.
