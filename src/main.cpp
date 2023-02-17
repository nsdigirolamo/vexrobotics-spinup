/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Nicholas DiGirolamo                                       */
/*    Created:      Thu Feb 16 2023                                           */
/*    Description:  UDEVX Team 2 Competition Code                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// const gearSetting RED_GEAR_CART = ratio36_1;
const gearSetting GREEN_GEAR_CART = ratio18_1;
// const gearSetting BLUE_GEAR_CART = ratio6_1;

competition compete;

controller primary_controller = controller(primary);

motor intake = motor(PORT1, GREEN_GEAR_CART, false);

motor rear_left_motor = motor(PORT11, GREEN_GEAR_CART, false);
motor front_left_motor = motor(PORT12, GREEN_GEAR_CART, false);
motor_group left_motor_group = motor_group(rear_left_motor, front_left_motor);

motor rear_right_motor = motor(PORT19, GREEN_GEAR_CART, true);
motor front_right_motor = motor(PORT20, GREEN_GEAR_CART, true);
motor_group right_motor_group = motor_group(rear_right_motor, front_right_motor);

double wheel_travel = 3.25;       // Circumference of the wheel.
double track_width = 12.0;        // Distance between the left and right wheels.
double wheel_base = 10.0;         // Distance between wheels on the same side.
double external_gear_ratio = 1.0; // Gear ratio.
drivetrain drive_train = drivetrain(left_motor_group, right_motor_group, wheel_travel, track_width, wheel_base, distanceUnits::in, external_gear_ratio);

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {

  bool is_left_stopped = true;
  bool is_right_stopped = true;
  bool is_intake_stopped = true;

  intake.setVelocity(100, percent);

  while (true) {

    // Left drive train control
    int left_drive_speed = primary_controller.Axis3.position();
    if (5 < abs(left_drive_speed)) {
      left_motor_group.setVelocity(left_drive_speed, percent);
      left_motor_group.spin(forward);
      is_left_stopped = false;
    } else if (!is_left_stopped) {
      left_motor_group.stop();
      is_left_stopped = true;
    }

    // Right drive train control
    int right_drive_speed = primary_controller.Axis2.position();
    if (5 < abs(right_drive_speed)) {
      right_motor_group.setVelocity(right_drive_speed, percent);
      left_motor_group.spin(forward);
      is_right_stopped = false;
    } else if (!is_right_stopped) {
      right_motor_group.stop();
      is_right_stopped = true;
    }

    // Intake control
    if (primary_controller.ButtonL1.pressing()) {
      intake.spin(forward);
      is_intake_stopped = false;
    } else if (primary_controller.ButtonL2.pressing()) {
      intake.spin(reverse);
      is_intake_stopped = false;
    } else {
      intake.stop();
      is_intake_stopped = true;
    }

    wait(20, msec);
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  compete.autonomous(autonomous);
  compete.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  // This was in the competition template. Is this really best practice?
  while (true) {
    wait(100, msec);
  }
}
