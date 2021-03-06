#include "NGLDraw.h"
#include <ngl/NGLInit.h>
#include <ngl/Material.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>

#include "Input.h"

const static float INCREMENT=0.01;
const static float ZOOM=0.05;
NGLDraw::NGLDraw()
{
    deltaTimeMiliseconds = 0;
    // mouse rotation values set to 0
    m_spinXFace=4000;
    m_spinYFace=0;

    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);
    // now to load the shader and set the values
    // grab an instance of shader manager
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    // we are creating a shader called Phong
    shader->createShaderProgram("Phong");
    // now we are going to create empty shaders for Frag and Vert
    shader->attachShader("PhongVertex",ngl::ShaderType::VERTEX);
    shader->attachShader("PhongFragment",ngl::ShaderType::FRAGMENT);
    // attach the source
    shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
    shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");
    // compile the shaders
    shader->compileShader("PhongVertex");
    shader->compileShader("PhongFragment");
    // add them to the program
    shader->attachShaderToProgram("Phong","PhongVertex");
    shader->attachShaderToProgram("Phong","PhongFragment");
    // now bind the shader attributes for most NGL primitives we use the following
    // layout attribute 0 is the vertex data (x,y,z)
    shader->bindAttribute("Phong",0,"inVert");
    // attribute 1 is the UV data u,v (if present)
    shader->bindAttribute("Phong",1,"inUV");
    // attribute 2 are the normals x,y,z
    shader->bindAttribute("Phong",2,"inNormal");

    // now we have associated this data we can link the shader
    shader->linkProgramObject("Phong");
    // and make it active ready to load values
    (*shader)["Phong"]->use();
    // the shader will use the currently active material and light0 so set them
    ngl::Material m(ngl::STDMAT::SILVER);
    // load our material values to the shader into the structure material (see Vertex shader)

    m.loadToShader("material");
    // Now we will create a basic Camera from the graphics library
    // This is a static camera so it only needs to be set once
    // First create Values for the camera position
    ngl::Vec3 from(0,1,5);
    ngl::Vec3 to(0,0,0);
    ngl::Vec3 up(0,1,0);

    // now load to our new camera
    m_cam= new ngl::Camera(from,to,up);
    // set the shape using FOV 45 Aspect Ratio based on Width and Height
    // The final two are near and far clipping planes of 0.5 and 10
    m_cam->setShape(45,(float)720.0/576.0,0.05,350);
    shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);

    // now create our light this is done after the camera so we can pass the
    // transpose of the projection matrix to the light to do correct eye space
    // transformations
    ngl::Mat4 iv=m_cam->getViewMatrix();
    iv.transpose();

    m_light = new ngl::Light(ngl::Vec3(0,5,1.5),ngl::Colour(.5,.45,.3,1),ngl::Colour(2,1.8,1.4,1),ngl::LightModes::POINTLIGHT );
    m_light->setTransform(iv);
    // load these values to the shader as well
    m_light->loadToShader("light");

    //Create a new map and initialize it (init will eventually be transferred to main menu call)
    m_map = new MapHandler("World1",3,8);
    m_map->InitMap(true,this);

    //Create a new player and initialize it (init will eventually be transferred to main menu call)
    m_player = new Player();
    m_player->initPlayer();

    m_player->SetMainMap(m_map);

    OnUpPress = false;
    OnDownPress = false;
    OnRightPress = false;
    OnLeftPress = false;

    Input *input = new Input();
}

NGLDraw::~NGLDraw()
{
    std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
    delete m_light;
    delete m_cam;
}

void NGLDraw::resize(int _w, int _h)
{
    glViewport(0,0,_w,_h);
    // now set the camera size values as the screen size has changed
    m_cam->setShape(45,(float)_w/_h,0.05,350);
}

