#include "Cube.h"
#include "CustomVertex.h"
#include "Debug.h"

Cube::Cube()
{
	
}

Cube::~Cube()
{
	
}

uint32_t Cube::size()
{
	return sizeof(CUSTOM_VERTEX) * NUM_VERTS;
}

Cube *Cube::Create()
{
	return new Cube();
}

uint32_t Cube::toVerticies(float *buff, double xoff, double zoff, double yoff, double tx_fact, double tx_x, double tx_y, double tx_page)
{
	static_assert(sizeof(CUSTOM_VERTEX) == 6*sizeof(float), "sizeof CUSTOM_VERTEX should be 24");
	
	float cube1[NUM_VERTS * 3] = { 
		1, 1, 1,  -1, 1, 1,  -1,-1, 1,      // v0-v1-v2 (front)
		-1,-1, 1,   1,-1, 1,   1, 1, 1,      // v2-v3-v0

		1, 1, 1,   1,-1, 1,   1,-1,-1,      // v0-v3-v4 (right)
		1,-1,-1,   1, 1,-1,   1, 1, 1,      // v4-v5-v0

		1, 1, 1,   1, 1,-1,  -1, 1,-1,      // v0-v5-v6 (top)
		-1, 1,-1,  -1, 1, 1,   1, 1, 1,      // v6-v1-v0

		-1, 1, 1,  -1, 1,-1,  -1,-1,-1,      // v1-v6-v7 (left)
		-1,-1,-1,  -1,-1, 1,  -1, 1, 1,      // v7-v2-v1

		-1,-1,-1,   1,-1,-1,   1,-1, 1,      // v7-v4-v3 (bottom)
		1,-1, 1,  -1,-1, 1,  -1,-1,-1,      // v3-v2-v7

		1,-1,-1,  -1,-1,-1,  -1, 1,-1,      // v4-v7-v6 (back)
		-1, 1,-1,   1, 1,-1,   1,-1,-1 
		};    // v6-v5-v4
                       
	float cube[NUM_VERTS * 3] = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f,-1.0f, // triangle 2 : begin
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triangle 2 : end
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};
	
	// range = (lowest - highest) * -1 == 1
	// offset = 0 - lowest == .5
	
	float *ptr = buff;
	for(int i = 0; i < NUM_VERTS; i++)
	{
		int idx = i*3;
		*ptr = cube1[idx] + xoff; ptr++;
		*ptr = cube1[idx+1] + yoff; ptr++;
		*ptr = cube1[idx+2] + zoff; ptr++;
		*ptr = (cube1[idx] + 0.5) * tx_fact + tx_x; ptr++;
		*ptr = (cube1[idx+1] + 0.5) * tx_fact + tx_y; ptr++;
		*ptr = tx_page;
		
		//Debug("v[%i]: %f,%f,%f,%f,%f,%f", *(ptr-5), *(ptr-4), *(ptr-3), *(ptr-2), *(ptr-1), *ptr);
	}
	
	return NUM_VERTS;
}

