/*
,* Copyright © 2013, Malcolm Sparks <malcolm@congreve.com>. All Rights Reserved.
,*
,* A program to convert USB firing events from the Dream Cheeky 'Big Red Button' to MQTT events.
,*/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LID_CLOSED 21
#define BUTTON_PRESSED 22
#define LID_OPEN 23

int main(int argc, char **argv)
{
  int fd;
  int i, res, desc_size = 0;
  char buf[256];

  printf("We are now running the program successfully.\n");

  /* Use a udev rule to make this device */
  fd = open("/dev/big_red_button", O_RDWR|O_NONBLOCK);

  printf("We are testing if we can open the device or not.\n");
  
  if (fd < 0) {
    perror("Unable to open device\n");
    return 1;
  }

  printf("It appears we are able to open the device.\n");

  int prior = LID_CLOSED;

  printf("Now that the device is open we enter an infinite loop while we wait for input from the device.\n");

  int j = 0;

  while (1) {

    if (j == 0)
      {
        printf("I think this code is trying to reset the button.\n");
      }
/*    memset(buf, 0x0, sizeof(buf));
    buf[0] = 0x08;
    buf[7] = 0x02;

    res = write(fd, buf, 8);
    if (res < 0) {
      perror("write");
      exit(1);
    }
*/
    if (j == 0)
      {
        printf("I think this code is reading the state of the button.\n");
      }

    memset(buf, 0x0, sizeof(buf));
    res = read(fd, buf, 8);

    if (res >= 0) {
      if (prior == LID_CLOSED && buf[0] == LID_OPEN) {
         printf("Ready to fire!\n");
         fflush(stdout);
      } else if (prior != BUTTON_PRESSED && buf[0] == BUTTON_PRESSED) {
        printf("Fire!\n");
        fflush(stdout);
      } else if (prior != LID_CLOSED && buf[0] == LID_CLOSED) {
        printf("Stand down!\n");
        fflush(stdout);
      }
      prior = buf[0];
    }

    if (j == 0)
      {
        j = 1;
      }
    usleep(20000); /* Sleep for 20ms*/
  }
}

