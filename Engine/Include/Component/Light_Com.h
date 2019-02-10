#pragma once
#include "Component_Base.h"

JEONG_BEGIN

class RenderState;
class Mesh;
class Shader;
class JEONG_DLL Light_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Light_Com* Clone() override;
	void AfterClone() override;

	void UpdateCBuffer();
	void SetLightType(LIGHT_TYPE eType);
	void SetLightRange(float Range);
	void SetLightAngle(float InAngle, float OutAngle);
	void SetLightFallOff(float FallOff);
	void SetLightAttenuation(const Vector3& Attenuation);
	void SetLightPos(const Vector3& Pos);
	void SetLightDirection(const Vector3& Dir);
	void SetLightInAngle(float InAngle);
	void SetLightOutAngle(float OutAngle);
	void SetLightColor(const Vector4& Ambient, const Vector4& Diffuse, const Vector4& Specular);
	LIGHT_TYPE GetLightType() const { return (LIGHT_TYPE)m_tLightInfo.LightType; }
	LightCBuffer GetLightInfo() const { return m_tLightInfo; }

private:
	LightCBuffer m_tLightInfo;
	RenderState* m_WireFrame;
	RenderState* m_DepthNone;
	RenderState* m_CullNone;
	Shader* m_PointShader;
	Shader* m_SpotShader;

	Mesh* m_Sphere;
	Mesh* m_Corn;

protected:
	Light_Com();
	Light_Com(const Light_Com& CopyData);
	~Light_Com();

public:
	friend class GameObject;
	friend class Scene;
};

JEONG_END