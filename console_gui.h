#ifndef CONSOLE_GUI_H
#define CONSOLE_GUI_H

#include "TArray.h"
#include "Callback.h"
#include "Observer.h"
#include "cd_game.h"

#if defined(_WIN32)

typedef TArray2D<CHAR_INFO> GUIBUFFER;

// Generic Win32 GUI element contains a buffer to store character data, a region
// variable and can be attached to a functor which does the work of translating
// the observed object into the view data
class GUIPanel : public Observer
{

	public:
		typedef BFunctor1<GUIBUFFER&, void> *FPCALLBACK;

		GUIPanel(HANDLE h, SHORT top, SHORT left, SHORT bottom, SHORT right);
		GUIPanel(HANDLE h, COORD2 pos, SHORT rows, SHORT cols);
		~GUIPanel();

		void SetCallback(FPCALLBACK fpFunc);

		// Inherited from Observer interface
		void notify();


	private:
		HANDLE		m_hOut;
		SMALL_RECT	m_srRegion;
		GUIBUFFER	m_ciBuffer;
		FPCALLBACK	m_fpCallback;

		void Draw();

};
#endif // _WIN32

#endif // GUIPANEL_H