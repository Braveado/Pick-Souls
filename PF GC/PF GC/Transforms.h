#ifndef _transf
#define _transf

#include<math.h>

enum Eje{X, Y, Z};

class Transforms
{
public:
	float Matriz[4][4];
	GLfloat MGL[16];

	Transforms()
	{
		for (int i = 0; i<4; i++)
		{
			for (int j = 0; j<4; j++)
			{
				if (i == j)
					Matriz[i][j] = 1;
				else
				Matriz[i][j] = 0;
			}
		}
	}

	void Identidad()
	{
		for (int i = 0; i<4; i++)
		{
			for (int j = 0; j<4; j++)
			{
				if (i == j)
					Matriz[i][j] = 1;
				else
					Matriz[i][j] = 0;
			}
		}
	}

	void MatrixGL()
	{
		MGL[0] = Matriz[0][0];
		MGL[1] = Matriz[0][1];
		MGL[2] = Matriz[0][2];
		MGL[3] = Matriz[0][3];
		MGL[4] = Matriz[1][0];
		MGL[5] = Matriz[1][1];
		MGL[6] = Matriz[1][2];
		MGL[7] = Matriz[1][3];
		MGL[8] = Matriz[2][0];
		MGL[9] = Matriz[2][1];
		MGL[10] = Matriz[2][2];
		MGL[11] = Matriz[2][3];
		MGL[12] = Matriz[3][0];
		MGL[13] = Matriz[3][1];
		MGL[14] = Matriz[3][2];
		MGL[15] = Matriz[3][3];
	}

	void matrixmult(float mat[4][4], float transf[4][4])
	{
		float aux[4][4];

		aux[0][0] = ((mat[0][0] * transf[0][0]) + (mat[0][1] * transf[1][0]) + (mat[0][2] * transf[2][0]) + (mat[0][3] * transf[3][0]));
		aux[0][1] = ((mat[0][0] * transf[0][1]) + (mat[0][1] * transf[1][1]) + (mat[0][2] * transf[2][1]) + (mat[0][3] * transf[3][1]));
		aux[0][2] = ((mat[0][0] * transf[0][2]) + (mat[0][1] * transf[1][2]) + (mat[0][2] * transf[2][2]) + (mat[0][3] * transf[3][2]));
		aux[0][3] = ((mat[0][0] * transf[0][3]) + (mat[0][1] * transf[1][3]) + (mat[0][2] * transf[2][3]) + (mat[0][3] * transf[3][3]));

		aux[1][0] = ((mat[1][0] * transf[0][0]) + (mat[1][1] * transf[1][0]) + (mat[1][2] * transf[2][0]) + (mat[1][3] * transf[3][0]));
		aux[1][1] = ((mat[1][0] * transf[0][1]) + (mat[1][1] * transf[1][1]) + (mat[1][2] * transf[2][1]) + (mat[1][3] * transf[3][1]));
		aux[1][2] = ((mat[1][0] * transf[0][2]) + (mat[1][1] * transf[1][2]) + (mat[1][2] * transf[2][2]) + (mat[1][3] * transf[3][2]));
		aux[1][3] = ((mat[1][0] * transf[0][3]) + (mat[1][1] * transf[1][3]) + (mat[1][2] * transf[2][3]) + (mat[1][3] * transf[3][3]));

		aux[2][0] = ((mat[2][0] * transf[0][0]) + (mat[2][1] * transf[1][0]) + (mat[2][2] * transf[2][0]) + (mat[2][3] * transf[3][0]));
		aux[2][1] = ((mat[2][0] * transf[0][1]) + (mat[2][1] * transf[1][1]) + (mat[2][2] * transf[2][1]) + (mat[2][3] * transf[3][1]));
		aux[2][2] = ((mat[2][0] * transf[0][2]) + (mat[2][1] * transf[1][2]) + (mat[2][2] * transf[2][2]) + (mat[2][3] * transf[3][2]));
		aux[2][3] = ((mat[2][0] * transf[0][3]) + (mat[2][1] * transf[1][3]) + (mat[2][2] * transf[2][3]) + (mat[2][3] * transf[3][3]));

		aux[3][0] = ((mat[3][0] * transf[0][0]) + (mat[3][1] * transf[1][0]) + (mat[3][2] * transf[2][0]) + (mat[3][3] * transf[3][0]));
		aux[3][1] = ((mat[3][0] * transf[0][1]) + (mat[3][1] * transf[1][1]) + (mat[3][2] * transf[2][1]) + (mat[3][3] * transf[3][1]));
		aux[3][2] = ((mat[3][0] * transf[0][2]) + (mat[3][1] * transf[1][2]) + (mat[3][2] * transf[2][2]) + (mat[3][3] * transf[3][2]));
		aux[3][3] = ((mat[3][0] * transf[0][3]) + (mat[3][1] * transf[1][3]) + (mat[3][2] * transf[2][3]) + (mat[3][3] * transf[3][3]));

		for (int i = 0; i<4; i++)
		{
			for (int j = 0; j<4; j++)
			{
				mat[i][j] = aux[i][j];
			}
		}
	}

