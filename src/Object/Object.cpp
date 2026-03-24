#include "Object.h"
#include "../GameLibrary/Mathematics/Matrix.h"
#include "Enemy/Enemy.h"
#include <GL/glut.h>

Object::Object()
{

}
	
Object::~Object()
{
	vertices.clear();
	area.clear();
	enemy.clear();
	enemy2.clear();
	enemy3.clear();
	camera.clear();
	shading.clear();
	wall.clear();
}


void Object::Initialize()
{
	switch (collision.type)
	{
	case ObjectType::ground:
		if (!this_help)
		{
			CreateFBX("Resource/fbx/ground2.fbx");
		}
		else
		{
			CreateFBX("Resource/fbx/help_ground.fbx");
		}

		break;
	case ObjectType::player:
		CreateFBX("Resource/fbx/player2.fbx");
		break;
	case ObjectType::enemy:
		CreateFBX("Resource/fbx/enemy.fbx");
		break;
	case ObjectType::goal:
		CreateFBX("Resource/fbx/goal.fbx");
		break;
	default:
		break;
	}
}

void Object::Update(double delta_second)
{

}
	
void Object::Draw()
{
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
	glPushMatrix();
	glTranslated(location.x, location.y, location.z);
	glRotated(dir, 0.0, 0.0, 1.0);
	DrawMeshRecursive(node);
	glPopMatrix();
}

void Object::Finalize()
{

}

void Object::SetLocation(const GameLib::Vector3& location)
{
	this->location = location;
	}

void Object::SetSize(const GameLib::Vector3& size)
{
	this->size = size;
}

void Object::SetVelocity(const GameLib::Vector3& velo)
{
	this->velocity = velo;
}

GameLib::Vector3 Object::GetLocation()
{
	return location;
}

GameLib::Vector3 Object::GetSize()
{
	return size;
}

GameLib::Vector3 Object::GetVelocity()
{
	return velocity;
}

const Collision& Object::GetCollision() const
{
	return collision;
}

int Object::GetHP() const
{
	return HP;
}

void Object::DecHP()
{
	HP--;
	ChangeState(State::damage);
}

void Object::DrawShadow(float groundZ)
{
	if(groundZ + DRAWPOS_Z_SHIFT >= location.z - size.z && collision.type == ObjectType::wepon)
	{
		DecHP();
	}
	else
	{
		glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-1.0f, -1.0f);
		glDepthMask(GL_FALSE);

		glPushMatrix();
		glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
		glTranslated(location.x, location.y, groundZ + DRAWPOS_Z_SHIFT);
		glRotated(dir, 0, 0, 1);
		glScalef(1.0f, 1.0f, 0.0f);
		if (collision.type == ObjectType::wepon) glutSolidSphere(size.x, DIVISION_NUM, DIVISION_NUM);
		else DrawMeshRecursive(node);
		glPopMatrix();

		glDepthMask(GL_TRUE);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
	}
}

bool Object::RayIntersectTriangle(const GameLib::Vector3& rayOrigin, const GameLib::Vector3& rayDir, const Vertex& tri, float& outT)
{ 
	const float EPSILON = 0.000001f;

	//三角形の辺を作成
	GameLib::Vector3 edge1 = tri.v1 - tri.v0;
	GameLib::Vector3 edge2 = tri.v2 - tri.v0;	

	//レイが三角形と並行か
	GameLib::Vector3 h = GameLib::Vector3::Cross(rayDir, edge2);
	float a = GameLib::Vector3::Dot(edge1, h);

	// レイと三角形が平行
	if (a > -EPSILON && a < EPSILON)
		return false;

	float f = 1.0f / a;
	GameLib::Vector3 s = rayOrigin - tri.v0;
	float u = f * GameLib::Vector3::Dot(s, h);

	// 三角形の外
	if (u < 0.0f || u > 1.0f)
		return false;

	GameLib::Vector3 q = GameLib::Vector3::Cross(s, edge1);
	float v = f * GameLib::Vector3::Dot(rayDir, q);

	// 三角形の外
	if (v < 0.0f || u + v > 1.0f)
		return false;

	float t = f * GameLib::Vector3::Dot(edge2, q);

	if (t > EPSILON)
	{
		outT = t;
		return true; 
	}

	return false;
}

float Object::FindGroundZFromFBX(std::vector<Vertex> triangles)
{
	float nearestT = FLT_MAX;
	GameLib::Vector3 rayOrigin(
		location.x,
		location.y,
		location.z + size.z
	);
	float hitZ = rayOrigin.z - nearestT;

	for (auto& tri : triangles)
	{
		float t;
		if (RayIntersectTriangle(rayOrigin, GameLib::Vector3(0, 0, -1), tri, t))
		{
			if (t < nearestT)
			{
				nearestT = t;
				hitZ = rayOrigin.z - t;
			}
		}
	}
	return hitZ;
}

