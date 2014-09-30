#include <cmath>
#include <sstream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_image.h>
#include <allegro5/shader.h>
#include <allegro5/allegro_primitives.h>

#include "Renderer.h"
#include "Debug.h"
#include "VBO.h"

Renderer::Renderer() : queue_(nullptr), tmr_(nullptr), dpy_(nullptr)
{

}

Renderer::~Renderer()
{
	Debug("dtor");
	uninit();
}

void Renderer::uninit()
{
	if(queue_)
		al_destroy_event_queue(queue_);
		
	if(tmr_)
		al_destroy_timer(tmr_);
	
	if(prg_)
	{
		al_use_shader(nullptr);
		al_destroy_shader(prg_);
	}
	
	if(dpy_)
		al_destroy_display(dpy_);
	
	if(al_is_system_installed())
		al_uninstall_system();
	
	tmr_ = nullptr;
	dpy_ = nullptr;
	queue_ = nullptr;
	prg_ = nullptr;
}

bool Renderer::init()
{
	if(!al_init())
	{
		Debug("al_init failed???");
		return false;
	}
	
	ALLEGRO_TIMER *tmr = nullptr;
	ALLEGRO_EVENT_QUEUE *queue = nullptr;
	ALLEGRO_DISPLAY *dpy = nullptr;
	ALLEGRO_BITMAP *bmp = nullptr;
	
	if(!al_install_keyboard())
		goto init_failed;
	
   if(!al_install_mouse())
		goto init_failed;
	
	if(!al_init_primitives_addon())
		goto init_failed;
	
	if(!al_init_image_addon())
		goto init_failed;
	
	tmr = al_create_timer(1.0/60.0);
	if(!tmr)
		goto init_failed;
	
	queue = al_create_event_queue();
	if(!queue)
		goto init_failed;
	
	// do display creation last so a display isn't created and instantly destroyed if any of the
	// preceeding initializations fail.
	al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_PROGRAMMABLE_PIPELINE | ALLEGRO_OPENGL_3_0);
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_REQUIRE);
   al_set_new_display_option(ALLEGRO_SAMPLES, 4, ALLEGRO_REQUIRE);
	al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 24, ALLEGRO_REQUIRE);
	dpy = al_create_display(1024, 768);
	
	if(!dpy)
	{
		Debug("display creation failed");
		goto init_failed;
	}
	
	if(!al_get_opengl_extension_list()->ALLEGRO_GL_EXT_framebuffer_object)
	{
		Debug("FBO GL extension is missing. bail");
		goto init_failed;
	}
	
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	
	if(!loadShaders("default.vtx", "default.pxl"))
	{
		Debug("shader init failed");
		goto init_failed;
	}
	
	bmp = al_load_bitmap("texture.png");
	if(!bmp)
		goto init_failed;
	
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_display_event_source(dpy));
	al_register_event_source(queue, al_get_timer_event_source(tmr));
	
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	queue_ = queue;
	tmr_ = tmr;
	dpy_ = dpy;
	tex_ = bmp;
	
	glFrontFace(GL_CCW);
	
	vbo_ = VBO::Create();
	
	// initial clear display
	// make things look purdy
	al_clear_to_color(al_map_rgb(0,0,0)); 
   al_flip_display();
	
	return true;
	
init_failed:
	if(bmp)
		al_destroy_bitmap(bmp);
	
	if(dpy)
		al_destroy_display(dpy);
	
	if(queue)
		al_destroy_event_queue(queue);
	
	al_uninstall_system();
	return false;
}

void Renderer::run()
{
	cam_ = {0, 0, -100, 0, 0, 0, 0};
	memset(key_state_, 0, sizeof(key_state_) * sizeof(key_state_[0]));
	
	al_start_timer(tmr_);
	
	Debug("run!");
	
	bool redraw = false;
	while(1)
	{
		ALLEGRO_EVENT ev;
      al_wait_for_event(queue_, &ev);
 
      if(ev.type == ALLEGRO_EVENT_TIMER)
		{
         redraw = true;
			cam_.rx = 1.0;
			//cam_.ry = 1.0;
			
			if(key_state_[ALLEGRO_KEY_UP])
				cam_.z+=0.5;
			
			if(key_state_[ALLEGRO_KEY_DOWN])
				cam_.z-=0.5;
			
			if(key_state_[ALLEGRO_KEY_LEFT])
				cam_.ra += 0.01;
			
			if(key_state_[ALLEGRO_KEY_RIGHT])
				cam_.ra -= 0.01;
      }
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			Debug("display close");
         break;
      }
      else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			//Debug("key down");
			key_state_[ev.keyboard.keycode] = true;
			
			if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			key_state_[ev.keyboard.keycode] = false;
		}

 
      if(redraw && al_is_event_queue_empty(queue_)) {
         redraw = false;
			al_clear_to_color(al_map_rgb(0,0,0));
         draw();
         al_flip_display();
      }
	}
	
	Debug("stop timer");
	al_stop_timer(tmr_);
	
	Debug("done");
}

