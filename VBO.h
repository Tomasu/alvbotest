#ifndef CUBE_H_GUARD
#define CUBE_H_GUARD


#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_primitives.h>
#include <vector>

class Cube;

class VBO
{
	public:
		static VBO *Create();
		
		void draw(ALLEGRO_BITMAP *);
		
	protected:
		VBO(float *data, uint32_t size);
		~VBO();
		
	private:
		ALLEGRO_VERTEX_BUFFER *vbo_;
		ALLEGRO_VERTEX_DECL *vtxdecl_;
		uint32_t size_;
};

#endif /* CUBE_H_GUARD */
