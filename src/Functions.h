#pragma once

void random_init(std::vector<std::unique_ptr<Object>>& objects)
{
	SetRandomSeed(time(NULL));
	int velocity = 100;

	size_t size = objects.size();
	for (size_t i = 0; i < size; i++)
	{
		objects[i]->position_x = GetRandomValue(0, SCREEN_WIDTH / 2.0f) + SCREEN_WIDTH / 4.0f;
		objects[i]->position_y = GetRandomValue(0, SCREEN_HEIGHT / 2.0f) + SCREEN_HEIGHT / 4.0f;

		objects[i]->velocity_x = GetRandomValue(-velocity, velocity);
		objects[i]->velocity_y = GetRandomValue(-velocity, velocity);

		objects[i]->mass = 1.0f * pow(10, 6);
	}
}

void custom_init(std::vector<std::unique_ptr<Object>>& objects)
{
	/* orbital velocity = sqrt(mass * G / distance) */


	objects[0]->position_x = SCREEN_WIDTH / 2.0f;
	objects[0]->position_y = SCREEN_HEIGHT / 2.0f;
	objects[0]->mass = 1.0f * pow(10, 7);
	objects[0]->radius = 20.0f;

	objects[1]->position_x = SCREEN_WIDTH / 2.0f;
	objects[1]->position_y = SCREEN_HEIGHT / 2.0f + 150.0f;
	objects[1]->angular_position = 270.0f;
	objects[1]->velocity_x = 258.2f;
	objects[1]->mass = 1.0f * pow(10, 0);

	objects[2]->position_x = SCREEN_WIDTH / 2.0f;
	objects[2]->position_y = SCREEN_HEIGHT / 2.0f + 125.0f;
	objects[2]->velocity_x = 282.84271f;
	objects[2]->mass = 1.0f * pow(10, 0);

	objects[3]->position_x = SCREEN_WIDTH / 2.0f;
	objects[3]->position_y = SCREEN_HEIGHT / 2.0f + 200.0f;
	objects[3]->velocity_x = 223.6068f;
	objects[3]->mass = 1.0f * pow(10, 0);
}

void set_boundry(std::vector<std::unique_ptr<Object>>& objects)
{
	float velocity_multiplier = 0.5f;

	size_t size = objects.size();
	for (size_t i = 0; i < size; i++)
	{
		if ((objects[i]->position_x + objects[i]->radius) > SCREEN_WIDTH * map_dimension_multiplier)
		{
			objects[i]->position_x = SCREEN_WIDTH * map_dimension_multiplier - objects[i]->radius;
			objects[i]->velocity_x = -objects[i]->velocity_x * velocity_multiplier;
		}
		if (objects[i]->position_x < (objects[i]->radius - SCREEN_WIDTH * (map_dimension_multiplier - 1)))
		{
			objects[i]->position_x = objects[i]->radius - SCREEN_WIDTH * (map_dimension_multiplier - 1);
			objects[i]->velocity_x = -objects[i]->velocity_x * velocity_multiplier;
		}
		if ((objects[i]->position_y + objects[i]->radius) > SCREEN_HEIGHT * map_dimension_multiplier)
		{
			objects[i]->position_y = SCREEN_HEIGHT * map_dimension_multiplier - objects[i]->radius;
			objects[i]->velocity_y = -objects[i]->velocity_y * velocity_multiplier;
		}
		if (objects[i]->position_y < (objects[i]->radius - SCREEN_HEIGHT * (map_dimension_multiplier - 1)))
		{
			objects[i]->position_y = objects[i]->radius - SCREEN_HEIGHT * (map_dimension_multiplier - 1);
			objects[i]->velocity_y = -objects[i]->velocity_y * velocity_multiplier;
		}
	}
}

void draw_boundry()
{
	Color border_color = DARKGRAY;
	float border_thickness = 7.0f;

	float x1 = -SCREEN_WIDTH * (map_dimension_multiplier - 1);
	float x2 = SCREEN_WIDTH * map_dimension_multiplier;
	float y1 = -SCREEN_HEIGHT * (map_dimension_multiplier - 1);
	float y2 = SCREEN_HEIGHT * map_dimension_multiplier;

	DrawLineEx({ x1, y1 }, { x2, y1 }, border_thickness, border_color);
	DrawLineEx({ x1, y1 }, { x1, y2 }, border_thickness, border_color);
	DrawLineEx({ x1, y2 }, { x2, y2 }, border_thickness, border_color);
	DrawLineEx({ x2, y2 }, { x2, y1 }, border_thickness, border_color);
}


void gravitational_attraction(std::vector<std::unique_ptr<Object>>& objects)
{
	float force_multiplier = 1.0f * pow(10, 0);

	size_t size = objects.size();
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = i; j < size; j++)
		{
			if (i != j && (objects[i]->position_x != objects[j]->position_x || objects[i]->position_y != objects[j]->position_y))
			{
				float distance_sqr = Vector2DistanceSqr({ objects[i]->position_x, objects[i]->position_y },
														{ objects[j]->position_x, objects[j]->position_y });

				if (distance_sqr > (objects[i]->radius + objects[j]->radius) * (objects[i]->radius + objects[j]->radius))
				{
					float gravitational_attraction = (objects[i]->mass * objects[j]->mass * force_multiplier) / distance_sqr;

					float distance = sqrtf(distance_sqr);

					float gravitational_attraction_x = gravitational_attraction * ((objects[j]->position_x - objects[i]->position_x) / distance);
					float gravitational_attraction_y = gravitational_attraction * ((objects[j]->position_y - objects[i]->position_y) / distance);

					objects[i]->force_x += gravitational_attraction_x;
					objects[i]->force_y += gravitational_attraction_y;

					objects[j]->force_x -= gravitational_attraction_x;
					objects[j]->force_y -= gravitational_attraction_y;
				}
			}
		}
	}
}

void draw_com(std::vector<std::unique_ptr<Object>>& objects)
{
	float com_x = objects[0]->position_x;
	float com_y = objects[0]->position_y;
	float com_mass = objects[0]->mass;


	size_t size = objects.size();
	for (size_t i = 1; i < size; i++)
	{
		com_x -= (com_x - objects[i]->position_x) * objects[i]->mass / (com_mass + objects[i]->mass);
		com_y -= (com_y - objects[i]->position_y) * objects[i]->mass / (com_mass + objects[i]->mass);
		com_mass += objects[i]->mass;
	}

	DrawCircleV({ com_x, SCREEN_HEIGHT - com_y }, 6.0f, RED);
}