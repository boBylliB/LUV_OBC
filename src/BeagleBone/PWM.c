#include <mraa/pwm.h>
#include <stdio.h>

int convertPowerToPWM(int power_level) {
    return (power_level * 255); // Convert power level (-1 to 1) to duty cycle (-255 to 255)
}

// "main" function below: 
float PWM(float* power) {

    int dutyCycle[4];
    
    //GPIO Pin stuff
    mraa_pwm_context pwm; 
    mraa_gpio_dir(pwm0, MRAA_GPIO_OUT); //sets the pins to be outputs???
    mraa_gpio_dir(pwm1, MRAA_GPIO_OUT);
    mraa_gpio_dir(pwm2, MRAA_GPIO_OUT);
    mraa_gpio_dir(pwm3, MRAA_GPIO_OUT);
    
    //Gets duty cycle for each power input
    dutyCycle(0) = convertPowerToPWM(power(0));
    dutyCycle(1) = convertPowerToPWM(power(1));
    dutyCycle(2) = convertPowerToPWM(power(2));
    dutyCycle(3) = convertPowerToPWM(power(3));
    
    // series of if statements to determine what direction we're going in and initiaties? the pins 
    if (power(0) < 0){
      pwm0R = mraa_pwm_init(ReversePWM0, HIGH); //Reverse motor0
      pwm0 = mraa_pwm_init(PWM0); //EHRPWM1A, Pin 14 Left
      Dutycycle(0) = Dutycycle(0)*(-1); // inverting to output a positive number
      
    } else if(power(0) >= 0){
      pwm0 = mraa_pwm_init(PWM0); //EHRPWM1A, Pin 14 Left
      
    } else {
      printf("Break");
    }
    
    if (power(1) < 0){
      pwm1R = mraa_pwm_init(ReversePWM1, HIGH); //Reverse motor1
      pwm1 = mraa_pwm_init(PWM1); //EHRPWM1B, Pin 16 Left
      Dutycycle(1) = Dutycycle(1)*(-1); // inverting to output a positive number
      
    } else if(power(1) >= 0){
      pwm1 = mraa_pwm_init(PWM1); //EHRPWM1B, Pin 16 Left
      
    } else {
      printf("Break");
    }
    
    if (power(2) < 0){
      pwm2R = mraa_pwm_init(ReversePWM2, HIGH); //Reverse motor2
      pwm2 = mraa_pwm_init(PWM2); //EHRPWM2A, Pin 13 Right
      Dutycycle(2) = Dutycycle(2)*(-1); // inverting to output a positive number
      
    } else if(power(2) >= 0){
      pwm2 = mraa_pwm_init(PWM2); //EHRPWM2A, Pin 13 Right
      
    } else {
      printf("Break");
    }
    
    
    if (power(3) < 0){
      pwm3R = mraa_pwm_init(ReversePWM3, HIGH); //Reverse motor3
      pwm3 = mraa_pwm_init(PWM3); //EHRPWM2B, Pin 19 Right
      Dutycycle(3) = Dutycycle(3)*(-1); // inverting to output a positive number
      
    } else if(power(3) >= 0){
      pwm3 = mraa_pwm_init(PWM3); //EHRPWM2B, Pin 19 Right
      
    } else {
      printf("Break");
    }


    return Dutycycle;
}
