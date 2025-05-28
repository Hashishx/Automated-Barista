# Automated-Barista
 A fully Autonomous robot arm that serves a coffee from A to Z based on Arduino
 
Main components:
1. Arduino Uno
2. Servo driver 16 channels
3. Mechnical design & 3d printed parts for robot and containers
4. CNC Base
5. Jumper wires
6. Powwer supply
7. Relay Modules
8. Water pumps
9. LCD 20 * 4
10. Push buttons & switches
11. Dispensers
12. Mixer

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
