/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Nicholas DiGirolamo                                       */
/*    Created:      Thursday, February 16, 2023                               */
/*    Description:  UDVEX Team 2 Competition Code                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// const gearSetting RED_GEAR_CART = ratio36_1;
const gearSetting GREEN_GEAR_CART = ratio18_1;
const gearSetting BLUE_GEAR_CART = ratio6_1;

competition compete;
brain my_brain;

controller primary_controller = controller(primary);

motor intake = motor(PORT1, GREEN_GEAR_CART, false);
motor strings = motor(PORT6, GREEN_GEAR_CART, false);
motor barrel = motor(PORT3, BLUE_GEAR_CART, false);
motor launcher = motor(PORT2, GREEN_GEAR_CART, false);

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
drivetrain drive_train = drivetrain(
  left_motor_group,
  right_motor_group,
  wheel_travel,
  track_width,
  wheel_base,
  distanceUnits::in,
  external_gear_ratio
);

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
  my_brain.Screen.print("Pre-Autonomous start!");
  my_brain.Screen.newLine();
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  my_brain.Screen.print("Pre-Autonomous complete.");
  my_brain.Screen.newLine();
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
  my_brain.Screen.print("Autonomous start!");
  my_brain.Screen.newLine();
  // Push discs forward
  left_motor_group.spin(forward);
  right_motor_group.spin(forward);
  wait(5.0, sec);
  left_motor_group.stop();
  right_motor_group.stop();
  // Move back from discs
  left_motor_group.spin(reverse);
  right_motor_group.spin(reverse);
  wait(8.0, sec);
  left_motor_group.stop();
  right_motor_group.stop();
  // Turn 90 degrees
  left_motor_group.spin(forward);
  right_motor_group.spin(reverse);
  wait(1.25, sec);
  right_motor_group.stop();
  left_motor_group.stop();
  // Reverse into barrel
  left_motor_group.spin(reverse);
  right_motor_group.spin(reverse);
  wait(1.0, sec);
  left_motor_group.stop();
  right_motor_group.stop();
  // Spin barrel
  barrel.spin(forward);
  wait(3.0, sec);
  barrel.stop();
  my_brain.Screen.print("Autonomous complete.");
  my_brain.Screen.newLine();
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
  my_brain.Screen.print("User Control start!");
  my_brain.Screen.newLine();

  bool is_intake_stopped = true;
  bool is_strings_stopped = true;
  bool is_barrel_stopped = true;
  bool is_launcher_stopped = true;
  bool is_left_stopped = true;
  bool is_right_stopped = true;

  intake.setVelocity(100, percent);
  strings.setVelocity(100, percent);
  barrel.setVelocity(100, percent);

  while (true) {

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

    // Strings control
    if (primary_controller.ButtonUp.pressing() && primary_controller.ButtonDown.pressing()) {
      strings.spin(forward);
      is_strings_stopped = false;
    } else {
      strings.stop();
      is_strings_stopped = true;
    }

    // Barrel Control
    if (primary_controller.ButtonR1.pressing()) {
      barrel.spin(forward);
      is_barrel_stopped = false;
    } else if (primary_controller.ButtonR2.pressing()) {
      barrel.spin(reverse);
      is_barrel_stopped = false;
    } else {
      barrel.stop();
      is_barrel_stopped = true;
    }

    // Launcher control
    if (primary_controller.ButtonA.pressing()) {
      launcher.spin(forward, 10, volt);
      is_launcher_stopped = false;
    } else if (primary_controller.ButtonX.pressing()) {
      launcher.spin(forward, 12, volt);
      is_launcher_stopped = false;
    } else if (primary_controller.ButtonB.pressing()) {
      launcher.stop();
      is_launcher_stopped = true;
    }

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
      right_motor_group.spin(forward);
      is_right_stopped = false;
    } else if (!is_right_stopped) {
      right_motor_group.stop();
      is_right_stopped = true;
    }

    wait(20, msec);
  }
  my_brain.Screen.print("User Control complete.");
  my_brain.Screen.newLine();
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  my_brain.Screen.print("Main start!");
  my_brain.Screen.newLine();
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
  my_brain.Screen.print("Main complete.");
  my_brain.Screen.newLine();
}
