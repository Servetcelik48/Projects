
Moving Camera Platform Project

In this project, STM32CudeIDE is used to program the STM32F303RE board.
This project is designed to address the challenge of minimizing blind spots while simultaneously reducing the
number of cameras required. Leveraging the versatile features of the STM32 microcontroller, including support
for various interfaces and peripherals like UART, SPI, and PWM signals, the project utilizes an STM32 board, 
a joystick, and two servo motors.

There are multiple modes in the project. The mode of operation is determined by user interrupts triggered by 
user button, allowing for flexibility in functionality. In sleep mode, the STM32 can conserve power and extend
the overall system lifespan while camera angle remaining constant. In manual mode, users can adjust the camera
angle using the joystick. The STM32 processes joystick input through the ADC and sends required PWM signals to 
control the servo motors accordingly.

Auto-scan mode offers automated coverage of large areas. In this mode, the STM32 continuously sends PWM signals
to the servo motors, enabling the platform to systematically scan and cover large regions. This functionality
enhances surveillance capabilities while reducing the need for constant manual intervention.

