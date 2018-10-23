#include "DebugSystem.h"
#include "extern.h"
#include "Parsers.h"

DebugSystem::~DebugSystem() {
    delete grid_shader_;
    delete icon_shader_;
}

void DebugSystem::init() {
    //init booleans
    draw_grid_ = false;
    draw_icons_ = false;
    draw_frustra_ = false;
    
    //compile debug shaders from strings in header file
    grid_shader_ = new Shader();
    grid_shader_->compileFromStrings(line_vertex_shader_, line_fragment_shader_);
    icon_shader_ = new Shader();
    icon_shader_->compileFromStrings(icon_vertex_shader_, icon_fragment_shader_);
    
    //create geometries
    createGrid_();
    createIcon_();
    createCube_();
    
    //create texture for light icon
    icon_light_texture_ = Parsers::parseTexture("data/assets/icon_light.tga");
    icon_camera_texture_ = Parsers::parseTexture("data/assets/icon_camera.tga");
}

//draws debug information or not
void DebugSystem::setActive(bool a) {
    draw_grid_ = a;
    draw_icons_ = a;
    draw_frustra_ = a;
}

//called once per frame
void DebugSystem::update(float dt) {
    
    //get the camera view projection matrix
    lm::mat4 vp = ECS.getComponentInArray<Camera>(ECS.main_camera).view_projection;
    

        
    //TODO: draw grid
       

    auto& cameras = ECS.getAllComponents<Camera>();
    for (auto& cc : cameras) {

		//TODO: draw camera frustum


    }

    auto& lights = ECS.getAllComponents<Light>();
    for (auto& curr_light : lights) {
        Transform& curr_light_transform = ECS.getComponentFromEntity<Transform>(curr_light.owner);
    
		//TODO: draw light icons

    }

        
    for (auto& curr_camera : cameras) {
        Transform& curr_cam_transform = ECS.getComponentFromEntity<Transform>(curr_camera.owner);
        
		//TODO: draw camera icons
            
    }

}

///////////////////////////////////////////////
// **** Functions to create geometry ********//
///////////////////////////////////////////////

//creates a simple quad to store a light texture
void DebugSystem::createIcon_() {
    float is = 0.5f;
    GLfloat icon_vertices[12]{-is, -is, 0, is, -is, 0, is, is, 0, -is, is, 0};
    GLfloat icon_uvs[8]{ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    GLuint icon_indices[6]{ 0, 1, 2, 0, 2, 3 };
    glGenVertexArrays(1, &icon_vao_);
    glBindVertexArray(icon_vao_);
    GLuint vbo;
    //positions
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,  sizeof(icon_vertices), icon_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //uvs
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(icon_uvs), icon_uvs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    //indices
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(icon_indices), icon_indices, GL_STATIC_DRAW);
    //unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void DebugSystem::createCube_() {
    
    //4th component is color!
    const GLfloat quad_vertex_buffer_data[] = {
        -1.0f,  -1.0f,  -1.0f,  0.0f,  //near bottom left
        1.0f,   -1.0f,  -1.0f,  0.0f,   //near bottom right
        1.0f,   1.0f,   -1.0f,  0.0f,    //near top right
        -1.0f,  1.0f,   -1.0f,  0.0f,   //near top left
        -1.0f,  -1.0f,  1.0f,   0.0f,   //far bottom left
        1.0f,   -1.0f,  1.0f,   0.0f,    //far bottom right
        1.0f,   1.0f,   1.0f,   0.0f,     //far top right
        -1.0f,  1.0f,   1.0f,   0.0f,    //far top left
    };
    
    const GLuint quad_index_buffer_data[] = {
        0,1, 1,2, 2,3, 3,0, //top
        4,5, 5,6, 6,7, 7,4, // bottom
        4,0, 7,3, //left
        5,1, 6,2, //right
    };
    
    glGenVertexArrays(1, &cube_vao_);
    glBindVertexArray(cube_vao_);
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_buffer_data), quad_vertex_buffer_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_index_buffer_data), quad_index_buffer_data, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//creates the debug grid for our scene
void DebugSystem::createGrid_() {
    
    std::vector<float> grid_vertices;
    const float size = 100.0f; //outer width and height
    const int div = 100; // how many divisions
    const int halfdiv = div / 2;
    const float step = size / div; // gap between divisions
    const float half = size / 2; // middle of grid
    
    float p; //temporary variable to store position
    for (int i = 0; i <= div; i++) {
        
        //lines along z-axis, need to vary x position
        p = -half + (i*step);
        //one end of line
        grid_vertices.push_back(p);
        grid_vertices.push_back(0);
        grid_vertices.push_back(half);
        if (i == halfdiv) grid_vertices.push_back(1); //color
        else grid_vertices.push_back(0);
        
        //other end of line
        grid_vertices.push_back(p);
        grid_vertices.push_back(0);
        grid_vertices.push_back(-half);
        if (i == halfdiv) grid_vertices.push_back(1); //color
        else grid_vertices.push_back(0);
        
        //lines along x-axis, need to vary z positions
        p = half - (i * step);
        //one end of line
        grid_vertices.push_back(-half);
        grid_vertices.push_back(0);
        grid_vertices.push_back(p);
        if (i == halfdiv) grid_vertices.push_back(3); //color
        else grid_vertices.push_back(0);
        
        //other end of line
        grid_vertices.push_back(half);
        grid_vertices.push_back(0);
        grid_vertices.push_back(p);
        if (i == halfdiv) grid_vertices.push_back(3); //color
        else grid_vertices.push_back(0);
    }
    
    //indices
    const int num_indices = (div + 1) * 4;
    GLuint grid_line_indices[num_indices];
    for (int i = 0; i < num_indices; i++)
        grid_line_indices[i] = i;
    
    grid_num_indices = num_indices;
    
    //gl buffers
    glGenVertexArrays(1, &grid_vao_);
    glBindVertexArray(grid_vao_);
    GLuint vbo;
    //positions
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, grid_vertices.size() * sizeof(float), &(grid_vertices[0]), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    
    //indices
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grid_line_indices), grid_line_indices, GL_STATIC_DRAW);
    
    //unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

