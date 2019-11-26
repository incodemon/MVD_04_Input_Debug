//
//  Game.cpp
//
//  Copyright � 2018 Alun Evans. All rights reserved.
//

#include "Game.h"
#include "Shader.h"
#include "extern.h"
#include "Parsers.h"

Game::Game() {
    
}

//Nothing here yet
void Game::init() {
    
    
    //******* INIT SYSTEMS *******//
    
    //init systems except debug, which needs info about scene
    control_system_.init();
    graphics_system_.init();
    
    //******** CREATE AND ADD RESOURCES **********//
    
    //shaders
	graphics_system_.loadShader("phong", "data/shaders/phong.vert", "data/shaders/phong.frag");
    
    //geometries
    int teapot_geom_id = graphics_system_.createGeometryFromFile("data/assets/teapot_small.obj");
    int plane_geom_id = graphics_system_.createPlaneGeometry();
    
    //materials and textures
	int default_mat_id = graphics_system_.createMaterial();
	graphics_system_.getMaterial(default_mat_id).diffuse_map = Parsers::parseTexture("data/assets/test.tga");;
	graphics_system_.getMaterial(default_mat_id).shader_id = graphics_system_.getShaderProgram("phong");
    
    
    //******* CREATE OTHER ENTITIES AND ADD COMPONENTS *******//
    
    int ent_teapot = ECS.createEntity("Teapot");
    Mesh& tmc = ECS.createComponentForEntity<Mesh>(ent_teapot);
    tmc.geometry = teapot_geom_id;
    tmc.material = default_mat_id;
    ECS.getComponentFromEntity<Transform>(ent_teapot).translate(0.0, -0.5, 0.0);
    
    int ent_light1 = ECS.createEntity("Light 1");
    ECS.createComponentForEntity<Light>(ent_light1);
    ECS.getComponentFromEntity<Transform>(ent_light1).translate(10.0f, 5.0f, 10.0f);
    ECS.getComponentFromEntity<Light>(ent_light1).color = lm::vec3(1.0f, 1.0f, 1.0f);
    
    int ent_light2 = ECS.createEntity("Light 2");
    ECS.createComponentForEntity<Light>(ent_light2);
    ECS.getComponentFromEntity<Transform>(ent_light2).translate(-10.0f, -10.0f, 10.0f);
    ECS.getComponentFromEntity<Light>(ent_light2).color = lm::vec3(0.5f, 0.5f, 1.0f);
    
    int ent_player = ECS.createEntity("Player");
    Camera& player_cam = ECS.createComponentForEntity<Camera>(ent_player);
    ECS.getComponentFromEntity<Transform>(ent_player).translate(0.0, 1.0, 5.0);
    player_cam.position = lm::vec3(0.0f, 1.0f, 5.0f);
    player_cam.forward = lm::vec3(0.0f, 0.0f, -1.0f);
    player_cam.setPerspective(60.0f*DEG2RAD, 1, 0.01f, 100.0f);
    
    int ent_second_cam = ECS.createEntity("Second Camera");
    Camera& second_cam = ECS.createComponentForEntity<Camera>(ent_second_cam);
    ECS.getComponentFromEntity<Transform>(ent_second_cam).translate(20.0f, 20.0f, 20.0f);
    second_cam.position = lm::vec3(20.0f, 20.0f, 20.0f);
    second_cam.forward = lm::vec3(-1.0f, -0.7f, -1.0f);
    second_cam.setPerspective(45.0f*DEG2RAD, 1, 1.0f, 100.0f);
    
    //set as the main camera
    ECS.main_camera = ECS.getComponentID<Camera>(ent_player);
    

    //******* INIT DEBUG SYSTEM *******//
    debug_system_.init();
    debug_system_.setActive(true);


	//TODO in ControlSystem
	// - investigate makeRotationMatrix function of lm::mat4
	// - create yaw transformation matrix  with y axis, and rotate camera front vector with it
	// - create pitch transformation matrix with side axis of camera, and rotate front vector
	// - if keys are pressed, move camera position along forward and side axes
	// - don't forget to multiply speed by delta time

	//TODO in DebugSystem
	// - have a look at createGrid_() to see how grid is made
	// - Use grid_shader_->program to draw grid
	// - use glGetUniformLocation to query uniforms (see GraphicsSystem for examples)
	// - use glUniformXXX to send uniform information
	//		- hint: to send array of colours use glUniform3fv(u_color, 4, grid_colors);
	// - draw 'grid_vao_' geometry using GL_LINES, and 'grid_num_indices_'

	// - have a look at createCube_() to see how -1 -> 1 cube is made
	// - Keep using grid_shader_ program to draw camera frustum
	// - for all camera components:
	// - pass uniforms (some may already be passed, no need to pass them twice)
	// - calculate inverse of view projection matrix of camera frustum to draw
	//		- careful, function .inverse() acts directly on the matrix! Make a copy
	// - calculate MVP:
	// - mvp = rendering_cam_view_projection * camera_to_draw_inverse_view_projection
	// - draw cube_vao_ with GL_LINES and 24 indices

	// - use icon_shader_->program
	// - set uniforms (see shader definition in DebugSystem.h)
	// - activate and bind texture to slot 0
	//		- icon_camera_texture_ for cameras, icon_light_texture_ for lights
	// - create mvp only using translation component of final mvp of lights/camera
	//	 i.e. only keep final column of MVP matrix, rest is identity
	// - bind icon_vao_ and draw GL_TRIANGLES with 6 indices

}

//Entry point for game update code
void Game::update(float dt) {
    //update each system in turn
    
    //update input
    control_system_.update(dt);
    
    //render
    graphics_system_.update(dt);
    
    //debug
    debug_system_.update(dt);
    
    
}

