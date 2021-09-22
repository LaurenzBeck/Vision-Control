# Vision Control -  Bachelor Project Repository

![](/Documentation/fh_logo.jpg)

01.03.2019 - 21.01.2020

![](/Documentation/setup_stand.jpg)

![](/Documentation/13_11_mae_plot.PNG)

---

## Goal

This thesis is based on a project which took place during the fourth and fifth semesters of the Systems Engineering Bachelor's degree programme at the Carinthia University of Applied Science.

During this project, we designed built and trained a Machine Learning vision sensor that can be used in our university's control laboratory.

A servo motor block from Quanser, which has a long shaft attached to its axis closely resembling an analogue clock or a speedometer, was chosen as the main actor. This servo motor has an optical 12-bit quadrature encoder enabling rotation measurements with a step size of 0.088$^\circ $.
This motor is controlled by a small custom Micro Controller Unit (MCU) which provides an interface from a desktop PC to the motor hardware. The last part of the experimental setup is a small PSEye Camera mounted on a Franka Emika robot.
This camera, which is connected to the central PC via USB, takes images with a resolution of 480x640 pixels from the motor with a constant frame rate of 60fps.

The idea behind this setup is the automation of labelled data acquisition. Having an interface to both the camera and the motor allows the PC to save pictures with their current position values, as well as determining the next set point of the motor. With all parts communicating with each other no human intervention is required during acquisition. The robotic arm is constantly adjusting the position of the camera to get some variation in the camera's perspective into the data set. 

The scope of this project can be roughly split into 6 parts:

   + Building and programming the control unit of the motor.
   + Designing an automated data acquisition system.
   + Acquiring a large data set required for training.
   + Selecting a Machine Learning model suited for the given task.
   + Designing and conducting the training of the vision sensor.
   + Test the sensor and discuss the results.



## Team

### Students

+ Laurenz Hundgeburth
+ Gregor Fritz

### Supervisors

+ FH-Prof. Dipl.-Ing. Dr. Wolfgang Werth
+ Heinz-Peter Liechtenecker, BSc MSc



## Repository Structure

### CNN Training

Includes all the scripts, logs and files related to the Training of the Vision Sensor.

However, due to file size constraints on Github, the acquired data set is not included in this project, but can be downloaded from: ...

Once downloaded, the images simply need to be extracted in the corresponding folders.

### Data Acquisition

This folder includes both the microcontroller files for the motor control and the central scripts controlling the data acquisition.

### Documentation

The presentation, project report, poster and the two  Bachelor Thesis can be found here.

## Data Set ![](https://zenodo.org/badge/DOI/10.5281/zenodo.5519956.svg)

![](/CNN_Training/images_train/1571743900.829366_0.1.png)

The Dataset is available on Zenodo: https://zenodo.org/record/5519956

The following data sets were acquired for training:

| name       | size   |
| ---------- | ------ |
| Training   | 80.000 |
| Validation | 10.000 |
| Test       | 10.000 |

These sets consists of folders containing the images, and DataFrames containing both the filenames of every image and their corresponding labels.

## Contact

If you have any questions or comments, feel free to contact me:

Laurenz.Hundgeburth@edu.fh-kaernten.ac.at

<script src="https://platform.linkedin.com/badges/js/profile.js" async defer type="text/javascript"></script>
<div class="badge-base LI-profile-badge" data-locale="en_US" data-size="medium" data-theme="light" data-type="HORIZONTAL" data-vanity="laurenz-hundgeburth-50156b219" data-version="v1"><a class="badge-base__link LI-simple-link" href="https://at.linkedin.com/in/laurenz-hundgeburth-50156b219?trk=profile-badge">Laurenz Hundgeburth</a></div>
              
