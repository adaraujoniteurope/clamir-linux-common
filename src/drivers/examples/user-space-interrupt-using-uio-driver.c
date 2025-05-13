//
// File .......... axi-gpio-zed-test.c
// Author ........ Steve Haywood
// Version ....... 1.0
// Date .......... 29 October 2022
// Description ...
//   Very simple program to access a GPIO device using the User Space I/O
// subsystem. Demonstrates a blocking wait on interrupt routine.
//

// url: https://spacewire.co.uk/tutorial/leds_switches_add_buttons_int

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define XGPIO_DATA_OFFSET           0x0
#define XGPIO_TRI_OFFSET            0x4
#define XGPIO_DATA2_OFFSET          0x8
#define XGPIO_TRI2_OFFSET           0xC
#define XGPIO_DATA3_OFFSET          0x10
#define XGPIO_TRI3_OFFSET           0x14
#define XGPIO_GIE_OFFSET            0x11C
#define XGPIO_ISR_OFFSET            0x120
#define XGPIO_IER_OFFSET            0x128
#define XGPIO_IR_MASK               0x7
#define XGPIO_IR_CH1_MASK           0x1
#define XGPIO_IR_CH2_MASK           0x2
#define XGPIO_IR_CH3_MASK           0x4
#define XGPIO_GIE_GINTR_ENABLE_MASK 0x80000000

#define BUTTON_CENTER 0x01
#define BUTTON_DOWN   0x02
#define BUTTON_LEFT   0x04
#define BUTTON_RIGHT  0x08
#define BUTTON_UP     0x10


// Read a value from a GPIO register.
unsigned int gpio_read(void *base, unsigned int offset)
{
  return *((volatile unsigned int *)(base + offset));
}


// Write a value to a GPIO register.
void gpio_write(void *base, unsigned int offset, unsigned int data)
{
  *((volatile unsigned int *)(base + offset)) = data;
}


// Rotate right
unsigned char ror(unsigned char num) {
  return (num >> 1) | (num << 7);
}


// Rotate left
unsigned char rol(unsigned char num) {
  return (num << 1) | (num >> 7);
}


// GPIO button decode & LED driver
void buttons_to_leds(void *base)
{
  unsigned int leds;
  unsigned int btns;
  unsigned int width = 0;

  // Obtain register values for LED's & buttons
  leds = gpio_read(base, XGPIO_DATA_OFFSET);
  btns = gpio_read(base, XGPIO_DATA3_OFFSET);

  // Print button status
  syslog(LOG_INFO,"Interrupt detected, button register = 0x%08x\n", btns);

  // Determine LED bar width
  for (unsigned int i=0; i<=7; i++) {
    if ((0b00000001 << i) & leds) {
      width++;
    }
  }

    // Set LED bar width to 4
    if (btns & BUTTON_CENTER) {
      leds = 0b00111100;
    }

    // Rotate LED's left
    if (btns & BUTTON_LEFT) {
      if (width == 0) {
        leds = 0b00000001;
      } else {
        leds = (unsigned char)rol(leds);
      }
    }

    // Rotate LED's right
    if (btns & BUTTON_RIGHT) {
      if (width == 0) {
        leds = 0b10000000;
      } else {
        leds = (unsigned char)ror(leds);
      }
    }

    // Increase illuminated LED's
    if (btns & BUTTON_UP) {
      if (width == 0) {
        leds = 0b00001000;
      } else if (width & 1) {
        leds = leds | rol(leds);
      } else {
        leds = leds | ror(leds);
      }
    }

    // Decrease illuminated LED's
    if (btns & BUTTON_DOWN) {
      if (width == 8) {
        leds = 0b01111111;
      } else if (width & 1) {
        leds = leds & rol(leds);
      } else {
        leds = leds & ror(leds);
      }
    }

  // Clear (all latched) Buttons
  gpio_write(base, XGPIO_DATA3_OFFSET, 0x0);

  // Update LED's
  gpio_write(base, XGPIO_DATA_OFFSET, leds);
}


