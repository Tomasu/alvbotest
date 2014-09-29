#include "Cube.h"
#include "CustomVertex.h"

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
	
	float cube[NUM_VERTS * 3] = {
		-0.5f,-0.5f,-0.5f, // triangle 1 : begin
		-0.5f,-0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f, // triangle 1 : end
		0.5f, 0.5f,-0.5f, // triangle 2 : begin
		-0.5f,-0.5f,-0.5f,
		-0.5f, 0.5f,-0.5f, // triangle 2 : end
		0.5f,-0.5f, 0.5f,
		-0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f, 0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f,-0.5f,
		0.5f,-0.5f, 0.5f,
		-0.5f,-0.5f, 0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f,-0.5f, 0.5f,
		0.5f,-0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f, 0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f,-0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f,-0.5f,
		-0.5f, 0.5f,-0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f,-0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f,-0.5f, 0.5f
	};
	
	// range = (lowest - highest) * -1 == 1
	// offset = 0 - lowest == .5
	
	float *ptr = buff;
	for(int i = 0; i < NUM_VERTS; i++)
	{
		int idx = i*3;
		*ptr = cube[idx] + xoff; ptr++;
		*ptr = cube[idx+1] + yoff; ptr++;
		*ptr = cube[idx+2] + zoff; ptr++;
		*ptr = (cube[idx] + 0.5) * tx_fact + tx_x; ptr++;
		*ptr = (cube[idx+1] + 0.5) * tx_fact + tx_y; ptr++;
		*ptr = tx_page; ptr++;
	}
	
	return NUM_VERTS;
}

