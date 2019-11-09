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

#include <stdlib.h>
#include <stdio.h>

int check_and_set(char** filename, double* communication_radius, double* capture_radius, int* size, 
                  double* phi, int* step, double* T_initial, int* nb_iterations, double* g_time, 
                  int* file_mode, int* graphic, int* progress, char* argv[],int argc)
{
	if(argc != 13)
		return 0;
	else {
		*filename 			  = argv[1];
		*communication_radius = strtod(argv[2], NULL)+0.00001;
		*capture_radius 	  = strtod(argv[3], NULL)+0.00001;
		*size 				  = strtol(argv[4], NULL, 0);
		*phi 				  = strtod(argv[5], NULL);
		*step 				  = strtol(argv[6], NULL, 0);
		*T_initial 			  = strtod(argv[7], NULL);
		*nb_iterations		  = strtol(argv[8], NULL, 0);
		*g_time				  = strtod(argv[9], NULL);
		*file_mode			  = strtol(argv[10], NULL,0);
		*graphic			  = strtol(argv[11], NULL,0);
		*progress			  = strtol(argv[12], NULL,0);

		return 1;
	}
}
