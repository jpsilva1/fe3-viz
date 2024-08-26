# Fe<sup>3</sup> Visualizer

## Overview
The original Fe<sup>3</sup> project is a web portal that takes data from flight plans and wind fields to predict flight trajectories, wind vectors and traffic density heat maps. With this information, vehicles in close proximity can detect and avoid each other. We used this trajectory data to build a 3D visualizer, which includes a variety of features to allow the user to control the simulation. This simulation was created with Unreal Engine 5, Cesium for Unreal, Visual Studio, and Autodesk Maya.

## How to install on Windows
1. Install Unreal Engine 5.4.3 through Epic Games launcher
    * If you want to edit the code, also install Visual Studio 2022
    * Recommended compiler version is 14.38.33130 but it should work with any version other than 14.39
2. From Epic Games Launcher, go to Marketplace then search for Cesium plugin and install to the engine
3. Clone this repository (or download zip)
4. Go to cloned directory and find Project.uproject file
5. Right click and hit open with Unreal Engine
    * This will create new folders and files, then launch the project
  
## How to use the simulator
1. Go to Fe<sup>3</sup> web portal and run a simulation
2. Download .txt trajectory file
     * Optionally also download full zip and save flight_id_mapping_list.txt
     * Can also use .csv file in following format: simulation time, plane id, plane type, latitude, longitude, altitude, destination, active
### Controls
![image](https://github.com/user-attachments/assets/c33b3a4c-cc44-41a2-a31e-1367209019fb)
From left to right, top to bottom:
* Vehicle dropdown: contains different options for 3D models of various airborne vehicles
        * Default is commercial airplane
* Plane dropdown: once files are uploaded, contains numbers corresponding to each vehicle in the scene. Select one to fly to its position
    * If you click '1' on your keyboard, you will follow the plane selected in the dropdown as it moves
    * Clicking '1' again will stop following the plane
* Playback dropdown: has options for different playback speeds and reverse playback
* Play/pause button: toggles whether the planes are moving along trajectory
* Time slider: if play button is enabled, moves automatically along playback speed. Can also slide it manually.
* Weather: has different options for weather effects
    * Currently has rain and snow
    * Warning - slows down simulator and takes a few seconds to load since they are particle effects
* Georeference dropdown: contains options for different airports
    * Default is SFO
    * If file uploaded is .txt file from web portal (and uses cartesian coordinates), planes will move relative to new airport
    * If file uploaded is .csv file (and uses latitude/longitude coordinates), planes will stay at given latitude and longitude
* File pickers: top file picker is for either .txt file from web portal or .csv file, bottom file picker is for flight_id_mapping_list.txt
    * For top file picker, if .csv file is used the columns must be in this order: simulation time, plane id, plane type, latitude, longitude, altitude, destination, active
    * Bottom file picker is optional, it only uses the .txt file to change the vehicle types
* Altitude slider: slide up/down to change player altitude
* Fog density slider: slide up/down to increase/decrease fog level
* Live data button: click to use live data from NASA server
    * Warning - still in progress
* Controls: click to toggle controls menu
