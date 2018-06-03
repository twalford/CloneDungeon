#include "input_handler.h"
#include <windows.h>

void InputHandler::UpdateKeyboardInput()
{
	for (int i = 0; i < 4; i++)
	{
		m_keyNewState[i] = GetAsyncKeyState((unsigned char)("WASD"[i]));

		m_keys[i].bPressed = false;
		m_keys[i].bReleased = false;

		if (m_keyNewState[i] != m_keyOldState[i])
		{
			if (m_keyNewState[i] & 0x8000)
			{
				m_keys[i].bPressed = !m_keys[i].bHeld;
				m_keys[i].bHeld = true;
			}
			else
			{
				m_keys[i].bReleased = true;
				m_keys[i].bHeld = false;
			}
		}

		m_keyOldState[i] = m_keyNewState[i];
	}
}