// Wait for an interrupt from GPIO device
void wait_interrupt(int fd, void *base)
{
  unsigned int pending = 0;
  unsigned int reenable = 1;
  unsigned int data;

  // Wait for User Space interrupt (blocking read)
  read(fd, (void *)&pending, sizeof(pending));

  // Read GPIO Interrupt Status Register
  data = gpio_read(base, XGPIO_ISR_OFFSET);

  // Check for interrupt on GPIO Channel 3 & clear if present
  if (data == XGPIO_IR_CH3_MASK)
    gpio_write(base, XGPIO_ISR_OFFSET, XGPIO_IR_CH3_MASK);

  // Decode buttons & drive LEDs
  buttons_to_leds(base);

  // Re-enable User Space interrupt
  write(fd, (void *)&reenable, sizeof(reenable));
}


// Get device driver memory size
unsigned int get_device_size(char *filename)
{
  FILE *fp;
  unsigned int size;

  // Open ASCII file
  fp = fopen(filename, "r");
  if (!fp) {
    syslog(LOG_INFO,"Error: Failed to open %s file\n", filename);
    exit(EXIT_FAILURE);
  }

  // Convert hexadecimal size string into a number
  fscanf(fp, "0x%08X", &size);

  // Close ASCII file
  fclose(fp);

  // Return device size
  return size;
}


// Main function
int main(int argc, char *argv[])
{
  char *devicename = "/dev/uio0";
  char *filename = "/sys/class/uio/uio0/maps/map0/size";
  int fd;
  unsigned int devicesize;
  void *deviceptr;

  // Open GPIO device with read/write access
  fd = open(devicename, O_RDWR);
  if (fd < 1) {
    syslog(LOG_INFO,"Error: Failed to open %s device\n", devicename);
    exit(EXIT_FAILURE);
  }

  // Get GPIO device memory size
  devicesize = get_device_size(filename);

  // Map GPIO device memory region
  deviceptr = mmap(NULL, devicesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (deviceptr == MAP_FAILED) {
    syslog(LOG_INFO,"Error: Failed to mmap");
    exit(EXIT_FAILURE);
  }

  // Print header
  syslog(LOG_INFO,"Simple User Space device driver example with interrupts\n\n");
  syslog(LOG_INFO,"Device in use ... %s\n", devicename);
  syslog(LOG_INFO,"Memory size ..... %u (obtained from %s)\n\n", devicesize, filename);
  syslog(LOG_INFO,"Use the buttons to control the LED's :-\n\n");
  syslog(LOG_INFO,"Centre - Reset LED's to 00111100\n");
  syslog(LOG_INFO,"Left   - Rotate LED's left\n");
  syslog(LOG_INFO,"Right  - Rotate LED's right\n");
  syslog(LOG_INFO,"Up     - Increase illuminated LED's\n");
  syslog(LOG_INFO,"Down   - Decrease illuminated LED's\n\n");
  syslog(LOG_INFO,"Press Ctrl-C to quit application\n");

  // Enable Interrupts for GPIO Channel 3
  gpio_write(deviceptr, XGPIO_GIE_OFFSET, XGPIO_GIE_GINTR_ENABLE_MASK);
  gpio_write(deviceptr, XGPIO_IER_OFFSET, XGPIO_IR_CH3_MASK);

  // Wait for Interrupt
  while (1) {
    wait_interrupt(fd, deviceptr);
  }

  // Disable Interrupts for GPIO Channel 3
  gpio_write(deviceptr, XGPIO_GIE_OFFSET, 0x0);
  gpio_write(deviceptr, XGPIO_IER_OFFSET, 0x0);

  // Unmap GPIO device memory
  munmap(deviceptr, devicesize);

  // Close GPIO device
  close(fd);

  return EXIT_SUCCESS;
} 