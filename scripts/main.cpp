#include <cstdlib>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <linux/input.h>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

bool debugMode = false;

void keyPressHandler(int code, int value);
void changeBrightness(bool upDown, std::string inputFileString, int ammount);
int main() {
  int fd;
  struct input_event ie;

  // Open the input device file
  fd = open("/dev/input/event6", O_RDONLY);
  if (fd == -1) {
    perror("Error opening device");
    return EXIT_FAILURE;
  }

  if (debugMode)
    printf("Listening to /dev/input/event6...\n");

  // Read events in a loop
  while (1) {
    if (read(fd, &ie, sizeof(struct input_event)) ==
        (ssize_t)sizeof(struct input_event)) {
      // Print out the event details

      if (debugMode)
        printf("Type: %d, Code: %d, Value: %d\n", ie.type, ie.code, ie.value);
      keyPressHandler(ie.code, ie.value);
    } else {
      perror("Error reading");
      break;
    }
  }

  // Close the device file
  close(fd);
  return EXIT_SUCCESS;
}

void keyPressHandler(int code, int value) {
  switch (code) {
  case 224: {
    if (value != 0)
      changeBrightness(true, "/sys/class/backlight/acpi_video0/brightness", 5);
  } break;

  case 225: {
    if (value != 0)
      changeBrightness(false, "/sys/class/backlight/acpi_video0/brightness", 5);
  } break;

  case 229: {
    if (value != 1)
      changeBrightness(true,
                       "/sys/class/input/input6/device/leds/"
                       "apple\:\:kbd_backlight/brightness",
                       20);
  } break;
  case 230: {
    if (value != 1)
      changeBrightness(false,
                       "/sys/class/input/input6/device/leds/"
                       "apple\:\:kbd_backlight/brightness",
                       20);
  } break;
  }
}

void changeBrightness(bool upDown, std::string inputFileString, int ammount) {

  std::ifstream inputFile(inputFileString);

  // Check if the file is successfully opened
  if (!inputFile.is_open()) {
    std::cerr << "Error opening the file!" << std::endl;
  }

  std::string line; // Declare a string variable to store each
                    // line of the file

  // Read each line of the file and print it to the
  // standard output stream
  //
  //
  std::string brightness;

  if (debugMode)
    std::cout << "File Content: " << std::endl;
  while (getline(inputFile, line)) {
    if (line != "") {
      brightness = line;
    }

    if (debugMode)
      std::cout << line << std::endl;
  }

  // Close the file
  inputFile.close();

  int brightnessInt = std::atoi(brightness.c_str());
  std::ofstream ofs(inputFileString, std::ofstream::trunc);

  ofs << (upDown ? brightnessInt -= ammount : brightnessInt += ammount);

  ofs.close();
}
