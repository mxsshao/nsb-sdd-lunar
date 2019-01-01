#pragma once

#include "global.h"
#include <vector>

class CStateBase;

class CStateManager
{
protected:
	CStateManager()
	{
		event_queue = NULL;
		thread = NULL;
		cond = NULL;
	};

private:
	vector<CStateBase*> states;
	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* event_queue;
	
	Controls::Canvas* canvas;
	Skin::TexturedBase* defaultSkin;
	Skin::TexturedBase* menuSkin;

	ALLEGRO_MOUSE_CURSOR* cursor;
	ALLEGRO_MOUSE_CURSOR* hand;
	ALLEGRO_MOUSE_CURSOR* text;

	ALLEGRO_MUTEX* mutex;
	ALLEGRO_COND* cond;
	bool ready;
	ALLEGRO_THREAD* thread;
	static void* CStateManager::Thread(ALLEGRO_THREAD *thread, void *arg);

	float buttonHeight;
	float buttonWidth;

	bool done;

public:
	static CStateManager MStateManager;
	void Init(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, Controls::Canvas* canvas, Skin::TexturedBase* defaultSkin, Skin::TexturedBase* menuSkin, ALLEGRO_MOUSE_CURSOR* cursor, ALLEGRO_MOUSE_CURSOR* hand, ALLEGRO_MOUSE_CURSOR* text);
	void Resize();
	void Cleanup();

	void ChangeState(CStateBase* state);
	void PushState(CStateBase* state);
	void PopState();

	void HandleEvents(ALLEGRO_EVENT& ev);
	void Render();

	ALLEGRO_DISPLAY* GetDisplay() {return display;};
	ALLEGRO_EVENT_QUEUE* GetEventQueue() {return event_queue;};
	Controls::Canvas* GetCanvas() {return canvas;};
	Skin::TexturedBase* GetDefaultSkin() {return defaultSkin;};
	Skin::TexturedBase* GetMenuSkin() {return menuSkin;};

	ALLEGRO_MOUSE_CURSOR* GetCursor() {return cursor;};
	ALLEGRO_MOUSE_CURSOR* GetHand() {return hand;};
	ALLEGRO_MOUSE_CURSOR* GetText() {return text;};

	ALLEGRO_MUTEX* GetMutex() {return mutex;};
	ALLEGRO_COND* GetCond() {return cond;};
	bool GetReady() {return ready;};
	void SetReady(bool r) {ready = r;};
	ALLEGRO_THREAD* GetThread() {return thread;};
	static void* GetThreadProcedure() {return Thread;};

	float GetButtonHeight() {return buttonHeight;};
	float GetButtonWidth() {return buttonWidth;};

	void Exit() {done = true;};
	bool XRunning() {return done;};
};