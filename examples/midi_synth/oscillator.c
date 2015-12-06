/**
 * @file oscillator.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2015, Erik Moqvist
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

#include "simba.h"
#include "oscillator.h"

/* Convert a floating point value to a fixed point Q26.5 value. */
#define FLOAT_TO_Q20_11(value) (q20_11_t)(value * 2048.0)

/* Convert an integer value to a fixed point Q26.5 value. */
#define INT_TO_Q20_11(value) (q20_11_t)(value << 11)

/* Convert a fixed point Q26.5 value to a float value. */
#define Q20_11_TO_FLOAT(value) ((float)(value) / 2048.0)

/* Convert a fixed point Q26.5 value to an integer value. */
#define Q20_11_TO_INT(value) (int32_t)((value >> 11))

static q20_11_t
frequency_to_phase_increment(float frequency,
                             size_t waveform_length,
                             int sample_rate)
{
    return FLOAT_TO_Q20_11(((frequency * waveform_length)
                            / sample_rate));
}

int oscillator_init(struct oscillator_t *self_p,
                    uint32_t *waveform_p,
                    size_t length,
                    float frequency,
                    float vibrato,
                    int sample_rate)
{
    self_p->sample_rate = sample_rate;
    self_p->sample_counter = 0;
    self_p->frequency = frequency;
    self_p->vibrato.value = vibrato;
    self_p->vibrato.phase_increment = FLOAT_TO_Q20_11(vibrato);
    self_p->phase = 0;
    self_p->phase_increment =
        frequency_to_phase_increment(frequency,
                                     length,
                                     sample_rate);
    self_p->waveform.length = length;
    self_p->waveform.buf_p = waveform_p;

    return (0);
}

int oscillator_set_frequency(struct oscillator_t *self_p,
                             float frequency)
{
    self_p->frequency = frequency;
    self_p->phase_increment =
        frequency_to_phase_increment(frequency,
                                     self_p->waveform.length,
                                     self_p->sample_rate);

    return (0);
}

int oscillator_read(struct oscillator_t *self_p,
                    uint32_t *samples_p,
                    size_t length)
{
    int i;
    int index;

    for (i = 0; i < length; i++) {
        if ((self_p->sample_counter % 4096) == 0) {
            self_p->vibrato.value *= -1.0;
            self_p->vibrato.phase_increment =
                FLOAT_TO_Q20_11(self_p->vibrato.value);
        }
        
        self_p->phase += self_p->phase_increment;
        self_p->phase += self_p->vibrato.phase_increment;
        index = (Q20_11_TO_INT(self_p->phase)
                 & (self_p->waveform.length - 1));
        samples_p[i] = self_p->waveform.buf_p[index];

        self_p->sample_counter++;
    }
    
    return (0);
}
