// rgbd2.c

#include "rgbd2.h"

rgbd2 rgbd2_create(float x_pos, float y_pos, float mass, float restitution, float size1, float size2) {
    rgbd2 rgbd2 = {
        .pos.x = x_pos, .pos.y = y_pos,
        .mass = mass, .inv_mass = 1.0f / mass,
        .inertia = (1.0f / 12.0f) * mass * (size1 * size1 + size2 * size2),
        .restitution = restitution, 
        .width = size1, .height = size2
    };

    rgbd2.inv_inertia = 1.0f / rgbd2.inertia;

    return rgbd2;
}

void rgbd2_integrateEuler(rgbd2* rgbd2, float dt) {
    // linear
    rgbd2->vel = vec2_add(rgbd2->vel, vec2_mult(rgbd2->acc, (vec2){dt, dt}));
    rgbd2->pos = vec2_add(rgbd2->pos, vec2_mult(rgbd2->vel, (vec2){dt, dt}));
    rgbd2->acc = (vec2){0, 0};

    // angular
    rgbd2->angle_vel += rgbd2->angle_acc * dt;
    rgbd2->angle += rgbd2->angle_vel * dt;
}

void rgbd2_applyForce(rgbd2* rgbd2, vec2 force, vec2 contact) {
    force = vec2_div(force, (vec2){rgbd2->mass, rgbd2->mass});
    rgbd2->acc = vec2_add(rgbd2->acc, force);

    vec2 r = vec2_sub(contact, rgbd2->pos);
    float torque = r.x * force.y - r.y * force.x;
    rgbd2->angle_vel += torque * rgbd2->inv_inertia;
}

void rgbd2_windowCollision(rgbd2* rgbd2, int window_width, int window_height) {

    // colliding right
    if (rgbd2->pos.x + rgbd2->width > window_width) {
        rgbd2->vel.x = -rgbd2->vel.x * rgbd2->restitution;
        rgbd2->pos.x = window_width - rgbd2->width;
    }
    // collide left
    if (rgbd2->pos.x < 0) {
        rgbd2->vel.x = -rgbd2->vel.x * rgbd2->restitution;
        rgbd2->pos.x = 0;
    }
    // collide up
    if (rgbd2->pos.y < 0) {
        rgbd2->vel.y = -rgbd2->vel.y * rgbd2->restitution;
        rgbd2->pos.y = 0;
    }
    // collide bottom
    if (rgbd2->pos.y + rgbd2->height > window_height) {
        rgbd2->vel.y = -rgbd2->vel.y * rgbd2->restitution;
        rgbd2->pos.y = window_height - rgbd2->height;
    }

}

void rgbd2_objectCollision(rgbd2* a, rgbd2* b)
{
    // First check if overlapping
    float dx = b->pos.x - a->pos.x;
    float dy = b->pos.y - a->pos.y;

    float px = (a->width  + b->width)  * 0.5f - fabs(dx);
    float py = (a->height + b->height) * 0.5f - fabs(dy);

    if (px <= 0.f || py <= 0.f)
        return; // no collision

    // Determine collision axis: smallest penetration wins
    if (px < py)
    {
        // Horizontal collision
        if (dx > 0)
        {
            // b is to the right of a → push b right, a left
            a->pos.x -= px * 0.5f;
            b->pos.x += px * 0.5f;
        }
        else
        {
            a->pos.x += px * 0.5f;
            b->pos.x -= px * 0.5f;
        }

        // Invert x velocities (simple response)
        float temp = a->vel.x;
        a->vel.x = b->vel.x;
        b->vel.x = temp;
    }
    else
    {
        // Vertical collision
        if (dy > 0)
        {
            a->pos.y -= py * 0.5f;
            b->pos.y += py * 0.5f;
        }
        else
        {
            a->pos.y += py * 0.5f;
            b->pos.y -= py * 0.5f;
        }

        // Invert y velocities
        float temp = a->vel.y;
        a->vel.y = b->vel.y;
        b->vel.y = temp;
    }
}
