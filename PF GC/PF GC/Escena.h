#ifndef __Escena
#define __Escena

#include <gl\glew.h>
#include <gl\GLU.h>
#include <gl\GL.h>
#include <time.h>
#include "Transforms.h"
#include "SkyDome.h"
#include "Terreno.h"
#include "Niebla.h"
#include "Water.h"
#include "Billboard.h"
#include "Model.h"

// lo que falta

// multitextura
// arreglar el movimiento senoidal del agua

// multiples proyectiles
// que maicol tenga sonido y deje loot

class Camara
{
public:

	float TPCamPos[3], TPCamDir[3];
	float FPCamPos[3], FPCamDir[3];
	float PlayerPos[3];
	float VeRot, HoRot;
	enum Perspectiva { TP, FP };
	int cam = FP;
	int reves = 1;

	void updateFP()
	{
		FPCamPos[0] = TPCamPos[0];
		FPCamPos[1] = TPCamPos[1];
		FPCamPos[2] = TPCamPos[2];
		FPCamDir[0] = TPCamDir[0];
		FPCamDir[1] = TPCamDir[1];
		FPCamDir[2] = TPCamDir[2];
		
		FPCamPos[0] = FPCamPos[0] + FPCamDir[0] * 4.75;
		FPCamPos[2] = FPCamPos[2] + FPCamDir[2] * 4.75;

		FPCamPos[1] = FPCamPos[1] - 1.15;
	}

	void CamaraI(Perspectiva view)
	{
		cam = view;

		VeRot = 0;
		HoRot = 0;

		TPCamPos[0] = -184;
		TPCamPos[1] = 31;
		TPCamPos[2] = 221;
		TPCamDir[0] = 0;
		TPCamDir[1] = 0;
		TPCamDir[2] = -1;
		
		CamaraRotacion(Y, -52);

		reves = 1;

		updateFP();
		PlayerPosU();

	}

	void CamaraU(int cam)
	{
		if (cam == Perspectiva::TP)
		{
			gluLookAt(
				TPCamPos[0], TPCamPos[1], TPCamPos[2],
				TPCamPos[0] + TPCamDir[0], TPCamPos[1] + TPCamDir[1], TPCamPos[2] + TPCamDir[2],
				0, 1, 0);
		}
		else if (cam == Perspectiva::FP)
		{
			gluLookAt(
				FPCamPos[0], FPCamPos[1], FPCamPos[2],
				FPCamPos[0] + FPCamDir[0], FPCamPos[1] + FPCamDir[1], FPCamPos[2] + FPCamDir[2],
				0, reves, 0);
		}
		PlayerPosU();
	}

	enum Direccion{F, B, L, R, U, D};

	void CamaraTraslacion(Direccion dir, float vel)
	{
		if (dir == Direccion::F)
		{
			TPCamPos[0] = TPCamPos[0] + TPCamDir[0] * vel;
			TPCamPos[2] = TPCamPos[2] + TPCamDir[2] * vel;

		}
		else if (dir == Direccion::B)
		{
			TPCamPos[0] = TPCamPos[0] - TPCamDir[0] * vel;
			TPCamPos[2] = TPCamPos[2] - TPCamDir[2] * vel;
		}
		if (dir == Direccion::L)
		{
			CamaraRotacion(Y, 90);
			TPCamPos[0] = TPCamPos[0] + TPCamDir[0] * vel;
			TPCamPos[2] = TPCamPos[2] + TPCamDir[2] * vel;
			CamaraRotacion(Y, -90);
		}
		if (dir == Direccion::R)
		{
			CamaraRotacion(Y, -90);
			TPCamPos[0] = TPCamPos[0] + TPCamDir[0] * vel;
			TPCamPos[2] = TPCamPos[2] + TPCamDir[2] * vel;
			CamaraRotacion(Y, 90);
		}
		else if (dir == Direccion::U)
		{
			TPCamPos[1] = TPCamPos[1] + vel;
		}
		else if (dir == Direccion::D)
		{
			TPCamPos[1] = TPCamPos[1] - vel;
		}
		updateFP();
		PlayerPosU();
	}

	void CamaraRotacion(Eje eje, float ang)
	{
		float angulo = ang * 3.1415 / 180.0;
		float rot[4][4];
		Identidad(rot);

		if (eje == Eje::Y)
		{
			HoRot += ang;

			rot[0][0] = cos(angulo);
			rot[0][2] = sin(angulo);
			rot[2][0] = -sin(angulo);
			rot[2][2] = cos(angulo);	
		}
		else if (eje == Eje::X)
		{
			VeRot += ang;
			rot[1][1] = cos(angulo);
			rot[1][2] = -sin(angulo);
			rot[2][1] = sin(angulo);
			rot[2][2] = cos(angulo);

			float horang = HoRot * 3.1415 / 180.0;
			float hrot[4][4];
			Identidad(hrot);
			hrot[0][0] = cos(horang);
			hrot[0][2] = sin(horang);
			hrot[2][0] = -sin(horang);
			hrot[2][2] = cos(horang);

			float horangn = -HoRot * 3.1415 / 180.0;
			float hrotn[4][4];
			Identidad(hrotn);
			hrotn[0][0] = cos(horangn);
			hrotn[0][2] = sin(horangn);
			hrotn[2][0] = -sin(horangn);
			hrotn[2][2] = cos(horangn);

			matrixmult(hrot, rot);
			matrixmult(hrot, hrotn);

			for (int i = 0; i<4; i++)
			{
				for (int j = 0; j<4; j++)
				{
					rot[i][j] = hrot[i][j];
				}
			}
		}

		float aux[3];
		aux[0] = (TPCamDir[0] * rot[0][0]) + (TPCamDir[1] * rot[0][1]) + (TPCamDir[2] * rot[0][2]);
		aux[1] = (TPCamDir[0] * rot[1][0]) + (TPCamDir[1] * rot[1][1]) + (TPCamDir[2] * rot[1][2]);
		aux[2] = (TPCamDir[0] * rot[2][0]) + (TPCamDir[1] * rot[2][1]) + (TPCamDir[2] * rot[2][2]);

		TPCamDir[0] = aux[0];
		TPCamDir[1] = aux[1];
		TPCamDir[2] = aux[2];

		updateFP();
		PlayerPosU();
	}

	void PlayerPosU()
	{
		PlayerPos[1] = TPCamPos[1] - 3;
		PlayerPos[0] = TPCamPos[0] + TPCamDir[0] * 5;
		PlayerPos[2] = TPCamPos[2] + TPCamDir[2] * 5;
	}

	void Identidad(float matriz[4][4])
	{
		for (int i = 0; i<4; i++)
		{
			for (int j = 0; j<4; j++)
			{
				if (i == j)
					matriz[i][j] = 1;
				else
					matriz[i][j] = 0;
			}
		}
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

};

class Escena : public Camara
{
public:
	HWND Hwnd;
	Transforms trans;
	SkyDome *noche;
	Terreno *cuenca;
	Niebla niebla;
	Water *lago;
	Billboard *cesped[150], *cesped2[15], *souls[2], *bonfire[10], *projectile[6], *splash[10], *hud[12];
	
	int arboles[96][4], cadsel[3], bonefirei = 0, pickups = 0, projectilei = 0, endwait = 0, splashi = 0, hudi = 0, cursor = 0;
	float ndens = 0.0045, piedras[50][6], piedras2[5][6], cadpos[3][5], soulsesc = 0, soulspartic[15][5], bonfirepartic[3][7], posproj[5], oriproj[3], posprojdest[3], projpartic[5][5], projvel[2];
	bool soulsescbool = true, nieblabool = true, projbool = false, dead = false, win = false, lose = false, bonfire_sound = false, cuenca_sound = true, splashbool = false, fxaux = false, newarea = true, intro = false, menup = true, menusound = true, menus = false, exit = false, hudtext = true, pickuphud = false;
	
	EDXFramework::Model *DS_Bridge, *DS_Stair, *DS_Hydra[4], *DS_Greatsword, *DS_Bonfire, *DS_Hollow[4], *DS_Stone, *Tree[3], *DS_CL[3], *DS_CG[3], *Crow[3];
	int crowi = 0, hydrai = 0, hydrauxi = 0, cgi = 0, cg2i = 0, cg3i = 0, cli = 0, hollowi = 0;
	bool crowbool = true, hydrabool = true, cg1bool = true, cg1aux = true, cg2bool = true, cg2aux = true, cg3bool = true, clbool = false, claux = true, hollowbool = true;
	float crowposX = 0, crowposY = 0, crowposZ = 0, crowang = 0;
	float hydraposX = 75, hydraposY = 15, hydraposZ = -95, hydrang = -15;
	float cg1posX = 135, cg1posY = 0, cg1posZ = 175, cg1ang = 180;
	float cg2posX = -60, cg2posY = 0, cg2posZ = -60, cg2ang = 45;
	float cg3posX = 0, cg3posY = 0, cg3posZ = 0, cg3dis = 25, cg3ang = 0;
	float clposX = -115, clposY = 0, clposZ = -25, clang = 0;
	float hollowposX = -166, hollowposY = 0, hollowposZ = 213;

