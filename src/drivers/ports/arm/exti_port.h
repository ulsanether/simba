/**
 * @file drivers/exti.h
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#define EXTI_PORT_TRIGGER_BOTH_EDGES   0
#define EXTI_PORT_TRIGGER_FALLING_EDGE 1
#define EXTI_PORT_TRIGGER_RISING_EDGE  2

struct exti_driver_t;

struct exti_device_t {
    struct exti_driver_t *drv_p;
    struct pin_device_t *pin_dev_p;
};

struct exti_driver_t {
    struct exti_device_t *dev_p;
    uint8_t trigger;
    void (*on_interrupt)(void *arg_p);
    void *arg_p;
};
