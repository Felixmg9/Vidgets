#include "..\hge181\include\hge.h"
#include "..\hge181\include\hgesprite.h"
#include "..\hge181\include\hgefont.h"
#include "..\hge181\include\hgeparticle.h"
#include "..\hge181\include\hgegui.h"
#include "widget.h"
#include <string.h>
#include <string>
#include <vcclr.h>
#include <iostream>

HGE *hge=0;
FILE* widg_log=fopen(".\out.txt","wb");

hgeSprite*			fon;
WidgetContainer*	widcnt;
WidgetContainer*	widcnt2;

bool FrameFunc()
{
	return false;
}

bool RenderFunc()
{
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	fon->Render(0, 0);

	widcnt->Render();
	widcnt2->Render();

	/*btn->Render();
	pict1->Render();
	pict2->Render();
	*/
	hge->Gfx_EndScene();
	return false;
}

void MyEnterPict1(Widget* Sender) {Sender->GetWidgetContainer()->GetWidget(11)->Show(); };
void MyLeavePict1(Widget* Sender) {Sender->GetWidgetContainer()->GetWidget(11)->Hide(); };
void MyPressBtn1(Widget* Sender) {widcnt2->Inv(); };

void AddEventHandler(Widget* Sender) 
{ 
	if (widg_log==0)
		return;
	char s[10];
	itoa(Sender->GetId(),s,10);
	fputs("Виджет № ",widg_log);
	fputs(s,widg_log);
	fputs("	-обратный вызов контейнера\n",widg_log);  
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_LOGFILE, "wd.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);	
	hge->System_SetState(HGE_FPS, 100);
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_USESOUND, false);
	hge->System_SetState(HGE_SCREENWIDTH, 800);
	hge->System_SetState(HGE_SCREENHEIGHT, 600);
	hge->System_SetState(HGE_SCREENBPP, 32);
	hge->System_SetState(HGE_HIDEMOUSE, false);
	hge->System_SetState(HGE_TITLE, "Тестовое задание С++ (Виджеты)");

	if(hge->System_Initiate()) 
	{
		fon=new hgeSprite(0, 0, 0, 800, 600);
		fon->SetColor(0xFFFFFFFF);
		
		widcnt=new WidgetContainer(5);
		widcnt->Add(1,1,96,24,184,82,"Btn_up_def.bmp","Btn_dw_def.bmp");
		widcnt->Add(1,10,96,134,187,186,"Pict_def.bmp");
		widcnt->Add(2,11,396,134,187,186,"Pict_def2.bmp");
		widcnt->GetWidget(11)->Hide();
		widcnt->GetWidget(10)->OnEnterLMouseButton=MyEnterPict1;
		widcnt->GetWidget(10)->OnLeaveLMouseButton=MyLeavePict1;
		widcnt->GetWidget(10)->OnAddEventHandler=AddEventHandler;
		widcnt->GetWidget(1)->OnPressLMouseButton=MyPressBtn1;

		widcnt2=new WidgetContainer(3);
		widcnt2->Add(3,2,196,270,184,82,"Btn_up_def.bmp","Btn_dw_def.bmp");
		widcnt2->Add(2,20,196,370,187,186,"Pict_def.bmp");	
		widcnt2->Hide();
	
		hge->System_Start();

		delete fon;
		delete widcnt;
		delete widcnt2;
		/*delete btn;
		delete pict1;
		delete pict2;*/
	}
	else
		MessageBox(NULL,"","Ошибка инициализации", 0);
	hge->System_Shutdown();
	hge->Release();
	fclose(widg_log);
	return 0;
}
