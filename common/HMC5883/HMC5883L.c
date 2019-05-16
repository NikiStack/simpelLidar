#include "./include/HMC5883L.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>

void hmc_magnetic_init(const int i2cAddress)
{
  int fd ;

  if ((fd = wiringPiI2CSetup (i2cAddress)) < 0)
    return fd ;

  return 0 ;
}