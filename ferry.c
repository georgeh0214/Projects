#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h> 
#include <stdbool.h>
#define capacity 100

int trkProbability, timeInterval, seed; sem_t capacityMutex; // arguments
pthread_t carThreads_id[capacity]; int i=0, j=0; sem_t jmutex, carThreads_idMutex; // array stores car threads info
pthread_t trkThreads_id[capacity]; int m=0, n=0; sem_t nmutex, trkThreads_idMutex; // array stores truck threads info
pthread_t carIDLoad, trkIDLoad; sem_t carIDLoadMutex, trkIDLoadMutex; // stores id of next loading car/truck
int cQueue=0, tQueue=0, numArrive=0; sem_t cQueueMutex, tQueueMutex, numArriveMutex;
// counter and mutexes for car queue, truck queue, number of ferry arrival

sem_t  cLoadMutex, cSailMutex, cUnloadMutex, cExitMutex, cTerminateMutex, cUnqueueMutex; // mutexes for car
sem_t  tLoadMutex, tSailMutex, tUnloadMutex, tExitMutex, tTerminateMutex, tUnqueueMutex; // mutexes for truck
sem_t  ferryAwayMutex; // mutex to stop creating vehicles while ferry is away from home port



void *trkThread(void* arg){
    printf("TRUCK:      Truck with threadID %lu queued\n", pthread_self());
    fflush(stdout);
    unsigned long nextLoad;
    while (nextLoad != pthread_self()){ // blocked until known it will be loaded next
        usleep(100000);
        sem_wait(&trkIDLoadMutex);
        nextLoad = trkIDLoad;
        sem_post(&trkIDLoadMutex);
        sem_wait(&numArriveMutex);
        if (numArrive>=5){
            sem_post(&numArriveMutex);
            printf("TRUCK:      Truck with threadID %lu is about to exit\n", pthread_self());
            fflush(stdout);
            pthread_exit(0);
        }
        sem_post(&numArriveMutex);
    }
    
    sem_wait(&tUnqueueMutex);

    printf("TRUCK:      Truck with threadID %lu leaving the queue to load\n", pthread_self());
    fflush(stdout);
    printf("TRUCK:      Truck with threadID %lu is onboard the ferry\n", pthread_self());
    fflush(stdout);
    sem_post(&tLoadMutex); // finished loading and unblock captain so it can proceed to next vehicle
    sem_wait(&tSailMutex); // blocked while sailing
    printf("TRUCK:      Truck with threadID %lu is now unloading\n", pthread_self());
    fflush(stdout);
    printf("TRUCK:      Truck with threadID %lu has unloaded\n", pthread_self());
    fflush(stdout);
    sem_post(&tUnloadMutex); // unblock captain
    sem_wait(&tExitMutex); 
    printf("TRUCK:      Truck with threadID %lu is about to exit\n", pthread_self());
    fflush(stdout);
    sem_post(&tTerminateMutex);
    pthread_exit(0);
}

void *carThread(void* arg){
    printf("CAR:        Car with threadID %lu queued\n", pthread_self());
    fflush(stdout);
    unsigned long nextLoad;
    while (nextLoad != pthread_self()){ // blocked until known it will be loaded next
        usleep(100000);
        sem_wait(&carIDLoadMutex);
        nextLoad = carIDLoad;
        sem_post(&carIDLoadMutex);
        sem_wait(&numArriveMutex);
        if (numArrive>=5){
            sem_post(&numArriveMutex);
            printf("CAR:        Car with threadID %lu is about to exit\n", pthread_self());
            fflush(stdout);
            pthread_exit(0);
        }
        sem_post(&numArriveMutex);
    }
    sem_wait(&cUnqueueMutex);
    
    printf("CAR:        Car with threadID %lu leaving the queue to load\n", pthread_self());
    fflush(stdout);
    printf("CAR:        Car with threadID %lu is onboard the ferry\n", pthread_self());
    fflush(stdout);
    sem_post(&cLoadMutex); // finished loading and unblock captain so it can proceed to next vehicle
    sem_wait(&cSailMutex); // blocked while sailing
    printf("CAR:        Car with threadID %lu is now unloading\n", pthread_self());
    fflush(stdout);
    printf("CAR:        Car with threadID %lu has unloaded\n", pthread_self());
    fflush(stdout);
    sem_post(&cUnloadMutex); // unblock captain
    sem_wait(&cExitMutex); 
    printf("CAR:        Car with threadID %lu is about to exit\n", pthread_self());
    fflush(stdout);
    sem_post(&cTerminateMutex);
    pthread_exit(0);
}

