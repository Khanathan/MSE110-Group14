// Contains relevant code examples for reference
// NOT TO BE COMPILED AND RUN
task main()
{
    // Text display and wait for 8 seconds
    displayCenteredTextLine(4, "Hello World");
    sleep(8000);
    eraseDisplay();

    //  Reflection
    int Light_grade;
    long redValue;
    long greenValue;
    long blueValue;
    Light_grade = getColorReflected(Light_Sensor); // This command measures the value returned to the color sensor with its red LED turned off
    // Values returned by the color sensor typically range from 0 to 100 -Dark objects return lower values
    displayStringAt(5, 100, "%d", Light_grade);
    //	writeDebugStreamLine("Colour detected: %d, %d, %d", redValue, greenValue, blueValue);

    // RGB Sensor
    // long redValue;
    // long greenValue;
    // long blueValue;
    getColorRGB(Light_Sensor, redValue, greenValue, blueValue);
    // Values returned by the color sensor typically range from 0 to 100 -Dark objects return lower values
    displayStringAt(3, 20, "%d", redValue);
    displayStringAt(4, 40, "%d", greenValue);
    displayStringAt(5, 60, "%d", blueValue);

    // Motor encoder
    // set to brake mode
    setMotorBrakeMode(LeftMotor, motorBrake);
    resetMotorEncoder(LeftMotor);
    int EnValue;
    while (1)
    {
        setMotorSpeed(LeftMotor, 30);
        EnValue = getMotorEncoder(LeftMotor);
        displayStringAt(40, 40, "%d", EnValue);
    }

    // Play a tone
    while (1)
    {
        playImmediateTone(2000, 200); // 100 duration In 30 msec =1000msec
        sleep(3000);
    }

    // Ultrasound
    float UltSou_grade;
    UltSou_grade = getUSDistance(UltSou_Sensor); // Returns the value of the Ultrasonic Distance sensor
    // Distance CM mode returns values in 0.1 cm increments. The sensor can detect objects up to 250.0 cm away in this mode
    displayStringAt(1, 100, "%f", UltSou_grade);
    //	displayCenteredTextLine(4, "%f",UltSou_grade);
}
