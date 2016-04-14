#include <rlglue/Environment_common.h>
#include <rlglue/utils/C/RLStruct_util.h>

#include "box.h"

static observation_t this_observation;
static reward_observation_terminal_t this_reward_observation;

double current_state = 0.0;
double current_velocity = 0.0;

int calculate_reward(double current_state, double current_velocity);
int check_terminal(double current_state, double current_velocity);

const char *env_init(){
	const char *task_spec_string = "[VERSION RL-GLue-3.0] [PROBLEMTYPE episodic] "
									"[OBSERVATIONS DOUBLE (0.0 ~ 50.0)] "
									"[ACTIONS INT (0 ~ 10)] [REWARDS(100 0)] "
									"[EXTRA environment(C/C++) by PSC, CBH]";

	allocateRLStruct(&this_observation,0,2,0);
	this_reward_observation.observation = &this_observation;
	this_reward_observation.reward = 0;
	this_reward_observation.terminal = 0;

	return task_spec_string;
}

const observation_t *env_start(){
	box_init();

	current_state = get_position();
	current_velocity = get_velocity();
	current_velocity += 50.0;

	this_observation.doubleArray[0] = current_state;
	this_observation.doubleArray[1] = current_velocity;
	
	return &this_observation; 
}

const reward_observation_terminal_t *env_step(const action_t *this_action){
	int force = this_action->intArray[0];
	apply_force(force);
	
	current_state = get_position();
	current_velocity = get_velocity();
	current_velocity += 50.0;
	
	this_reward_observation.observation->doubleArray[0] = current_state;
	this_reward_observation.observation->doubleArray[1] = current_velocity;
	
	this_reward_observation.reward = calculate_reward(current_state, current_velocity);
	this_reward_observation.terminal = check_terminal(current_state, current_velocity);
	
	return &this_reward_observation;
}

void env_cleanup(){
	clearRLStruct(&this_observation);
}

const char* env_message(const char* inMessage){
	return inMessage;
}

int calculate_reward(double current_state, double current_velocity){
	if((int)current_state==45 && (int)current_velocity==50){
		return 100;
	}
	return 0;
}

int check_terminal(double current_state, double current_velocity){
	if((int)current_state==45 && (int)current_velocity==50){
		return 1;
	}
	if(current_state > 51.0 || current_state < 0.0){
		return 1;
	}
	return 0;
}