void createCar(void){ // function call to create car thread
    sem_wait(&cQueueMutex);
    sem_wait(&capacityMutex);
    if (cQueue == capacity){ // car queue full
        sem_post(&capacityMutex);
        sem_post(&cQueueMutex);
        return;
    } 
    sem_wait(&carThreads_idMutex);
    sem_wait(&jmutex);
    pthread_create(&carThreads_id[j], NULL, &carThread, NULL);
    printf("CREATEVEHICLE:      Created a car thread\n");
    fflush(stdout);
    j++;    
    cQueue++;
    sem_post(&jmutex);
    sem_post(&carThreads_idMutex);
    sem_post(&capacityMutex);
    sem_post(&cQueueMutex);
    return;
}

void createTruck(void){ // function call to create truck thread
    sem_wait(&tQueueMutex);
    sem_wait(&capacityMutex);
    if (tQueue == capacity){ // truck queue full
        sem_post(&capacityMutex);
        sem_post(&tQueueMutex);
        return;
    } 
    sem_wait(&trkThreads_idMutex);
    sem_wait(&nmutex);
    pthread_create(&trkThreads_id[n], NULL, &trkThread, NULL);
    printf("CREATEVEHICLE:      Created a truck thread\n");
    fflush(stdout);
    n++;
    tQueue++;
    sem_post(&nmutex);
    sem_post(&trkThreads_idMutex);
    sem_post(&capacityMutex);
    sem_post(&tQueueMutex);
    return;
}

int loadCar(void){ 
    sem_wait(&cQueueMutex);
    if (cQueue <= 0){sem_post(&cQueueMutex); return 1;} // car queue empty
    sem_wait(&carIDLoadMutex);
    sem_wait(&carThreads_idMutex);
    carIDLoad = carThreads_id[i];
    sem_post(&carThreads_idMutex);
    cQueue--;
    i++; // i does not need mutual exclusion because it is only used in this function
    sem_post(&carIDLoadMutex);
    sem_post(&cQueueMutex);
    return 0;
}

int loadTruck(void){
    sem_wait(&tQueueMutex);
    if (tQueue <= 0){sem_post(&tQueueMutex); return 1;} // truck queue empty
    sem_wait(&trkIDLoadMutex);
    sem_wait(&trkThreads_idMutex);
    trkIDLoad = trkThreads_id[m];
    sem_post(&trkThreads_idMutex);
    tQueue--;
    m++; // m does not need mutual exclusion because it is only used in this function
    sem_post(&trkIDLoadMutex);
    sem_post(&tQueueMutex);
    return 0;
}

bool atLeastEight(void){
    int num;
    sem_wait(&cQueueMutex);
    sem_wait(&tQueueMutex);
    num = cQueue + tQueue;
    sem_post(&tQueueMutex);
    sem_post(&cQueueMutex);
    if (num >= 8){return true;}
    else{return false;}
}


void *createVehicle(void* arg){ // create vehicle thread
    unsigned int waitTime=0, vehicleProb, nextArrivalTime = 0, elapseTime = 0;
    printf("CREATEVEHICLE:      Vehicle creation thread has been started\n");
    fflush(stdout);
    srand(seed);
    while (1){
        sem_wait(&ferryAwayMutex); // block if ferry is away from home dock
        sem_post(&ferryAwayMutex); 

        sem_wait(&numArriveMutex); 
        if (numArrive>=5){sem_post(&numArriveMutex); break;} // if ferry arrives for 5th load, break and exit
        sem_post(&numArriveMutex);

        usleep(waitTime*1000); 
        elapseTime += waitTime;
        waitTime = 1000 + rand() % (timeInterval - 1000); 
        printf("CREATEVEHICLE:      Elapsed time %d msec\n", elapseTime);
        fflush(stdout);

        if (elapseTime != 0 && elapseTime>= nextArrivalTime){ 
            vehicleProb = rand() % 100; // truck or car
            if (vehicleProb < trkProbability){ // create truck, < because trkProbability can be 0
                createTruck();
            }
            else { // create car
                createCar();
            }
        }
        nextArrivalTime += waitTime;
        printf("CREATEVEHICLE:      Next arrival time %d msec\n", nextArrivalTime);
        fflush(stdout);
    }// while
    pthread_exit(0); // exit
}



