#include <rlglue/Agent_common.h>
#include <rlglue/utils/C/RLStruct_util.h>
#include <rlglue/utils/C/TaskSpec_Parser.h>

#include "box.h"

action_t this_action;
action_t last_action;
observation_t *last_observation = 0;

double* value_function=0;
double sarsa_stepsize = 0.1;
double sarsa_epsilon = 0.5;
double sarsa_gamma = 1.0;

int numActions=0;
int numStates=0;
int numVelocity=0;

int extendRange = 1;

int policy_frozen=0;
int exploring_frozen=0;

int randInRange(int max);
int egreedy(double state, double velocity);
int calculateArrayIndex(double theState, double theVelocity, int theAction);
void save_value_function(const char *fileName);
void load_value_function(const char *fileName);

void agent_init(const char* task_spec){
//	printf("void agent_init\n");
	allocateRLStruct(&this_action,1,0,0);
	allocateRLStruct(&last_action,1,0,0);

	last_observation=allocateRLStructPointer(0,2,0);

	numActions = 11; // -5 ~ 5
	numStates = 51*extendRange; // 0 ~ 50
	
	numVelocity = 101; // -50 ~ 50

	srand(time(0));
	
	value_function=(double *)calloc(numActions*numStates*numVelocity,sizeof(double));
/*	
	int i;
	for(i=0;i<numActions*numStates*numVelocity;i++){
		if(i%numActions == 5){
			value_function[i] = 2;
		}
	}
*/	
}

const action_t *agent_start(const observation_t *this_observation){	

//	printf("const action_t *agent_start\n");	
	int theIntAction=egreedy(this_observation->doubleArray[0], this_observation->doubleArray[1]);
	this_action.intArray[0] = theIntAction;
//	printf("const action_t *agent_start %d\n",theIntAction);
//	printf("%d\n",4/0);
//	this_action.intArray[0]=rand() % 11;

	replaceRLStruct(&this_action, &last_action);
	replaceRLStruct(this_observation, last_observation);

	return &this_action;
}

const action_t *agent_step(double reward, const observation_t *this_observation){
	//	printf("const action_t *agent_step\n");

	double newState=this_observation->doubleArray[0];
	double newVelocity = this_observation->doubleArray[1];
	double lastState=last_observation->doubleArray[0];
	double lastVelocity = last_observation->doubleArray[1];

	int lastAction=last_action.intArray[0];

	int newAction=egreedy(newState, newVelocity);
//	printf("const action_t *agent_step %d\n",newAction);
	
	double Q_sa=value_function[calculateArrayIndex(lastState,lastVelocity, lastAction)];
	double Q_sprime_aprime=value_function[calculateArrayIndex(newState, newVelocity, newAction)];
	
	double new_Q_sa=Q_sa + sarsa_stepsize * (reward + sarsa_gamma * Q_sprime_aprime - Q_sa);

	/*
	   학습하지 않을 때 : policy_frozen=1
	   학습하고 있을 때 : policy_frozen=0
	*/
	if(!policy_frozen){
		if(value_function[calculateArrayIndex(lastState, lastVelocity, lastAction)] < new_Q_sa){
			value_function[calculateArrayIndex(lastState,lastVelocity, lastAction)]=new_Q_sa;
		}
	}
	this_action.intArray[0]=newAction;

	replaceRLStruct(&this_action, &last_action);
	replaceRLStruct(this_observation, last_observation);
	return &this_action;	
}

void agent_end(double reward){
//	printf("void agent_end\n");
	double lastState=last_observation->doubleArray[0];
	double lastVelocity=last_observation->doubleArray[1];
if(reward==100) printf("laststate %lf, lastvel %lf, rewward %lf  \n",lastState,lastVelocity, reward);
	int lastAction=last_action.intArray[0];

	double Q_sa=value_function[calculateArrayIndex(lastState,lastVelocity, lastAction)];
	double new_Q_sa=Q_sa + sarsa_stepsize * (reward - Q_sa);

	/*
	   학습하지 않을 때 : policy_frozen=1
	   학습하고 있을 때 : policy_frozen=0
	*/
	if(!policy_frozen){
		if(value_function[calculateArrayIndex(lastState, lastVelocity, lastAction)] < new_Q_sa){
	//		printf("value function %lf new Qsa %lf\n", value_function[calculateArrayIndex(lastState, lastVelocity, lastAction)], new_Q_sa);
			value_function[calculateArrayIndex(lastState,lastVelocity, lastAction)]=new_Q_sa;
		}
	}
	clearRLStruct(&last_action);
	clearRLStruct(last_observation);

	box_cleanup();
}

