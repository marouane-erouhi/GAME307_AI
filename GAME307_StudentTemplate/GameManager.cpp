#include "GameManager.h"
#include "Scene1.h"
#include "Scene2.h"

GameManager::GameManager() {
	windowPtr = nullptr;
	timer = nullptr;
	isRunning = true;
	currentScene = nullptr;
    player = nullptr;
    changeSceneEventType = 0;
}

bool GameManager::OnCreate() {
    // My display is 1920 x 1080 but the following seems to work best to fill the screen.
    //const int SCREEN_WIDTH = 1540;
    //const int SCREEN_HEIGHT = 860;

    // Use 1000x600 for less than full screen
    const int SCREEN_WIDTH = 1000;
    const int SCREEN_HEIGHT = 600;

    windowPtr = new Window(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (windowPtr == nullptr) {
		OnDestroy();
		return false;
	}
	if (windowPtr->OnCreate() == false) {
		OnDestroy();
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		OnDestroy();
		return false;
	}

    // select scene for specific assignment
    // DEFAUALT SCENE
    currentScene = new Scene1(windowPtr->GetSDL_Window(), this);
    
    // create player
    float mass = 1.0f;
    float radius = 0.5f;
    float orientation = 0.0f;
    float rotation = 0.0f;
    float angular = 0.0f;
    // These are not very good defaults, but they will prevent compiler warnings.
    float maxSpeed = 5.0f;
    float maxAcceleration = 10.0f;
    float maxRotation = 1.0f;
    float maxAngular = 1.0f;
    Vec3 position(0.5f * currentScene->getxAxis(), 0.5f * currentScene->getyAxis(), 0.0f);
    Vec3 velocity(0.0f, 0.0f, 0.0f);
    Vec3 acceleration(0.0f, 0.0f, 0.0f);

    player = new PlayerBody
    (
        position,
        velocity,
        acceleration,
        mass,
        radius,
        orientation,
        rotation,
        angular,
        maxSpeed,
        maxAcceleration,
        maxRotation,
        maxAngular,
        this
    );
    if ( player->OnCreate() == false ) {
        OnDestroy();
        return false;
    }

    // need to create Player before validating scene
    if (!ValidateCurrentScene()) {
        OnDestroy();
        return false;
    }
    
    // create some user defined event
    changeSceneEventType = SDL_RegisterEvents(1);
    if ( changeSceneEventType == ((Uint32) - 1 ) )
    {
        OnDestroy();
        return false;
    }
       
	return true;
}


/// Here's the whole game loop
void GameManager::Run() {
    
	timer->Start();
    // control if current scene's update() is called each tick
    
	while (isRunning)
	{
            
		handleEvents();
        
		timer->UpdateFrameTicks();
		if ( launched )
		{
		    currentScene->Update(timer->GetDeltaTime());
		    // launched boolean just helps user stop and start action
		    // and useful for debugging and teaching
		}
		currentScene->Render();

		/// Keep the event loop running at a proper rate
		SDL_Delay(timer->GetSleepTime(60)); ///60 frames per sec
	}
}

void GameManager::handleEvents()
{
	SDL_Event event;

        // Let's add mouse movement and position
        // https://wiki.libsdl.org/SDL_GetMouseState

        SDL_PumpEvents();  // make sure we have the latest mouse state.

        //https://www.youtube.com/watch?v=SYrRMr4BaD4&list=PLM7LHX-clszBIGsrh7_3B2Pi74AhMpKhj&index=3
        while( SDL_PollEvent(&event) )
        {
            if( event.type == SDL_QUIT )
            {
                isRunning = false;
            }
            else if( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.scancode )
                {
                    case SDL_SCANCODE_ESCAPE:
                        isRunning = false;
                        break;
                    case SDL_SCANCODE_Q:
                        isRunning = false;
                        break;
                    case SDL_SCANCODE_DELETE:
                        isRunning = false;
                        break;
                    case SDL_SCANCODE_L:
                        //toggle launched with L key)
                        launched = !launched;
                        break;
                    case SDL_SCANCODE_1:
                        LoadScene( 1 );
                        break;
                    default:
                        break;
                }
            }
            currentScene->HandleEvents( event );
        }
}

GameManager::~GameManager() {}

void GameManager::OnDestroy(){
	if (windowPtr) delete windowPtr;
	if (timer) delete timer;
	if (currentScene) delete currentScene;
}

float GameManager::getSceneHeight() const
{ return currentScene->getyAxis(); }

float GameManager::getSceneWidth() const
{ return currentScene->getxAxis(); }

Matrix4 GameManager::getProjectionMatrix() const
{ return currentScene->getProjectionMatrix(); }

SDL_Renderer* GameManager::getRenderer()
{
    // [TODO] might be missing some SDL error checking
    SDL_Window* window = currentScene->getWindow();
    SDL_Renderer* renderer = SDL_GetRenderer(window);
    return renderer;
}

void GameManager::RenderPlayer(float scale)
{
    player->Render(scale);
}

void GameManager::LoadScene( int i )
{
    // cleanup of current scene before loading anothe one
    currentScene->OnDestroy();
    delete currentScene;
    switch ( i )
    {
        case 1:
            currentScene = new Scene1( windowPtr->GetSDL_Window(), this);
            break;
        case 2:
            currentScene = new Scene2(windowPtr->GetSDL_Window(), this);
            break;
        default:
            currentScene = new Scene2( windowPtr->GetSDL_Window(), this );
            break;
    }
    // using ValidateCurrentScene() to safely run OnCreate
    if (!ValidateCurrentScene())
    {
        isRunning = false;
    }
    launched = true;
}

bool GameManager::ValidateCurrentScene()
{
    if (currentScene == nullptr) {
        return false;
    }
    if (currentScene->OnCreate() == false) {
        return false;
    }
    return true;
}