void *captain(void* arg){
    printf("CAPTAIN:        Captain thread started\n");

    fflush(stdout);
    int currentCar, currentTrk, loadCapacity, trkSelected;
    bool enough = atLeastEight();
    while (enough == false){ // block until at least 8 vehicles in queue
        usleep(1000000);
        enough = atLeastEight();
    }

    while (1){
        sem_wait(&numArriveMutex); // increment load
        numArrive++;
        if (numArrive>=5){sem_post(&numArriveMutex); break;} // if ferry arrives for 5th load, break and exit
        sem_post(&numArriveMutex);

        loadCapacity = 6, trkSelected = 0;

        sem_wait(&cQueueMutex);
        currentCar = cQueue; // current number of cars
        sem_post(&cQueueMutex);
        sem_wait(&tQueueMutex);
        currentTrk = tQueue; // current number of trucks
        sem_post(&tQueueMutex);

        while (loadCapacity > 0){ // ship not full
            while (loadCapacity >= 2 && currentTrk > 0 && trkSelected < 2){ // load truck from queue
                if (loadTruck()==0){
                    printf("CAPTAIN:        Truck selected for loading\n");
                    fflush(stdout);
                    sem_post(&tUnqueueMutex);
                    currentTrk--;
                    loadCapacity -= 2;
                    trkSelected ++;
                    sem_wait(&tLoadMutex); // blocked until truck finishes loading
                    printf("CAPTAIN:        Captain known truck is loaded\n");
                    fflush(stdout);
                    sem_trywait(&tLoadMutex);
                }
                // else{printf("Truck queue empty!\n"); fflush(stdout);}
            }
            
            while (loadCapacity > 0 && currentCar > 0){ // load car from queue
                if (loadCar()==0){
                    printf("CAPTAIN:        Car selected for loading\n");
                    fflush(stdout);
                    sem_post(&cUnqueueMutex);
                    currentCar--;
                    loadCapacity--;
                    sem_wait(&cLoadMutex); // blocked until car finishes loading
                    printf("CAPTAIN:        Captain known car is loaded\n");
                    fflush(stdout);
                    sem_trywait(&cLoadMutex);
                }
                // else{printf("Car queue empty!\n"); fflush(stdout);}
            }

            while (loadCapacity > 0){ // Not enough, select from late arrivals
                usleep(500000); // wait for some time
                if (loadCapacity >= 2 &&  trkSelected < 2){ // can load a truck
                    if (loadTruck()==0){ // try to load a truck if there is one
                    printf("CAPTAIN:        Truck selected for loading\n");
                    fflush(stdout);                     
                        sem_post(&tUnqueueMutex);
                        loadCapacity -= 2;
                        trkSelected++;
                        sem_wait(&tLoadMutex); // blocked until truck finishes loading
                        printf("CAPTAIN:        Captain known truck is loaded\n");
                        fflush(stdout);
                        sem_trywait(&tLoadMutex);
                    } 
                    // else{printf("Truck queue empty!\n"); fflush(stdout);}
                }
                else { // try to load a car if there is one
                    if (loadCar()==0){
                        printf("CAPTAIN:        Car selected for loading\n");
                        fflush(stdout);
                        sem_post(&cUnqueueMutex);
                        loadCapacity--;
                        sem_wait(&cLoadMutex); // blocked until car finishes loading
                        printf("CAPTAIN:        Captain known car is loaded\n");
                        fflush(stdout);
                        sem_trywait(&cLoadMutex);
                    }
                    // else{printf("Car queue empty!\n"); fflush(stdout);}
                }
            }
        } // ship is full
        sem_wait(&ferryAwayMutex); // block createVehicle thread before leaving home
        printf("\nCAPTAIN:      Ferry is full, starting to sail\n");
        fflush(stdout);
        usleep(2000000); // sailing to the destination dock takes 2 seconds
        printf("\nCAPTAIN:      Ferry has reached the destination port\n\n");
        fflush(stdout);
        int numOfCar = 6 - 2*trkSelected;
        for (int a = 0; a < (numOfCar + trkSelected); a++){ // unload vehicles

            if (a<trkSelected){
                sem_post(&tSailMutex); // tell trucks they can unload
                sem_wait(&tUnloadMutex);
                printf("CAPTAIN:        Captain knows a truck has unloaded from the ferry\n");
                fflush(stdout);
                sem_post(&tExitMutex);
                sem_wait(&tTerminateMutex);
                printf("CAPTAIN:        Captain sees a truck leaving the ferry terminal\n");
                fflush(stdout);
            }
            else {
                sem_post(&cSailMutex); // tell cars they can unload
                sem_wait(&cUnloadMutex);
                printf("CAPTAIN:        Captain knows a car has unloaded from the ferry\n");
                fflush(stdout);
                sem_post(&cExitMutex);
                sem_wait(&cTerminateMutex);
                printf("CAPTAIN:        Captain sees a car leaving the ferry terminal\n");
                fflush(stdout);
            }
        }
        
        
        usleep(2000000); // sailing to home dock takes 2 seconds
        printf("\nCAPTAIN:      Ferry has reached the home port\n\n");
        fflush(stdout);
        sem_post(&ferryAwayMutex);
        
    }
    pthread_exit(0);
}



