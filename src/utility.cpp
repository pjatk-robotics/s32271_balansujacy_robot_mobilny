// #include <Wire.h>
// #include <Arduino.h>
// #include "steppers.h"
// #include "pad.h"
// #include <Preferences.h>
// #include "utility.h"

// Preferences prefs;  
// const int DEBUG_INTERVAL = 50; 
// unsigned long lastDebugTime = 0;

// void debug(float pitch,  unsigned long current_time ){
//       if (current_time - lastDebugTime >= DEBUG_INTERVAL)
//   {
//     // lastDebugTime = current_time;
//     // Serial.print("pitch=");
//     // Serial.print(pitch, 2);
//     // Serial.print(" | out=");
//     // Serial.print(output, 1);
//     // Serial.print(" | stpA=");
//     // Serial.print(stepsA);
//     // Serial.print(" | stpB=");
//     // Serial.print(stepsB);
//     // Serial.print(" | lY=");
//     // Serial.print(getLAxisY());
//     // Serial.print(" lX=");
//     // Serial.print(getLAxisX());
//     // Serial.print(" rY=");
//     // Serial.print(getRAxisY());
//     // Serial.print(" target=");
//     // Serial.print(targetSpeed);
//     // Serial.print(" curr=");
//     // Serial.println(currentSpeed);
//   }
// }

// void pid_adjust(){
//   if (Serial.available() > 0)
//   {
//     String input = Serial.readStringUntil('\n');
//     input.trim();

//     if (input.startsWith("p:"))
//     {
//       Kp = input.substring(2).toFloat();
//       Serial.print("Kp = ");
//       Serial.println(Kp);
//     }

//     else if (input.startsWith("i:"))
//     {
//       Ki = input.substring(2).toFloat();
//       Serial.print("Ki = ");
//       Serial.println(Ki);
//     }

//     else if (input.startsWith("d:"))
//     {
//       Kd = input.substring(2).toFloat();
//       Serial.print("Kd = ");
//       Serial.println(Kd);
//     }

//     else if (input.startsWith("s:"))
//     {
//       baseSetpoint = input.substring(2).toFloat();
//       Serial.print("Setpoint = ");
//       Serial.println(baseSetpoint);
//     }

//     else if (input == "pid")
//     {
//       Serial.print("Kp=");
//       Serial.print(Kp);
//       Serial.print(" Ki=");
//       Serial.print(Ki);
//       Serial.print(" Kd=");
//       Serial.println(Kd);
//     }
// else if (input == "save")
// {
//     prefs.putFloat("setpoint", baseSetpoint);
//     Serial.print("Zapisano pion: ");
//     Serial.println(baseSetpoint);
// }
//     else
//     {
//       Serial.println("Komendy: p:XX  i:XX  d:XX  s:XX  pid");
//     }
//   }
// }