void Object::DrawMesh()
{
	if (collision.type == ObjectType::ground && !this_help)
	{

		glEnable(GL_BLEND);
		glDisable(GL_COLOR_MATERIAL);  // 必須

		GLfloat matDiffuse[] = { 0.2f, 0.8f, 1.0f, 0.2f };
		GLfloat matAmbient[] = { 0.1f, 0.1f, 0.1f, 0.1f };
		GLfloat matSpecular[] = { 0.1f, 0.1f, 0.1f, 0.1f };
		GLfloat matEmission[] = { 0.0f, 0.0f, 0.0f, 0.1f };

		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0f);

		glDepthMask(GL_FALSE);

		glBegin(GL_TRIANGLES);
		for (const Vertex& tri : shading)
		{

			glNormal3f(tri.normal.x, tri.normal.y, tri.normal.z);
			glVertex3f(tri.v0.x, tri.v0.y, tri.v0.z);
			glVertex3f(tri.v1.x, tri.v1.y, tri.v1.z);
			glVertex3f(tri.v2.x, tri.v2.y, tri.v2.z);
		}
		glEnd();

		matDiffuse[0] = 0.0f;
		matDiffuse[1] = 1.0f;
		matDiffuse[2] = 0.5f;
		matDiffuse[3] = 0.1f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
		glBegin(GL_TRIANGLES);
		for (const Vertex& tri : wall)
		{

			glNormal3f(tri.normal.x, tri.normal.y, tri.normal.z);
			glVertex3f(tri.v0.x, tri.v0.y, tri.v0.z);
			glVertex3f(tri.v1.x, tri.v1.y, tri.v1.z);
			glVertex3f(tri.v2.x, tri.v2.y, tri.v2.z);
		}
		glEnd();

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
	else
	{
		glDisable(GL_BLEND);
		glBegin(GL_TRIANGLES);
		for (const Vertex& tri : shading)
		{

			glNormal3f(tri.normal.x, tri.normal.y, tri.normal.z);
			glVertex3f(tri.v0.x, tri.v0.y, tri.v0.z);
			glVertex3f(tri.v1.x, tri.v1.y, tri.v1.z);
			glVertex3f(tri.v2.x, tri.v2.y, tri.v2.z);
		}
		if(collision.type == ObjectType::goal)
		{
			float another_color[4] = { 0.7f,0.7f,0.7f,1.0f };
			glMaterialfv(GL_FRONT, GL_DIFFUSE, another_color);
			for (const Vertex& tri : vertices)
			{

				glNormal3f(tri.normal.x, tri.normal.y, tri.normal.z);
				glVertex3f(tri.v0.x, tri.v0.y, tri.v0.z);
				glVertex3f(tri.v1.x, tri.v1.y, tri.v1.z);
				glVertex3f(tri.v2.x, tri.v2.y, tri.v2.z);
			}
		}
		glEnd();
	}
}

void Object::DrawMeshRecursive(FbxNode* node)
{
	if (!node) return;


	FbxMesh* mesh = node->GetMesh();
	if (mesh)
	{
		DrawMesh();
	}

	int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; i++)
		DrawMeshRecursive(node->GetChild(i));

}

void Object::SetVertices(FbxNode* node, std::vector<Vertex>& out)
{
	FbxMesh* mesh = node->GetMesh();
	if (!mesh) return;

	FbxVector4* ctrlPoints = mesh->GetControlPoints();
	FbxAMatrix transformMatrix = node->EvaluateGlobalTransform();

	// 法線変換行列
	FbxAMatrix normalMatrix = transformMatrix;
	normalMatrix.SetT(FbxVector4(0, 0, 0));
	normalMatrix = normalMatrix.Inverse().Transpose();

	for (int i = 0; i < mesh->GetPolygonCount(); i++)
	{
		GameLib::Vector3 v[3]; GameLib::Vector3 n[3];
		for (int j = 0; j < 3; j++)
		{
			int idx = mesh->GetPolygonVertex(i, j); 
			
			FbxVector4 p = ctrlPoints[idx]; 
			FbxVector4 worldPos = transformMatrix.MultT(p); 
			
			v[j] = GameLib::Vector3((float)worldPos[0], (float)worldPos[2], (float)worldPos[1]); 
			FbxVector4 normal;
			if (mesh->GetPolygonVertexNormal(i, j, normal))
			{
				FbxVector4 worldNormal = normalMatrix.MultT(normal); 
				worldNormal.Normalize(); 
				n[j] = GameLib::Vector3((float)worldNormal[0], (float)worldNormal[2], (float)worldNormal[1]);
			}
		} 
		GameLib::Vector3 triNormal; 
		if (n[0].LengthSquared() > 0 && n[1].LengthSquared() > 0 && n[2].LengthSquared() > 0)
		{
			triNormal = GameLib::Vector3::Cross(v[1] - v[0], v[2] - v[0]); triNormal.Normalize();
		}
		else
		{
			triNormal = GameLib::Vector3::Cross(v[1] - v[0], v[2] - v[0]); triNormal.Normalize();
		}
		if (triNormal.z < 0) triNormal = -triNormal; 
		out.push_back({ v[0], v[1], v[2], triNormal });
	}
}

