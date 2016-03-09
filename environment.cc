#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <rlglue/Environment_common.h>
#include <rlglue/utils/C/RLStruct_util.h>

#include <Box2D/Box2D.h>
#include "box.h"

static observation_t this_observation;
static reward_observation_terminal_t this_reward_observation;

double current_state = 0.0;

int calculate_reward(double current_state);
int check_terminal(double current_state);

const char *env_init(){
//	printf("const char env_init\n");
	const char *task_spec_string = "[VERSION RL-GLue-3.0] [PROBLEMTYPE episodic] "
									"[OBSERVATIONS DOUBLE (0.0 ~ 50.0)] "
									"[ACTIONS INT (0 ~ 10)] [REWARDS(?.? ?.?)] "
									"[EXTRA environment(C/C++) by PSC, CBH]";

	allocateRLStruct(&this_observation,0,1,0);
	this_reward_observation.observation = &this_observation;
	this_reward_observation.reward = 0;
	this_reward_observation.terminal = 0;

//	box_init();

	return task_spec_string;
}
/*
   env_start() chooses the intial state of the environment and returns the corresponding observation.
 */
const observation_t *env_start(){
//	printf("const observation_t *env_start\n");
	box_init();

	current_state = get_position();
	this_observation.doubleArray[0] = current_state;
	
	return &this_observation; 
}

const reward_observation_terminal_t *env_step(const action_t *this_action){
//	printf("const reward_observation_terminal_t *env_step\n");
//	fprintf(stderr,"evn");
	double the_reward = 0.0;
	int force = this_action->intArray[0];
	apply_force(force);
	current_state = get_position();

	the_reward = calculate_reward(current_state);	
//	printf("current state : %lf, apply force : %d\n",current_state, this_action->intArray[0]);
	this_reward_observation.observation->doubleArray[0] = current_state;
	this_reward_observation.reward = the_reward;
	this_reward_observation.terminal = check_terminal(current_state);
//	printf("ter:%d,pos:%lf",this_reward_observation.terminal,current_state);
	return &this_reward_observation;
}

void env_cleanup(){
	clearRLStruct(&this_observation);
//	box_cleanup();
}

const char* env_message(const char* inMessage){
	return inMessage;
}

int calculate_reward(double current_state){
	if(current_state>40.1 && current_state<40.3){
		return 100;
	}
	if(current_state > 50.01 || current_state < -0.01){
		return -100;
	}
	return -1;
}

int check_terminal(double current_state){
//	printf("int check_terminal\n");
	if(current_state>40.1 && current_state<40.3){
		return 1;
	}
	if(current_state > 50.01 || current_state < -0.01){
		return 1;
	}
	return 0;
}
