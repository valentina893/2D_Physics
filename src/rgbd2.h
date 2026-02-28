// rgbd2.h

#ifndef RGBD2_H
#define RGBD2_H

#include "vec2.h"

#define gravity 980.0f
#define delta_time (1.0f / 60.0f)

/*
2D rigidbody struct that stores position, velocity, acceleration, mass, restitution, and dimensions.
*/
typedef struct rgbd2 {

    vec2 pos, vel, acc;

    float angle, angle_vel, angle_acc;

    float mass, inv_mass;

    float inertia, inv_inertia;

    float restitution;

    float width, height;

} rgbd2;

/*
Initializes rgbd2 with position, mass, restitution, and dimensions.
*/
rgbd2 rgbd2_create(float x_pos, float y_pos, float mass, float restitution, float size1, float size2);

/*
Updates rgbd2 position via Euler integration.
*/
void rgbd2_integrateEuler(rgbd2* rgbd2, float dt);

/*
Updates rgbd2 acceleration with given force (can be gravity).
*/
void rgbd2_applyForce(rgbd2* rgbd2, vec2 force, vec2 contact);

/*
Checks rgbd2 for colliding with window borders.
*/
void rgbd2_windowCollision(rgbd2* rgbd2, int window_width, int window_height);

/*
Checks if two rgbd2 structs a and b are colliding with eachother.
*/
void rgbd2_objectCollision(rgbd2* a, rgbd2* b);

#endif
