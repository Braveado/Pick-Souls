#ifndef __Niebla
#define __Niebla

#include <gl\GLU.h>
#include <gl\GL.h>

class Niebla
{
public:
	GLfloat Color[4];
	GLfloat densidad;
	//GLfloat source;

	Niebla()
	{
		Color[0] = 0.2;
		Color[1] = 0.2;
		Color[2] = 0.275;
		Color[3] = 1;
	}

	void enable()
	{
		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_FOG);
	}

	void draw() 
	{
		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_FOG); 
		glFogi(GL_FOG_MODE, GL_EXP2); 
		glFogfv(GL_FOG_COLOR, Color);
		//glFogf(GL_FOG_COORD_SRC, GL_FOG_COORD);
		glFogf(GL_FOG_DENSITY, densidad);
		glHint(GL_FOG_HINT, GL_NICEST);
		//glFogf(GL_FOG_START, 250);
		//glFogf(GL_FOG_END, -250);
	}

	void disable()
	{
		//glDisable(GL_DEPTH_TEST);
		glDisable(GL_FOG);
	}

};

#endif