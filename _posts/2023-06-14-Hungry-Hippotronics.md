---
layout: post
title: Hungry Hippotronics
subtitle: Cal Poly ME 507 Final Project
post_description: This project aimed to create a robot that could autonomously collect & sort balls in a competition. This was the final project for Cal Poly SLO's Mechanical Control System Design course. Students learned to use Fusion 360 to design a circuit board and program in C and C++ on integrated
thumbnail-img: /assets/img/competition-ring.png
tags: [engineering, robotics]
---
### This project has [**code**][code] and [**documentation**][docs] be sure to check those out! ###

This project aimed to create a robot that could autonomously collect & sort balls in a competition. This was the final project for Cal Poly SLO's Mechanical Control System Design course. Students learned to use Fusion 360 to design a circuit board and program in C and C++ on integrated hardware in order to create the final project. 

The project began by learning to use Fusion 360 to create an electrical design. I had never done this before, but I was eager to learn. These skills were first used in a homework assignment to design an 8-bit dip-switch "DAC." It was challenging to get used to new CAD software and consider the intricacies of a new type of design, but in the end, I was able to create the circuit you can see below.

![Homework 1 Routing][Homework 1 image]

This circuit is very simple, and my routing is not optimal, but as my first try using electrical CAD software, I am happy with the results.

Next, we moved on to creating custom parts by making a minimum functional module for the microcontroller we planned to use, an STM32F411. The minimum functional module provided a good stepping-off point for the creation of our final design. The feedback I got from the instructor on this design largely informed my final design for the class. This schematic is shown below.

![Homework 4 Routing][Homework 4 image]

Finally, the time came to design the final board. Using what I had learned so far in the course, along with the instructor's feedback on the first revision of my circuit, I created my final design. This design included connectors to attach my motors and sensors and set my pin allocation for the final project. This design can be seen below.

![Final Routing][Final Schematic]

After receiving this board from the manufacturer, we added all the components we selected to get the final result.

![Circuit Board][Board Image]
After assembling, I tested the board to determine that it worked exactly as expected, with all the voltage rails at the correct values and the MCU running code as expected. After everything checked out, I moved on to developing the code for the project.

The code for this project was written in C++ and made use of the HAL API for the STM32F411. The code was written in the STM32 Cube IDE, which made selecting pin functions easy. The code for this project is not complete, as the project as a whole fell short. As such, I do not have a main function loop running a finite-state machine. One was planned and would have used the FSM below or a similar one.

![Finite State Machine][fsm]

To allow for the easy polling of sensor data and control of peripherals, I wrote several C++ classes for sensor and motor control. 

The first of these was a class to allow for the use of a Pixy2 camera. This camera performs onboard image processing and can detect objects based on color. I wrote a simple class that sends the correct request data over a Serial Peripheral Interface (SPI) connection and listens for a response. When a response is captured, the data can be checked using a checksum to be sure of fidelity, and then the data is processed and can be stored in public variables to be accessed by the main program loop at a later time. I gave this code to several classmates since others also used a Pixy2 camera. This class can be seen in action in the video below.

{% include youtube.html id="Dv3oLQeqjDw" %}

I also wrote a driver for an I2C-based color sensor. This presented a challenge because the I2C protocol appeared to be incorrectly implemented on the device I was using. This deviation from the standard was not noted in the documentation and was extremely difficult to find/correct. Eventually, I was able to get the sensor to return color data which seemed to correlate correctly with the color placed in front of the sensor. This class can be seen in the video below, along with a Bluetooth UART debugging line.

{% include youtube.html id="gWmsTxVh03Y" %}

To control the motors, I created a motor driver to work with the motor driver ICs I chose when designing the board. This driver allows for directly controlling the motor effort (±100%). Or allows for position control with one of two algorithms. The first is a standard PID controller. The second implements a two-state controller, which allows for the integral action to be turned on only when the system gets close to the final position. This means the integral error term does not accrue while the system is saturated from the proportional gain. Testing showed that the two-state system had a significantly better response and did not overshoot as the more traditional system was prone to do. This motor driver class can be seen in the below video.

{% include youtube.html id="QQV3AZaJCNU" %}

Finally, to communicate with the board, I wrote a class that used a standard communication protocol over UART to communicate with a Python program running on my laptop which could ping a [classmates][nathanurl] system to detect ball locations over the entire arena. This system provided the thumbnail image for this project.

A full code listing is available [here][code], and complete documentation can be found [here][docs]

My groupmate, Jack,  was primarily responsible for creating the mechanical system. His portfolio for this class can be found [here][jackurl]. The CAD for our mechanical system can be seen below.

![System-Cad][CAD]

Due to commitments to our senior project course, Jack and I were not able to complete this project by the deadline for the class. We also had a last-minute electrical failure where the 5V rail started outputting 10.7 V. This burned out one of our servos and made it impossible to plug in any of our 5V sensors without damaging them. I tried replacing the 5V switching regulator, but this did not fix the problem, so I think there must have been a short downstream somewhere. I was not able to find it, but the troubleshooting we did ate up most of my remaining time, during which I was planning on finishing the code and helping with the mechanical assembly. 

Despite the status of the project, I learned a lot in this class. I had never designed or assembled a circuit board before, and I also had never programmed anything in C++ before (except for some very simple Arduino projects). Overall, I am happy with how this project went, and I am looking forward to applying the skills I learned to my future projects.








[Homework 1 image]: /assets/img/Homework-1-Schematic.png
[Homework 4 image]: /assets/img/Homework-4-Schematic.png
[Final Schematic]: /assets/img/Final-Board-Schematic.png
[Board Image]: /assets/img/circuit-board.jpg
[fsm]: /assets/img/fsm.png
[CAD]: /assets/img/CAD.png
[nathanurl]: www.google.com
[jackurl]:  www.google.com
[code]: https://github.com/Reesverleur/Reesverleur.github.io/tree/master/ME_507_code/Final-Project-V1
[docs]: /ME_507_code/Final-Project-V1/Core/docs/html/index.html

