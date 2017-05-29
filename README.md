# ESP8266-Arduino-Home-Automation
ESP8266 and Arduino based home automation system to control appliances from the internet.
In this project, 4 appliances (2 lights and 2 fans) and controlled them with the internet. 
I have also included a current sensor which displays the current usage on the website. The current sensor used is ACS-712.
There is a fire sensor which is attached to the buzzer (look arduino code for sensor and buzzer pin). You can hook-up one of the fire alarms instead of the buzzer.
There is a PIR sensor that is included in the code, the PIR sensor can be switched ON and OFF from the code. (You do not want to get alerts when you are at home). 

#Arduino Code
The arduino code is commented to make changes wherever needed. Please read the comments.

#Email Notification
To get email notification whenever human activity is detected, edit the email address in esp.php file.

#Libraries
Download and include the Metro library. The library can be found at https://github.com/thomasfredericks/Metro-Arduino-Wiring