void agent_cleanup(){
	clearRLStruct(&this_action);
	clearRLStruct(&last_action);
	freeRLStructPointer(last_observation);

	if(value_function!=0){
		free(value_function);
		value_function=0;
	}
}

const char* agent_message(const char* inMessage){
//	printf("const cahr* agent_message\n");
	static char buffer[128];

	if(strcmp(inMessage,"freeze learning")==0){
		policy_frozen=1;
		return "message understood, policy frozen";
	}

	if(strcmp(inMessage,"unfreeze learning")==0){
		policy_frozen=0;
		return "message understood, policy unfrozen";
	}

	if(strcmp(inMessage,"freeze exploring")==0){
		exploring_frozen=1;
		return "message understood, exploring frozen";
	}

	if(strcmp(inMessage,"unfreeze exploring")==0){
		exploring_frozen=0;
		return "message understood, exploring unfrozen";
	}

	if(strncmp(inMessage,"save_policy",11)==0){
		strncpy(buffer,inMessage+12,sizeof(buffer));
		printf("Saving value function...");
		save_value_function(buffer);
		printf("Saved.\n");
		return "message understood, saving policy";
	}

	if(strncmp(inMessage,"load_policy",11)==0){
		strncpy(buffer,inMessage+12,sizeof(buffer));
		printf("Loading value function...");
		load_value_function(buffer);
		printf("Loaded.\n");
		return "message understood, loading policy";
	}
	
	return "agent.c does not understand your message.";
}

void save_value_function(const char *fileName){
	FILE *fp;
	fp=fopen(fileName, "wb");

	fwrite(value_function,sizeof(double),numStates*numActions*numVelocity,fp);
	fclose(fp);
}

void load_value_function(const char *fileName){
	FILE *fp;
	fp=fopen(fileName, "rb");

	fread(value_function,sizeof(double),numStates*numActions*numVelocity,fp);
	fclose(fp);
}

int egreedy(double state, double velocity){
//	printf("int egreedy numAction: %d\n",numActions);
/*
	int maxIndex = 0;
	int a = 1;
	int randFrequency=(int)(1.0f/sarsa_epsilon);

	if(!exploring_frozen){
		if((rand() % randFrequency == 1)) {
			return randInRange(numActions-1);
		}
	}
*/
	/*otherwise choose the greedy action*/
/*	maxIndex = 0;
	for(a = 1; a < numActions; a++){
		if(value_function[calculateArrayIndex(state,velocity,a)] > value_function[calculateArrayIndex(state,velocity,maxIndex)]) {
			maxIndex = a;
		}
	}
	return maxIndex;
*/
	int maxIndex = 0;
	int a= 1;
	int randFrequency = (int)(1.0f/sarsa_epsilon);

	for(a=1;a<numActions;a++){
		if(value_function[calculateArrayIndex(state,velocity,a)] > value_function[calculateArrayIndex(state,velocity,maxIndex)]){
			maxIndex = a;
		}
	}

	if(value_function[calculateArrayIndex(state,velocity,maxIndex)]==0.0){
		return randInRange(numActions-1);
	}

	//printf("max index %d\n",4/0);
	
	return maxIndex;
}

int randInRange(int max){
//cnt++;	printf("int randInRage %d \n",cnt);
	double r, x;
	r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
	x = (r * (max+1));
	return (int)x;
}

int calculateArrayIndex(double theState, double theVelocity, int theAction){
//	printf("int calculateArrayIndex(%lf, %lf, %d)\n",theState, theVelocity, theAction);
	assert(theState<numStates);
	assert(theAction<numActions);

//	printf(" (int)State%d*actions%d*numvel%d + vel%d*ac%d +theAction%d\n",(int)theState, numActions, numVelocity, (int)theVelocity, numActions, theAction);
	int value = ((int)theState)*numActions*numVelocity+((int)theVelocity)*numActions+theAction;
//	printf("value : %d\n", value);
	return value;
//	return (int)(theState*extendRange)*numActions*(theVelocity)+theAction;
}
