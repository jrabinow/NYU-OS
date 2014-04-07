/*
 *  Copyright (C) 2014 Julien Rabinow <jnr305@nyu.edu>
 *
 *  This file is part of Lab3-Mem-manager.
 *
 *  Lab3-Mem-manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Lab3-Mem-manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Lab3-Mem-manager. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAIN_H
#define MAIN_H

/* standard header files */
#include <getopt.h>

/* header files that are specific to this project (passed to gcc with -I option) */
#include <vmm.h>
#include <utils.h>

#define ERR_MSG			"Error: invalid VMM algorithm specified\n\n"

int parse_args(int argc, char *argv[], VMM *mmu, Verbose_Flag *vflags);

#define new(builder, ...)\
	((__##builder##__).lt->new((const Builder) &__##builder##__,\
	##__VA_ARGS__))
#define	delete(obj)		lt->delete(obj);
#define clone(obj)		lt->clone(obj);
#define print(obj)		lt->print(obj);
#define read_input(vmm, file)	lt->read_input(vmm, file)
#define run(vmm, flags, trace)	lt->run(vmm, flags, trace);

#endif
