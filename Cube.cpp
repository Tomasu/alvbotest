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

uint32_t Cube::toVerticies(CUSTOM_VERTEX* buff, double xoff, double zoff, double yoff, double tx_fact, double tx_x, double tx_y, double tx_page)
{
	static_assert(sizeof(CUSTOM_VERTEX) == 6*sizeof(float), "sizeof CUSTOM_VERTEX should be 24");
	
	/*
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
                       
	float cube[NUM_VERTS * 5] = {
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
	
	float txCoords[NUM_VERTS * 2] =
	{
		0.0, 1.0,	//upper left
		0.0, 0.0,	//lower left
		1.0, 1.0,   //upper right

		//lower right triangle (viewed face on)
		1.0, 1.0,	//upper right again
		0.0, 0.0,	//lower left
		1.0, 0.0,	//lower right

		//left face (viewers left when looking at front face

		//upper left triangle (viewed face on)
		0.0, 1.0,	//upper left
		0.0, 0.0,	//lower left
		1.0, 1.0,	//upper right

		//lower right triangle (viewed face on)
		1.0, 1.0,	//upper right again
		0.0, 0.0,	//lower left
		1.0, 0.0,	//lower right

		//right face (viewers right when looking at front face

		//upper left triangle (viewed face on)
		0.0, 1.0,	//upper left
		0.0, 0.0,	//lower left
		1.0, 1.0,	//upper right

		//lower right triangle (viewed face on)
		1.0, 1.0,	//upper right again
		0.0, 0.0,	//lower left
		1.0, 0.0,	//lower right

		//back face

		//upper left triangle (viewed face on)
		0.0, 1.0,	//upper left
		0.0, 0.0,	//lower left
		1.0, 1.0,	//upper right

		//lower right triangle (viewed face on)
		1.0, 1.0,	//upper right again
		0.0, 0.0,	//lower left
		1.0, 0.0,	//lower right

		//top face
			
		//upper left triangle (viewed face on)
		0.0, 1.0,	//upper left
		0.0, 0.0,	//lower left
		1.0, 1.0,	//upper right

		//lower right triangle (viewed face on)
		1.0, 1.0,	//upper right again
		0.0, 0.0,	//lower left
		1.0, 0.0,	//lower right

		//bottom face

		//upper left triangle (viewed face on)
		0.0, 1.0,	//upper left
		0.0, 0.0,	//lower left
		1.0, 1.0,	//upper right

		//lower right triangle (viewed face on)
		1.0, 1.0,	//upper right again
		0.0, 0.0,	//lower left
		1.0, 0.0		//lower right
	};*/
	
	const int NUM_VERTS_SHARED = 8;
	VF3 verts[NUM_VERTS_SHARED] = {
		{ 1.000000, -1.000000, -1.000000 },
		{ 1.000000, -1.000000, 1.000000 },
		{ -1.000000, -1.000000, 1.000000 },
		{ -1.000000, -1.000000, -1.000000 },
		{ 1.000000, 1.000000, -1.000000 },
		{ 1.000000, 1.000000, 1.000000 },
		{ -1.000000, 1.000000, 1.000000 },
		{ -1.000000, 1.000000, -1.000000 }
	};
	
	VF2 txcs[NUM_VERTS_SHARED] = {
		{ 0.000000, -1.000000 },
		{ -1.000000, -1.000000 },
		{ -1.000000, 0.000000 },
		{ -1.000000, 1.000000 },
		{ -0.000000, -0.000000 },
		{ -0.000000, 1.000000 },
		{ 1.000000, 1.000000 },
		{ 1.000000, 0.000000 }
	};
	
	const int NUM_FACES = 12;
	VI3 vtxFaces[NUM_FACES] = {
		{ 2, 3, 4 },
		{ 8, 7, 6 },
		{ 1, 5, 6 },
		{ 2, 6, 7 },
		{ 7, 8, 4 },
		{ 1, 4, 8 },
		{ 1, 2, 4 },
		{ 5, 8, 6 },
		{ 2, 1, 6 },
		{ 3, 2, 7 },
		{ 3, 7, 4 },
		{ 5, 1, 8 }
	};
	
	
	VI3 txFaces[NUM_FACES] = {
		{ 1, 2, 3 },
		{ 4, 3, 5 },
		{ 5, 6, 4 },
		{ 5, 6, 4 },
		{ 6, 7, 8 },
		{ 1, 2, 3 },
		{ 5, 1, 3 },
		{ 6, 4, 5 },
		{ 3, 5, 4 },
		{ 3, 5, 4 },
		{ 5, 6, 8 },
		{ 5, 1, 3 }
	};
	
	// range = (lowest - highest) * -1
	// offset = 0 - lowest

	//float lowest = -1;
	//float highest = 1;
	//float range = (lowest - highest) * -1.0;
	//float offset = 0.0 - lowest;
	
	// for 1/-1
	// range = (-1 - 1) * -1 == 2
	// offset = 0 - -2 == 2
	
	CUSTOM_VERTEX *ptr = buff;
	
	for(int i = 0; i < NUM_FACES; i++)
	{
		VI3 &vtxface = vtxFaces[i];
		VI3 &txface = txFaces[i];
		
		for(int j = 0; j < 3; j++)
		{
			VF3 &vert = verts[vtxface.i[j]-1];
			VF2 &txc = txcs[txface.i[j]-1];
			
			CUSTOM_VERTEX &cv = *ptr++;
			
			cv.pos = { vert.f1 + xoff, vert.f2 + yoff, vert.f3 + zoff };
			cv.txcoord = txc;
			cv.tx_page = 1.0;
		}
	}
	
	/*for(int i = 0; i < NUM_VERTS; i++)
	{
		int idx = i*3;
		*ptr = cube[idx] + xoff; ptr++;
		*ptr = cube[idx+1] + yoff; ptr++;
		*ptr = cube[idx+2] + zoff; ptr++;
		
		float absx = (cube[idx] + offset);
		float absy = (cube[idx+1] + offset);
		
		int txidx = i*2;
		*ptr = txCoords[txidx]; ptr++; // (absx / range) * tx_fact + tx_x; ptr++;
		*ptr = txCoords[txidx+1]; ptr++; // (absy / range) * tx_fact + tx_y; ptr++;
		*ptr = tx_page; ptr++;
		
		Debug("v[%i]: %f,%f,%f,%f,%f,%f", i, *(ptr-6), *(ptr-5), *(ptr-4), *(ptr-3), *(ptr-2), *(ptr-1));
	}*/
	
	return NUM_VERTS;
}

