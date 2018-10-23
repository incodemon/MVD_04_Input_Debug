### La Salle - Universitat Ramon Llull : Master in Advanced Videogame Development
## Engine Programming - 03-Lights_Camera

### TODO 

#### in ControlSystem
 - investigate makeRotationMatrix function of lm::mat4
 - create yaw transformation matrix  with y axis, and rotate camera front vector with it
 - create pitch transformation matrix with side axis of camera, and rotate front vector
 - if keys are pressed, move camera position along forward and side axes
 - don't forget to multiply speed by delta time

#### in DebugSystem
 - have a look at createGrid_() to see how grid is made
 - Use grid_shader_->program to draw grid
 - use glGetUniformLocation to query uniforms (see GraphicsSystem for examples)
 - use glUniformXXX to send uniform information
		- hint: to send array of colours use glUniform3fv(u_color, 4, grid_colors);
 - draw 'grid_vao_' geometry using GL_LINES, and 'grid_num_indices_'

 - have a look at createCube_() to see how -1 -> 1 cube is made
 - Keep using grid_shader_ program to draw camera frustum
 - for all camera components:
 - pass uniforms (some may already be passed, no need to pass them twice)
 - calculate inverse of view projection matrix of camera frustum to draw
		- careful, function .inverse() acts directly on the matrix! Make a copy
 - calculate MVP:
 - mvp = rendering_cam_view_projection * camera_to_draw_inverse_view_projection
 - draw cube_vao_ with GL_LINES and 24 indices

 - use icon_shader_->program
 - set uniforms (see shader definition in DebugSystem.h)
 - activate and bind texture to slot 0
		- icon_camera_texture_ for cameras, icon_light_texture_ for lights
 - create mvp only using translation component of final mvp of lights/camera
	 i.e. only keep final column of MVP matrix, rest is identity
 - bind icon_vao_ and draw GL_TRIANGLES with 6 indices