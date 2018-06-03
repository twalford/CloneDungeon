#ifndef CHARACTER_H
#define CHARACTER_H

#include <queue>
#include <list>

//#include "Control.h"
#include "GameTypes.h"
#include "Observer.h"
#include "console_gui.h"

class CharacterState : public Observable
{
	private:
		COORD2		m_sPosition;
		ENTITY		m_eStanding;
		DIRECTION	m_eDirection;
		Observer	*m_pObs;
		bool		m_bSliding;
		SHORT		m_sDepth;
		SHORT		m_sCoins;

	public:
		CharacterState(short const &x = 16, short const &y = 39);
		void SetPosition(COORD2 const& c);
		COORD2 const& GetPosition() const;

		void SetStanding(ENTITY e) { m_eStanding = e; }
		ENTITY GetStanding() { return m_eStanding; }

		void SetDirection(DIRECTION d) { m_eDirection = d; }
		DIRECTION GetDirection() {return m_eDirection; }

		void SetSliding(bool b) { m_bSliding = b; }
		bool GetSliding() { return m_bSliding; }

		SHORT GetDepth() { return m_sDepth; }
		void UpdateDepth(SHORT s) { m_sDepth -= s; }

		SHORT GetCoins() { return m_sCoins; }
		void UpdateCoins(SHORT c) { m_sCoins += c; }

		void attach(Observer *);

		void Draw(GUIBUFFER&);
};

#endif