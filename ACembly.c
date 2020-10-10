#include<windows.h>
#include<commctrl.h>
#include<Wincon.h>
#include<Richedit.h>
#include<string.h>
#include<stdio.h>

#define ABRIR 1
#define EXIT 2
#define FONTE 3
#define NOVO 4
#define SALVAR 5
#define COMPILAR 6
#define EXECULTAR 7

//funções 
void caixa_dialogo(HWND,int);

LRESULT CALLBACK ACembly(HWND,UINT,WPARAM,LPARAM);

int WINAPI WinMain(HINSTANCE Principal, HINSTANCE Principal2, LPSTR lpCmdLine, int modo){
	HBRUSH color = (HBRUSH) CreateSolidBrush(RGB(0,255,0));
	HWND WinCode;
	MSG message;
	WNDCLASSEX Win_Code;
	Win_Code.cbSize = sizeof(Win_Code);
	Win_Code.cbClsExtra = 0;
	Win_Code.cbWndExtra = 0;
	Win_Code.hInstance = Principal;
	Win_Code.hbrBackground = (HBRUSH) color;
	Win_Code.hIcon = LoadImage(Principal,"Imagens/CA.ico",IMAGE_ICON,0,0,LR_LOADFROMFILE);
	Win_Code.hIconSm = LoadImage(Principal,"Imagens/CA.ico",IMAGE_ICON,0,0,LR_LOADFROMFILE);
	Win_Code.hCursor = LoadCursor(NULL, IDC_ARROW);
	Win_Code.lpfnWndProc = ACembly;
	Win_Code.lpszMenuName = NULL;
	Win_Code.lpszClassName = "Area_Code";
	Win_Code.style = CS_DBLCLKS;
	if(!RegisterClassEx(&Win_Code)){
		MessageBox(NULL,"Não foi possivel fazer o registro da classe \"Win_Code\"","Erro ao Registrar!",0x10);
		return 0;
	}
	WinCode = CreateWindowEx(0,"Area_Code","ACembly 1.1",WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX,225,50,1079,720,NULL,NULL,Principal,NULL);
	ShowWindow(WinCode,modo);
	while(GetMessage(&message,NULL,0,0)){
		TranslateMessage(&message);
		DispatchMessage(&message); 
	} 
	return message.wParam;
}

HINSTANCE RichEdit,groupBox,nov,salv,compi,exec,abri,loc;
HWND novo,salvar,compilar,execultar,barra,rich,abrir,localiza;

char diretorio[100] = "\0";

