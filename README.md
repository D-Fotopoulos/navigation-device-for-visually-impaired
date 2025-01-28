# navigation-device-for-visually-impaired

This project aims to create a simple, wearable arduino based device which senses obstacles for visually impaired users. The Device use time-of-flight sensors to determine the distance to the object, and plays a tone to indicate the obstacle to the user. The device plays a different tone to differentiate between obstacles in front of the user, and objects coming up behind the user. The device also indicates objects on the sides of the user by only playing a tone in the corresponding ear. 

The First prototype version of the device uses an arduino nano ESP32, which was chosen for its large amount of program memory. The device uses ams OSRAM TMF8820 dTOF sensors.
