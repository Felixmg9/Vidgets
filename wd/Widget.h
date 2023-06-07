// Widget.h

/* Тестовое задание для AWEM-Studio
   Выполнял Киселёв Ф.Ф.  18.10.2014
   использование hgeGUI 
*/

#pragma once

//using namespace std;

#include "..\hge181\include\hge.h"
#include "..\hge181\include\hgegui.h"
#include "..\hge181\include\hgesprite.h"

#include <string>
#include <vcclr.h>
#include <cliext/vector>
#include <iostream>

class Widget;
class WidgetContainer;

typedef void (*TNotifyEvent)(Widget* Sender);   // шаблон метода класса Widget без аргументов

public class Widget : public hgeGUIObject
{ 
protected:	
	hgeSprite* spr;
	hgeSprite* spr2;	
	int id;         // идентификатор 
	int order;		// порядoк отрисовки
public:
	WidgetContainer* container;
	Widget(int id, float x, float y, float w, float h, const char* pict, const char* pict2=0);
	~Widget() {delete spr; if (spr2!=NULL) delete spr2; };
	int GetId(void) { return id; };
	void SetId(int id) { Widget::id=id;}; // установить строковый идентификатор 
	int GetOrder(void) { return order;};
	void SetOrder(int order) {Widget::order=order;}; 
				// для изменения порядка отрисовки виджетов на экране (аппаратный z-order не используется)
	void SetPos(float x, float y) {rect.x1=x; rect.y1=y;}; // положение виджета
	void SetSize(float w, float h) {rect.x2=rect.x1+w; rect.y2=rect.y1+h;}; // размер виджета. Предполагается что все виджеты – прямоугольники
	//virtual void Update(float);  // обновление состояния виджета   - не является обязательным
	void Show() { bVisible=true; }; // показать виджет
	void Hide() { bVisible=false; }; // скрыть виджет
	virtual void Render(void);     // отрисовка виджета
	WidgetContainer* GetWidgetContainer(void);
	TNotifyEvent OnPressLMouseButton; // нажатие левой кнопки мыши
    TNotifyEvent OnReleaseLMouseButton; // отпускание левой кнопки мыши
	TNotifyEvent OnEnterLMouseButton; // попадание мыши в виджет
    TNotifyEvent OnLeaveLMouseButton; // выход мыши за пределы виджета
	TNotifyEvent OnAddEventHandler; // регистрация фунций обратного вызова для различных событий виджета. 
};

Widget::Widget(int id, float x, float y, float w, float h, const char* pict, const char* pict2)
{ 
	::hgeGUIObject;
	Widget::id=id;
	SetPos(x,y);
	SetSize(w,h);
	HTEXTURE tex=hge->Texture_Load(pict);
	if (!tex)  // если не загружается, то по умолчанию
	{
		MessageBox(NULL,"","Ошибка загрузки ресурса. Ресурс будет вызван по умолчанию", 0);
		tex=hge->Texture_Load("Btn_up_def.bmp");
		SetSize(184,82);
	}
	spr=new hgeSprite(tex,0,0,rect.x2-rect.x1,rect.y2-rect.y1);
	spr->SetColor(0xFFFFFFFF);	
	spr2=NULL;
	if (pict2)    //  для кнопки  (нажатое состояние)
	{
		tex=hge->Texture_Load(pict2);
		if (!tex)
		{
			tex=hge->Texture_Load("Btn_dw_def.bmp");		
			MessageBox(NULL,"","Ошибка загрузки ресурса. Ресурс будет вызван по умолчанию", 0);
		}
		spr2=new hgeSprite(tex,0,0,rect.x2-rect.x1,rect.y2-rect.y1);
	}
	OnPressLMouseButton=NULL;    // инициализация обработчиков
	OnReleaseLMouseButton=NULL;
	OnEnterLMouseButton=NULL;
	OnLeaveLMouseButton=NULL;	
	OnAddEventHandler=NULL; 
}

void Widget::Render(void)	   // отрисовка виджета
{	
	float x,y;
	hge->Input_GetMousePos(&x,&y);
	if (rect.TestPoint(x,y))   // указатель мыши на виджете
	{
		if (hge->Input_KeyDown(HGEK_LBUTTON) & (OnPressLMouseButton!=NULL)) 
			((TNotifyEvent)OnPressLMouseButton)(this);			
		if (hge->Input_KeyUp(HGEK_LBUTTON) & (OnReleaseLMouseButton!=NULL))
			((TNotifyEvent)OnReleaseLMouseButton)(this);
		if (OnEnterLMouseButton!=NULL)
			((TNotifyEvent)OnEnterLMouseButton)(this);
	}
	else
		if (OnLeaveLMouseButton!=NULL)
			((TNotifyEvent)OnLeaveLMouseButton)(this);	
	if (bVisible)   
		if (hge->Input_GetKeyState(HGEK_LBUTTON) & (spr2!=NULL) & rect.TestPoint(x,y))   // другой спрайт нажатой кнопки
			spr2->Render(rect.x1, rect.y1);      
		else
			spr->Render(rect.x1, rect.y1);       // непосредственная отрисовка виждета  
};

WidgetContainer* Widget::GetWidgetContainer(void)
{	if (OnAddEventHandler!=NULL)
		((TNotifyEvent)OnAddEventHandler)(this);
	return(container); 
};

public class WidgetContainer
{ 
protected:
	Widget (**widgets); 
	int maxsize;
	int size;	
	boolean bVisible;
public:	   	
	WidgetContainer(int s=10) {maxsize=s; size=0; widgets=new (Widget(*[s])); bVisible=true; }; 
	~WidgetContainer() { for (int i=0;i<size;i++) delete widgets[i];    delete widgets; };
	int GetMaxsize(void) { return maxsize; };
	int GetSize(void) { return size; };
	void Add(int ord, int id, float x, float y, float w, float h, const char* pict, const char* pict2=0);
	Widget* GetWidget(int id); 
	void Render(void);
	void Show() { bVisible=true; }; 
	void Hide() { bVisible=false; };
	void Inv() { bVisible=!bVisible; };
};

void WidgetContainer::Add(int ord, int id, float x, float y, float w, float h, const char* pict, const char* pict2)
{
	if ((size<maxsize-1) && (ord>0))
	{
		widgets[size]=new Widget(id, x, y, w, h, pict, pict2);
		widgets[size]->SetOrder(ord);
		widgets[size++]->container=this;
	}
}

Widget* WidgetContainer::GetWidget(int id) 
{	for (int i=0;i<size;i++)	
		if (widgets[i]->GetId()==id)
			return widgets[i]; 
	return NULL; 
}; 

void WidgetContainer::Render(void)
{ 
	if (!bVisible)
		return;
	int min_order=1;
	int r=0;
	do
	{
		for (int i=0;i<size;i++)
		if (widgets[i]->GetOrder()==min_order)
		{
			widgets[i]->Render();
			r++;	
		};
		++min_order;
	}
	while (r<size);
}

