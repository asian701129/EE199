/*
,* Copyright © 2013, Malcolm Sparks <malcolm@congreve.com>. All Rights Reserved.
,*
,* A program to convert USB firing events from the Dream Cheeky 'Big Red Button' to MQTT events.
,*/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>




#include <errno.h>

#define LID_CLOSED 21
#define BUTTON_PRESSED 22
#define LID_OPEN 23


  int fd = 0;
  int i = 0, res = 0;
  char buf[256];

void write_for_some_reason()
{
    memset(buf, 0x0, sizeof(buf));
    buf[0] = 0x08;
    buf[7] = 0x02;
    
    int j = 0;
    int val;
    for (j = 0; j < 256; j++)
    {
       printf("The contents of the buffer at location %d is %d, okay?\n", j, buf[j]);
    }

    // Investigate this ASAP

    val = write(fd, &buf, 8);

    printf("The result of the write() was %d.\n", val);

    if (val < 0)
    {
      printf("%d\n", errno);
      perror("write");
      exit(1);
    }
    // Once we solve this we're good
}

void read_after_write()
{
    memset(buf, 0x0, sizeof(buf));
    res = read(fd, buf, 8);
    printf("The result of the read() was %d.\n", res);
    if (res < 0)
    {
      printf("%d\n", errno);
      perror("read");
      exit(1);
    }
}

int main(int argc, char **argv)
{

  printf("Test\n");
  /* Use a udev rule to make this device */
  fd = open("/dev/big_red_button", O_RDWR);
  
  if (fd < 0)
  {
    //printf ("Unable to open device");
    perror("Unable to open device");
    return 1;
  }
  printf("The open() function assigned the number %d to refer to the big red button.\n",fd);

  int prior = LID_CLOSED;
  
  while (1)
  {
    i++;
    printf("Infinite Loop #%d\n", i);
    write_for_some_reason();
    read_after_write();

    if (res >= 0)
    {
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
    usleep(20000); /* Sleep for 20ms*/
  }
}



