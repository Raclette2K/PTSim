# PTSim

Simulation for a project called "PetTracker" made in High School.
The goal of this project was to track people's pets using an embedded device.

## How this should have worked

This project would have worked this way:

1. A small device is attached to a collar on the pet, for instance a dog or better, a cat.  
2. The device is getting its GPS position every minute and logging them on a storage device onboard.  
3. The device uses the LoRa protocol to communicate its last positions to a server which, in its turn, logs them  
4. The pet owner can then log onto a website and see a GPS map of his pet's route.  

## How it ended up

Due to time constraints and lack of resources the project couldn't be completed.
Furthermore, the LoRa receiver coulnd't work reliably as the API for it was very lacking. 

In the end, the communication was simulated using virtual COM ports as the receiver was working this way.
