# PokeRoyale
PokeRoyale is a two player hardware based game where two Pokemons (Rowlet and Psyduck) are in a battle. The battle starts once the two Pokemons get physically connected together. When the characters are connected they interact through two butt connectors (in and out) that send signals based on button presses. The winner is the person who presses their Pokemon’s button first. The game is more fun when combined with a race or any other competition to get to the Pokemons. The project was conducted by myself and a fellow student with each of us producing the code, hardware and physical casing of our unit. I was responsible for the Rowlet unit and my fellow student was responsible for the Psyduck unit.

This repository holds all the software and hardware files to recreate this project. 


## Project buildup
Description of contents by folder:
- Casings: Holds 3D files (.stl & .mix) and 2D files (.svg) for the casings of both units.
- Code: Holds the Arduino code for both units
- PCBBoards: Holds Eagle .brd files for all pcb boards used in units.

## Characteristic Behaviors
Each Pokemon has its characteristic behaviors in and out of a battle.
These characteristics can be described as follows:
- In idle mode (out of battle) the behaviors are not dependent on the other player’s actions. For example, Rowlet’s player can change the character’s color by holding down their button and releasing it when the desired color is displayed; Psyduck changes its color once per button press.
-	Fight mode (in battle) starts once the two pieces are connected and the characteristic behaviors depend on the outcome of the battle. 
  -	When Rowlet strikes first (player presses their button first and wins the battle) it'll flap its wings using a solenoid and change its color to green. When Psyduck strikes first it'll change its color to purple.
  -	When Rowlet is defeated it’ll change its color to red. When Psyduck is defeated it'll change its color to red and play a noise from the buzzer.

## Hardware
The tech behind the Pokemons is fairly simple. 

Rowlet has 4 components communicating with each other: a 5V Adafruit Trinket microcontroller, a push button, a single RGB Smart Neopixel and a medium 5V push-pull solenoid with a diode (1N4001), a resistor (220\(\Omega\)) and a transistor (MPS2222A) for wiring.

Psyduck also has 4 components communicating with each other: a 5V Adafruit Trinket microcontroller, a push button, a single RGB Smart Neopixel and a piezo buzzer.

<img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/RowletWiring.PNG">
<img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/PsyduckWiring.PNG">

## Fabrication
The casing of the Pokemon units used several fabrication methods including laser cutting, 3D printing and milling. The 3D model for Rowlet and the bottom halves of the puzzle platforms were designed from scratch using Autodesk Fusion 360. The puzzles as well as the bottom half of Rowlet's character were printed out in white PLA using the Ultimaker 3 and everything else was printed using clear photopolymer resin using the Formlabs Form 2 printers. The top plate for both puzzle platforms was designed in 2D using Adobe Illustrator and cut out from 3mm clear acrylic using the Formlabs laser cutters. The custom made PCBs were designed using Eagle and milled out using the Bantam Tools Desktop PCB Milling Machine.

<img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/Rowlet3D.PNG"><img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/Psyduck3D.PNG"><img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/Puzzles3D.PNG">
<img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/RowletMainPCB.png"><img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/PsyduckMainPCB.png"><img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/OtherPCB.png">

## Software
The microprocessor that was used for this project was the Adafruit Trinket that reads Arduino code, therefore the software development was done in the Arduino IDE. The software works as follows: 
1.	Once the microprocessor boots up the software sets itself up, sets the emit communications pin to a high voltage (about 5V here) and enters idle mode. 
2.	In idle mode the pokemon is constantly checking whether the receiving communications pin has detected a high voltage signal or whether there has been a button press. 
  -	If Rowlet detects a long button press it will start randomly changing the color of the pokemon until the button is released. If Psyduck detects a button press it will randomly change the color of the pokemon once.
  -	If Rowlet detects a significant enough voltage on the receive pin it will enter fight mode. Same applys for Psyduck.
3.	Fight mode is similar to idle mode, but we’re listening for a low not high voltage signal and a single click instead of a long press.
  -	A button click indicates an attack action from the user. If no attack from the other player has been previously detected, a button click will set the emit pin to a low voltage for a short period of time and perform its winning action which in Rowlet’s case is changing its body color to green and flapping wings and in Psyduck's case is changing its body color to purple. 
  -	A low voltage on the receive pin might indicate either an attack from the other player or a disconnection. The difference is established with timing as during an attack the voltage will go low for just a short predetermined period while a disconnect will make the voltage low for a longer period. 
    -	If an attack from the other player is detected the pokemon will perform its loosing reaction. In Rowlet’s case that is changing its body color to red and in Psyduck's case it is changing its body color to red and emitting a sound from the buzzer. 
    -	If disconnect is detected the pokemons will go back to idle mode (step 2).

## Approach
We conducted this project in 3 stages: low-fidelity, medium-fidelity and high-fidelity prototyping. 

During low-fidelity prototyping we were ideating for concepts, drawing scetches for the ideas and crafting very rough casing ideas to better understand the design requirements for our idea. We used materials like foam and cardboard to see how the two units would physically come together and discussed simple interactions each unit could perform to establish a list of technologies to be incorporated. 

<img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/LowFidPrototype.PNG"><img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/LowFidCardboard.jpg">
<img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/LowFidDrawings.PNG"><img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/LowFidMeasure.jpg">

During medium-fidelity prototyping we started to create simple 3D models for the units, writing software for the microcontroller as well as using breadboards for wiring together hardware pieces. The 3D models were printed on the fastest settings and sometimes only partially to test physical connections. Software was mainly written to ensure that each piece of electronics could be controlled with the microcontroller. Using breadboards allowed us to easily change out components and wiring when we run into compatibility issues.

<img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/MedFid3D.PNG"><img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/MedFidBreadBoard.jpg"><img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/MedFidelityProtoype.PNG">

During high-fidelity prototyping we added more details to the previously created 3D models, started using more sophisticated materials, combined software together and changed out the breadboards to custom made PCBs. For example, we added screw bosses to the units to securely attach together different pieces, changed out cardboard to acrylic, soldered all the electronics to the PCBs and put everything together for the final prototype as it can be seen in this repository.

<img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/HighFidAssembly.PNG"><img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/FinalAttached.PNG">
<img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/FinalLit.jpg"><img src="https://github.com/BetyMehide/PokeRoyale/blob/master/Img/Final.PNG">

## Reflection
This project perfectly demonstrated the complexity of producing any hardware product. The final prototype was definitely not perfect and if we were to produce antoher iteration we would change the following aspects:
- Change the microcontroller to something a bit more powerful/reliable. We often ran into situations where the software would start acting up but would work perfectly again once reinstalled. 
- Use a thinner sheet of acrylik on the top of the puzzle parts. The button of the unit is placed underneath the acrylic layer with the intention that the user will slightly press on the top layer to get to the button. However, the current top layer is not flexible enough and is prone to breaking at the pressure point so a thinner more flexible layer could fix that issue.  
- Change the character print material to something that diffuses the light better and/or add more LEDs.
- Reevaluate the gap needed for the wing movement on Rowlet or create a mechanical transition system that would flap the wings along a longer distance. The current gaps on the sides are too long and are not being utilised in their full length.
- Use a thinner buzzer, increase the height of the inside of the puzzle piece or move the buzzer to a new location on Psyduck. Currently the buzzer sound can't really be heard as it's pressed against the top layer too hard.  
