#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>


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
		//x += (double)rand()/max;
    }
	g = g-25.0/2.0;
	g = g/sqrt(25.0/12.0);
	return g;
}

/** 
* Parameters: p processors, m memory modules, and d for distribution
* Return: arithmetic average W¯ (Sc(p, m, d)) of all processors time-cumulative averages
*/
float S(int p, int m, char d){

    int request[p];        // processor's request 
    float w = 0;           // time-cumulative average of the access-time for all processors

    //reset request for all processors
    for(int i=0; i<p; ++i) {
        request[i] = -1;
    }
    int access[p];       // keeps track of number of granted accesses for each processor
    int memory[m];       // 0 for available, 1 for taken
    float p_average[p];  // time-cumulative average of the access-time for each processor

    // initial access time is 0
    for(int i=0; i<p; i++) {
        p_average[i] = 0;
    }

    reset(access, p);     // reset the access counter
    reset(memory, m);     // reset all memory to available

    if(d == 'n') {  // normal distribution
        
        // choose random memory module as mean for normal distribution
        int u_p[p];
        for(int i=0; i<p; i++) {
            u_p[i] = rand()%p;
        }
        
        int start = 0;  // starting from 0th processor

        for(int c=1; c<=1000000; c++) {  // limited to a max of 10^6 cycles

            float new_w = 0;
            reset(memory, m); // free memory before each cycle

            // -1 means request did not fail
            for(int i=0; i<p; i++) {
              request[i] = -1;
            }

            int accessFail = 0; // boolean to keep track of whether we found a starving processor yet

            // computing new p_average for each processor starting from the previous starving processor
            for(int j=start; j<p+start; j++) {

                int p_cur = j%p;    // the current processor
                int req;            // store next request

                // if previous request for memory failed
                if(request[p_cur] != -1) {    
                    req = request[p_cur];
                } 
                // generate new request
                else {   
                    int x = round(generateGaussian()*(m/6) + u_p[p_cur]);  // generate random number in normal distribution
                    req = x % m;
                    if(req<0) {
                        req *= -1;
                    }
                }

                // if requested memory is free
                if(!memory[req]) {   
                    memory[req] = 1;                                    // mark memory as taken
                    access[p_cur]++;                                    // increment num of accesses for processor p_cur 
                    p_average[p_cur] = (float)c/(float)access[p_cur];   // update time-cumulative average for processor p_cur
                    request[p_cur] = -1;                                // make sure request is marked as 'not failed==-1'
                }
                // memory is not free
                else {
                    if(access[p_cur]) p_average[p_cur] = (float)c/(float)access[p_cur];   // update time-cumulative average for processor p
                    request[p_cur] = req;                                                 // remember previous request (request[p_cur] != -1 means it failed)
                    // if starving processor is not yet chosen
                    if(!accessFail) { 
                        start = p_cur;  // update next starting processor                           
                        accessFail = 1; // mark as chosen
                    }                    
                }
            }

            // compute w (average p_average for all processors)
            for(int i=0; i<p; i++) {
                // if processor never accessed, mark with infinite time and quit computing
                if(p_average[i] == 0){   
                    new_w = FLT_MAX;
                    break;
                }
                new_w += p_average[i];      // summing p_averages
            }
            if(new_w != FLT_MAX) new_w /= p; // compute average
            
            // 2% tolerance and assuming all processors have been accessed at least once
            if(new_w!=0 && new_w!=FLT_MAX && fabs(1-w/new_w)<0.02) { 
                w = new_w;
                break;
            } else {
                w = new_w;
            }    
        }

    } else{   //uniform distribution
            int start=0,t=0;    //the first starving processor
            for(int c=1; c<=1000000; ++c){   //limited to a max of 10^6 cycles
                reset(memory,m);    //reset the memory
                float new_w=0;    //the new average for the new cycle
                for(int i=start; i<p; ++i){ //start from the first starving processor
                    if(request[i]==-1){ //previous request was successful
                        int r=rand()%m; //the requested memory
                        if(memory[r]==0){    //request a free memory
                            memory[r]=1;    //memory change state to taken
                            p_average[i]=(float)c/(float)++access[i]; //compute the time-cumulative average of processor i's memory access at cycle c
                        }
                        else{
                            request[i]=r;
                            if(access[i]!=0){   //have been granted access bofore in case of infinite access-time
                                p_average[i]=(float)c/(float)access[i]; //compute the time-cumulative average of processor i's memory access at cycle c
                            }
                            t=i;    //change the starving processor to i
                        }
                    }
                    else{
                        if(memory[request[i]]==0){    //request a free memory
                            memory[request[i]]=1;    //memory change state to taken
                            request[i]=-1;  //reset the request because it was successfully accessed this time
                            p_average[i]=(float)c/(float)++access[i]; //compute the time-cumulative average of processor i's memory access at cycle c
                        }
                        else{
                            if(access[i]!=0){   //have been granted access bofore in case of infinite access-time
                                p_average[i]=(float)c/(float)access[i]; //compute the time-cumulative average of processor i's memory access at cycle c
                            }
                            t=i;    //change the starving processor to i
                        }
                    }
                }
                for(int i=0; i<start; ++i){ //loop from the first processor so that every processor have been dealt with
                    if(request[i]==-1){ //previous request was successful
                        int r=rand()%m; //the requested memory
                        if(memory[r]==0){    //request a free memory
                            memory[r]=1;    //memory change state to taken
                            p_average[i]=(float)c/(float)++access[i]; //compute the time-cumulative average of processor i's memory access at cycle c
                        }
                        else{
                            request[i]=r;
                            if(access[i]!=0){   //have been granted access bofore in case of infinite access-time
                                p_average[i]=(float)c/(float)access[i]; //compute the time-cumulative average of processor i's memory access at cycle c
                            }
                            t=i;    //change the starving processor to i
                        }
                    }
                    else{
                        if(memory[request[i]]==0){    //request a free memory
                            memory[request[i]]=1;    //memory change state to taken
                            request[i]=-1;  //reset the request because it was successfully accessed this time
                            p_average[i]=(float)c/(float)++access[i]; //compute the time-cumulative average of processor i's memory access at cycle c
                        }
                        else{
                            if(access[i]!=0){   //have been granted access bofore in case of infinite access-time
                                p_average[i]=(float)c/(float)access[i]; //compute the time-cumulative average of processor i's memory access at cycle c
                            }
                            t=i;    //change the starving processor to i
                        }
                    }
                }
    
                //update the starving processor for the next cycle
                start=t;
    
                //arithmetic average W of all processors’ time-cumulative averages.
                for(int i=0; i<p; ++i){
                    if(p_average[i]==0){   //check if there is a processor with infinite access-time (a.k.a. never get to access any memory before)
                        new_w=FLT_MAX;
                        break;
                    }
                    new_w+=p_average[i];
                }
                if(new_w!=FLT_MAX){
                    new_w/=p;
                }
                //The system average access time between the current W(new_w) and previous average W (w) is less than a certain tolerance ε(2%) and all processors have at least
                // get an access once
                if(new_w!=0 && new_w!=FLT_MAX && fabs(1-w/new_w)<0.02){
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
    
    // The simulation part
    for(int i=0; i<2048; ++i){
        printf("Processors:%d Memory modules:%d Distribution:%c\t", p, i+1, d);
        printf("W:%.4f\n", S(p,i+1,d));
    }
    
    return 0;   
}