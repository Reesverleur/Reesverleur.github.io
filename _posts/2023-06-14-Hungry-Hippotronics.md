---
layout: post
title: Hungry Hippotronics
subtitle: Cal Poly ME 507 Final Project
thumbnail-img: /assets/img/competition-ring.png
tags: [engineering, robotics]
---
This project aimed to create a robot that could autonomously collect & sort balls in a competition. This was the final project for Cal Poly SLO's Mechanical Control System Design course. Students learned to use Fusion 360 to design a circuit board and program in C and C++ on integrated hardware in order to create the final project. 

The project began by learning how to use Fusion 360 to create an electrical design. I had never done this before, but I was eager to learn. These skills were first used in a homework assignment to design an 8-bit dip-switch "DAC." It was challenging to get used to new CAD software and consider the intricacies of a new type of design, but in the end, I was able to create the circuit you can see below.

![Homework 1 Routing][Homework 1 image]

This circuit is very simple, and my routing is not optimal, but as my first try using electrical CAD software, I am happy with the results.

Next, we moved on to creating custom parts by making a minimum functional module for the microcontroller we planned to use, an STM32F411. The minimum functional module provided a good stepping off point for the creation of our final design. The feedback I got from the instructor on this design largely informed my final design for the class. This schematic is shown below.

![Homework 4 Routing][Homework 4 image]

Finally, the time came to design the final board. Using what I had learned so far in the course, along with the instructor feedback on the first revision of my circuit, I created my final design. This design included connectors to attach my motors and sensors and set my pin allocation for the final project. This design can be seen below.

![Final Routing][Final Schematic]

After receiving this board from the manufacturer, we added all the components we selected to get the final result.

![Circuit Board][Board Image]
After assembling, I tested the board to determine that it worked exactly as expected, with all the voltage rails at the correct values and the MCU running code as expected. After everything checked out, I moved on to developing the code for the project.

The code for this project was written in C++ and made use of the HAL API for the STM32F411. The code was written in the STM32 Cube IDE which made selecting pin functions easy. The code for this project is not complete, as the project as a whole fell short. As such, I do not have a main function loop running a finite state machine. One was planned and would have used the FSM below or a very similar one.
![Finite State Machine][fsm]


A full code listing is available [here][code], and complete documentation can be found [here][docs]








[Homework 1 image]: /assets/img/Homework-1-Schematic.png
[Homework 4 image]: /assets/img/Homework-4-Schematic.png
[Final Schematic]: /assets/img/Final-Board-Schematic.png
[Board Image]: /assets/img/circuit-board.jpg
[fsm]: /assets/img/fsm.png
[code]: https://github.com/Reesverleur/Reesverleur.github.io/tree/master/ME_507_code/Final-Project-V1
[docs]: /ME_507_code/Final-Project-V1/Core/docs/html/index.html

