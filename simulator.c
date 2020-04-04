#include <stdio.h>
#include <stdlib.h>



// reset access counter and memory
void reset(int* access, int p) {
    for(int i=0; i<p; i++) {
        access[i] = 0;
    }
}

// generates gaussian number with mean 0 and SD 1
double generateGaussian() {
	double g = 0;
	for(int i=0; i<25; i++) {
        int max = RAND_MAX;
		x += (double)rand()/max;
    }
	g = g-25.0/2.0;
	g = g/sqrt(25.0/12.0);
	return g;
}

// Parameters: p processors, m memory modules, and d for distribution
// Return: arithmetic average W¯ (Sc(p, m, d)) of all processors time-cumulative averages
int S(int p, int m, char d){
    int request[p];      // processor's request (maybe delete this)
    

    int access[p];       // keeps track of number of granted accesses for each processor
    int memory[m];       // 0 for available, 1 for taken
    float p_average[p];  // time-cumulative average of the access-time for each processor
    float w=0;           // time-cumulative average of the access-time for all processors

    reset(access, p);   // reset the access counter
    reset(memory, p);   // reset all memory to available

    if(d == 'n'){  // normal distribution

        // each processor selects random memory module that will serve as mean
        int u_p[p];
        for(int i=0; i<p; i++) {
            u_p[i] = rand()%p;
        }

        int start = 0;
        for(int c=0; c<1000000; c++) {  // limited to a max of 10^6 cycles

            reset(memory, p); // free memory before each cycle

            for(int j=start; j<p; j++) { // first processor in array gets priority

                int x = round(generateGaussian()*(m/6) + u_p[j]);  // generate random number in normal distribution
                int request = x % m;
                
                // if requested memory is free
                if(request<m && !memory[request]) {   
                    memory[request] = 1;          // mark memory as taken
                    access[j]++;                  // increment num of accesses for processor j 
                    p_average[j] = c/access[j];   // update time-cumulative average for processor j
                }
                else {

                }

/*  
                if granted access
                    access[j] += 1;
                    w[j] =  c/access[j]
                sum += w[j]
*/              if(abs(1-w/new_w)<0.02){   
                    w=new_w;
                    break;
                }
            }

/*
         // compute w
            w = sum/p
*/      
        }

    }
    else{   //normal distribution
        int start=0;    //the first starving processor
        for(int c=0; c<1000000; ++c){   //limited to a max of 10^6 cycles
            reset(memory,m);    //reset the memory
            float new_w=0;    //the new average for the new cycle
            for(int i=start; start<p; ++i){ //start from the first starving processor
                if(memory[rand()%m]==0){    //request a free memory
                    ++access[i];    //add access count for processor i
                    memory[i]=1;    //memory change state to taken
                    p_average[i]=(float)c/(float)access[i]; //compute the time-cumulative average of processor i's memory access at cycle c 
                }
                else{
                    p_average[i]=(float)c/(float)access[i]; //compute the time-cumulative average of processor i's memory access at cycle c
                    start=i;    //change the starving processor to i
                }
            }
            for(int i=0; i<start; ++i){ //loop from the first processor so that every processor have been dealt with
                if(memory[rand()%m]==0){    //request a free memory
                    ++access[i];    //add access count for processor i
                    memory[i]=1;    //memory change state to taken
                    p_average[i]=(float)c/(float)access[i]; //compute the time-cumulative average of processor i's memory access at cycle c 
                }
                else{
                    p_average[i]=(float)c/(float)access[i]; //compute the time-cumulative average of processor i's memory access at cycle c
                    start=i;    //change the starving processor to i
                }
            }

            //arithmetic average W of all processors’ time-cumulative averages.
            for(int i=0; i<p; ++i){
                new_w+=p_average[i];
            }
            new_w/=p;   

            //The system average access time between the current W(new_w) and previous average W (w) is less than a certain tolerance ε(2%)
            if(new_w!=0 && abs(1-w/new_w)<0.02){    
                w=new_w;
                break;
            }

            
            w=new_w;
        }
    /*
    At cycle c:
        if p get access=> access[p]+=1
        Wp=c/access[p]
    for all p:
        W+=Wp
    W=W/p
    */

    }
    return w;
}



int main(int argc, char** argv) {


    int p = atoi(argv[1]);
    char d = *argv[2];
    
    //The simulation part
    for(int i=0; i<2048; ++i){
        printf("Processors:%d Memory modules:%d Distribution:%c\t", p, i+1, d);
        printf("W:%d\n", S(p,i,d));
    }

    S()
    

    
    return 0;   
}