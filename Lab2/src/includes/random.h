/*
 *  Copyright (C) 2014 Julien Rabinow <jnr305@nyu.edu>
 *
 *  This file is part of Lab2-Scheduler.
 *
 *  Lab2-Scheduler is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Lab2-Scheduler is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Lab2-Scheduler. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RANDOM_H
#define RANDOM_H

#include <utils.h>

void init_random(char *input_path);
int myrandom(int burst);
void delete_random(void);

#endif