void NGLDraw::draw()
{
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotation based on the mouse position for our global transform
    ngl::Mat4 rotX;
    ngl::Mat4 rotY;
    // create the rotation matrices
    rotX.rotateX( m_spinXFace );
    rotY.rotateY( m_spinYFace );
    // multiply the rotations
    m_mouseGlobalTX = rotY * rotX;
    // add the translations
    m_mouseGlobalTX.m_m[ 3 ][ 0 ] = m_modelPos.m_x;
    m_mouseGlobalTX.m_m[ 3 ][ 1 ] = m_modelPos.m_y;
    m_mouseGlobalTX.m_m[ 3 ][ 2 ] = m_modelPos.m_z;

    // grab an instance of the shader manager
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Phong"]->use();

    m_player->ManagePlayerInput();

    m_player->DrawPlayer();
    m_map->DrawMap();

    //temporary - also temp is the .49 (bcos round sometimes pushed  at 7.5)
    m_map->GetChunk((m_map->GetChunkVectorDimension()-1)/2,(m_map->GetChunkVectorDimension()-1)/2)->SetBlock(-int(m_map->GetTransform()->getPosition().m_x - 0.49),-int(m_map->GetTransform()->getPosition().m_z - 0.49),0);

}


void NGLDraw::loadMatricesToShader(ngl::Mat4 _trans)
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
    ngl::Mat4 M;
    M=_trans * m_mouseGlobalTX;
    MV=  M*m_cam->getViewMatrix();
    MVP= MV*m_cam->getProjectionMatrix();
    normalMatrix=MV;
    normalMatrix.inverse();
    shader->setShaderParamFromMat4("MV",MV);
    shader->setShaderParamFromMat4("MVP",MVP);
    shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
    shader->setShaderParamFromMat4("M",M);
}

//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::mouseMoveEvent (const SDL_MouseMotionEvent &_event)
{

}

//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::mousePressEvent (const SDL_MouseButtonEvent &_event)
{

}

//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::mouseReleaseEvent (const SDL_MouseButtonEvent &_event)
{

}

//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::wheelEvent(const SDL_MouseWheelEvent &_event)
{

    // check the diff of the wheel position (0 means no change)
    if(_event.y > 0)
    {
        m_modelPos.m_z+=ZOOM;
        this->draw();
    }
    else if(_event.y <0 )
    {
        m_modelPos.m_z-=ZOOM;
        this->draw();
    }

    // check the diff of the wheel position (0 means no change)
    if(_event.x > 0)
    {
        m_modelPos.m_x-=ZOOM;
        this->draw();
    }
    else if(_event.x <0 )
    {
        m_modelPos.m_x+=ZOOM;
    }

}
//----------------------------------------------------------------------------------------------------------------------

//  this needs work
void NGLDraw::getKey(){

    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if(keystate[SDL_SCANCODE_LEFT]){
        if(OnLeftPress == false)
            m_player->ModInputVector(ngl::Vec2(1,0));
        OnLeftPress = true;
    }else{
        if(OnLeftPress == true)
            m_player->ModInputVector(ngl::Vec2(-1,0));
        OnLeftPress = false;
    }

    if(keystate[SDL_SCANCODE_RIGHT]){
        if(OnRightPress == false)
            m_player->ModInputVector(ngl::Vec2(-1,0));
        OnRightPress = true;
    }else{
        if(OnRightPress == true)
            m_player->ModInputVector(ngl::Vec2(1,0));
        OnRightPress = false;
    }

    if(keystate[SDL_SCANCODE_UP]){
        if(OnUpPress == false)
            m_player->ModInputVector(ngl::Vec2(0,1));
        OnUpPress = true;
    }else{
        if(OnUpPress == true)
            m_player->ModInputVector(ngl::Vec2(0,-1));
        OnUpPress = false;
    }

    if(keystate[SDL_SCANCODE_DOWN]){
        if(OnDownPress == false)
            m_player->ModInputVector(ngl::Vec2(0,-1));
        OnDownPress = true;
    }else{
        if(OnDownPress == true)
            m_player->ModInputVector(ngl::Vec2(0,1));
        OnDownPress = false;
    }
}
