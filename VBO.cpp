#include "VBO.h"
#include "Cube.h"
#include "Debug.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <string>

#include "CustomVertex.h"

VBO::VBO(float *data, uint32_t size) : vbo_(0), size_(size)
{
	static_assert(sizeof(CUSTOM_VERTEX) == 6*sizeof(float), "sizeof CUSTOM_VERTEX should be 24");
	
	ALLEGRO_VERTEX_ELEMENT elements[] = {
		{ ALLEGRO_PRIM_POSITION, ALLEGRO_PRIM_FLOAT_3, offsetof(CUSTOM_VERTEX, x) },
		{ ALLEGRO_PRIM_TEX_COORD, ALLEGRO_PRIM_FLOAT_2, offsetof(CUSTOM_VERTEX, u) },
		{ ALLEGRO_PRIM_USER_ATTR, ALLEGRO_PRIM_FLOAT_1, offsetof(CUSTOM_VERTEX, tx_page) },
		{ 0, 0, 0 }
	};
	
	vtxdecl_ = al_create_vertex_decl(elements, sizeof(CUSTOM_VERTEX));
	if(!vtxdecl_)
		Debug("failed to create vertex decl");
	
	vbo_ = al_create_vertex_buffer(vtxdecl_, data, size_, 0);
	if(!vbo_)
		Debug("failed to create vertex buffer :(");
	
	Debug("new chunk: size:%.02fMB", ((double)size_*sizeof(CUSTOM_VERTEX))/1024.0/1024.0);
}

VBO::~VBO()
{
	al_destroy_vertex_buffer(vbo_);
	al_destroy_vertex_decl(vtxdecl_);
}

void VBO::draw()
{
	al_draw_vertex_buffer(vbo_, NULL, 0, size_, ALLEGRO_PRIM_TRIANGLE_LIST);
}

VBO *VBO::Create(ALLEGRO_BITMAP *tex)
{
	uint32_t DATA_SIZE = 16 * 16 * 16 * sizeof(CUSTOM_VERTEX);
	
	float *data = new float[DATA_SIZE];
	if(!data)
		return nullptr;
	
	float *dptr = data;
	
	int32_t xPos = 0;
	int32_t zPos = 0;
	int32_t y = 0;
	
	uint32_t total_size = 0;

	for(int dy = 0; dy < 16; dy++)
	{
		for(int dz = 0; dz < 16; dz++)
		{
			for(int dx = 0; dx < 16; dx++)
			{
				float tx_fact = 0.0, tx_page = 0.0, tx_x = 0.0, tx_y = 0.0;
				
				Cube *cube = Cube::Create();
				if(!cube)
					continue;
				
				std::string texName = "texture.png";
		
				tx_page = 1.0;
				//Debug("tx_page: %f", tx_page);
				tx_fact = 1.0;
				tx_x = 0.0 * tx_fact;
				tx_y = 0.0 * tx_fact;
				
				uint32_t num_idx = cube->toVerticies(dptr, xPos + dx, zPos + dz, y + dy, tx_fact, tx_x, tx_y, tx_page);
				//Debug("%s nidx: %i", BlockName(block_data[idx], 0), num_idx);
				dptr += num_idx*sizeof(CUSTOM_VERTEX);
				total_size += num_idx;
				
				delete cube;
			}
		}
	}
	
	VBO *cdata = new VBO(data, total_size);
	delete[] data;
	
	return cdata;
}
