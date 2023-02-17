#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  Brain.Screen.print("Initializing...");
  Brain.Screen.newLine();
  // Nothing to initialize
  Brain.Screen.print("Initialization complete.");
  Brain.Screen.newLine();
}