void Object::SetVerticesShading(FbxNode* node, std::vector<Vertex>& out)
{
	FbxMesh* mesh = node->GetMesh();
	if (!mesh) return;

	FbxVector4* ctrlPoints = mesh->GetControlPoints();
	FbxAMatrix transformMatrix = node->EvaluateGlobalTransform();

	// 法線変換行列
	FbxAMatrix normalMatrix = transformMatrix.Inverse();
	normalMatrix = normalMatrix.Transpose();

	for (int i = 0; i < mesh->GetPolygonCount(); i++)
	{
		GameLib::Vector3 v[3];
		GameLib::Vector3 n[3];

		for (int j = 0; j < 3; j++)
		{
			int idx = mesh->GetPolygonVertex(i, j);
			FbxVector4 p = ctrlPoints[idx];
			FbxVector4 worldPos = transformMatrix.MultT(p);
			v[j] = GameLib::Vector3((float)worldPos[0], (float)worldPos[2], (float)worldPos[1]);

			FbxVector4 normal;
			if (mesh->GetPolygonVertexNormal(i, j, normal))
			{
				FbxVector4 worldNormal = normalMatrix.MultT(normal);
				worldNormal.Normalize();
				n[j] = GameLib::Vector3((float)worldNormal[0], (float)worldNormal[2], (float)worldNormal[1]);
			}
		}

		GameLib::Vector3 triNormal;
		if (n[0].LengthSquared() > 0 && n[1].LengthSquared() > 0 && n[2].LengthSquared() > 0)
		{
			triNormal = (n[0] + n[1] + n[2]) / 3.0f;
			triNormal.Normalize();
		}
		else
		{
			triNormal = GameLib::Vector3::Cross(v[1] - v[0], v[2] - v[0]);
			triNormal.Normalize();
		}

		out.push_back({ v[0], v[1], v[2], triNormal });
	}
}

void Object::CreateFBX(const char* filename) {
	FbxManager* manager = FbxManager::Create(); 
	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT); 
	manager->SetIOSettings(ios); 
	FbxImporter* importer = FbxImporter::Create(manager, ""); 
	
	if (!importer->Initialize(filename, -1, manager->GetIOSettings())) 
	{ 
		printf("Failed to initialize importer: %s\n", importer->GetStatus().GetErrorString()); 
		return;
	} 
	
	scene = FbxScene::Create(manager, "myScene"); 
	importer->Import(scene); 

	FbxGeometryConverter converter(scene->GetFbxManager()); 
	converter.Triangulate(scene, true); importer->Destroy(); 
	
	scene->FillAnimStackNameArray(AnimStackNameArray); 
	node = scene->GetRootNode(); 
	
	// Triangulate後に再取得 
	DrawMeshRecursive(node); 
	
	vertices.clear(); 
	area.clear(); 
	enemy.clear(); 
	enemy2.clear();
	enemy3.clear();
	shading.clear(); 
	wall.clear(); 
	SetVerticesRecursive(node); 
}

std::vector<Vertex> Object::GetVertices()
{
	return vertices;
}

std::vector<Vertex> Object::GetEnemyArea(int num)
{
	switch (num)
	{
	case 1:
		return enemy;
		break;
	case 2:
		return enemy2;
		break;
	case 3:
		return enemy3;
		break;
	}
}

void Object::SetVerticesRecursive(FbxNode* node)
{
	if (!node) return;

	FbxMesh* mesh = node->GetMesh();
	if (mesh)
	{
		std::string nodeName = node->GetName();

		// 一時バッファに頂点を格納
		std::vector<Vertex> temp;
		std::vector<Vertex> buf;
		SetVertices(node, temp);
		SetVerticesShading(node, buf);

		// 名前で振り分ける
		if (nodeName == "area")
		{
			area.insert(area.end(), temp.begin(), temp.end());
		}
		else if (nodeName == "enemy1")
		{
			enemy.insert(enemy.end(), temp.begin(), temp.end());
		}
		else if (nodeName == "enemy2")
		{
			enemy2.insert(enemy2.end(), temp.begin(), temp.end());
		}
		else if (nodeName == "enemy3")
		{
			enemy3.insert(enemy3.end(), temp.begin(), temp.end());
		}
		else if (nodeName == "camera")
		{
			camera.insert(camera.end(), temp.begin(), temp.end());
		}
		else if (nodeName == "wall1")
		{
			wall.insert(wall.end(), buf.begin(), buf.end());
			vertices.insert(vertices.end(), buf.begin(), buf.end());
		}
		else if (nodeName == "appearance")
		{
			shading.insert(shading.end(), buf.begin(), buf.end());
		}
		else
		{
			vertices.insert(vertices.end(), temp.begin(), temp.end());
			if (collision.type != ObjectType::goal)
			{
				shading.insert(shading.end(), buf.begin(), buf.end());
			}
		}
	}

	// 再帰的に子ノードを処理
	int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; i++)
		SetVerticesRecursive(node->GetChild(i));
}

State Object::GetState() const
{
	return state;
}

void Object::ChangeState(State st)
{
	state = st;
}