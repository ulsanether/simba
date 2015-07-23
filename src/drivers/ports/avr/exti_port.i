/**
 * @file exti_port.i
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

#include <avr/interrupt.h>

#define PIN(sfr) ((sfr) + 0)
#define DDR(sfr) ((sfr) + 1)
#define PORT(sfr) ((sfr) + 2)

ISR(INT0_vect)
{
    struct exti_driver_t *drv_p = exti_device[0].drv_p;

    if (drv_p != NULL) {
        drv_p->on_interrupt(drv_p->arg_p);
    }
}

ISR(INT1_vect)
{
    struct exti_driver_t *drv_p = exti_device[1].drv_p;

    if (drv_p != NULL) {
        drv_p->on_interrupt(drv_p->arg_p);
    }
}

static int exti_port_start(struct exti_driver_t *drv_p)
{
    struct exti_device_t *dev_p = drv_p->dev_p;

    dev_p->drv_p = drv_p;
    *DDR(dev_p->pin_p->sfr_p) &= ~(dev_p->pin_p->mask);
    EICRA |= (drv_p->trigger << (2 * dev_p->id));
    EIFR = _BV(dev_p->id);
    EIMSK |= _BV(dev_p->id);

    return (0);
}

static int exti_port_stop(struct exti_driver_t *drv_p)
{
    struct exti_device_t *dev_p = drv_p->dev_p;

    EIMSK &= ~_BV(dev_p->id);
    drv_p->dev_p = NULL;

    return (0);
}

static int exti_port_clear(struct exti_driver_t *drv_p)
{
    /* Clear the interrupt flag. */
    EIFR = _BV(drv_p->dev_p->id);

    return (0);
}
