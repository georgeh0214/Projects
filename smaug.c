#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>

sem_t *stopThiefCreationMutex;
const char *stopThiefCreation = "/stopCreatingThief";

sem_t *stopHunterCreationMutex;
const char *stopHunterCreation = "/stopCreatingHunter";

sem_t *thiefExitMutex;
const char* thiefExit = "/thiefExit";

sem_t *hunterExitMutex;
const char* hunterExit = "/hunterExit";

sem_t *thiefCountSem;
const char *numOfThief = "/thiefCount";

sem_t *hunterCountSem;
const char *numOfHunter = "/hunterCount";

sem_t *chosenToPlayMutex;
const char *playing = "/playing";

sem_t *chosenToFightMutex;
const char *fighting = "/fighting";

sem_t *leaveMutex;
const char *leaving = "/leaving";

sem_t *sleepMutex;
const char *sleeping = "/sleeping";

sem_t *finishEventMutex;
const char *finished = "/finished";

sem_t *releaseThiefMutex;
const char *releaseThief = "/unblockAllThief";

sem_t *releaseHunterMutex;
const char *releaseHunter = "/unblockAllHunter";

sem_t *winMutex;	// winMutex = 1 means smaug won, = 0 means thief won
const char *win = "/winning";

void cleanUp(void){
	sem_close(stopThiefCreationMutex);
	sem_close(stopHunterCreationMutex);
	sem_close(thiefExitMutex);
	sem_close(hunterExitMutex);
	sem_close(thiefCountSem);
	sem_close(hunterCountSem);
	sem_close(chosenToPlayMutex);
	sem_close(chosenToFightMutex);
	sem_close(leaveMutex);
	sem_close(sleepMutex);
	sem_close(finishEventMutex);
	sem_close(releaseThiefMutex);
	sem_close(releaseHunterMutex);
	sem_close(winMutex);

	if (sem_unlink(stopThiefCreation) < 0)printf("1Unlink Failed!!!!!\n");
	if (sem_unlink(stopHunterCreation) < 0)printf("2Unlink Failed!!!!!\n");
	if (sem_unlink(thiefExit) < 0)printf("3Unlink Failed!!!!!\n");
	if (sem_unlink(hunterExit) < 0)printf("4Unlink Failed!!!!!\n");
	if (sem_unlink(numOfThief) < 0)printf("5Unlink Failed!!!!!\n");
	if (sem_unlink(numOfHunter) < 0)printf("6Unlink Failed!!!!!\n");
	if (sem_unlink(playing) < 0)printf("7Unlink Failed!!!!!\n");
	if (sem_unlink(fighting) < 0)printf("8Unlink Failed!!!!!\n");
	if (sem_unlink(leaving) < 0)printf("9Unlink Failed!!!!!\n");
	if (sem_unlink(sleeping) < 0)printf("10Unlink Failed!!!!!\n");
	if (sem_unlink(finished) < 0)printf("11Unlink Failed!!!!!\n");
	if (sem_unlink(releaseThief) < 0)printf("12Unlink Failed!!!!!\n");
	if (sem_unlink(releaseHunter) < 0)printf("13Unlink Failed!!!!!\n");
	if (sem_unlink(win) < 0)printf("14Unlink Failed!!!!!\n");

}

int *thiefTerminate;	// shared int among all thief processes
int *hunterTerminate;	// shared int among all hunter processes