void Renderer::setupProjection(ALLEGRO_TRANSFORM *m)
{
   int dw = al_get_display_width(dpy_);
   int dh = al_get_display_height(dpy_);
	
	double zNear = 1.0, zFar = 1000.0, fov = 45.0, aspect = dw / dh;
	
	double left, right;
	double bottom, top;
	top = tan (fov*ALLEGRO_PI/360.0)*zNear;
	bottom = -top;
	left = aspect*bottom;
	right = aspect*top;
 
	//Debug("rect: %f %f %f %f", left, top, right, bottom);
	
	al_perspective_transform(m, left, top, zNear,
      right, bottom, zFar);
	
   al_set_projection_transform(dpy_, m);
}

void Renderer::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//ALLEGRO_STATE state;
	//al_store_state(&state, ALLEGRO_STATE_TRANSFORM);
	
	//al_use_shader(nullptr);
	
	int dw = al_get_display_width(dpy_);
   int dh = al_get_display_height(dpy_);
	
	ALLEGRO_TRANSFORM trans;
	/*al_identity_transform(&trans);
	al_orthographic_transform(&trans, 0, 0, -1, dw, dh, 1);
	al_set_projection_transform(dpy_, &trans);
	al_identity_transform(&trans);
	al_use_transform(&trans);
	
	al_draw_bitmap(resManager_->getAtlas()->getSheet(0)->alBitmap(), 0, 0, 0);
	*/
	
	glBindVertexArray(vao_);
	if(!al_use_shader(prg_))
	{
		Debug("failed to use shader");
	}
	
	al_identity_transform(&trans);
	setupProjection(&trans);
	
	al_identity_transform(&trans);
	al_rotate_transform_3d(&trans, cam_.rx, cam_.ry, cam_.rz, cam_.ra);
	al_translate_transform_3d(&trans, cam_.x, cam_.y, cam_.z);
	al_use_transform(&trans);
	
	setShaderSampler(tex_);
	vbo_->draw(tex_);
	
	//al_use_shader(nullptr);
	
	//al_restore_state(&state);
}

void Renderer::drawCube()
{

}

bool Renderer::loadShaders(const char *vertex_file_path, const char *fragment_file_path)
{
	ALLEGRO_SHADER *prg = al_create_shader(ALLEGRO_SHADER_GLSL);
	if(!prg)
		goto load_fail;
	
	if(!al_attach_shader_source_file(prg, ALLEGRO_VERTEX_SHADER, vertex_file_path))
	{
		Debug("failed to attach vertex shader:\n %s", al_get_shader_log(prg));
		goto load_fail;
	}
	
	if(!al_attach_shader_source_file(prg, ALLEGRO_PIXEL_SHADER, fragment_file_path))
	{
		Debug("failed to attach pixel shader:\n %s", al_get_shader_log(prg));
		goto load_fail;
	}
	
	if(!al_build_shader(prg))
	{
		Debug("failed to build shader:\n %s", al_get_shader_log(prg));
		goto load_fail;
	}
	
	if(!al_use_shader(prg))
	{
		Debug("failed to use shader");
		goto load_fail;
	}
	
	prg_ = prg;
	
	return true;
	
load_fail:
	if(prg)
		al_destroy_shader(prg);
	
	return false;
}

bool Renderer::setShaderSampler(ALLEGRO_BITMAP *sheet)
{
	std::stringstream sstr;
	sstr << "atlas_sheet_";
	sstr << 0;
	
	if(!al_set_shader_sampler(sstr.str().c_str(), sheet, 2))
	{
		Debug("failed to set sampler %s", sstr.str().c_str());
		return false;
	}

	return true;
}