	void Traslada(float x, float y, float z)
	{
		float tras[4][4];
		//transpuesta porque gl se pone nena
		for (int i = 0; i<4; i++)
		{
			for (int j = 0; j<4; j++)
			{
				if (i == j)
					tras[i][j] = 1;
				else if (i == 3 && j == 0)
					tras[i][j] = x;
				else if (i == 3 && j == 1)
					tras[i][j] = y;
				else if (i == 3 && j == 2)
					tras[i][j] = z;
				else
					tras[i][j] = 0;
			}
		}
		matrixmult(Matriz, tras);
	}

	void Escala(float x, float y, float z)
	{
		float esc[4][4];
		for (int i = 0; i<4; i++)
		{
			for (int j = 0; j<4; j++)
			{
				if (i == 0 && j == 0)
					esc[i][j] = x;
				else if (i == 1 && j == 1)
					esc[i][j] = y;
				else if (i == 2 && j == 2)
					esc[i][j] = z;
				else if (i == 3 && j == 3)
					esc[i][j] = 1;
				else
					esc[i][j] = 0;
			}
		}
		matrixmult(Matriz, esc);
	}

	void Rota(float ang, int eje)
	{
		float angulo = ang * 3.1416 / 180;
		float rot[4][4];
		//transpuestas porque gl se pone nena
		if (eje == Eje::X)
		{

			for (int i = 0; i<4; i++)
			{
				for (int j = 0; j<4; j++)
				{
					if (i == 0 && j == 0)
						rot[i][j] = 1;
					else if (i == 1 && j == 1)
						rot[i][j] = cos(angulo);
					else if (i == 1 && j == 2)
						rot[i][j] = sin(angulo);
					else if (i == 2 && j == 1)
						rot[i][j] = -sin(angulo);
					else if (i == 2 && j == 2)
						rot[i][j] = cos(angulo);
					else if (i == 3 && j == 3)
						rot[i][j] = 1;
					else
						rot[i][j] = 0;
				}
			}
		}
		else if (eje == Eje::Y)
		{
			for (int i = 0; i<4; i++)
			{
				for (int j = 0; j<4; j++)
				{
					if (i == 0 && j == 0)
						rot[i][j] = cos(angulo);
					else if (i == 0 && j == 2)
						rot[i][j] = -sin(angulo);
					else if (i == 1 && j == 1)
						rot[i][j] = 1;
					else if (i == 2 && j == 0)
						rot[i][j] = sin(angulo);
					else if (i == 2 && j == 2)
						rot[i][j] = cos(angulo);
					else if (i == 3 && j == 3)
						rot[i][j] = 1;
					else
						rot[i][j] = 0;
				}
			}
		}
		else if (eje == Eje::Z)
		{
			for (int i = 0; i<4; i++)
			{
				for (int j = 0; j<4; j++)
				{
					if (i == 0 && j == 0)
						rot[i][j] = cos(angulo);
					else if (i == 0 && j == 1)
						rot[i][j] = sin(angulo);
					else if (i == 1 && j == 0)
						rot[i][j] = -sin(angulo);
					else if (i == 1 && j == 1)
						rot[i][j] = cos(angulo);
					else if (i == 2 && j == 2)
						rot[i][j] = 1;
					else if (i == 3 && j == 3)
						rot[i][j] = 1;
					else
						rot[i][j] = 0;
				}
			}
		}
		matrixmult(Matriz, rot);
	}

};

#endif