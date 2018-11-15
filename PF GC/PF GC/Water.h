#ifndef _wata
#define _wata
#include "Geometrias.h"
#include "Imagenes.h"
//#include "ShaderDemo.h"

class Water : public Primitivos, public Imagenes
{

private:
	float anchof;
	float proff;
	float deltax, deltaz;
	//ShaderDemo *gpuDemo;

public:

	Maya wata;
	int verx, verz;
	//el nombre numerico de la textura en cuestion, por lo pronto una
	unsigned int planoTextura;

	Water(HWND hWnd, WCHAR alturas[], WCHAR textura[], float ancho, float prof)
	{
		anchof = ancho;
		proff = prof;
		//cargamos la textura de la figura
		Carga(alturas);
		//en caso del puntero de la imagen sea nulo se brica esta opcion
		wata = Plano(Ancho(), Alto(), ancho, prof, Dir_Imagen(), 30);
		deltax = anchof / Ancho();
		deltaz = proff / Alto();
		verx = Ancho();
		verz = Alto();
		//disponemos la textura del gdi.
		Descarga();

		Carga(textura);
		glGenTextures(1, &planoTextura);
		glBindTexture(GL_TEXTURE_2D, planoTextura);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, Ancho(), Alto(), GL_RGBA, GL_UNSIGNED_BYTE, Dir_Imagen());
		Descarga();

		//gpuDemo = new ShaderDemo("basic.vert", "basic.frag");
		//gpuDemo->ligador(gpuDemo->vertShader, gpuDemo->fragShader);
	}

	~Water()
	{
		//nos aseguramos de disponer de los recursos previamente reservados
		delete wata.maya;
		delete wata.indices;
		glDeleteTextures(1, &planoTextura);
	}

	void Draw()
	{

		static float MovS = 0;
		MovS += 1;
		float A = sin(MovS * 3.1415 / 180);

		static float water_factor_min = 20.0f;
		static float water_factor_max = 30.0f;
		static float water_size = 200;
		water_factor_min += .001f;
		water_factor_max += .001f;
		if (water_factor_min > 40)
		{
			water_factor_min = 20.0f;
			water_factor_max = 30.0f;
		}

		//glPushAttrib(GL_CURRENT_BIT | GL_TEXTURE_BIT);
		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		//glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_COLOR);


		glBegin(GL_QUADS);

		glTexCoord2f(water_factor_min, water_factor_min);
		glColor4f(0.2f, 0.2f, 0.2f, 0.2f);
		glVertex3f(-water_size, A, water_size);
		//
		glTexCoord2f(water_factor_max, water_factor_min);
		glColor4f(0.2f, 0.2f, 0.2f, 0.2f);
		glVertex3f(water_size, A, water_size);
		//
		glTexCoord2f(water_factor_max, water_factor_max);
		glColor4f(0.2f, 0.2f, 0.2f, 0.2f);
		glVertex3f(water_size, A, -water_size);
		//
		glTexCoord2f(water_factor_min, water_factor_max);
		glColor4f(0.2f, 0.2f, 0.2f, 0.2f);
		glVertex3f(-water_size, A, -water_size);


		//for (int k = 0; k < 30; k++)
		//{
		//
		//	for (int i = 0; i < 30; i++)
		//	{
		//		movs += 0.000001;
		//		float ang = sin(movs * 31415 / 180);
		//
		//		if (k == 0 && i == 0)
		//			glTexCoord2f(water_factor_min, water_factor_min);
		//		else if (k == 29 && i == 0)
		//			glTexCoord2f(water_factor_max, water_factor_min);
		//		else if (k == 29 && i == 29)
		//			glTexCoord2f(water_factor_max, water_factor_max);
		//		else if (k == 0 && i == 29)
		//			glTexCoord2f(water_factor_min, water_factor_max);
		//
		//		//glTexCoord2f(i * 3, k * 3);
		//		glColor4f(0.2f, 0.2f, 0.2f, 0.2f);
		//		glVertex3f(i * 3, ang, k * 3);
		//
		//		//glTexCoord2f(i * 3 + 3, k * 3);
		//		glColor4f(0.2f, 0.2f, 0.2f, 0.2f);
		//		glVertex3f(i * 3 + 3, ang, k * 3);
		//
		//		//glTexCoord2f(i * 3 + 3, k * 3 + 3);
		//		glColor4f(0.2f, 0.2f, 0.2f, 0.2f);
		//		glVertex3f(i * 3 + 3, ang, k * 3 + 3);
		//
		//		//glTexCoord2f(i * 3, k * 3 + 3);
		//		glColor4f(0.2f, 0.2f, 0.2f, 0.2f);
		//		glVertex3f(i * 3, ang, k * 3 + 3);
		//	}
		//}


		glEnd();


		glPopAttrib();
		//glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		//glDepthMask(GL_TRUE);
	}
};

#endif 