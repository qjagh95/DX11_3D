#pragma once
#include "Component_Base.h"

JEONG_BEGIN

class JEONG_DLL Animation3D_Com : public Component_Base
{
public:
	Animation3D_Com();
	Animation3D_Com(const Animation3D_Com& CopyData);
	~Animation3D_Com();
};

JEONG_END
