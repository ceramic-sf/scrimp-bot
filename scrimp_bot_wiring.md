## Scrimpt bot wiring

This is the wiring diagram for scrimp bot hardware. There are five stepper motors (2 sets of 2 muscle motor modules for arms and one for the claw), each with a linear potentiometer for control.

```mermaid
graph LR
    Laptop -- "SSH" --> Script

    subgraph "24v-34v power supply"
        PowerSupply[Vmot Power]
        PowerGnd[Vmot Ground]
    end


    subgraph SERVER
        subgraph Peripherals
            USBArduino
            USBCamera1[USB Camera 1]
            USBCamera2[USB Camera 2]
            USBCamera3[USB Camera 3]
        end


        LLM[Transformer Model]
            subgraph Script [Python Script]
                Input[Accept phrase]
                Photos[Take photos]
                Model[Send to model]
                Wire[Transmit to Arduino]
                Stop[Model self terminates]
                Input --> Photos --> Model --> Wire --> Photos
                Wire --> Stop
            end
    end



    subgraph DIVERBOX[Driver Box]
        subgraph ARDUINO
        ArduinoDigital[Arduino digital]
        ArduinoAnalog[Arduino analog]
        ArduinoGnd[Arduino ground]
        ArduinoPower[Arduino 5v]
        end

        subgraph DRIVERS
        Driver1[TMC2208 Stepper Motor Driver 1]
        Driver2[TMC2208 Stepper Motor Driver 2]
        end

        IOPower[IO 5v]
        IOGnd[IO Ground]

        MotorPower
        MotorGnd
        Cap[100uF Capacitor]
    end



    subgraph CONTROLLER[Manual Drive Controller]
        subgraph POTENTIOMETERS[Variable voltage dividers]
        Pot1[Linear pot + 10kOhm 1]
        Pot2[Linear pot + 10kOhm 2]
        end
        ControllerPower
        ControllerGND
    end


    EStop[e-stop]



    subgraph MOTORS
    StepperMotor1[Stepper Motor 1]
    StepperMotor2[Stepper Motor 2]
    end

    USBArduino -- "USB Tx/Rx on D0/D1" --> ArduinoDigital
    ArduinoGnd --> IOGnd
    ArduinoPower --> IOPower
    IOGnd --> Driver1
    IOGnd --> Driver2
    IOGnd --> ControllerGND
    ControllerGND --> Pot1
    ControllerGND --> Pot2
    ArduinoDigital -- "STEP" --> Driver1
    ArduinoDigital -- "STEP" --> Driver2
    ArduinoDigital -- "DIR" --> Driver1
    ArduinoDigital -- "DIR" --> Driver2
    IOPower --> Driver1
    IOPower --> Driver2
    IOPower --> ControllerPower
    ControllerPower --> Pot1
    ControllerPower --> Pot2
    ArduinoAnalog --> Pot1
    ArduinoAnalog --> Pot2

    MotorPower --> Cap
    Cap --> MotorGnd
    PowerGnd --> MotorGnd
    PowerSupply --> EStop
    EStop --> MotorPower
    MotorPower --> Driver1
    MotorPower --> Driver2

    MotorGnd --> Driver1
    MotorGnd --> Driver2
    Driver1 -- "Motor Connections (1A, 1B, 2A, 2B)" --> StepperMotor1
    Driver2 -- "Motor Connections (1A, 1B, 2A, 2B)" --> StepperMotor2

```