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
double current_velocity = 0.0;

int calculate_reward(double current_state, double current_velocity);
int check_terminal(double current_state, double current_velocity);

const char *env_init(){
//	printf("const char env_init\n");
	const char *task_spec_string = "[VERSION RL-GLue-3.0] [PROBLEMTYPE episodic] "
									"[OBSERVATIONS DOUBLE (0.0 ~ 50.0)] "
									"[ACTIONS INT (0 ~ 10)] [REWARDS(?.? ?.?)] "
									"[EXTRA environment(C/C++) by PSC, CBH]";

	allocateRLStruct(&this_observation,0,2,0);
	this_reward_observation.observation = &this_observation;
	this_reward_observation.reward = 0;
	this_reward_observation.terminal = 0;

	return task_spec_string;
}
/*
   env_start() chooses the intial state of the environment and returns the corresponding observation.
 */
const observation_t *env_start(){
//	printf("const observation_t *env_start\n");
	box_init();

	current_state = get_position();
	current_velocity = get_velocity();
	current_velocity += 50.0;

	this_observation.doubleArray[0] = current_state;
	this_observation.doubleArray[1] = current_velocity;
	
	return &this_observation; 
}

const reward_observation_terminal_t *env_step(const action_t *this_action){
//	printf("const reward_observation_terminal_t *env_step\n");
//	fprintf(stderr,"evn");
	double the_reward = 0.0;
	int force = this_action->intArray[0];
	apply_force(force);
	
	current_state = get_position();
	current_velocity = get_velocity();
	current_velocity += 50.0;

	//	printf("current velocity %lf\n",current_velocity);
	
	the_reward = calculate_reward(current_state, current_velocity);	
//	printf("current state : %lf, apply force : %d\n",current_state, this_action->intArray[0]);
	this_reward_observation.observation->doubleArray[0] = current_state;
	this_reward_observation.observation->doubleArray[1] = current_velocity;
	this_reward_observation.reward = the_reward;
	this_reward_observation.terminal = check_terminal(current_state, current_velocity);
//	printf("ter:%d,pos:%lf",this_reward_observation.terminal,current_state);
	return &this_reward_observation;
}

void env_cleanup(){
	clearRLStruct(&this_observation);
}

const char* env_message(const char* inMessage){
	return inMessage;
}

int calculate_reward(double current_state, double current_velocity){
	if (current_state>=45.0	&& current_state<46.0 && current_velocity<51.0 && current_velocity>=50.0) {
		printf("reward 100    ");
		return 100;
	}
	return 1;
}

int check_terminal(double current_state, double current_velocity){
//	printf("int check_terminal\n");
	if(current_state>=45.0 && current_state<46.0 && current_velocity<51.0 && current_velocity>=50.0){
		printf("terminal\n");
		return 1;
	}
	if(current_state > 51.0 || current_state < 0.0){
//		printf("terminal2\n");
		return 1;
	}
	return 0;
}
