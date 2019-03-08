#include "stdafx.h"
#include "NavigationMesh.h"

JEONG_USING

NavigationMesh::NavigationMesh()
{
}

NavigationMesh::NavigationMesh(const NavigationMesh & CopyData)
	:Component_Base(CopyData)
{
}

NavigationMesh::~NavigationMesh()
{
}

bool NavigationMesh::Init()
{
	return true;
}

int NavigationMesh::Input(float DeltaTime)
{
	return 0;
}

int NavigationMesh::Update(float DeltaTime)
{
	return 0;
}

int NavigationMesh::LateUpdate(float DeltaTime)
{
	return 0;
}

void NavigationMesh::Collision(float DeltaTime)
{
}

void NavigationMesh::CollisionLateUpdate(float DeltaTime)
{
}

void NavigationMesh::Render(float DeltaTime)
{
}

NavigationMesh * NavigationMesh::Clone()
{
	return new NavigationMesh(*this);
}