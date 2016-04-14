#include "box.h"
#include <stdio.h>
#include <Box2D/Box2D.h>

b2Body* bodies;
b2Vec2 gravity(0.0f, -10.0f);
b2World* world;
float timeStep = 1.0f / 60.0f;
int velocityIterations = 6;
int positionIterations = 2;

/* Box2D 초기화 */
void box_init(){
	world = new b2World(gravity, true);

	/* body 정의 */
	b2BodyDef dynamicBodyDef, staticBodyDef;
	dynamicBodyDef.type = b2_dynamicBody;
	staticBodyDef.type = b2_staticBody;

	/* shape 정의 */
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(1, 1);

	/* fixture 정의 */
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &polygonShape;
	myFixtureDef.density = 1;

	/* dynamicBody 위치값 (35,1)으로 세팅 */
	dynamicBodyDef.position.Set(35,1);
	bodies = world->CreateBody(&dynamicBodyDef);
	bodies->CreateFixture(&myFixtureDef);
	
	/* staticBody 위치값 세팅 */
	staticBodyDef.position.Set(0, 0);
	polygonShape.SetAsEdge(b2Vec2(-100000000000, 0), b2Vec2(100000000000, 0) );
	world->CreateBody(&staticBodyDef)->CreateFixture(&myFixtureDef);
}

/* bodies의 x축 위치값을 받아옵니다. */
double get_position(){
	world->Step(timeStep, velocityIterations, positionIterations);
	double posx=0.0;

	posx = bodies->GetPosition().x;
	return posx;
}

/* bodies의 x축 속도값을 받아옵니다. */
double get_velocity(){
	world->Step(timeStep, velocityIterations, positionIterations);

	double velx = bodies->GetLinearVelocity().x;
	return velx;
}

/* bodies에 force값 만큼 힘을 가합니다. */
void apply_force(int force){
	world->Step(timeStep, velocityIterations, positionIterations);

	bodies->ApplyForce(b2Vec2((force-5)*100, 0), bodies->GetWorldCenter() );

//	bodies->ApplyLinearImpulse(b2Vec2(random_number-5, 0), bodies->GetWorldCenter() );
}

/* Box2D 삭제 free */
void box_cleanup(){
	delete world;
}