LRESULT CALLBACK ACembly(HWND handle, UINT comando, WPARAM wpa, LPARAM lpa){
	switch(comando){
		case WM_CREATE:{
			//Carrega imagens e armazena em uma variavel do tipo Bitmap
			HBITMAP novos = (HBITMAP) LoadImage(NULL,"Imagens/folha.bmp",IMAGE_BITMAP,25,25,LR_LOADFROMFILE);
			HBITMAP salva = (HBITMAP) LoadImage(NULL,"Imagens/save.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
			HBITMAP comp = (HBITMAP) LoadImage(NULL,"Imagens/compilar.bmp",IMAGE_BITMAP,25,25,LR_LOADFROMFILE);
			HBITMAP exe = (HBITMAP) LoadImage(NULL,"Imagens/execute.bmp",IMAGE_BITMAP,25,25,LR_LOADFROMFILE);
			HBITMAP abr = (HBITMAP) LoadImage(NULL,"Imagens/Abrir.bmp",IMAGE_BITMAP,25,25,LR_LOADFROMFILE);
			HBITMAP loca = (HBITMAP) LoadImage(NULL,"Imagens/local.bmp",IMAGE_BITMAP,25,25,LR_LOADFROMFILE);
			//Ocultar o cmd
			ShowWindow(GetConsoleWindow(),0); 
			//Criando o rich edit 
			LoadLibrary("DLL/msftedit.dll");
			rich = CreateWindowEx(0,"RICHEDIT50W","",WS_CHILD|WS_BORDER|WS_VISIBLE|ES_MULTILINE|WS_VSCROLL|WS_HSCROLL,5,50,1062,615,handle,NULL,RichEdit,NULL);
			//Criando o menu
			HMENU menu = CreateMenu(); 
			HMENU ferramentas = CreateMenu();
			HMENU arquivos = CreateMenu(); 
			AppendMenu(menu,MF_POPUP,(UINT_PTR) arquivos,"Arquivos");
			AppendMenu(arquivos,MF_STRING,(UINT_PTR) ABRIR, "Abrir");
			AppendMenu(arquivos,MF_SEPARATOR,(UINT_PTR) NULL,NULL);
			AppendMenu(arquivos,MF_STRING,(UINT_PTR) EXIT, "Exit");
			AppendMenu(menu,MF_POPUP, (UINT_PTR) ferramentas,"Ferramentas");
			AppendMenu(ferramentas,MF_STRING, (UINT_PTR) FONTE, "Fonte");
			SetMenu(handle,menu);
			SetMenu(handle,arquivos);
			SetMenu(handle,ferramentas); 
			//GroupBox
			barra = CreateWindow(WC_STATIC,"",WS_CHILD|WS_VISIBLE,5,5,1062,40,handle,NULL,groupBox,NULL);
			//Criação dos botões
			novo = CreateWindowEx(0,"BUTTON",NULL, BS_BITMAP|WS_CHILD|WS_VISIBLE,10,10,30,30,handle,(HMENU) NOVO,nov,NULL);
			SendMessage(novo, BM_SETIMAGE,IMAGE_BITMAP, (LPARAM) novos);
			abrir = CreateWindowEx(0,"BUTTON",NULL, BS_BITMAP|WS_CHILD|WS_VISIBLE,45,10,30,30,handle,(HMENU) ABRIR,abri,NULL);
			SendMessage(abrir, BM_SETIMAGE,IMAGE_BITMAP, (LPARAM) abr);
			salvar = CreateWindowEx(0,"BUTTON",NULL, BS_BITMAP|WS_CHILD|WS_VISIBLE,80,10,30,30,handle,(HMENU) SALVAR,salv,NULL);
			SendMessage(salvar, BM_SETIMAGE,IMAGE_BITMAP, (LPARAM) salva);
			compilar = CreateWindowEx(0,"BUTTON",NULL, BS_BITMAP|WS_CHILD|WS_VISIBLE,115,10,30,30,handle,(HMENU) COMPILAR,compi,NULL);
			SendMessage(compilar, BM_SETIMAGE,IMAGE_BITMAP, (LPARAM) comp);
			execultar = CreateWindowEx(0,"BUTTON",NULL, BS_BITMAP|WS_CHILD|WS_VISIBLE,150,10,30,30,handle,(HMENU) EXECULTAR,exec,NULL);
			SendMessage(execultar, BM_SETIMAGE,IMAGE_BITMAP, (LPARAM) exe);
			localiza = CreateWindowEx(0,"BUTTON",NULL, BS_BITMAP|WS_CHILD|WS_VISIBLE,185,10,30,30,handle,(HMENU) 20,loc,NULL);
			SendMessage(localiza, BM_SETIMAGE,IMAGE_BITMAP, (LPARAM) loca);
			break;
		}
		case WM_COMMAND:{
			switch(wpa){
				case ABRIR:{
					caixa_dialogo(handle,1); 
					FILE *arq;
					if((arq=fopen(diretorio,"r"))== NULL){
						MessageBox(NULL,"Não foi possivel Abrir o arquivo","OPS...",0x10);
					}
					fseek(arq,0,SEEK_END);
					int tamanho = ftell(arq);
					rewind(arq);
					char espaco[tamanho];
					strcpy(espaco," ");
					fread(espaco,tamanho,1,arq);
					espaco[tamanho] ='\0';
					SetWindowText(rich,espaco);
					fclose(arq);
					break;
				}
				case SALVAR:{
					caixa_dialogo(handle,2);
					FILE *arq;
					if((arq=fopen(diretorio,"w")) == NULL){
						MessageBox(NULL,"Não foi possivel Abrir o arquivo","OPS...",0x10);
					}
					DWORD lol=0;
					GETTEXTLENGTHEX tamanho;
					tamanho.codepage = 1200;
					tamanho.flags = GTL_DEFAULT;
					lol = SendMessage(rich,EM_GETTEXTLENGTHEX,(WPARAM) &tamanho,0);
					char teste[lol];
					GETTEXTEX obter;
					obter.cb = lol;
					obter.flags = GT_DEFAULT;
					obter.codepage = CP_ACP;
					obter.lpDefaultChar = NULL;
					obter.lpUsedDefChar = NULL;
					SendMessage(rich,EM_GETTEXTEX,(WPARAM)&obter,(LPARAM)&teste);
					teste[lol] = '\0';
					fwrite(teste,lol,1,arq);
					fclose(arq);
					break;
				}
				case NOVO:{
					SetWindowText(rich,"");
					SetFocus(rich); 
					break;
				}
				case EXIT:{
					exit(0);
					break;
				}
				case COMPILAR:{
					char compilacao[300] = {"c:\\\"Program Files\"/NASM/nasm -f elf64 "};
					strcat(compilacao,diretorio);					
					system(compilacao);			
					strcpy(compilacao,"ld c:\\\"Program Files\"/NASM/nasm -o ");	
					break;
				}
				case 20:{
					/*
					*/
					int vo,cont,teste;
					teste = SendMessage(rich,EM_GETLINECOUNT,0,0);
					char loc[1000];
					loc[0] = (char) 1000;
					for(cont=0;cont<teste;cont++){
						SendMessage(rich,EM_GETLINE,(WPARAM) cont,(LPARAM)loc);
						MessageBox(NULL,loc,"LOL",0x40);
						strstr(loc,"porra");
					}
					break;
				}
			}
			break;
		}
		case WM_CTLCOLORSTATIC:{
			return (UINT_PTR) CreateSolidBrush(RGB(211,211,211));
			break;
		}
		case WM_DESTROY:{
			PostQuitMessage(0); 
			break;
		}
		default:{
			DefWindowProc(handle,comando,wpa,lpa);
			break;
		}
	}
}

void caixa_dialogo(HWND handle, int flag){ 
	OPENFILENAME ab;
	ZeroMemory(&ab,sizeof(OPENFILENAME));				
	ab.lStructSize = sizeof(OPENFILENAME);
	ab.hwndOwner = handle;
	ab.lpstrFilter = "All files\0*.*\0Assembly Code\0*.asm\0Text File\0*.TXT";
	ab.nFilterIndex = 1;
	ab.lpstrFile = diretorio;
	ab.nMaxFile = 100;
	ab.lpstrTitle = "ACembly";
	flag == 1? GetOpenFileName(&ab):GetSaveFileName(&ab);
}
