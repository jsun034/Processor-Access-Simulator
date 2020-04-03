#include <stdio.h>
#include <stdlib.h>



// reset access counter
void reset(int* access, int p) {
    for(int i=0; i<p; i++) {
        access[i] = 0;
    }
}

// Parameters: p processors, m memory modules, and d for distribution
// Return: arithmetic average W¯ (Sc(p, m, d)) of all processors time-cumulative averages
int S(int p, int m, char d){
    int request[p];      // processor's request
    int access[p];       // processor access counter
    int memory[m];       // 0 for available, 1 for taken
    float w=0;

    reset(access, p);

    if(d=='u'){   //uniform distribution
        
        for(int c=0; c<1000000; c++) {  // limited to a max of 10^6 cycles
            for(int j=0; j<p; j++) { // first processor in array gets priority
                int denied[p]; // keep track of which processors were denied access to their memory module
                
                generateRequest()
                int r = rand(); 
/*
                if granted access
                    access[j] += 1;
                    w[j] =  c/access[j]
                sum += w[j]
*/             
            }

/*
         // compute w
            w = sum/p
*/      
        }

    }
    else{   //normal distribution
        float new_w;    //the new average for the new cycle
        for(int i=0; i<1000000; ++i){   //limited to a max of 10^6 cycles


            

            if(abs(1-w/new_w)<0.02){    //The system average access time between the current W(new_w) and previous average W (w) is less than a certain tolerance ε(2%)
                w=new_w;
                break;
            }
            
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
    

    
    return 0;   
}