int main(){

	// cleanUp();		// uncomment this if program did not exit normally, to clean up the kernal persistent sems

	thiefTerminate = mmap(NULL, sizeof *thiefTerminate, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	hunterTerminate = mmap(NULL, sizeof *hunterTerminate, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	*thiefTerminate = 0;	
	*hunterTerminate = 0;

	stopThiefCreationMutex = sem_open(stopThiefCreation, O_CREAT, 0600, 0);
	stopHunterCreationMutex = sem_open(stopHunterCreation, O_CREAT, 0600, 0);
	thiefExitMutex = sem_open(thiefExit, O_CREAT, 0600, 1);
	hunterExitMutex = sem_open(hunterExit, O_CREAT, 0600, 1);
	thiefCountSem = sem_open(numOfThief, O_CREAT, 0600, 0);
	hunterCountSem = sem_open(numOfHunter, O_CREAT, 0600, 0);
	chosenToPlayMutex = sem_open(playing, O_CREAT, 0600, 0);
	chosenToFightMutex = sem_open(fighting, O_CREAT, 0600, 0);
	leaveMutex = sem_open(leaving, O_CREAT, 0600, 0);
	sleepMutex = sem_open(sleeping, O_CREAT, 0600, 0);
	finishEventMutex = sem_open(finished, O_CREAT, 0600, 0);
	releaseThiefMutex = sem_open(releaseThief, O_CREAT, 0600, 0);	
	releaseHunterMutex = sem_open(releaseHunter, O_CREAT, 0600, 0);	
	winMutex = sem_open(win, O_CREAT, 0600, 0);

	int MaximumHunterInterval, MaximumThiefInterval, winProb;
	printf("Please enter the value of Maximum Hunter Interval in seconds: ");
	scanf("%d", &MaximumHunterInterval);
	printf("Please enter the value of Maximum Thief Interval in seconds: ");
	scanf("%d", &MaximumThiefInterval);
	printf("Please enter the probability of winning 0~100: ");
	scanf("%d", &winProb);

	pid_t thiefGenerator = fork();	

	if (thiefGenerator == 0){	// child that creates thieves
		int i=0, end = 0;
		pid_t pid[100];
		unsigned int waitTime, startTime;
		srand(time(NULL));

		for (i; i<100 && sem_trywait(stopThiefCreationMutex)!=0; i++){	// loop that creates thieves
		// trywait successful meaning stopThiefCreationMutex = 1 and simulation should end
		
			startTime = 0;
			waitTime = rand() % (MaximumThiefInterval+1);
			while(startTime < waitTime){	// timer and check if simulation is over
				sleep(1);
				startTime++;
				if (sem_trywait(stopThiefCreationMutex)==0){
					end=1;
					break;
				}
			}
			if (end)break;

			pid[i] = fork();
			if (!pid[i]){	// thief process
				int prob;
				srand(time(NULL)+i);	// rng for prob
				pid_t thiefPID = getpid();
				printf("\nThief %d is travelling to the valley\n", thiefPID);
				printf("Thief %d is wandering in the valley\n", thiefPID);

				sem_post(thiefCountSem);	// number of thief ++

				sem_wait(chosenToPlayMutex);	// wait for signal from smaug process

				sem_wait(thiefExitMutex);	
				if (*thiefTerminate == 1){	// thiefTerminate = 1 means simulation is over and thief process should exit
					sem_post(thiefExitMutex);
					exit(0);
				}
				sem_post(thiefExitMutex);	// else release mutex and continue

				printf("Smaug is playing with a thief\n");
				printf("Thief %d is playing with Smaug\n", thiefPID);

				prob = rand() % 100;
				printf("winning probability = %d and Random number is = %d\n", winProb, prob);

				if (prob < winProb){	// thief won
					printf("Smaug has been defeated by a thief\n");
					printf("Thief %d has won and receives treasure\n", thiefPID);
				}
				else{	// smaug won
					printf("Smaug has defeated a thief\n");
					printf("Thief %d has been defeated and pays the price\n", thiefPID);
					sem_post(winMutex);	// change winMutex to 1
				}
				
				sem_post(finishEventMutex);	// signal smaug process to continue

				sem_wait(leaveMutex);	// wait signal from smaug process
				printf("Thief %d leaves\n", thiefPID);	
				sem_post(sleepMutex);	// signal the smaug process that he can sleep now
				exit(0);
			}	// thief process

		}	// loop that creates thieves


		for (int j=0; j<i; j++){	// wait for every thief
			int status;
			waitpid(pid[j], &status, 0); 
		}
		sem_post(releaseThiefMutex);	// signal smaug that every thief has been waited
		exit(0);

	}	//child that is responsible for creating thieves



	pid_t hunterGenerator = fork();	

	if (hunterGenerator == 0){	// child that creates hunters
		int i=0, end = 0;
		pid_t pid[100];
		unsigned int waitTime, startTime;
		srand(time(NULL)*2);
		
		for (i; i<100 && sem_trywait(stopHunterCreationMutex)!=0; i++){	// loop that creates hunters
		// trywait successful meaning stopHunterCreationMutex = 1 and simulation should end

			startTime = 0;
			waitTime = rand() % (MaximumHunterInterval+1);
			while(startTime < waitTime){		// timer and check if simulation is over
				sleep(1);
				startTime++;
				if (sem_trywait(stopHunterCreationMutex)==0){
					end=1;
					break;
				}
			}
			if (end)break;

			pid[i] = fork();
			if (!pid[i]){	// hunter process
				int prob;
				srand(time(NULL)*2+i);
				pid_t hunterPID = getpid();
				printf("\nHunter %d is travelling to the valley\n", hunterPID);
				printf("Hunter %d is wandering in the valley\n", hunterPID);

				sem_post(hunterCountSem);	// number of hunter ++

				sem_wait(chosenToFightMutex);	// wait for signal from smaug process

				
				sem_wait(hunterExitMutex);
				if (*hunterTerminate==1){		// hunterTerminate = 1 means simulation is over and hunter process should exit
					sem_post(hunterExitMutex);
					exit(0);
				}
				sem_post(hunterExitMutex);	// else release mutex and continue

				printf("Smaug is playing with a hunter\n");
				printf("Hunter %d is playing with Smaug\n", hunterPID);

				prob = rand() % 100;
				printf("winning probability = %d and Random number is = %d\n", winProb, prob);

				if (prob < winProb){	// hunter won
					printf("Smaug has been defeated by a hunter\n");
					printf("hunter %d has won and receives treasure\n", hunterPID);
				}
				else{	// smaug won
					printf("Smaug has defeated a hunter\n");
					printf("hunter %d has been defeated and pays the price\n", hunterPID);
					sem_post(winMutex);	// change winMutex to 1
				}
				
				sem_post(finishEventMutex);	// signal smaug process to continue

				sem_wait(leaveMutex);	// wait signal from smaug process
				printf("hunter %d leaves\n", hunterPID);	
				sem_post(sleepMutex);	// signal the smaug process that he can sleep now
				exit(0);
			}	// hunter process

		}	// loop that creates hunters

		

		for (int j=0; j<i; j++){	// wait for every hunter
			int status;
			waitpid(pid[j], &status, 0); 
		}
		sem_post(releaseHunterMutex);		// signal smaug that every hunter has been waited
		exit(0);

	}	//child that is responsible for creating hunters





	int jewel = 30, hunterDefeated = 0, thiefDefeated = 0, prob;
	srand(time(NULL));

	while (jewel > 0 && jewel < 80 && hunterDefeated < 4 && thiefDefeated < 3){
		sleep(1);

		if (sem_trywait(thiefCountSem)==0){	// trywait successful means there is at least one thief
			printf("Smaug has been woken up\n");
			printf("Smaug takes a deep breath\n");
			printf("Smaug smells a thief\n");

			sem_post(chosenToPlayMutex);

			sem_wait(finishEventMutex);
			if (sem_trywait(winMutex)==0){	// trywait successful means winMutex = 1 which means smaug has won
				jewel += 20;
				thiefDefeated++;
				printf("Smaug has added to his treasure he now has %d jewels\n", jewel);
			}
			else{	// smaug lost
				jewel -= 8;
				printf("Smaug has lost some treasure he now has %d jewels\n", jewel);
			}
			
			printf("Smaug has finished a game\n");
			sem_post(leaveMutex);	// signal thief process to continue
			sem_wait(sleepMutex);	// wait for signal from thief process
			printf("Smaug is going to sleep\n\n");
		}
		else if (sem_trywait(hunterCountSem)==0){	//trywait successful means there is no thief but at least one hunter exists
			printf("Smaug has been woken up\n");
			printf("Smaug takes a deep breath\n");
			printf("Smaug smells a hunter\n");

			sem_post(chosenToFightMutex);

			sem_wait(finishEventMutex);
			if (sem_trywait(winMutex)==0){	// trywait successful means winMutex = 1 which means smaug has won
				jewel += 5;
				hunterDefeated++;
				printf("Smaug has added to his treasure he now has %d jewels\n", jewel);
			}
			else{	// smaug lost
				jewel -= 10;
				printf("Smaug has lost some treasure he now has %d jewels\n", jewel);
			}
			
			printf("Smaug has finished a game\n");
			sem_post(leaveMutex);	// signal hunter process to continue
			sem_wait(sleepMutex);	// wait for signal from hunter process
			printf("Smaug is going to sleep\n\n");
		}


	}

	sem_post(stopThiefCreationMutex);		// signal thiefGenerator to stop creating thieves
	sem_post(stopHunterCreationMutex);		// signal hunterGenerator to stop creating hunters

	sem_wait(thiefExitMutex);
	*thiefTerminate = 1;			// signal each thief process to stop
	sem_post(thiefExitMutex);

	sem_wait(hunterExitMutex);
	*hunterTerminate = 1;		// signal each hunter process to stop
	sem_post(hunterExitMutex);

	if (jewel <= 0){
		printf("Smaug has no more treasure\n");
	}
	if (jewel >= 80){
		printf("Smuag has more than 80 jewels: %d\n", jewel);
	}
	if (thiefDefeated >= 3){
		printf("Smaug has defeated 3 thieves\n");
	}
	if (hunterDefeated >= 4){
		printf("Smaug has defeated 4 hunters\n");
	}

	
	sleep(1);
	while (sem_trywait(releaseThiefMutex) != 0){	
		sem_post(chosenToPlayMutex);				// unblock all remaining thief processes so they can terminate
	}
	while (sem_trywait(releaseHunterMutex) != 0){	
		sem_post(chosenToFightMutex);				// unblock all remaining hunter processes so they can terminate
	}

	int thiefNum, hunterNum;
	sem_getvalue(thiefCountSem, &thiefNum);
	sem_getvalue(hunterCountSem, &hunterNum);

	if (thiefNum > 0) printf("%d thieves released\n", thiefNum);
	if (hunterNum > 0) printf("%d hunters released\n", hunterNum);

	int status;
	waitpid(thiefGenerator, &status, 0);
	waitpid(hunterGenerator, &status, 0);
	
	cleanUp();

	exit(0);

}
