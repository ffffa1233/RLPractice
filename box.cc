#include "box.h"
#include <stdio.h>
#include <Box2D/Box2D.h>

b2Body* bodies;
b2Vec2 gravity(0.0f, -10.0f);
b2World* world;
float timeStep = 1.0f / 60.0f;
int velocityIterations = 6;
int positionIterations = 2;

//initialize box2d 
void box_init(){
//	printf("void box_init\n");
	world = new b2World(gravity, true);

	//body definition
	b2BodyDef dynamicBodyDef, staticBodyDef;
	dynamicBodyDef.type = b2_dynamicBody;
	staticBodyDef.type = b2_staticBody;

	//shape definition
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(1, 1);

	//fixture defnition
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &polygonShape;
	myFixtureDef.density = 1;

	dynamicBodyDef.position.Set(35,1);
	bodies = world->CreateBody(&dynamicBodyDef);
	bodies->CreateFixture(&myFixtureDef);
	
	staticBodyDef.position.Set(0, 0);
	polygonShape.SetAsEdge(b2Vec2(-100000000000, 0), b2Vec2(100000000000, 0) );
	world->CreateBody(&staticBodyDef)->CreateFixture(&myFixtureDef);
}

double get_position(){
//	printf("double get_position\n");
	world->Step(timeStep, velocityIterations, positionIterations);
	double posx=0.0;

	posx = bodies->GetPosition().x;
	return posx;
}

double get_velocity(){
	world->Step(timeStep, velocityIterations, positionIterations);
	b2Vec2 vel = bodies->GetLinearVelocity();
	return vel.x;
}

void apply_force(int random_number){
//	printf("void apply_force\n");
	world->Step(timeStep, velocityIterations, positionIterations);

//	printf("i:%d\n",i);
	bodies->ApplyForce(b2Vec2((random_number-5)*100, 0), bodies->GetWorldCenter() );

//	bodies->ApplyLinearImpulse(b2Vec2(random_number-5, 0), bodies->GetWorldCenter() );
	world->Step(timeStep,velocityIterations, positionIterations);

}

void box_cleanup(){
	delete world;
}
