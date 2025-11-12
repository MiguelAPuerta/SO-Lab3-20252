/*
 *  OpenMP lecture exercises
 *  Copyright (C) 2011 by Christian Terboven <terboven@rz.rwth-aachen.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

void* CalcPiParallel(void* arg);
double GetTime();

typedef struct {
    int id;
    int n;
    int T;
    double partial_sum;
} thread_data_t;

int main(int argc, char **argv)
{
	if (argc != 3) {
		fprintf	(stderr, "Uso: %s <n> <T>\n", argv[0]);
		return 1;
	}
	
    int n = atoi(argv[1]);
    int T = atoi(argv[2]);
    
    pthread_t threads[T];
    thread_data_t tdata[T];
    
    const double fPi25DT = 3.141592653589793238462643;
    double threadSum = 0.0;
    double fH, fPi;
    double fTimeStart, fTimeEnd;
    
    // get initial time 
	fTimeStart = GetTime();
	
    /* the calculation is done here*/
    // create threads
    for (int i = 0; i < T; i++) {
		tdata[i].id = i;
		tdata[i].n = n;
		tdata[i].T = T;
		pthread_create(&threads[i], NULL, CalcPiParallel, &tdata[i]);
	}
	
    // wait for threads and sum results
    for (int i = 0; i < T; i++){
		pthread_join(threads[i], NULL);
		threadSum += tdata[i].partial_sum;
	}
    fH = 1.0 / (double)n;
    // pi results
    fPi = fH * threadSum;
    //get final fime
    fTimeEnd = GetTime();
    
    printf("\npi is approximately = %.20f \nError               = %.20f\n",
           fPi, fabs(fPi - fPi25DT));
    // report time
	printf("Tiempo calculo      = %.20f \n", fTimeEnd - fTimeStart);
	
	return 0;
}

double GetTime() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec * 1e-6;
}

double f(double a)
{
    return (4.0 / (1.0 + a*a));
}


void* CalcPiParallel(void* arg)
{
	thread_data_t* data = (thread_data_t*)arg;
    int n = data->n;
    int T = data->T;
    int id = data->id;
    double fH = 1.0 / (double)n;
    double local_sum = 0.0;
    double fX;
    
     // División del trabajo
    int start = (n / T) * id;
    int end = (id == T - 1) ? n : (n / T) * (id + 1);

    for (int i = start; i < end; i++) {
        fX = fH * ((double)i + 0.5);
        local_sum += f(fX);
    }
    data->partial_sum = local_sum;
    return NULL;
}
