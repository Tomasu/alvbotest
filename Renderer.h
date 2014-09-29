#ifndef RENDERER_H_GUARD
#define RENDERER_H_GUARD

#include <unordered_map>
#include <utility>
#include <allegro5/allegro.h>

class Renderer
{
	public:
		const static int MAX_VERTEX_BUFFERS = 16;
		
		Renderer();
		~Renderer();
		
		bool init();
		void uninit();
		void run();
		
		void draw();
		void drawCube();
		
		bool setShaderSampler(ALLEGRO_BITMAP *);
		
	private:
		uint32_t vao_;
		
		bool key_state_[ALLEGRO_KEY_MAX];
		
		ALLEGRO_EVENT_QUEUE *queue_;
		ALLEGRO_TIMER *tmr_;
		ALLEGRO_DISPLAY *dpy_;
		ALLEGRO_SHADER *prg_;
		ALLEGRO_BITMAP *tex_;
		
		struct { 
			double x, y, z;
			double rx, ry, rz, ra;
		} cam_;
		
		bool loadShaders(const char *vertex_file_path, const char *fragment_file_path);
		
		void setupProjection(ALLEGRO_TRANSFORM *m);
};

#endif /* RENDERER_H_GUARD */
