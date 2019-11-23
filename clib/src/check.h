/*

Copyright (c) 2019-20208, Adrien BLASSIAU and Corentin JUVIGNY

Permission to use, copy, modify, and/or distribute this software
for any purpose with or without fee is hereby granted, provided
that the above copyright notice and this permission notice appear
in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

*/

/** @file check.h
 *
 * @brief function to check and initialize the main constants of our program.
 */

#ifndef __CHECK__
#define __CHECK__

/**
 * This function is used to check and set all the parameters
 * @param filename             The name of the file.
 * @param communication_radius The communication radius.
 * @param capture_radius       The capture radius.
 * @param size                 The size of the grid.
 * @param phi                  The phi parameter of the simulated annealing.
 * @param step                 The step of the simulated annealing.
 * @param T_initial            The initial temperature of the simulated
 *                             annealing.
 * @param nb_iterations        The number of iterations of the simulated
 *                             annealing.
 * @param g_time 		       The duration of graphic render.
 * @param file_mode 		   If we use a file or not.
 * @param graphic 		   	   If we use the graphical mode or not.
 * @param progress 		   	   If we want a progress bar for the simulated
 *                         	   annealing.
 * @param argv        		   The parameters of the program.
 * @param argc        		   The number of parameters of the program.
 *
 * @return 					   An integer.
 */
int check_and_set(char** filename, double* communication_radius, double* capture_radius, 
                  int* size, double* phi, int* step, double* T_initial, int* nb_iterations, 
                  double* g_time, int* file_mode, int* graphic, int* progress, char* argv[],int argc);

#endif
