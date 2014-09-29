#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_image.h>
#include <allegro5/shader.h>
#include "Renderer.h"
#include "Debug.h"

int main(int, char **)
{
	Renderer *renderer = new Renderer();
	
	if(!renderer->init())
	{
		Debug("failed to init renderer");
		return -1;
	}
	
	renderer->run();
	
	delete renderer;
	
	return 0;
}
