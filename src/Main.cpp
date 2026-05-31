
/* TO DO */
/*
    spostare camera con mouse
*/

#include <vector>
#include <iostream>
#include <memory>
#include "raylib.h"
#include "raymath.h"
#include "time.h"

static constexpr int target_fps = 120 ;
static constexpr float sim_speed_factor = 0.05f;

static constexpr float SCREEN_HEIGHT = 1440.0f;
static constexpr float SCREEN_WIDTH = 2560.0f;

static constexpr float map_dimension_multiplier = 3.0f;

#include "Objects.h"
#include "Functions.h"


/* keyboard inputs handler */

void toggle_state(int key, bool* toggled)
{
    if (IsKeyPressed(key) && (*toggled)) *toggled = false;
    else if (IsKeyPressed(key) && !(*toggled)) *toggled = true;
}

int main(void)
{
    /* window init */

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "gravity_sim");
    ToggleBorderlessWindowed();
    SetTargetFPS(target_fps);

    float delta_time = 0.0f;

    /* 2d camera */

    float camera_pos_x = SCREEN_WIDTH / 2.0f;
    float camera_pos_y = SCREEN_HEIGHT / 2.0f;

    Camera2D camera = { 0 };
    camera.target = { camera_pos_x, camera_pos_y };
    camera.offset = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 5.0f;

    /* states */

    bool show_vectors = false;
    bool show_com = false;
    bool paused = false;
    bool apply_boundry = false;
    bool slow_motion = false;
    bool player_camera = false;


    /* simulation init */

    std::vector<std::unique_ptr<Object>> objects;

    size_t n_objects = 4;
    for (size_t i = 0; i < n_objects; ++i)
    {
        objects.push_back(std::make_unique<Particle>());
    }

    objects.push_back(std::make_unique<Player>());

    custom_init(objects);
    //random_init(objects);

    while (!WindowShouldClose())
    {

        /* keyboard inputs (camera and toggles) */

        toggle_state(KEY_KP_1, &show_vectors);
        toggle_state(KEY_KP_2, &show_com);
        toggle_state(KEY_KP_3, &apply_boundry);
        toggle_state(KEY_KP_4, &slow_motion);
        toggle_state(KEY_K, &paused);
        toggle_state(KEY_P, &player_camera);
        if (IsKeyPressed(KEY_F11)) ToggleBorderlessWindowed();

        if (IsKeyDown(KEY_A) && camera_pos_x > -SCREEN_WIDTH * (map_dimension_multiplier - 1) && !player_camera) camera_pos_x -= 5.0f;
        if (IsKeyDown(KEY_D) && camera_pos_x < SCREEN_WIDTH * map_dimension_multiplier && !player_camera) camera_pos_x += 5.0f;
        if (IsKeyDown(KEY_W) && camera_pos_y > -SCREEN_HEIGHT * (map_dimension_multiplier - 1) && !player_camera) camera_pos_y -= 5.0f;
        if (IsKeyDown(KEY_S) && camera_pos_y < SCREEN_HEIGHT * map_dimension_multiplier && !player_camera) camera_pos_y += 5.0f;

        camera.target = { camera_pos_x, camera_pos_y };
        camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.5f));
        if (camera.zoom <= 0.5f / map_dimension_multiplier) camera.zoom = 0.5f / map_dimension_multiplier;
        if (camera.zoom >= 10.0f) camera.zoom = 10.0f;

        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 1.0f;
            camera_pos_x = SCREEN_WIDTH / 2.0f;
            camera_pos_y = SCREEN_HEIGHT / 2.0f;
        }
        if (player_camera)
        {
            camera.target = { objects[n_objects]->position_x, SCREEN_HEIGHT - objects[n_objects]->position_y };
        }

        /* physics update */

        if (!paused)
        {
            /* updating objects if simulation is not paused */

            update_particle_v(objects, delta_time);
            null_force_setter_v(objects);

            if (apply_boundry) set_boundry(objects);
            gravitational_attraction(objects);
        }

        /* buffer init */

        BeginDrawing();
        {
            ClearBackground(BLACK);

            BeginMode2D(camera);
            {

                /* drawing objects */

                draw_particle_v(objects);
                if (show_vectors) draw_vectors_v(objects);
                if (show_com) draw_com(objects);

                /* drawing bounding box */

                if (apply_boundry) draw_boundry();

            }
            EndMode2D();

            /* drawing ui */

            DrawFPS(10, 10);

        }
        EndDrawing();

        /* delta time */

        if (slow_motion) delta_time = GetFrameTime() * sim_speed_factor;
        else delta_time = GetFrameTime();
    }

    CloseWindow();

    return 0;
}