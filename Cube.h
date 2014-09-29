#ifndef BLOCKDATA_H_GUARD
#define BLOCKDATA_H_GUARD
#include <stdint.h>

class Cube
{
	public:
		const static int32_t NUM_VERTS = 36;
		
		static Cube *Create();
		uint32_t toVerticies(float *buff, double xoff, double zoff, double yoff, double tx_fact, double tx_x, double tx_y, double tx_page);
		
		uint32_t size();
		
		Cube();
		virtual ~Cube();
		
};

#endif /* BLOCKDATA_H_GUARD */
