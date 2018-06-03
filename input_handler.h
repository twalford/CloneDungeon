#include <cstring>

class InputHandler
{
	protected:

		struct sKeyState
		{
			bool bPressed;
			bool bReleased;
			bool bHeld;
		} m_keys[4];

		short m_keyOldState[4] = { 0 };
		short m_keyNewState[4] = { 0 };

	public:
		InputHandler()
		{
			memset(m_keyNewState, 0, 4 * sizeof(short));
		}

		void UpdateKeyboardInput();
		sKeyState GetKey(int nKeyID) { return m_keys[nKeyID]; }

		~InputHandler()
		{}
		
};