	Escena(HWND hWnd)
	{
		glEnable(GL_NORMALIZE);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);

		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, TRUE);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		GLfloat White[] = { 1, 1, 1, 1 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, White);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, White);
		glMaterialfv(GL_FRONT, GL_SPECULAR, White);
		glMaterialf(GL_FRONT, GL_SHININESS, 80.0);

		Hwnd = hWnd;
		noche = new SkyDome(hWnd, 32, 32, 500, L"Imagenes//World//Night_v2.jpg");
		cuenca = new Terreno(hWnd, L"Imagenes//World//Darkroot Basin Heightmap.jpg", L"Imagenes//World//floor.jpg", L"Imagenes//World//water.bmp", 512, 512);
		lago = new Water(hWnd, NULL, L"Imagenes//World//water.bmp", 300, 300);
		//arboles atras cuenca
		arboles_setup(-280, 280, 20, 80, -285, H, arboles, 0);
		//adelante cuenca
		arboles_setup(-280, 280, 20, 80, 285, H, arboles, 28);
		//derecha cuenca
		arboles_setup(-250, 250, 25, 80, -285, V, arboles, 56);
		//izquierda cuenca
		arboles_setup(-250, 250, 25, 80, 285, V, arboles, 76);
		//vegetacion
		cesped_setup(cesped, 150, suelo, hWnd);
		cesped_setup(cesped2, 15, puente, hWnd);
		//piedras
		piedras_setup(piedras, 50, suelo);
		piedras_setup(piedras2, 5, puente);
		//pickups
		cadaveres_setup(cadsel, cadpos);	
		souls_setup(souls, hWnd);
		//bonfire
		bonfire_setup(bonfire, hWnd);
		//projectile
		projectile_setup(projectile, hWnd);
		splash_setup(splash, hWnd);
		//hud
		hud_setup(hud, hWnd);

		DS_Bridge = new EDXFramework::Model("Modelos//DS_Bridge//DS_Bridge.obj", "Modelos//DS_Bridge//DS_Bridge.bmp", 0);
		DS_Stair = new EDXFramework::Model("Modelos//DS_Stair//DS_Stair.obj", "Modelos//DS_Stair//DS_Stair.bmp", 0);
		DS_Hydra[0] = new EDXFramework::Model("Modelos//DS_Hydra//DS_Hydra.obj", "Modelos//DS_Hydra//DS_Hydra.bmp", 1);
		DS_Hydra[1] = new EDXFramework::Model("Modelos//DS_Hydra//DS_Hydra_Flotando01.obj", "Modelos//DS_Hydra//DS_Hydra.bmp", 1);
		DS_Hydra[2] = new EDXFramework::Model("Modelos//DS_Hydra//DS_Hydra_Flotando02.obj", "Modelos//DS_Hydra//DS_Hydra.bmp", 1);
		DS_Hydra[3] = new EDXFramework::Model("Modelos//DS_Hydra//DS_Hydra_Disparando.obj", "Modelos//DS_Hydra//DS_Hydra.bmp", 1);
		DS_Greatsword = new EDXFramework::Model("Modelos//DS_Greatsword//DS_Greatsword.obj", "Modelos//DS_Greatsword//DS_Greatsword.bmp", 0);
		DS_Bonfire = new EDXFramework::Model("Modelos//DS_Bonfire//DS_Bonfire.obj", "Modelos//DS_Bonfire//DS_Bonfire.bmp", 1);
		DS_Hollow[0] = new EDXFramework::Model("Modelos//DS_Hollow//DS_Hollow.obj", "Modelos//DS_Hollow//DS_Hollow.bmp", 1);
		DS_Hollow[1] = new EDXFramework::Model("Modelos//DS_Hollow//DS_Hollow_Sentandose.obj", "Modelos//DS_Hollow//DS_Hollow.bmp", 1);
		DS_Hollow[2] = new EDXFramework::Model("Modelos//DS_Hollow//DS_Hollow_Sentado.obj", "Modelos//DS_Hollow//DS_Hollow.bmp", 1);
		DS_Hollow[3] = new EDXFramework::Model("Modelos//DS_Hollow//DS_Hollow_Cadaver01.obj", "Modelos//DS_Hollow//DS_Hollow.bmp", 1);
		DS_Stone = new EDXFramework::Model("Modelos//DS_Stone//DS_Stone.obj", "Modelos//DS_Stone//DS_Stone.bmp", 1);
		Tree[0] = new EDXFramework::Model("Modelos//Tree_Fat//Tree_Fat.obj", "Modelos//Tree_Fat//Tree_Fat.bmp", 1);
		Tree[1] = new EDXFramework::Model("Modelos//Tree_Tall//Tree_Tall.obj", "Modelos//Tree_Tall//Tree_Tall.bmp", 1);
		Tree[2] = new EDXFramework::Model("Modelos//Tree_Spiky//Tree_Spiky.obj", "Modelos//Tree_Spiky//Tree_Spiky.bmp", 1);
		DS_CG[0] = new EDXFramework::Model("Modelos//DS_CG//DS_CG.obj", "Modelos//DS_CG//DS_CG.bmp", 0);
		DS_CG[1] = new EDXFramework::Model("Modelos//DS_CG//DS_CG_Walking01.obj", "Modelos//DS_CG//DS_CG.bmp", 0);
		DS_CG[2] = new EDXFramework::Model("Modelos//DS_CG//DS_CG_Walking02.obj", "Modelos//DS_CG//DS_CG.bmp", 0);
		DS_CL[0] = new EDXFramework::Model("Modelos//DS_CL//DS_CL.obj", "Modelos//DS_CL//DS_CL.bmp", 1);
		DS_CL[1] = new EDXFramework::Model("Modelos//DS_CL//DS_CL_Running01.obj", "Modelos//DS_CL//DS_CL.bmp", 1);
		DS_CL[2] = new EDXFramework::Model("Modelos//DS_CL//DS_CL_Running02.obj", "Modelos//DS_CL//DS_CL.bmp", 1);
		Crow[0] = new EDXFramework::Model("Modelos//Crow//Crow.obj", "Modelos//Crow//Crow.bmp", 1);
		Crow[1] = new EDXFramework::Model("Modelos//Crow//Crow_Planeando01.obj", "Modelos//Crow//Crow.bmp", 1);
		Crow[2] = new EDXFramework::Model("Modelos//Crow//Crow_Planeando02.obj", "Modelos//Crow//Crow.bmp", 1);

		CamaraI(FP);
	}

	enum Altura { suelo, puente };

	void cesped_setup(Billboard *bills[], int cantidad, Altura alt, HWND hWnd)
	{
		int bx, by, bz;
		if (alt == Altura::suelo)
		{
			for (int i = 0; i < cantidad; i++)
			{
				do
				{
					bx = rand() % 490 + 1;
					bx -= 245;
					bz = rand() % 490 + 1;
					bz -= 245;
					by = cuenca->Superficie(bx, bz) * 4;
				} while (by > 35 || by < 15 || ((bx < -150 && bx > -170) && (bz > 200 && bz < 220)));
				bills[i] = new Billboard(hWnd, L"Imagenes//World//grass7_v2.png", 6, 3, bx, by, bz);
			}
		}
		else if (alt == Altura::puente)
		{
			for (int i = 0; i < cantidad; i++)
			{
				do
				{
					bx = rand() % 135 + 90;
					bx *= -1;
					bz = rand() % 115 + 10;
					bz *= -1;
					by = cuenca->Superficie(bx, bz) * 4;
				} while (by > 65 || by < 60);
				bills[i] = new Billboard(hWnd, L"Imagenes//World//grass7_v2.png", 6, 3, bx, by, bz);
			}
		}
	}

	void cesped_draw(Billboard *bills1[150], Billboard *bills2[15])
	{
		for (int i = 0; i < 150; i++)
			bills1[i]->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2]);
		for (int i = 0; i < 15; i++)
			bills2[i]->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2]);
	}

	void souls_setup(Billboard *bills[2], HWND hWnd)
	{
		bills[0] = new Billboard(hWnd, L"Imagenes//Soul//Soul01.png");
		bills[1] = new Billboard(hWnd, L"Imagenes//Soul//Soul02.png");
	}

	void souls_draw(Billboard *bills[2], float pos[3][5], float &esc, bool &escbool)
	{
		if (pos[0][4] == 1 || pos[1][4] == 1 || pos[2][4] == 1)
		{
			if (escbool == true)
				esc += 0.005;
			else if (escbool == false)
				esc -= 0.005;

			if (esc >= 0.5)
				escbool = false;
			else if (esc <= 0)
				escbool = true;

			for (int i = 0; i < 3; i++)
			{
				if (pos[i][4] == 1)
					bills[0]->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2], 4 + esc, 4 + esc, esc, pos[i][0], pos[i][1], pos[i][2]);

			}
		}
	}
	
	void souls_draw_partic(Billboard *bill, float posX, float posY, float posZ, float &randlimY, float &randY, float &randX, float &randZ, float &esc)
	{
		if (randY >= randlimY)
		{
			randY = 0;
			randX = 0;
			randZ = 0;
			esc = 0;
			randlimY = (rand() % 100) + 101;
			randlimY = randlimY / 100;
		}
		if (randX == 0)
		{
			randX = (rand() % 150) + 1;
			randX = randX / 100;
			randX -= 0.75;
		}
		if (randZ == 0)
		{
			randZ = (rand() % 150) + 1;
			randZ = randZ / 100;
			randZ -= 0.75;
		}
		if (esc == 0)
		{
			{
				esc = (rand() % 200) + 101;
				esc = esc / 100;
			}
		}

		bill->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2], esc, esc, 0, posX + randX, posY + randY + 1.25, posZ + randZ);

		if (randY < randlimY)
			randY += 0.01;
		//if (randX > 0)
		//	randX += 0.001;
		//else if (randX < 0)
		//	randX -= 0.001;
		//if (randZ > 0)
		//	randZ += 0.001;
		//else if (randZ < 0)
		//	randZ -= 0.001;
		if (randX > 0)
		{
			int random = (rand() % 60) + 1;
			if (random <= 45)
				randX += 0.003;
			else
				randX -= 0.001;
		}
		else if (randX < 0)
		{
			int random = (rand() % 60) + 1;
			if (random <= 45)
				randX -= 0.003;
			else
				randX += 0.001;
		}
		if (randZ > 0)
		{
			int random = (rand() % 60) + 1;
			if (random <= 45)
				randZ += 0.003;
			else
				randZ -= 0.001;
		}
		else if (randZ < 0)
		{
			int random = (rand() % 60) + 1;
			if (random <= 45)
				randZ -= 0.003;
			else
				randZ += 0.001;
		}


	}

	void bonfire_setup(Billboard *bills[10], HWND hWnd)
	{
		bills[0] = new Billboard(hWnd, L"Imagenes//Fire//Fire01.png");
		bills[1] = new Billboard(hWnd, L"Imagenes//Fire//Fire02.png");
		bills[2] = new Billboard(hWnd, L"Imagenes//Fire//Fire03.png");
		bills[3] = new Billboard(hWnd, L"Imagenes//Fire//Fire04.png");
		bills[4] = new Billboard(hWnd, L"Imagenes//Fire//Fire05.png");
		bills[5] = new Billboard(hWnd, L"Imagenes//Fire//Fire06.png");
		bills[6] = new Billboard(hWnd, L"Imagenes//Fire//Fire07.png");
		bills[7] = new Billboard(hWnd, L"Imagenes//Fire//Fire08.png");
		bills[8] = new Billboard(hWnd, L"Imagenes//Fire//Fire09.png");
		bills[9] = new Billboard(hWnd, L"Imagenes//Fire//Fire10.png");
	}

	void bonefire_draw(Billboard *bills[10], float posX, float posY, float posZ, int &index)
	{
		index ++;
		if (index >= 27)
			index = 0;
		int i = index / 3;

		bills[i]->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2], 3, 6, 0, posX, posY, posZ);
	}

	void bonefire_draw_partic(Billboard *bill, float posX, float posY, float posZ, float &randY, float &randX, float &randZ, float &esc, float &ang, float &anglim, float &rev, float oriX, float oriZ)
	{
		if (ang >= anglim || ang <= -anglim)
		{
			randY = 0;
			randX = 0;
			randZ = 0;
			esc = 0;
			ang = 0;
			anglim = (rand() % 271) + 90;			
			rev = (rand() % 30) + 1;
		}
		if (randY == 0)
		{
			randY = (rand() % 501) + 100;
			randY = randY / 100;
		}	
		if (randX == 0)
		{
			randX = (rand() % 600) + 1;
			randX = randX / 100;
			randX -= 3;
		}
		if (randZ == 0)
		{
			randZ = (rand() % 600) + 1;
			randZ = randZ / 100;
			randZ -= 3;
		}
		if (esc == 0)
		{
			{
				esc = (rand() % 150) + 151;
				esc = esc / 100;
			}
		}

		posX = posX + randX;
		posY = posY + randY;
		posZ = posZ + randZ;

		float s = sin(ang * 3.1415 / 180);
		float c = cos(ang * 3.1415 / 180);

		posX = posX - oriX;
		posZ = posZ - oriZ;

		float nposX = posX * c - posZ * s;
		float nposZ = posX * s + posZ * c;

		posX = nposX + oriX;
		posZ = nposZ + oriZ;

		bill->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2], esc, esc, 0, posX, posY, posZ);

		if (rev <= 15)
			ang += 1;
		else if (rev <= 30)
			ang -= 1;
	}

	void cadaveres_setup(int sel[3], float pos[3][5])
	{
		sel[0] = rand() % 7 + 1;
		do
		{
			sel[1] = rand() % 7 + 1;
		} while (sel[1] == sel[0]);
		do
		{
			sel[2] = rand() % 7 + 1;
		} while (sel[2] == sel[1] || sel[2] == sel[0]);

		for (int i = 0; i < 3; i++)
		{
			if (sel[i] == 1)
			{
				pos[i][0] = -105;
				pos[i][1] = cuenca->Superficie(-105, 90) * 4 + 0.25;
				pos[i][2] = 95;
				pos[i][3] = -30;
				pos[i][4] = 1;
			}
			else if (sel[i] == 2)
			{
				pos[i][0] = 70;
				pos[i][1] = cuenca->Superficie(70, 120) * 4 + 0.25;
				pos[i][2] = 120;
				pos[i][3] = 30;
				pos[i][4] = 1;
			}
			else if (sel[i] == 3)
			{
				pos[i][0] = 135;
				pos[i][1] = cuenca->Superficie(135, 210) * 4 + 0.25;
				pos[i][2] = 210;
				pos[i][3] = 90;
				pos[i][4] = 1;
			}
			else if (sel[i] == 4)
			{
				pos[i][0] = 155;
				pos[i][1] = cuenca->Superficie(155, 10) * 4 + 0.25;
				pos[i][2] = 10;
				pos[i][3] = 15;
				pos[i][4] = 1;
			}
			else if (sel[i] == 5)
			{
				pos[i][0] = -60;
				pos[i][1] = cuenca->Superficie(-60, -165) * 4 + 0.25;
				pos[i][2] = -165;
				pos[i][3] = 225;
				pos[i][4] = 1;
			}
			else if (sel[i] == 6)
			{
				pos[i][0] = -102.5;
				pos[i][1] = cuenca->Superficie(-102.5, -112.5) * 4 + 0.25;
				pos[i][2] = -112.5;
				pos[i][3] = -60;
				pos[i][4] = 1;
			}
			else if (sel[i] == 7)
			{
				pos[i][0] = -175;
				pos[i][1] = cuenca->Superficie(-175, -77.5) * 4 + 0.25;
				pos[i][2] = -77.5;
				pos[i][3] = -90;
				pos[i][4] = 1;
			}
		}
	}

	void cadaveres_draw(float pos[3][5])
	{
		for (int i = 0; i < 3; i++)
		{
			glPushMatrix();
			trans.Identidad();

			trans.Rota(pos[i][3], Y);
			trans.Escala(2, 2, 2);
			trans.Traslada(pos[i][0], pos[i][1], pos[i][2]);
			trans.MatrixGL();
			glMultMatrixf(trans.MGL);
			DS_Hollow[3]->Draw();

			trans.Identidad();
			glPopMatrix();
		}
	}

	void collect_pickups(float ppos[3][5], float cposX, float cposY, float cposZ)
	{
		for (int i = 0; i < 3; i++)
			if (ppos[i][4] == 1 && cposY <= ppos[i][1] + 10 && cposX <= ppos[i][0] + 10 && cposX >= ppos[i][0] - 10 && cposZ <= ppos[i][2] + 10 && cposZ >= ppos[i][2] - 10)
			{
				pickups++;
				ppos[i][4] = 0;
			}
	}

	void projectile_setup(Billboard *bills[6], HWND hWnd)
	{
		bills[0] = new Billboard(hWnd, L"Imagenes//Water//Water01.png");
		bills[1] = new Billboard(hWnd, L"Imagenes//Water//Water02.png");
		bills[2] = new Billboard(hWnd, L"Imagenes//Water//Water03.png");
		bills[3] = new Billboard(hWnd, L"Imagenes//Water//Water04.png");
		bills[4] = new Billboard(hWnd, L"Imagenes//Water//Water05.png");
		bills[5] = new Billboard(hWnd, L"Imagenes//Water//Water06.png");
	}
	
	void projectile_draw(Billboard *bills[6], float posX, float posY, float posZ, int &index)
	{
		index++;
		if (index >= 10)
			index = 0;
		int i = index / 2;

		bills[i]->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2], 20, 20, 0, posX, posY, posZ);
	}

	void projectile_draw_partic(Billboard *bill, float posX, float posY, float posZ, float &randlimY, float &randY, float &randX, float &randZ, float &esc)
	{
		if (randY <= randlimY)
		{
			randY = 0;
			randX = 0;
			randZ = 0;
			esc = 0;
			randlimY = (rand() % 651) + 350;
			randlimY = randlimY / 100;
			randlimY *= -1;
		}
		if (randX == 0)
		{
			randX = (rand() % 750) + 1;
			randX = randX / 100;
			randX -= 3.5;
		}
		if (randZ == 0)
		{
			randZ = (rand() % 750) + 1;
			randZ = randZ / 100;
			randZ -= 3.5;
		}
		if (esc == 0)
		{
			{
				esc = (rand() % 251) + 250;
				esc = esc / 100;
			}
		}

		bill->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2], esc, esc, 0, posX + randX, posY + randY + 5, posZ + randZ);

		if (randY > randlimY)
			randY -= 0.5;
		if (randX > 0)
			randX += 0.05;
		else if (randX < 0)
			randX -= 0.05;
		if (randZ > 0)
			randZ += 0.05;
		else if (randZ < 0)
			randZ -= 0.05;
	}

	void splash_setup(Billboard *bills[10], HWND hWnd)
	{
		bills[0] = new Billboard(hWnd, L"Imagenes//Water//Water_Splash01.png");
		bills[1] = new Billboard(hWnd, L"Imagenes//Water//Water_Splash02.png");
		bills[2] = new Billboard(hWnd, L"Imagenes//Water//Water_Splash03.png");
		bills[3] = new Billboard(hWnd, L"Imagenes//Water//Water_Splash04.png");
		bills[4] = new Billboard(hWnd, L"Imagenes//Water//Water_Splash05.png");
		bills[5] = new Billboard(hWnd, L"Imagenes//Water//Water_Splash06.png");
		bills[6] = new Billboard(hWnd, L"Imagenes//Water//Water_Splash07.png");
		bills[7] = new Billboard(hWnd, L"Imagenes//Water//Water_Splash08.png");
		bills[8] = new Billboard(hWnd, L"Imagenes//Water//Water_Splash09.png");
		bills[9] = new Billboard(hWnd, L"Imagenes//Water//Water06.png");
	}

	void splash_draw(Billboard *bills[10], float posX, float posY, float posZ, int &index)
	{
		index++;
		if (index >= 27)
		{
			index = 0;
			splashbool = false;
		}
		else
		{
			int i = index / 3;
			bills[i]->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2], 25, 20, 0, posX, posY, posZ);
		}
	}

	void projectile_collision()
	{
		if (crowposX >= posproj[0] - 8 && crowposX <= posproj[0] + 8 &&
			crowposY >= posproj[1] + 2 && crowposY <= posproj[1] + 18 &&
			crowposZ >= posproj[2] - 8 && crowposZ <= posproj[2] + 8)
		{
			dead = true;
			reves = -1;
		}
	}

	void hud_setup(Billboard *bills[12], HWND hWnd)
	{
		bills[0] = new Billboard(hWnd, L"Imagenes//HUD//Intro.png");
		bills[1] = new Billboard(hWnd, L"Imagenes//HUD//Pickup.png");
		bills[2] = new Billboard(hWnd, L"Imagenes//HUD//Victory.png");
		bills[3] = new Billboard(hWnd, L"Imagenes//HUD//You_Died.png");
		bills[4] = new Billboard(hWnd, L"Imagenes//HUD//MM_Jugar.png");
		bills[5] = new Billboard(hWnd, L"Imagenes//HUD//MM_Salir.png");
		bills[6] = new Billboard(hWnd, L"Imagenes//HUD//M_Objetivo.png");
		bills[7] = new Billboard(hWnd, L"Imagenes//HUD//M_Controles.png");
		bills[8] = new Billboard(hWnd, L"Imagenes//HUD//M_MenuP.png");
		bills[9] = new Billboard(hWnd, L"Imagenes//HUD//M_Objetivo_text.png");
		bills[10] = new Billboard(hWnd, L"Imagenes//HUD//M_Controles_text.png");
		bills[11] = new Billboard(hWnd, L"Imagenes//HUD//MM_Aux.png");
	}

	void hud_draw(Billboard *bills[12])
	{
		//Main menu
		if (menup == true && hudtext == true)
			bills[11]->Draw(FPCamPos[0], FPCamPos[1], FPCamPos[2], 2.5, 2.5, 0, FPCamPos[0] + FPCamDir[0] * 1.5, FPCamPos[1] - 1.25, FPCamPos[2] + FPCamDir[2] * 1.5);
		else if (menup == true && cursor == 0)
			bills[4]->Draw(FPCamPos[0], FPCamPos[1], FPCamPos[2], 2.5, 2.5, 0, FPCamPos[0] + FPCamDir[0] * 1.5, FPCamPos[1] - 1.25, FPCamPos[2] + FPCamDir[2] * 1.5);
		else if (menup == true && cursor == 1)
			bills[5]->Draw(FPCamPos[0], FPCamPos[1], FPCamPos[2], 2.5, 2.5, 0, FPCamPos[0] + FPCamDir[0] * 1.5, FPCamPos[1] - 1.25, FPCamPos[2] + FPCamDir[2] * 1.5);
		//Game menu
		if (menus == true && cursor == 0 && hudtext == true)
		{
			if (cam == TP)
				bills[9]->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2], 3, 3, 0, crowposX + TPCamDir[0] * 2.5, crowposY + 1.75 + (crowang * 1.25), crowposZ + TPCamDir[2] * 2.5);
			else if (cam == FP)
				bills[9]->Draw(FPCamPos[0], FPCamPos[1], FPCamPos[2], 0.75, 0.75, 0, FPCamPos[0] + FPCamDir[0] * 1.5, FPCamPos[1] - 0.25 + (crowang / 3), FPCamPos[2] + FPCamDir[2] * 1.5);

		}
		else if (menus == true && cursor == 1 && hudtext == true)
		{
			if (cam == TP)
				bills[10]->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2], 3, 3, 0, crowposX + TPCamDir[0] * 2.5, crowposY + 1.75 + (crowang * 1.25), crowposZ + TPCamDir[2] * 2.5);
			else if (cam == FP)
				bills[10]->Draw(FPCamPos[0], FPCamPos[1], FPCamPos[2], 0.75, 0.75, 0, FPCamPos[0] + FPCamDir[0] * 1.5, FPCamPos[1] - 0.25 + (crowang / 3), FPCamPos[2] + FPCamDir[2] * 1.5);

		}
		else if (menus == true && cursor == 0)
		{
			if (cam == TP)
				bills[6]->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2], 3, 3, 0, crowposX + TPCamDir[0] * 2.5, crowposY + 1.75 + (crowang * 1.25), crowposZ + TPCamDir[2] * 2.5);
			else if (cam == FP)
				bills[6]->Draw(FPCamPos[0], FPCamPos[1], FPCamPos[2], 0.75, 0.75, 0, FPCamPos[0] + FPCamDir[0] * 1.5, FPCamPos[1] - 0.25 + (crowang / 3), FPCamPos[2] + FPCamDir[2] * 1.5);

		}
		else if (menus == true && cursor == 1)
		{
			if (cam == TP)
				bills[7]->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2], 3, 3, 0, crowposX + TPCamDir[0] * 2.5, crowposY + 1.75 + (crowang * 1.25), crowposZ + TPCamDir[2] * 2.5);
			else if (cam == FP)
				bills[7]->Draw(FPCamPos[0], FPCamPos[1], FPCamPos[2], 0.75, 0.75, 0, FPCamPos[0] + FPCamDir[0] * 1.5, FPCamPos[1] - 0.25 + (crowang / 3), FPCamPos[2] + FPCamDir[2] * 1.5);
		}
		else if (menus == true && cursor == 2)
		{
			if (cam == TP)
				bills[8]->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2], 3, 3, 0, crowposX + TPCamDir[0] * 2.5, crowposY + 1.75 + (crowang * 1.25), crowposZ + TPCamDir[2] * 2.5);
			else if (cam == FP)
				bills[8]->Draw(FPCamPos[0], FPCamPos[1], FPCamPos[2], 0.75, 0.75, 0, FPCamPos[0] + FPCamDir[0] * 1.5, FPCamPos[1] - 0.25 + (crowang / 3), FPCamPos[2] + FPCamDir[2] * 1.5);
		}
		// intro
		if (intro == true && hudi <= 132)
		{
			hudi++;
			if (cam == TP)
				bills[0]->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2], 3, 3, 0, crowposX, crowposY + 2 + (crowang * 1.25), crowposZ);
			else if (cam == FP)
				bills[0]->Draw(FPCamPos[0], FPCamPos[1], FPCamPos[2], 0.75, 0.75, 0, FPCamPos[0] + FPCamDir[0] * 1.3, FPCamPos[1] - 0.25 + (crowang / 3), FPCamPos[2] + FPCamDir[2] * 1.3);
		}
		else if (intro == true && hudi > 132)
		{
			hudi = 0;
			intro = false;
		}
		// pickup
		if (pickuphud == true && hudi <= 132)
		{
			hudi++;
		if (cam == TP)
			bills[1]->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2], 3, 3, 0, crowposX + TPCamDir[0] * 2.5, crowposY + 1.9 + (crowang), crowposZ + TPCamDir[2] * 2.5);
		else if (cam == FP)
			bills[1]->Draw(FPCamPos[0], FPCamPos[1], FPCamPos[2], 0.75, 0.75, 0, FPCamPos[0] + FPCamDir[0] * 1.5, FPCamPos[1] - 0.25 + (crowang / 2.25), FPCamPos[2] + FPCamDir[2] * 1.5);
		}
		else if (pickuphud == true && hudi > 132)
		{
			hudi = 0;
			pickuphud = false;
		}
		//lose
		if (lose == true && hudi <= 165)
		{
			hudi++;
			if (cam == TP)
				bills[3]->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2], 12, 12, 0, crowposX + TPCamDir[0] * 1.5, crowposY - 3 + crowang, crowposZ + TPCamDir[2] * 1.5);
			else if (cam == FP)
				bills[3]->Draw(FPCamPos[0], FPCamPos[1], FPCamPos[2], 3, 3, 0, FPCamPos[0] + FPCamDir[0] * 1.5, FPCamPos[1] - 1.5 + (crowang / 2.5) , FPCamPos[2] + FPCamDir[2] * 1.5);
		}
		//win
		if (win == true && hudi <= 330)
		{
			hudi++;
			if (cam == TP)
				bills[2]->Draw(TPCamPos[0], TPCamPos[1], TPCamPos[2], 12, 12, 0, crowposX + TPCamDir[0] * 1.5, crowposY - 3 + crowang, crowposZ + TPCamDir[2] * 1.5);
			else if (cam == FP)
				bills[2]->Draw(FPCamPos[0], FPCamPos[1], FPCamPos[2], 3, 3, 0, FPCamPos[0] + FPCamDir[0] * 1.5, FPCamPos[1] - 1.5 + (crowang / 2.5), FPCamPos[2] + FPCamDir[2] * 1.5);
		}
	}

	void piedras_setup(float arr[][6], int cantidad, Altura alt)
	{
		if (alt == Altura::suelo)
		{
			for (int i = 0; i < cantidad; i++)
			{
				do
				{
					arr[i][0] = rand() % 490 + 1;
					arr[i][0] -= 245;
					arr[i][2] = rand() % 490 + 1;
					arr[i][2] -= 245;
					arr[i][1] = cuenca->Superficie(arr[i][0], arr[i][2]) * 4;
				} while (arr[i][1] > 30 || arr[i][1] < 15);
				arr[i][3] = rand() % 90 + 1;
				arr[i][4] = rand() % 3;
				arr[i][5] = ((rand() % 100 + 1.0) / 100.0) + 0.25;
			}
		}
		else if (alt == Altura::puente)
		{
			for (int i = 0; i < cantidad; i++)
			{
				do
				{
					arr[i][0] = rand() % 135 + 90;
					arr[i][0] *= -1;
					arr[i][2] = rand() % 115 + 10;
					arr[i][2] *= -1;
					arr[i][1] = cuenca->Superficie(arr[i][0], arr[i][2]) * 4;
				} while (arr[i][1] > 65 || arr[i][1] < 60);
				arr[i][3] = rand() % 90 + 1;
				arr[i][4] = rand() % 3;
				arr[i][5] = ((rand() % 100 + 1.0) / 100.0) + 0.25;
			}
		}
	}

	void piedras_draw(float arr[][6], int cantidad)
	{
		for (int i = 0; i < cantidad; i++)
		{
			glPushMatrix();
			trans.Identidad();

			trans.Rota(arr[i][3], arr[i][4]);
			trans.Escala(arr[i][5], arr[i][5], arr[i][5]);
			trans.Traslada(arr[i][0], arr[i][1], arr[i][2]);

			trans.MatrixGL();
			glMultMatrixf(trans.MGL);
			DS_Stone->Draw();

			trans.Identidad();
			glPopMatrix();
		}
	}

	void arbol(int x, int z, int r, EDXFramework::Model *modelo)
	{
		int y = cuenca->Superficie(x, z) * 4 - 1;

		glPushMatrix();
		trans.Identidad();

		trans.Rota(r , Y);
		//trans.Escala(0.5, 0.5, 0.5);
		trans.Traslada(x, y, z);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		modelo->Draw();

		trans.Identidad();
		glPopMatrix();
	}

	enum Orientacion { H, V };

	void arboles_setup(int lmin, int lmax, int espacio, int alt, int aux, Orientacion orientacion, int arr[104][4], int index)
	{
		int randval = 0;
		for (int i = lmin; i <= lmax; i += espacio)
		{
			randval = rand() % 20 + 1;
			if (orientacion == Orientacion::H)
			{
				arr[index][0] = i;
				arr[index][1] = alt;
				arr[index][2] = aux + randval;
			}
			else if (orientacion == Orientacion::V)
			{
				arr[index][0] = aux + randval;
				arr[index][1] = alt;
				arr[index][2] = i;
			}
			randval = rand() % 50 + 1;
			arr[index][3] = randval;
			index++;
		}
	}

	void arboles_draw(int arr[104][4], int index, int cantidad, Orientacion orientacion, EDXFramework::Model *modelo[3])
	{		
		for (int i = index; i <= cantidad; i++)
		{
			glPushMatrix();
			trans.Identidad();

			if (orientacion == Orientacion::H)
				trans.Rota(arr[i][0], Y);
			else if (orientacion == Orientacion::V)
				trans.Rota(arr[i][2], Y);
			//trans.Escala(0.5, 0.5, 0.5);
			trans.Traslada(arr[i][0], arr[i][1], arr[i][2]);
			trans.MatrixGL();
			glMultMatrixf(trans.MGL);
			if (arr[i][3] <= 20)
				modelo[0]->Draw();
			else if (arr[i][3] > 20 && arr[i][3] <= 40)
				modelo[1]->Draw();
			else if (arr[i][3] > 40)
				modelo[2]->Draw();

			trans.Identidad();
			glPopMatrix();
		}
	}

	void movimiento(int &cont, int vel, EDXFramework::Model *modelo[3])
	{
		cont++;
		if (cont <= vel)
			modelo[1]->Draw();
		else if (cont <= vel * 2)
			modelo[0]->Draw();
		else if (cont <= vel * 3)
			modelo[2]->Draw();
		else if (cont <= vel * 4)
			modelo[0]->Draw();
		else if (cont > vel * 4)
		{
			cont = 0;
			modelo[1]->Draw();
		}
	}

	void planear(int &cont, int vel, EDXFramework::Model *modelo[3])
	{
		cont++;
		if (cont <= vel * 10)
			modelo[0]->Draw();
		else if (cont <= vel * 11)
			modelo[1]->Draw();
		else if (cont <= vel * 12)
			modelo[2]->Draw();
		else if (cont > vel * 12)
		{
			cont = 0;
			modelo[0]->Draw();
		}
	}

	bool world_collision()
	{
		if (PlayerPos[1] >= 85 || PlayerPos[1] <= cuenca->Superficie(PlayerPos[0], PlayerPos[2]) * 4 + 2 || PlayerPos[1] <= 20)
			return true;
		else
			return false;
	}

	bool box_collision(bool center_upwards,float posX, float posY, float posZ, float boxX, float boxY, float boxZ)
	{
		if (center_upwards == true)
		{
			if (PlayerPos[0] >= posX - boxX && PlayerPos[0] <= posX + boxX &&
				PlayerPos[1] >= posY - boxY && PlayerPos[1] <= posY + boxY &&
				PlayerPos[2] >= posZ - boxZ && PlayerPos[2] <= posZ + boxZ)
				return true;
			else
				return false;
		}
		else if (center_upwards == false)
		{
			if (PlayerPos[0] >= posX - boxX && PlayerPos[0] <= posX + boxX
				&& PlayerPos[1] >= posY && PlayerPos[1] <= posY + boxY &&
				PlayerPos[2] >= posZ - boxZ && PlayerPos[2] <= posZ + boxZ)
				return true;
			else
				return false;
		}
	}

	bool check_colisions()
	{
		if (world_collision() == false) // terreno y skydome
			if (box_collision(false, hollowposX, 23, hollowposZ, 3.3, 9.25, 3.3) == false) //hollow
				if (box_collision(true, -160, 25.5, 210, 3, 4.25, 3) == false) //bonfire
					if (box_collision(false, -112, 57, -72.5, 7, 5, 35) == false) //bridge
						if (box_collision(false, -89, cuenca->Superficie(-89, -112) * 4, -112, 3, 30, 4) == false) // stair
						if (box_collision(false, -200, 17, -80, 3, 9.25, 3) == false) //greatsword
							if (box_collision(true, 5, 22, 70, 3, 4, 3) == false) // stone
								if (box_collision(false, hydraposX, cuenca->Superficie(hydraposX, hydraposZ) * 4, hydraposZ, 50, 60, 50) == false) // hydra
									if (box_collision(true, cg1posX, cg1posY, cg1posZ, 8, 15, 7) == false) // cg1
										if (box_collision(true, cg2posX, cg2posY, cg2posZ, 8, 15, 7) == false) // cg2
											if (box_collision(true, cg3posX, cg3posY, cg3posZ, 8, 15, 7) == false) // cg3
												if (box_collision(false, -115, cuenca->Superficie(-115, 175) * 4, 175, 7, 80, 7) == false) //tree_tall
													if (box_collision(false, 60, cuenca->Superficie(60, 190) * 4, 190, 7, 80, 7) == false) //tree_tall
														if (box_collision(false, 40, cuenca->Superficie(40, 120) * 4, 120, 7, 80, 7) == false) //tree_tall
															if (box_collision(false, 110, cuenca->Superficie(110, 155) * 4, 155, 7, 80, 7) == false) //tree_tall
																if (box_collision(false, -45, cuenca->Superficie(-45, 130) * 4, 130, 7, 80, 7) == false) //tree_tall
																	if (box_collision(false, -90, cuenca->Superficie(-90, 120) * 4, 120, 7, 80, 7) == false) //tree_tall
																		if (box_collision(false, -135, cuenca->Superficie(-135, 100) * 4, 100, 7, 80, 7) == false) //tree_tall
																			if (box_collision(false, 100, cuenca->Superficie(100, 120) * 4, 120, 8, 60, 8) == false) //tree_fat
																				if (box_collision(false, 80, cuenca->Superficie(80, 90) * 4, 90, 8, 60, 8) == false) //tree_fat
																					if (box_collision(false, 120, cuenca->Superficie(120, 100) * 4, 90, 8, 60, 8) == false) //tree_fat
																						if (box_collision(false, -105, cuenca->Superficie(-105, 55) * 4, 55, 8, 60, 8) == false) //tree_fat																	
																							if (box_collision(false, -40, cuenca->Superficie(-40, 65) * 4, 65, 8, 60, 8) == false) //tree_fat
																								if (box_collision(false, -50, cuenca->Superficie(-50, 10) * 4, 10, 6, 50, 6) == false) //tree_spiky
																									if (box_collision(false, 50, cuenca->Superficie(50, 45) * 4, 45, 6, 50, 6) == false) //tree_spiky
																										if (box_collision(false, 125, cuenca->Superficie(125, 35) * 4, 35, 6, 50, 6) == false) //tree_spiky
																											if (box_collision(false, -60, cuenca->Superficie(-60, -180) * 4, -180, 6, 50, 6) == false) //tree_spiky																						
																												return false;
		else
			return true;		
	}

	void reiniciar(bool menu_reset)
	{
		//arboles atras cuenca
		arboles_setup(-280, 280, 20, 80, -285, H, arboles, 0);
		//adelante cuenca
		arboles_setup(-280, 280, 20, 80, 285, H, arboles, 28);
		//derecha cuenca
		arboles_setup(-250, 250, 25, 80, -285, V, arboles, 56);
		//izquierda cuenca
		arboles_setup(-250, 250, 25, 80, 285, V, arboles, 76);
		//vegetacion
		cesped_setup(cesped, 150, suelo, Hwnd);
		cesped_setup(cesped2, 15, puente, Hwnd);
		//piedras
		piedras_setup(piedras, 50, suelo);
		piedras_setup(piedras2, 5, puente);
		//pickups
		cadaveres_setup(cadsel, cadpos);

		if (menu_reset == true)
		{
			menup = true;
			menusound = true;
			CamaraI(FP);
			newarea = true;
		}
		else if (menu_reset == false)
		{
			VeRot = 0;
			HoRot = 0;
			TPCamPos[0] = -184;
			TPCamPos[1] = 31;
			TPCamPos[2] = 221;
			TPCamDir[0] = 0;
			TPCamDir[1] = 0;
			TPCamDir[2] = -1;
			CamaraRotacion(Y, -52);
			updateFP();
			reves = 1;

			bonfire_sound = true;
		}
		
		intro = false;
		win = false;
		lose = false;
		dead = false;
		hudi = 0;
		pickups = 0;
		hydrauxi = 0;
		projbool = false;
		splashbool = false;
		splashi = 0;
		fxaux = false;
		menus = false;
		cursor = 0;
		endwait = 0;

		crowi = 0;
		hydrai = 0;
		hydrauxi = 0;
		cgi = 0;
		cg2i = 0;
		cg3i = 0;
		cli = 0;
		hollowi = 0;

		crowposX = 0;
		crowposY = 0;
		crowposZ = 0;
		crowang = 0;
		cg1posX = 135;
		cg1posY = 0; 
		cg1posZ = 175;
		cg1ang = 180;
		cg2posX = -60;
		cg2posY = 0;
		cg2posZ = -60;
		cg2ang = 45;
		cg3posX = 0;
		cg3posY = 0;
		cg3posZ = 0;
		cg3dis = 25;
		cg3ang = 0;
		clbool = false;
		claux = true;
		clposX = -115;
		clposY = 0;
		clposZ = -25;
		clang = 0;
	}

	void render(HDC hDC)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);
		glLoadIdentity();

		CamaraU(cam);

		GLfloat Position[] = { 0, 300, 250, 1 };
		glLightfv(GL_LIGHT0, GL_POSITION, Position);

		//Skydome luz
		GLfloat skyAmbient[] = { 0.9, 0.9, 0.9, 1 };
		GLfloat skyDiffuse[] = { 0.1, 0.1, 0.1, 1 };
		GLfloat skySpecular[] = { 1.0, 1.0, 1.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_AMBIENT, skyAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, skyDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, skySpecular);
		
		//Niebla
		if (nieblabool == true)
			niebla.enable();
		else if (nieblabool == false)
			niebla.disable();

		//Skydome niebla
		niebla.densidad = ndens / 2;
		niebla.draw();

		glPushMatrix();
		glTranslatef(0, 40, 50);
		gluSphere(gluNewQuadric(), 10, 32, 32);
		glPopMatrix();

		//Skydome
		glPushMatrix();
		trans.Identidad();

		trans.Rota(90, Y);
		trans.Traslada(0, -100, 0);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		noche->Draw();

		trans.Identidad();
		glPopMatrix();
		
		//Terreno luz
		GLfloat terAmbient[] = { 0.15, 0.15, 0.3, 1 };
		GLfloat terDiffuse[] = { 0.85, 0.85, 0.7, 1 };
		GLfloat terSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_AMBIENT, terAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, terDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, terSpecular);

		//Terreno niebla
		niebla.densidad = ndens;
		niebla.draw();

		//Terreno		
		glPushMatrix();
		trans.Identidad();

		trans.Escala(1, 4, 1);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		cuenca->Draw();

		trans.Identidad();
		glPopMatrix();

		////Agua
		//glPushMatrix();
		//trans.Identidad();
		//
		//trans.Traslada(50, 12, -100);
		//trans.MatrixGL();
		//glMultMatrixf(trans.MGL);
		//lago->Draw();
		//
		//trans.Identidad();
		//glPopMatrix();

		//Modelos luz
		GLfloat modAmbient[] = { 0.1, 0.1, 0.2, 1 };
		GLfloat modDiffuse[] = { 0.9, 0.9, 0.8, 1 };
		GLfloat modSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_AMBIENT, modAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, modDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, modSpecular);

		//Crow	tercera persona
		crowposX = PlayerPos[0];
		crowposY = PlayerPos[1];
		crowposZ = PlayerPos[2];
		crowang = (VeRot * pow(3.1416, 2)) / 180;

		glPushMatrix();
		trans.Identidad();
		
		if (dead == false)
			trans.Traslada(0, crowang, 0);
		if (dead == true)
		{
			if (VeRot > -45 && crowposY > cuenca->Superficie(crowposX, crowposZ) * 4 + 1 && crowposY > 20)
				CamaraRotacion(X, -1);

			if (crowposY > cuenca->Superficie(crowposX, crowposZ) * 4 + 1 && crowposY > 20)
			CamaraTraslacion(D, 1);

			trans.Rota(180, Z);
			trans.Traslada(0, crowang + 3, 0);
			
		}
		trans.Rota(180 + HoRot, Y);
		trans.Traslada(crowposX, crowposY, crowposZ);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		if (cam == TP)
		{
			if (dead == true)
				Crow[1]->Draw();
			else
				planear(crowi, 15, Crow);
		}

		trans.Identidad();
		glPopMatrix();

		//Hydra	flota
		if (hydrabool == true)
			hydraposY += 0.0125;
		else if (hydrabool == false)
			hydraposY -= 0.0125;
		if (hydraposY >= 17)
			hydrabool = false;
		else if (hydraposY <= 13)
			hydrabool = true;

		hydrang = atan2((crowposX - hydraposX), (crowposZ - hydraposZ)) * 180 / 3.14159265;

		glPushMatrix();
		trans.Identidad();

		trans.Rota(hydrang, Y);
		trans.Escala(5, 5, 5);
		trans.Traslada(hydraposX, hydraposY, hydraposZ);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		if (win == false && lose == false && menup == false)
			hydrauxi++;
		else
			hydrauxi = 0;
		if (hydrauxi <= 350)
			movimiento(hydrai, 40, DS_Hydra);
		else if (hydrauxi <= 400)
			DS_Hydra[3]->Draw();
		else if (hydrauxi > 400)
		{
			hydrauxi = 0;
			DS_Hydra[3]->Draw();
		}

		trans.Identidad();
		glPopMatrix();

		if (hydrauxi == 350)
		{
			//posicion de origen
			oriproj[0] = hydraposX;
			oriproj[1] = hydraposY + 10;
			oriproj[2] = hydraposZ + 40;
			posproj[0] = oriproj[0];
			posproj[1] = oriproj[1] + projvel[1];
			posproj[2] = oriproj[2] + projvel[0];
			posproj[3] = -hydrang;

			//angulos de origen
			//posproj[0] = oriproj[0];
			//posproj[1] = oriproj[1];
			//posproj[2] = oriproj[2];
			//posproj[3] = -hydrang;
			//float s = sin(posproj[3] * 3.1415 / 180);
			//float c = cos(posproj[3] * 3.1415 / 180);
			//posproj[0] = posproj[0] - hydraposX;
			//posproj[2] = posproj[2] - hydraposZ;
			//float npos1 = posproj[0] * c - posproj[2] * s;
			//float npos2 = posproj[0] * s + posproj[2] * c;
			//posproj[0] = npos1 + hydraposX;
			//posproj[2] = npos2 + hydraposZ;
			//float dp = (posproj[0] * crowposX) + (posproj[1] * crowposY) + (posproj[2] * crowposZ);
			//float mags = sqrt(pow(posproj[0], 2) + pow(posproj[1], 2) + pow(posproj[2], 2)) * sqrt(pow(crowposX, 2) + pow(crowposY, 2) + pow(crowposZ, 2));
			//posproj[4] = acos(dp / mags) * 180 / 3.1415;
			//posproj[4] = atan2((crowposY - posproj[1]), (crowposZ - posproj[2]));
			//posproj[4] *= -1;

			//reset
			projvel[0] = 0;
			projvel[1] = 0;
			projbool = true;
			fxaux = true;
		}
		if (projbool == true)
		{
			//pivote
			posproj[0] = oriproj[0];
			posproj[1] = oriproj[1] + projvel[1];
			posproj[2] = oriproj[2] + projvel[0];

			////rotar en X
			//float rotY[4][4] = { 1, 0, 0, 0,
			//	0, cos(posproj[4]), -sin(posproj[4]), 0,
			//	0, sin(posproj[4]), cos(posproj[4]), 0,
			//	0, 0, 0, 1 };
			//float aux[3];
			//aux[0] = (posproj[0] * rotY[0][0]) + (posproj[1] * rotY[0][1]) + (posproj[2] * rotY[0][2]);
			//aux[1] = (posproj[0] * rotY[1][0]) + (posproj[1] * rotY[1][1]) + (posproj[2] * rotY[1][2]);
			//aux[2] = (posproj[0] * rotY[2][0]) + (posproj[1] * rotY[2][1]) + (posproj[2] * rotY[2][2]);
			//posproj[0] = aux[0];
			//posproj[1] = aux[1];
			//posproj[2] = aux[2];

			////rotar en X
			//float s = sin(posproj[4] * 3.1415 / 180);
			//float c = cos(posproj[4] * 3.1415 / 180);
			//posproj[1] = posproj[1] - hydraposX;
			//posproj[2] = posproj[2] - hydraposZ;
			//float npos1 = posproj[1] * c - posproj[2] * s;
			//float npos2 = posproj[1] * s + posproj[2] * c;
			//posproj[1] = npos1 + hydraposX;
			//posproj[2] = npos2 + hydraposZ;

			//rotar en Y
			float s = sin(posproj[3] * 3.1415 / 180);
			float c = cos(posproj[3] * 3.1415 / 180);
			posproj[0] = posproj[0] - hydraposX;
			posproj[2] = posproj[2] - hydraposZ;
			float npos1 = posproj[0] * c - posproj[2] * s;
			float npos2 = posproj[0] * s + posproj[2] * c;
			posproj[0] = npos1 + hydraposX;
			posproj[2] = npos2 + hydraposZ;

			//colision
			projectile_collision();

			//movimiento
			if (posproj[1] < (cuenca->Superficie(posproj[0], posproj[2]) * 4 - 10))
			{
				projbool = false;
				splashbool = true;
				fxaux = true;
			}
			else
			{
				projvel[0] += 2;
				projvel[1] -= 0.175;
			}
								
		}

		//CG 1	recta / zig-zag / rotacion
		cg1posY = cuenca->Superficie(cg1posX, cg1posZ) * 4 + 9;

		if (cg1bool == true && cg1ang == 180)
			cg1posZ -= 0.15;
		else if (cg1bool == false && cg1ang == 360)
			cg1posZ += 0.15;
		if (cg1posZ <= 50)
			cg1bool = false;
		else if (cg1posZ >= 175)
			cg1bool = true;

		if (cg1ang == 180 || cg1ang == 360)
		{
			if (cg1aux == true)
				cg1posX -= 0.025;
			else if (cg1aux == false)
				cg1posX += 0.025;
			if (cg1posX <= 135)
				cg1aux = false;
			else if (cg1posX >= 140)
				cg1aux = true;
		}

		if (cg1bool == false && cg1ang < 360)
			cg1ang += 3;
		else if (cg1bool == true && cg1ang > 180)
			cg1ang -= 3;

		glPushMatrix();
		trans.Identidad();

		trans.Rota(cg1ang, Y);
		trans.Escala(4.5, 4.5, 4.5);
		trans.Traslada(cg1posX, cg1posY, cg1posZ);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		if (cg1ang == 180 || cg1ang == 360)
			movimiento(cgi, 20, DS_CG);
		else
			DS_CG[0]->Draw();

		trans.Identidad();
		glPopMatrix();

		//CG 2	doble recta / rotacion
		cg2posY = cuenca->Superficie(cg2posX, cg2posZ) * 4 + 9;

		if (cg2bool == true && cg2ang == 45)
		{
			cg2posX += 0.1;
			cg2posZ += 0.1;
		}
		else if (cg2bool == false && cg2ang == 225)
		{
			cg2posX -= 0.1;
			cg2posZ -= 0.1;
		}
		if (cg2posX >= 30 && cg2posZ >= 30)
			cg2bool = false;
		else if (cg2posX <= -60 && cg2posZ <= -60)
			cg2bool = true;

		if (cg2bool == false && cg2ang < 225)
			cg2ang += 3;
		else if (cg2bool == true && cg2ang > 45)
			cg2ang -= 3;

		glPushMatrix();
		trans.Identidad();

		trans.Rota(cg2ang, Y);
		trans.Escala(4.5, 4.5, 4.5);
		trans.Traslada(cg2posX, cg2posY, cg2posZ);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		if (cg2ang == 45 || cg2ang == 225)
			movimiento(cg2i, 20, DS_CG);
		else
			DS_CG[0]->Draw();

		trans.Identidad();
		glPopMatrix();

		//CG 3	vuelta / zig-zag
		cg3posX = 5 + cg3dis * cos(3.1415 * cg3ang / 180);
		cg3posZ = 70 + cg3dis * sin(3.1415 * cg3ang / 180);
		cg3posY = cuenca->Superficie(cg3posX, cg3posZ) * 4 + 9;

		if (cg3bool == true)
			cg3dis += 0.025;
		else if (cg3bool == false)
			cg3dis -= 0.025;
		if (cg3dis >= 30)
			cg3bool = false;
		else if (cg3dis <= 25)
			cg3bool = true;

		cg3ang += 0.2;
		if (cg3ang >= 360)
		{
			cg3ang = 0;
		}

		glPushMatrix();
		trans.Identidad();

		trans.Rota(-cg3ang, Y);
		trans.Escala(4.5, 4.5, 4.5);
		trans.Traslada(cg3posX, cg3posY, cg3posZ);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		movimiento(cg3i, 20, DS_CG);

		trans.Identidad();
		glPopMatrix();

		//CL	trigger / recta / zig-zag / rotacion
		clposY = cuenca->Superficie(clposX, clposZ) * 4 + 0.5;

		if (crowposX <= -100 && crowposX >= -130 && crowposY >= 60 && crowposY <= 80 && crowposZ <= -10 && crowposZ >= -40)
			clbool = true;
		else if (clposX <= -230)
			clbool = false;

		if (clbool == true)
		{
			clposX -= 0.25;

			if (claux == true)
				clposZ += 0.1;
			else if (claux == false)
				clposZ -= 0.1;

			if (clposZ >= -23.5)
				claux = false;
			else if (clposZ <= -26.5)
				claux = true;
		}
		else if (clbool == false && clposX <= -230 && clang < 180)
			clang += 9;

		glPushMatrix();
		trans.Identidad();

		trans.Rota(clang, Z);
		trans.Rota(-90, Y);
		trans.Escala(0.8, 0.8, 0.8);
		trans.Traslada(clposX, clposY, clposZ);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		if (clbool == false)
			DS_CL[0]->Draw();
		else if (clbool == true)
			movimiento(cli, 10, DS_CL);

		trans.Identidad();
		glPopMatrix();

		//Hollow
		if (hollowbool == true)
			hollowi++;
		else if (hollowbool == false)
			hollowi--;
		if (hollowi >= 200)
			hollowbool = false;
		else if (hollowi <= 0)
			hollowbool = true;

		if (hollowi <= 70)
			hollowposY = cuenca->Superficie(hollowposX, hollowposZ) * 4 + 5;
		else if (hollowi <= 100)
			hollowposY = cuenca->Superficie(hollowposX, hollowposZ) * 4 + 4.4;
		else if (hollowi <= 200)
			hollowposY = cuenca->Superficie(hollowposX, hollowposZ) * 4;

		glPushMatrix();
		trans.Identidad();

		trans.Rota(120, Y);
		trans.Escala(2, 2, 2);
		trans.Traslada(hollowposX, hollowposY, hollowposZ);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		if (hollowi <= 70)
			DS_Hollow[0]->Draw();
		else if (hollowi <= 100)
			DS_Hollow[1]->Draw();
		else if (hollowi <= 200)
			DS_Hollow[2]->Draw();

		trans.Identidad();
		glPopMatrix();

		//Arboles zona de golems
		arbol(60, 190, 30, Tree[1]);
		arbol(40, 120, 120, Tree[1]);
		arbol(50, 45, 90, Tree[2]);
		arbol(125, 35, 120, Tree[2]);
		arbol(120, 100, 300, Tree[0]);
		arbol(110, 155, 180, Tree[1]);
		arbol(80, 90, 210, Tree[0]);
		arbol(100, 120, 240, Tree[0]);
		arbol(-115, 175, 270, Tree[1]);
		arbol(-45, 130, 300, Tree[1]);
		arbol(-50, 10, 330, Tree[2]);
		arbol(-105, 55, 360, Tree[0]);
		arbol(-90, 120, 30, Tree[1]);
		arbol(-135, 100, 60, Tree[1]);
		arbol(-40, 65, 180, Tree[0]);
		arbol(-60, -180, 135, Tree[2]);
		//arboles en las montañas
		arbol(-225, 155, 30, Tree[0]);
		arbol(-205, 75, 60, Tree[0]);
		arbol(-205, 10, 90, Tree[1]);
		arbol(-160, 18, 120, Tree[0]);
		arbol(-155, -160, 150, Tree[0]);
		arbol(-135, -220, 180, Tree[1]);
		arbol(-205, -155, 210, Tree[0]);
		arbol(200, 120, 240, Tree[0]);
		//atras cuenca
		arboles_draw(arboles, 0, 27, H, Tree);
		//adelante cuenca
		arboles_draw(arboles, 28, 55, H, Tree);
		//derecha cuenca
		arboles_draw(arboles, 56, 75, V, Tree);
		//izquierda cuenca
		arboles_draw(arboles, 76, 95, V, Tree);

		//Piedras aleatorias
		piedras_draw(piedras, 50);
		piedras_draw(piedras2, 5);

		glPushMatrix();
		trans.Identidad();

		trans.Escala(1.5, 1.5, 1.5);
		trans.Traslada(5, 22, 70);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		DS_Stone->Draw();

		trans.Identidad();
		glPopMatrix();

		//Bridge
		glPushMatrix();
		trans.Identidad();

		trans.Escala(1, 1, 2);
		trans.Traslada(-112, 60.5, -72.5);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		DS_Bridge->Draw();

		trans.Identidad();
		glPopMatrix();

		//Stair
		glPushMatrix();
		trans.Identidad();

		trans.Rota(90, Y);
		trans.Rota(12, Z);
		trans.Escala(1.5, 1.5, 1.5);
		trans.Traslada(-86, 26.5, -113);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		DS_Stair->Draw();

		trans.Identidad();
		trans.Traslada(0, 8, 0);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		DS_Stair->Draw();

		trans.Identidad();
		trans.Traslada(0, 8, 0);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		DS_Stair->Draw();

		trans.Identidad();
		glPopMatrix();

		//Bonfire
		glPushMatrix();
		trans.Identidad();

		trans.Rota(180, Y);
		trans.Escala(0.8, 0.8, 0.8);
		trans.Traslada(-160, 25.5, 210);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		DS_Bonfire->Draw();

		trans.Identidad();
		glPopMatrix();

		//Greatsword
		glPushMatrix();
		trans.Identidad();

		trans.Rota(90, Y);
		trans.Rota(180, X);
		trans.Escala(1.5, 1.5, 1.5);
		trans.Traslada(-200, 24, -80);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		DS_Greatsword->Draw();

		trans.Identidad();
		glPopMatrix();

		//Agua
		glPushMatrix();
		trans.Identidad();

		trans.Traslada(50, 12, -100);
		trans.MatrixGL();
		glMultMatrixf(trans.MGL);
		lago->Draw();

		trans.Identidad();
		glPopMatrix();

		//Pickups
		cadaveres_draw(cadpos);

		souls_draw(souls, cadpos, soulsesc, soulsescbool);
		if (cadpos[0][4] == 1)
		{
			for (int i = 0; i < 5; i++)
				souls_draw_partic(souls[1], cadpos[0][0], cadpos[0][1], cadpos[0][2], soulspartic[i][0], soulspartic[i][1], soulspartic[i][2], soulspartic[i][3], soulspartic[i][4]);
		}
		if (cadpos[1][4] == 1)
		{
			for (int j = 5; j < 10; j++)
				souls_draw_partic(souls[1], cadpos[1][0], cadpos[1][1], cadpos[1][2], soulspartic[j][0], soulspartic[j][1], soulspartic[j][2], soulspartic[j][3], soulspartic[j][4]);
		}
		if (cadpos[2][4] == 1)
		{
			for (int k = 10; k < 15; k++)
				souls_draw_partic(souls[1], cadpos[2][0], cadpos[2][1], cadpos[2][2], soulspartic[k][0], soulspartic[k][1], soulspartic[k][2], soulspartic[k][3], soulspartic[k][4]);
		}

		//bonfire fire
		bonefire_draw(bonfire, -160.2, 21.75, 209.75, bonefirei);
		for (int i = 0; i < 3; i++)
			bonefire_draw_partic(bonfire[9], -160.2, 21.75, 209.75, bonfirepartic[i][0], bonfirepartic[i][1], bonfirepartic[i][2], bonfirepartic[i][3], bonfirepartic[i][4], bonfirepartic[i][5], bonfirepartic[i][6], -160.2, 209.75);

		//projectile
		if (projbool == true)
		{
			projectile_draw(projectile, posproj[0], posproj[1], posproj[2], projectilei);
			for (int i = 0; i < 5; i++)
				projectile_draw_partic(projectile[5], posproj[0], posproj[1], posproj[2], projpartic[i][0], projpartic[i][1], projpartic[i][2], projpartic[i][3], projpartic[i][4]);
		}
		if (splashbool == true)
		{
			splash_draw(splash, posproj[0], posproj[1] + 7, posproj[2], splashi);
		}

		//vegetacion aleatoria
		cesped_draw(cesped, cesped2);

		//hud
		hud_draw(hud);

		//Niebla disable
		if (nieblabool == true)
			niebla.disable();

		SwapBuffers(hDC);
	}
};
#endif
