Instrument cluster [![License](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/BEASTSM96/flopnite/blob/v2/LICENSE)
===

Instrument cluster v1 is a project developed and managed by Pravaig Dynamics.
The main objective is to create a cluster that will display some data related
to the current state of the vehicle for the driver. This project is developed
in Unreal Engine 4.27.2 and is deployed in Linux Arm 64 - architecture systems.

## Architecture overview
The architecture of the project is made with combination and integration of
tools like Unreal Blueprints, Unreal Widgets and Unreal C++. Unreal engine
provides HMI\_Kit which is also used in this project.

* **Instrument Cluster_Cpp**: contains all the vehicle realted variables used
in UI Widgets and sends/receives data to/from the server application.

* **Socket Communication_Cpp**: handles back-end communication to the server.

## License
This project is licensed under the MIT License - see the LICENSE.md file for details.