int main (void){
    
    sem_init(&cQueueMutex, 0, 1);
    sem_init(&tQueueMutex, 0, 1);
    sem_init(&carIDLoadMutex, 0, 1);
    sem_init(&trkIDLoadMutex, 0, 1);
    sem_init(&capacityMutex, 0, 1);
    sem_init(&carThreads_idMutex, 0, 1);
    sem_init(&trkThreads_idMutex, 0, 1);
    sem_init(&jmutex, 0, 1);
    sem_init(&nmutex, 0, 1);
    sem_init(&ferryAwayMutex, 0, 1);
    sem_init(&numArriveMutex, 0, 1);

    sem_init(&cLoadMutex, 0, 0);
    sem_init(&tLoadMutex, 0, 0);
    sem_init(&cSailMutex, 0, 0);
    sem_init(&tSailMutex, 0, 0);
    sem_init(&cUnloadMutex, 0, 0);
    sem_init(&tUnloadMutex, 0, 0);
    sem_init(&cExitMutex, 0, 0);
    sem_init(&tExitMutex, 0, 0);
    sem_init(&cTerminateMutex, 0, 0);
    sem_init(&tTerminateMutex, 0, 0);
    sem_init(&cUnqueueMutex, 0, 0);
    sem_init(&tUnqueueMutex, 0, 0);
    
    printf("Please enter integer values for the following variables\n");
    fflush(stdout);
    printf("Enter the percent probability that the next vehicle is a truck\n");
    fflush(stdout);
    scanf("%d", &trkProbability);
    if (trkProbability<0 || trkProbability>=100){printf("Invalid Probability\n"); return 1;}// trk probability cannot be 100, infinite loop
    printf("Enter the maximum length of the interval between vehicles\ntime interval should be >1000\n");
    fflush(stdout);
    scanf("%d", &timeInterval);
    if (timeInterval<1000 || timeInterval>5000){printf("Invalid time interval\n"); return 1; fflush(stdout);}
    printf("Enter the seed for random number generation\n");
    fflush(stdout);
    scanf("%d", &seed);
    if (seed<=2 || seed>=RAND_MAX){printf("Invalid seed\n"); return 1;}



    pthread_t createVehicle_id;
    pthread_create(&createVehicle_id, NULL, &createVehicle, NULL);
    

    pthread_t captain_id;
    pthread_create(&captain_id, NULL, &captain, NULL);

    pthread_join(createVehicle_id, NULL);
    pthread_join(captain_id, NULL);

    int carIndex, truckIndex;
    sem_wait(&jmutex);
    carIndex = j;
    sem_post(&jmutex);
    sem_wait(&nmutex);
    truckIndex = n;
    sem_post(&nmutex);

    for (int t=0; t<truckIndex; t++){ // join truck threads
        pthread_join(trkThreads_id[t], NULL);
    }
    for (int c=0; c<carIndex; c++){ // join car threads
        pthread_join(carThreads_id[c], NULL);
    }

    sem_destroy(&cQueueMutex);
    sem_destroy(&tQueueMutex);
    sem_destroy(&carIDLoadMutex);
    sem_destroy(&trkIDLoadMutex);
    sem_destroy(&capacityMutex);
    sem_destroy(&carThreads_idMutex);
    sem_destroy(&trkThreads_idMutex);
    sem_destroy(&jmutex);
    sem_destroy(&nmutex);
    sem_destroy(&ferryAwayMutex);
    sem_destroy(&numArriveMutex);

    sem_destroy(&cLoadMutex);
    sem_destroy(&tLoadMutex);
    sem_destroy(&cSailMutex);
    sem_destroy(&tSailMutex);
    sem_destroy(&cUnloadMutex);
    sem_destroy(&tUnloadMutex);
    sem_destroy(&cExitMutex);
    sem_destroy(&tExitMutex);
    sem_destroy(&cTerminateMutex);
    sem_destroy(&tTerminateMutex);
    sem_destroy(&cUnqueueMutex);
    sem_destroy(&tUnqueueMutex);

    return 0;
} // main
