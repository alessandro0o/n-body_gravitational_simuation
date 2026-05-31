#pragma once

class Object
{
public:

	virtual ~Object() = default;

	float mass = 1.0f;
	float radius = 5.0f;

	float angular_position = 0.0f;
	float angular_velocity = 0.0f;
	float angular_acceleration = 0.0f;

	float position_x = 0.0f;
	float position_y = 0.0f;

	float velocity_x = 0.0f;
	float velocity_y = 0.0f;

	float acceleration_x = 0.0f;
	float acceleration_y = 0.0f;

	float force_x = 0.0f;
	float force_y = 0.0f;

	virtual void null_force_setter()
	{
		force_x = 0.0f;
		force_y = 0.0f;
	}

	virtual void update_object(float delta_time)
	{
		acceleration_x = force_x / mass;
		acceleration_y = force_y / mass;

		velocity_x += acceleration_x * delta_time;
		velocity_y += acceleration_y * delta_time;

		position_x += velocity_x * delta_time;
		position_y += velocity_y * delta_time;
	}

	virtual void draw_object() = 0;

	virtual void draw_vectors()
	{
		float velocity_length_multiplier = 0.2f;
		float force_length_multiplier = 4.0f * pow(10, -7);
		DrawLineEx({ position_x, SCREEN_HEIGHT - position_y }, { position_x + velocity_x * velocity_length_multiplier, SCREEN_HEIGHT - (position_y + velocity_y * velocity_length_multiplier) }, 5.0f, BLUE); /* velocity vector */
		DrawLineEx({ position_x, SCREEN_HEIGHT - position_y }, { position_x + force_x * force_length_multiplier, SCREEN_HEIGHT - (position_y + force_y * force_length_multiplier) }, 5.0f, LIGHTGRAY); /* force vector */
	}
};

class Particle : public Object
{
public:

	void draw_object() override
	{
		DrawCircleV({ position_x, SCREEN_HEIGHT - position_y }, radius, PURPLE);
	}
};

class Player : public Object
{
private:

	float angular_velocity = 400.0f;
	float thrust = 1.0f * pow(10, 3);

public:

	void update_object(float delta_time) override
	{

		if (IsKeyDown(KEY_LEFT)) angular_position -= angular_velocity * delta_time;
		if (IsKeyDown(KEY_RIGHT)) angular_position += angular_velocity * delta_time;
		if (IsKeyDown(KEY_UP))
		{
			force_x += cosf(-angular_position * DEG2RAD) * thrust;
			force_y += sinf(-angular_position * DEG2RAD) * thrust;
		}

		acceleration_x = force_x / mass;
		acceleration_y = force_y / mass;

		velocity_x += acceleration_x * delta_time;
		velocity_y += acceleration_y * delta_time;

		position_x += velocity_x * delta_time;
		position_y += velocity_y * delta_time;
	}

	void draw_object() override
	{
		DrawPolyLinesEx({ position_x, SCREEN_HEIGHT - position_y }, 3, 10.0f, angular_position, 2.0f, YELLOW);
		DrawLineEx({ position_x, SCREEN_HEIGHT - position_y }, { position_x + cosf(angular_position * DEG2RAD) * 10.0f, SCREEN_HEIGHT - position_y + sinf(angular_position * DEG2RAD) * 10.0f }, 2.0f, YELLOW);
	}
};

void null_force_setter_v(std::vector<std::unique_ptr<Object>>& objects)
{
	size_t size = objects.size();
	for (size_t i = 0; i < size; i++)
	{
		objects[i]->null_force_setter();
	}
}

void update_particle_v(std::vector<std::unique_ptr<Object>>& objects, float delta_time)
{
	size_t size = objects.size();
	for (size_t i = 0; i < size; i++)
	{
		objects[i]->update_object(delta_time);
	}
}

void draw_particle_v(std::vector<std::unique_ptr<Object>>& objects)
{
	size_t size = objects.size();
	for (size_t i = 0; i < size; i++)
	{
		objects[i]->draw_object();
	}
}

void draw_vectors_v(std::vector<std::unique_ptr<Object>>& objects)
{
	size_t size = objects.size();
	for (size_t i = 0; i < size; i++)
	{
		objects[i]->draw_vectors();
	}
}