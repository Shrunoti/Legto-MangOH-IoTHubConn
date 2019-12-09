/*
 * AUTO-GENERATED interface.h for the sysfsGpio component.

 * Don't bother hand-editing this file.
 */

#ifndef __sysfsGpio_COMPONENT_INTERFACE_H_INCLUDE_GUARD
#define __sysfsGpio_COMPONENT_INTERFACE_H_INCLUDE_GUARD

#ifdef __cplusplus
extern "C" {
#endif

#include "api/4f33761436c6dc37adc956530e707788/client/le_cfg_interface.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin1_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin2_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin3_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin4_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin5_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin6_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin7_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin8_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin9_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin10_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin11_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin12_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin13_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin14_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin15_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin16_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin17_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin18_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin19_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin20_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin21_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin22_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin23_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin24_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin25_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin26_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin27_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin28_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin29_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin30_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin31_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin32_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin33_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin34_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin35_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin36_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin37_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin38_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin39_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin40_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin41_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin42_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin43_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin44_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin45_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin46_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin47_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin48_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin49_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin50_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin51_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin52_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin53_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin54_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin55_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin56_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin57_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin58_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin59_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin60_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin61_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin62_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin63_server.h"
#include "api/837f3de7ac4e328ef0f2f0b0b354939f/server/le_gpioPin64_server.h"

#ifdef __cplusplus
}
#endif

#endif // __sysfsGpio_COMPONENT_INTERFACE_H_INCLUDE_GUARD
