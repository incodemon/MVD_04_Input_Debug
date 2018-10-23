#include "ControlSystem.h"
#include "extern.h"

//set initial state of input system
void ControlSystem::init() {
	//set all keys and buttons to 0
	for (int i = 0; i < GLFW_KEY_LAST; i++) input[i] = 0;
}

//called from hardware input (via game)

void ControlSystem::key_mouse_callback(int key_button, int action, int mods) {
	
	if (action == GLFW_PRESS) input[key_button] = true;
	if (action == GLFW_RELEASE) input[key_button] = false;
}

//called from hardware input (via game)
void ControlSystem::updateMousePosition(int new_x, int new_y) {
	mouse_.delta_x = new_x - mouse_.x;
	mouse_.delta_y = new_y - mouse_.y;
	mouse_.x = new_x;
	mouse_.y = new_y;
}

//called once per frame

void ControlSystem::update(float dt) {
	if (control_type == ControlTypeFree) {
		updateFree(dt);
    }
}

//update an entity with a free movement control component 
void ControlSystem::updateFree(float dt) {
	
	Camera& camera = ECS.getComponentInArray<Camera>(ECS.main_camera);
    Transform& transform = ECS.getComponentFromEntity<Transform>(camera.owner);
	
	//rotate camera if clicking the mouse - update camera.forward
	if (input[GLFW_MOUSE_BUTTON_LEFT]) {
		
		//TODO:
		// - rotate camera front vector according to mouse._delta_x and mouse_.delta_y
	}

	//TODO:
	// - move transform position along cam forward and side vectors

	//update camera position
	camera.position = transform.position();
    
}

