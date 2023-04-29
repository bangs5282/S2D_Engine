#ifndef _CObject
#define _CObject


#include "Vec2.h"

class CObject {

private:
	Vec2 m_Pos;
	Vec2 m_Scale;

public:
	Vec2 getPos() {
		return m_Pos;
	}
	Vec2 getScale() {
		return m_Scale;
	}

	void setPos(Vec2 _vPos) {
		m_Pos = _vPos;
	}
	void setScale(Vec2 _vScale) {
		m_Scale = _vScale;
	}

	CObject();
	~CObject();

};

#endif // !_CObject