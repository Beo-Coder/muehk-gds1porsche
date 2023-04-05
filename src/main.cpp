#include <Arduino.h>
#include <Joystick.h>
#include <EEPROM.h>
#include <Axis.h>
#include <Button.h>



#define X_AXIS_PIN A0
#define Y_AXIS_PIN A1
#define Z_AXIS_PIN A0


#define EEPROM_ADDRESS_CALIBRATION_DATA_X_AXIS_BEGIN 0x00
#define EEPROM_ADDRESS_CALIBRATION_DATA_Y_AXIS_BEGIN 0x06
#define EEPROM_ADDRESS_CALIBRATION_DATA_Z_AXIS_BEGIN 0x0C


Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, 1, 0,
                   true, true, true, false, false,
                   false, false, false, false, false, false);



Axis xAxis(X_AXIS_PIN, "x-Achse", true, false,EEPROM_ADDRESS_CALIBRATION_DATA_X_AXIS_BEGIN);
Axis yAxis(Y_AXIS_PIN, "y-Achse", true, false, EEPROM_ADDRESS_CALIBRATION_DATA_Y_AXIS_BEGIN);
Axis zAxis(Z_AXIS_PIN, "z-Achse", false, true,EEPROM_ADDRESS_CALIBRATION_DATA_Z_AXIS_BEGIN);

Button button(9, true, true);

void setup() {

    pinMode(9, INPUT_PULLUP);
    Serial.begin(9600);
    Serial.println("begin");


    Joystick.setXAxisRange(-1028, 1028);
    Joystick.setYAxisRange(-1028, 1028);
    Joystick.setZAxisRange(-1028, 1028);

    /*
    xAxis.calibrateNew(9, true);
    yAxis.calibrateNew(9, true);
    zAxis.calibrateNew(9, true);
    */

    xAxis.loadCalibration();
    yAxis.loadCalibration();
    zAxis.loadCalibration();


    Joystick.begin();
    delay(100);
    Joystick.setXAxis(0);
    Joystick.setYAxis(0);
    Joystick.setZAxis(0);

}


int lastButtonState = 0;


void loop() {

    if(xAxis.valueChanged()){
        Joystick.setXAxis(xAxis.getValue());
    }
    if(yAxis.valueChanged()){
        Joystick.setYAxis(yAxis.getValue());
    }
    if(zAxis.valueChanged()){
        Joystick.setZAxis(zAxis.getValue());
    }
    if(button.stateChanged()){
        Joystick.setButton(0,button.getState());
    }


}