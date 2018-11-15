#include<Windows.h>
#include "Escena.h"
#include "resource.h"
#include <gl\glew.h>
#include <gl\GLU.h>
#include <gl\GL.h>
#include<mmsystem.h>
#pragma comment (lib, "glew32.lib") 
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib, "winmm.lib")

#define Timer1 100
HDC hContextoVentana;
Escena *escena;
bool renderiza = false;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void DefPixelFormat(HDC hDC);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hwndVentana;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	//wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = "HUUUH";
	RegisterClassEx(&wc);
	RECT wr = { 0, 0, 1080, 720 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hwndVentana = CreateWindowEx(NULL,
		"HUUUH",
		"GC SOULS",
		WS_OVERLAPPEDWINDOW,
		800,
		100,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwndVentana, nCmdShow);

	glewInit();

	escena = new Escena(hwndVentana);

	SetTimer(hwndVentana, Timer1, 30, NULL);

	MSG msg = { 0 };
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			if (renderiza)
			{
				escena->render(hContextoVentana);
				renderiza = false;
			}
		}
	}

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HGLRC hContextoGL;
	//static HDC hContextoAux;
	int ancho, alto;

	//char epx[6] = "";
	//char epz[6] = "";
	//char epy[6] = "";
	//char edx[6] = "";
	//char edz[6] = "";
	//char edy[6] = "";
	//char ec[80] = "";

	switch (message)
	{
		case WM_CREATE:
		{
			srand(time(NULL));

			//hContextoAux = GetDC(hWnd);
			hContextoVentana = GetDC(hWnd);
			DefPixelFormat(hContextoVentana);
			hContextoGL = wglCreateContext(hContextoVentana);
			wglMakeCurrent(hContextoVentana, hContextoGL);

			mciSendString("open \"Sonidos//Menu.mp3\" alias MM", NULL, 0, 0);
			mciSendString("open \"Sonidos//New_Area.mp3\" alias NA", NULL, 0, 0);
			mciSendString("open \"Sonidos//Darkroot_Basin.mp3\" alias DB", NULL, 0, 0);
			mciSendString("open \"Sonidos//Bonfire.mp3\" alias BF", NULL, 0, 0);
			mciSendString("open \"Sonidos//Jugar.mp3\" alias GS", NULL, 0, 0);
			mciSendString("open \"Sonidos//Cursor_Select.mp3\" alias CS", NULL, 0, 0);
			mciSendString("open \"Sonidos//Cursor_Ok.mp3\" alias CO", NULL, 0, 0);
			mciSendString("open \"Sonidos//Cursor_Cancel.mp3\" alias CC", NULL, 0, 0);
			mciSendString("open \"Sonidos//Item_Pickup.mp3\" alias IP", NULL, 0, 0);
			mciSendString("open \"Sonidos//Victory_Long.mp3\" alias VY", NULL, 0, 0);
			mciSendString("open \"Sonidos//Hydra_Projectile.mp3\" alias HP", NULL, 0, 0);
			mciSendString("open \"Sonidos//Water_Splash.mp3\" alias WS", NULL, 0, 0);
			mciSendString("open \"Sonidos//You_Died.mp3\" alias YD", NULL, 0, 0);
		}
		break;
		//case WM_MENUSELECT:
		//{
		//	if ((HIWORD(wParam) & MF_POPUP) != MF_POPUP)
		//		mciSendString("play CS from 0", NULL, 0, 0);
		//}
		//break;
		//case WM_COMMAND:
		//{
		//	switch (LOWORD(wParam))
		//	{
		//		case ID_AYUDA_OBJETIVO:
		//		{
		//			mciSendString("play CS from 0", NULL, 0, 0);
		//			if(MessageBox(hWnd, "-Victoria:\n\tRecolectar las 3 almas en el mapa.\n\n-Derrota:\n\tRecibir un impacto de la hydra.", "Objetivo", MB_OK) == IDOK)
		//				mciSendString("play CO from 0", NULL, 0, 0);
		//			else
		//				mciSendString("play CC from 0", NULL, 0, 0);
		//		}
		//		break;
		//		case ID_AYUDA_CONTROLES:
		//		{
		//			mciSendString("play CS from 0", NULL, 0, 0);
		//			if(MessageBox(hWnd, "-Jugador:\n\tW - Adelante.\n\tS - Atras.\n\tA - Izquierda.\n\tD - Derecha\n\tQ - Arriba\n\tE - Abajo\n\tC - Recolectar alma\n\tV - Cambiar vista\n\n-Camara:\n\t^ - Arriba\n\tv - Abajo\n\t< - Izquierda\n\t> - Derecha\n\n-Niebla:\n\tN - Activar/Desactivar\n\tH - Reducir\n\tJ - Aumentar", "Controles", MB_OK) == IDOK)
		//				mciSendString("play CO from 0", NULL, 0, 0);
		//			else
		//				mciSendString("play CC from 0", NULL, 0, 0);
		//		}
		//		break;
		//		case ID_AYUDA_REINICIAR:
		//		{
		//			if (escena->menup == false)
		//			{
		//				mciSendString("play CS from 0", NULL, 0, 0);
		//				switch (MessageBox(hWnd, "¿Quiere reiniciar el juego?", "Reiniciar", MB_YESNO))
		//				{
		//					case IDYES:
		//					{
		//						mciSendString("play CO from 0", NULL, 0, 0);
		//						escena->reiniciar(false);
		//					}
		//					break;
		//					case IDNO:
		//					{
		//						mciSendString("play CC from 0", NULL, 0, 0);
		//					}
		//					break;
		//				}
		//			}
		//			else if (escena->menup == true)
		//				mciSendString("play CC from 0", NULL, 0, 0);
		//		}
		//		break;
		//		case ID_AYUDA_MENUPRINCIPAL:
		//		{
		//			if (escena->menup == false)
		//			{
		//				mciSendString("play CS from 0", NULL, 0, 0);
		//				switch (MessageBox(hWnd, "¿Quiere regresar al menu principal?", "Menu Principal", MB_YESNO))
		//				{
		//					case IDYES:
		//					{
		//						mciSendString("play CO from 0", NULL, 0, 0);
		//						escena->reiniciar(true);
		//					}
		//					break;
		//					case IDNO:
		//					{
		//						mciSendString("play CC from 0", NULL, 0, 0);
		//					}
		//					break;
		//				}
		//			}
		//			else if (escena->menup == true)
		//				mciSendString("play CC from 0", NULL, 0, 0);
		//		}
		//		break;
		//		case ID_AYUDA_SALIR:
		//		{
		//			mciSendString("play CS from 0", NULL, 0, 0);
		//			switch (MessageBox(hWnd, "¿Quiere salir del juego?", "Salir", MB_YESNO))
		//			{
		//				case IDYES:
		//				{
		//					mciSendString("play CO from 0", NULL, 0, 0);
		//					SendMessage(hWnd, WM_CLOSE, NULL, NULL);
		//				}
		//				break;
		//				case IDNO:
		//				{
		//					mciSendString("play CC from 0", NULL, 0, 0);
		//				}
		//				break;
		//			}
		//		}
		//		break;
		//	}
		//}
		//break;
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_RETURN:
				{
					if (escena->menup == true)
					{
						if (escena->hudtext == true)
						{
							mciSendString("play CO from 0", NULL, 0, 0);
							escena->hudtext = false;
						}
						else if (escena->cursor == 0)
						{
							mciSendString("Stop MM", NULL, 0, 0);
							mciSendString("play GS from 0", NULL, 0, 0);
							escena->menup = false;
							escena->cam = Camara::Perspectiva::TP;
							renderiza = true;
						}
						else if (escena->cursor == 1)
						{
							mciSendString("Stop MM", NULL, 0, 0);
							mciSendString("play CO from 0", NULL, 0, 0);
							escena->exit = true;							
						}
					}
					else if (escena->menus == true)
					{
						if (escena->hudtext == true)
						{
							mciSendString("play CC from 0", NULL, 0, 0);
							escena->hudtext = false;
						}
						else if (escena->cursor == 0)
						{
							mciSendString("play CO from 0", NULL, 0, 0);
							escena->hudtext = true;
						}
						else if (escena->cursor == 1)
						{
							mciSendString("play CO from 0", NULL, 0, 0);
							escena->hudtext = true;
						}
						else if (escena->cursor == 2)
						{
							mciSendString("play CO from 0", NULL, 0, 0);
							escena->reiniciar(true);
							mciSendString("stop DB", NULL, 0, 0);
							escena->cuenca_sound = true;
						}
					}
				}
				break;
				case VK_ESCAPE:
				{
					if (escena->dead == false)
					{
						if (escena->menup == false)
						{
							if (escena->menus == false)
							{
								mciSendString("play CS from 0", NULL, 0, 0);
								escena->menus = true;
							}
							else if (escena->menus == true)
							{
								mciSendString("play CC from 0", NULL, 0, 0);
								escena->menus = false;
								if (escena->hudtext == true)
									escena->hudtext = false;
							}
						}
					}
				}
				break;
				case 0x4E: //N
				{
					if (escena->menup == false && escena-> menus == false)
					{
						if (escena->nieblabool == false)
						{
							escena->nieblabool = true;
							escena->ndens = 0.0045;
						}
						else if (escena->nieblabool == true)
							escena->nieblabool = false;
						renderiza = true;
					}
				}
				break;
				case 0x48: //H
				{
					if (escena->menup == false && escena->menus == false)
					{
						escena->ndens -= 0.0001;
						renderiza = true;
					}
				}
				break;
				case 0x4A: //J
				{
					if (escena->menup == false && escena->menus == false)
					{
						escena->ndens += 0.0001;
						renderiza = true;
					}
				}
				break;
				case 0x56: //V
				{
					if (escena->menup == false && escena->menus == false)
					{
						if (escena->cam == Camara::Perspectiva::TP)
							escena->cam = Camara::Perspectiva::FP;
						else if (escena->cam == Camara::Perspectiva::FP)
							escena->cam = Camara::Perspectiva::TP;
						renderiza = true;
					}
				}
				break;
				case 0x43: //C
				{
					if (escena->menup == false && escena->menus == false)
					{
						if (escena->dead == false && escena->pickups < 3)
						{
							int aux = escena->pickups;
							escena->collect_pickups(escena->cadpos, escena->crowposX, escena->crowposY, escena->crowposZ);
							if (escena->pickups > aux)
							{
								mciSendString("play IP from 0", NULL, 0, 0);
								escena->pickuphud = true;
							}
						}
						renderiza = true;
					}
				}
				break;
				case VK_UP:
				{
					if (escena->menus == true && escena->cursor > 0)
					{
						mciSendString("play CS from 0", NULL, 0, 0);
						escena->cursor--;
					}
					if (escena->menup == true && escena->cursor > 0 && escena->exit == false)
					{
						mciSendString("play CS from 0", NULL, 0, 0);
						escena->cursor--;
					}
					else if (escena->menup == false && escena->menus == false)
					{
						if (escena->dead == false)
						{
							if (escena->VeRot < 15)
							{
								if (escena->check_colisions() == false)
								{
									escena->CamaraRotacion(Eje::X, 1);
									if (escena->check_colisions() == true)
										escena->CamaraRotacion(Eje::X, -1);
								}
							}
						}
						renderiza = true;
					}
				}
				break;
				case VK_DOWN:
				{
					if (escena->menus == true && escena->cursor < 2)
					{
						mciSendString("play CS from 0", NULL, 0, 0);
						escena->cursor++;
					}
					if (escena->menup == true && escena->cursor < 1 && escena->hudtext == false)
					{
						mciSendString("play CS from 0", NULL, 0, 0);
						escena->cursor++;
					}
					else if (escena->menup == false && escena->menus == false)
					{
						if (escena->dead == false)
						{
							if (escena->VeRot > -45)
							{
								if (escena->check_colisions() == false)
								{
									escena->CamaraRotacion(Eje::X, -1);
									if (escena->check_colisions() == true)
										escena->CamaraRotacion(Eje::X, 1);
								}
							}
						}
						renderiza = true;
					}
				}
				break;
				case VK_LEFT:
				{
					if (escena->menup == false && escena->menus == false)
					{
						if (escena->dead == false)
						{
							if (escena->check_colisions() == false)
							{
								escena->CamaraRotacion(Eje::Y, 1.5);
								if (escena->check_colisions() == true)
									escena->CamaraRotacion(Eje::Y, -1.5);
							}
						}
						renderiza = true;
					}
				}
				break;
				case VK_RIGHT:
				{
					if (escena->menup == false && escena->menus == false)
					{
						if (escena->dead == false)
						{
							if (escena->check_colisions() == false)
							{
								escena->CamaraRotacion(Eje::Y, -1.5);
								if (escena->check_colisions() == true)
									escena->CamaraRotacion(Eje::Y, 1.5);
							}
						}
						renderiza = true;
					}
				}
				break;
				case 0x57: //W
				{
					if (escena->menup == false && escena->menus == false)
					{
						if (escena->dead == false)
						{
							if (escena->check_colisions() == false)
							{
								escena->CamaraTraslacion(Camara::Direccion::F, 1.5);
								if (escena->check_colisions() == true)
									escena->CamaraTraslacion(Camara::Direccion::B, 1.5);
							}
						}
						renderiza = true;
					}
				}
				break;
				case 0x53: //S
				{
					if (escena->menup == false && escena->menus == false)
					{
						if (escena->dead == false)
						{
							if (escena->check_colisions() == false)
							{
								escena->CamaraTraslacion(Camara::Direccion::B, 0.5);
								if (escena->check_colisions() == true)
									escena->CamaraTraslacion(Camara::Direccion::F, 0.5);
							}
						}
						renderiza = true;
					}
				}
				break;
				case 0x41: //A
				{
					if (escena->menup == false && escena->menus == false)
					{
						if (escena->dead == false)
						{
							if (escena->check_colisions() == false)
							{
								escena->CamaraTraslacion(Camara::Direccion::L, 1);
								if (escena->check_colisions() == true)
									escena->CamaraTraslacion(Camara::Direccion::R, 1);
							}
						}
						renderiza = true;
					}
				}
				break;
				case 0x44: //D
				{
					if (escena->menup == false && escena->menus == false)
					{
						if (escena->dead == false)
						{
							if (escena->check_colisions() == false)
							{
								escena->CamaraTraslacion(Camara::Direccion::R, 1);
								if (escena->check_colisions() == true)
									escena->CamaraTraslacion(Camara::Direccion::L, 1);
							}
						}
						renderiza = true;
					}
				}
				break;
				case 0x51: //Q
				{
					if (escena->menup == false && escena->menus == false)
					{
						if (escena->dead == false)
						{
							if (escena->check_colisions() == false)
							{
								escena->CamaraTraslacion(Camara::Direccion::U, 1);
								if (escena->check_colisions() == true)
									escena->CamaraTraslacion(Camara::Direccion::D, 1);
							}
						}
						renderiza = true;
					}
				}
				break;
				case 0x45: //E
				{
					if (escena->menup == false && escena->menus == false)
					{
						if (escena->dead == false)
						{
							if (escena->check_colisions() == false)
							{
								escena->CamaraTraslacion(Camara::Direccion::D, 1);
								if (escena->check_colisions() == true)
									escena->CamaraTraslacion(Camara::Direccion::U, 1);
							}
						}
						renderiza = true;
					}
				}
				break;
			}
		}
		break;
		case WM_TIMER:
		{
			renderiza = true;

			if (escena->menup == true && escena->menusound == true)
			{
				mciSendString("play MM from 0 repeat", NULL, 0, 0);
				escena->menusound = false;
			}
			if (escena->exit == true && escena->endwait <= 33)
				escena->endwait++;
			else if (escena->exit == true && escena->endwait > 33)
				SendMessage(hWnd, WM_CLOSE, NULL, NULL);
			if (escena->menup == false)
			{
				if (escena->newarea == true && escena->endwait <= 165)
					escena->endwait++;
				if (escena->newarea == true && escena->endwait > 165)
				{
					mciSendString("play NA from 0", NULL, 0, 0);
					escena->newarea = false;
					escena->endwait = 0;
					escena->intro = true;
				}
				if (escena->cuenca_sound == true)
				{
					mciSendString("play DB repeat", NULL, 0, 0);
					escena->cuenca_sound = false;
				}
				if (escena->bonfire_sound == true)
				{
					mciSendString("play BF from 0", NULL, 0, 0);
					escena->bonfire_sound = false;
				}

				if (escena->projbool == true && escena->fxaux == true)
				{
					mciSendString("play HP from 0", NULL, 0, 0);
					escena->fxaux = false;
				}
				else if (escena->projbool == false && escena->hydrauxi > 0)
					mciSendString("stop HP", NULL, 0, 0);

				if (escena->splashbool == true && escena->fxaux == true)
				{
					mciSendString("play WS from 0", NULL, 0, 0);
					escena->fxaux = false;
				}

				if (escena->pickups == 3 && escena->endwait <= 33 && escena->win == false && escena->dead == false)
					escena->endwait++;
				else if (escena->pickups == 3 && escena->endwait > 33 && escena->win == false && escena->dead == false)
				{
					mciSendString("play VY from 0", NULL, 0, 0);
					//KillTimer(hWnd, Timer1);
					//MessageBox(hWnd, "THE LEGEND NEVER DIES", "VICTORY", MB_OK);
					escena->win = true;
					escena->endwait = 0;
					//SetTimer(hWnd, Timer1, 30, NULL);			
				}
				if (escena->dead == true && escena->endwait <= 33 && escena->lose == false && escena->pickups < 3)
					escena->endwait++;
				else if (escena->dead == true && escena->endwait > 33 && escena->lose == false && escena->pickups < 3)
				{
					mciSendString("play YD from 0", NULL, 0, 0);
					//KillTimer(hWnd, Timer1);
					//MessageBox(hWnd, "What r u casul?", "YOU DIED", MB_OK);
					escena->lose = true;
					escena->endwait = 0;
					//SetTimer(hWnd, Timer1, 30, NULL);
				}
				if (escena->dead == true && escena->menus == true)
					escena->menus = false;
				if (escena->lose == true && escena->hudi >= 165)
					escena->reiniciar(false);
			}
		}
		break;
		case WM_DESTROY:
		{
			KillTimer(hWnd, Timer1);
			delete escena->noche;
			delete escena->cuenca;
			delete escena->lago;
			delete escena->DS_Bridge;
			delete escena->DS_Stair;
			delete escena->DS_Hydra[0];
			delete escena->DS_Hydra[1];
			delete escena->DS_Hydra[2];
			delete escena->DS_Hydra[3]; 
			delete escena->DS_Greatsword;
			delete escena->DS_Bonfire;
			delete escena->DS_Hollow[0];
			delete escena->DS_Hollow[1];
			delete escena->DS_Hollow[2];
			delete escena->DS_Hollow[3];
			delete escena->DS_Stone;
			delete escena->Tree[0];
			delete escena->Tree[1];
			delete escena->Tree[2];
			delete escena->DS_CL[0];
			delete escena->DS_CL[1];
			delete escena->DS_CL[2];
			delete escena->DS_CG[0];
			delete escena->DS_CG[1];
			delete escena->DS_CG[2];
			for (int i = 0; i < 150; i++)
				delete escena->cesped[i];
			for (int i = 0; i < 15; i++)
				delete escena->cesped2[i];
			for (int i = 0; i < 2; i++)
				delete escena->souls[i];
			for (int i = 0; i < 10; i++)
				delete escena->bonfire[i];
			for (int i = 0; i < 6; i++)
				delete escena->projectile[i];
			for (int i = 0; i < 10; i++)
				delete escena->splash[i];
			for (int i = 0; i < 12; i++)
				delete escena->hud[i];
			wglMakeCurrent(hContextoVentana, NULL);
			wglDeleteContext(hContextoGL);

			PostQuitMessage(0);
		}
		break;
		case WM_SIZE:
		{
			alto = HIWORD(lParam);
			ancho = LOWORD(lParam);
			if (alto == 0)
				alto = 1;
			glViewport(0, 0, ancho, alto);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0f, (GLfloat)ancho / (GLfloat)alto, 1.0f, 10000.0f);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void DefPixelFormat(HDC hDC)
{
	int bestmatch;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), //tamaño de la estructura
		1, //numero de version
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //soporta la ventana, el opengl y manejara doble buffer
		PFD_TYPE_RGBA, //formato de 32 bits rgba
		32, //tamaño del color en 32 bits
		0, 0, 0, 0, 0, 0, //bits de color, no se usan
		0, //no hay buffer para el alfa
		0, //ignore el bit de corrimiento
		0, //no hay buffer de acumulacion
		0, 0, 0, 0, //no hay bits de acumulacion
		16, //tamaño del flotante para el buffer z
		0, //no hay buffers de stencil
		0, //no hay buffers auxiliares
		PFD_MAIN_PLANE, //plano principal para dibujo
		0, //reservado
		0, 0, 0 //mascaras de capas ignoradas
	};

	bestmatch = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, bestmatch, &pfd);
}
