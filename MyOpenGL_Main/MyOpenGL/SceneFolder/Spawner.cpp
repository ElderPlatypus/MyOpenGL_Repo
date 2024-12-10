#include "Spawner.h"

//Constructor
Spawner::Spawner(int spawnAmount, MeshShape shape)
{
	if (spawnAmount <= 0)
	{
		std::cerr << "[WARING]:Spawn Amount must be greater than 0 \n";
		return;
	}

	//Init vector and reserve memory
	//std::vector<std::shared_ptr<Actor>> actors;
	spawnVector.reserve(spawnAmount);

	//Spawn the vectors
	for (int i = 0; i < spawnAmount; i++)
	{
		std::shared_ptr<Actor> actor = CreateActor(shape, i);
		if (!actor) continue;

		glm::vec3 defaultPos(0.0f, 0.0f, 0.0f);
		actor->SetLocalPosition(defaultPos);
		actor->rigidB->pos = defaultPos;
		spawnVector.emplace_back(actor);
	}
}

Spawner::Spawner(int spawnAmount, float scale, MeshShape shape)
{
	if (spawnAmount <= 0)
	{
		std::cerr << "[WARING]:Spawn Amount must be greater than 0 \n";
		return;
	}

	//Init vector and reserve memory
	//std::vector<std::shared_ptr<Actor>> actors;
	spawnVector.reserve(spawnAmount);

	//Spawn the vectors
	for (int i = 0; i < spawnAmount; i++)
	{
		std::shared_ptr<Actor> actor = CreateActor(shape, i, scale);
		if (!actor) continue;

		glm::vec3 defaultPos(0.0f, 0.0f, 0.0f);
		actor->SetLocalPosition(defaultPos);
		actor->rigidB->pos = defaultPos;
		spawnVector.emplace_back(actor);
	}
}

Spawner::Spawner(int spawnAmount, float distriButionX, float distriButionZ, MeshShape shape)
{
	if (spawnAmount <= 0)
	{
		std::cerr << "[WARING]:Spawn Amount must be greater than 0 \n";
		return;
	}

	//Init vector and reserve memory
	//std::vector<std::shared_ptr<Actor>> actors;
	spawnVector.reserve(spawnAmount);

	// Random data setup
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_real_distribution<float> distrX(-distriButionX, distriButionX);
	std::uniform_real_distribution<float> distrZ(-distriButionZ, distriButionZ);

	//Spawn the vectors
	for (int i = 0; i < spawnAmount; i++)
	{
		std::shared_ptr<Actor> actor = CreateActor(shape, i);
		if (!actor) continue;

		glm::vec3 randomPosition(distrX(eng), 0.0f, distrZ(eng));
		actor->SetLocalPosition(randomPosition);
		actor->rigidB->pos = randomPosition;
		spawnVector.emplace_back(actor);
	}
}

Spawner::Spawner(int spawnAmount, float distriButionX, float distriButionZ, float scale, MeshShape shape)
{
	if (spawnAmount <= 0)
	{
		std::cerr << "[WARING]:Spawn Amount must be greater than 0 \n";
		return;
	}

	//Init vector and reserve memory
	std::vector<std::shared_ptr<Actor>> actors;
	actors.reserve(spawnAmount);

	// Random data setup
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_real_distribution<float> distrX(-distriButionX, distriButionX);
	std::uniform_real_distribution<float> distrZ(-distriButionZ, distriButionZ);

	//Spawn the vectors
	for (int i = 0; i < spawnAmount; i++)
	{
		std::shared_ptr<Actor> actor = CreateActor(shape, i, scale);
		if (!actor) continue;

		glm::vec3 randomPosition(distrX(eng), 0.0f, distrZ(eng));
		actor->SetLocalPosition(randomPosition);
		actor->rigidB->pos = randomPosition;
		actors.emplace_back(actor);
	}
}

Spawner::Spawner(const std::shared_ptr<Actor>& actor, float dt)
{
	float timer = dt;
	int index = 0;
	if (actor->isShooting)
	{
		index++;
		std::shared_ptr<Actor> projectileActor = CreateActor(MeshShape::Sphere, index);
		projectileActor->SetLocalPosition(actor->GetLocalPosition().z + actor->mExtent * 0.5f + (10.f * dt));
		projectileActor->SetLocalRotation(actor->GetLocalRotation());
		projectileList.emplace_back(projectileActor);
	}
	actor->isShooting = false;
}

Spawner::~Spawner()
{
	//Fail test to see if the method contains any actors to delete, if yes progress
	if (spawnVector.empty())
	{
		//std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
		return;
	}

	for (const auto& actor : spawnVector)
	{
		actor->mSurfaceMesh->~Mesh();
		//std::cout << "[LOG]:Actor BarySurface Unloaded Successfully \n";
	}
	//std::cout << "\n";
}

bool Spawner::IsExpired() const
{
	return elapsedTime >= maxLifeTime; 
}

//Render
void Spawner::SetShaderAll(const std::shared_ptr<Shader>& shader)
{
	for (const auto& content : spawnVector)
	{
		content->SetShader(shader);
	}
}

void Spawner::UpdateAll(float dt)
{
	elapsedTime += dt;
	for (const auto& actor : spawnVector)
	{
		actor->UpdateActors(dt);
	}
}

void Spawner::ClearSpawnvector_single(const std::shared_ptr<Actor>& actor)
{
	//Fail test to see if the method contains any actors to delete, if yes progress
	if (spawnVector.empty())
	{
		std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
		return;
	}

	//using std::find to locate the specified actor in the list
	auto it = FindActor(actor);

	//Iterating over list of actors in list
	for (auto& actor : spawnVector)
	{
		//If the actor is not the last element in the list it gets deleted
		if (it != spawnVector.end())
		{
			spawnVector.erase(it);
			std::cout << "[LOG]:Actor Deleted Successfully \n";
		}
		else
		{
			std::cerr << "[WARNING]:Actor not found in the list \n";
		}
	}
	std::cout << "\n";
}

void Spawner::ClearSpawnvector_all()
{
	//Fail test to see if the method contains any actors to delete, if yes progress
	if (spawnVector.empty())
	{
		std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
		return;
	}

	//Iterating over list of actors in list
	for (const auto& actor : spawnVector)
	{
		actor->~Actor();
	}
	spawnVector.clear();
	std::cout << "[LOG]:Actors Deleted Successfully \n";
	std::cout << "\n";
}

void Spawner::UseTexture_all(TextureType textureType)
{
	//Fail test to see if the method contains any actors to delete, if yes progress
	if (spawnVector.empty())
	{
		std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
		return;
	}

	for (const auto& actor : spawnVector)
	{
		actor->UseTexConfig(true, textureType);
		//std::cout << "[LOG]:Actor Texture Updated Successfully \n";
	}
	//std::cout << "\n";
}

void Spawner::UseTexture_single(const std::shared_ptr<Actor>& actor, TextureType textureType)
{
	//Fail test to see if the method contains any actors to delete, if yes progress
	if (spawnVector.empty())
	{
		std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
		return;
	}

	//using std::find to locate the specified actor in the list
	auto it = FindActor(actor);

	//Iterating over list of actors in list
	for (auto& actor : spawnVector)
	{
		//If the actor is not the last element in the list it gets deleted
		if (it != spawnVector.end())
		{
			it->get()->UseTexConfig(true, textureType);
			std::cout << "[LOG]:Actor Texture Updated Successfully \n";
		}
		else
		{
			std::cerr << "[WARNING]:Actor not found in the list \n";
		}
	}
}

void Spawner::UseLight_all(LightType lightType)
{
	//Fail test to see if the method contains any actors to delete, if yes progress
	if (spawnVector.empty())
	{
		std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
		return;
	}

	for (const auto& actor : spawnVector)
	{
		actor->UseLightConfig(true, lightType);
		//std::cout << "[LOG]:Actor Lighting Updated Successfully \n";
	}
	//std::cout << "\n";
}

void Spawner::UseLight_single(const std::shared_ptr<Actor>& actor, LightType lightType)
{
	//Fail test to see if the method contains any actors to delete, if yes progress
	if (spawnVector.empty())
	{
		std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
		return;
	}

	//using std::find to locate the specified actor in the list
	auto it = FindActor(actor);

	//Iterating over list of actors in list
	for (auto& actor : spawnVector)
	{
		//If the actor is not the last element in the list it gets deleted
		if (it != spawnVector.end())
		{
			it->get()->UseLightConfig(true, lightType);
			//std::cout << "[LOG]:Actor Lighting Updated Successfully \n";
		}
		else
		{
			std::cerr << "[WARNING]:Actor not found in the list \n";
		}
	}
	//std::cout << "\n";
}

//Surface attachment
void Spawner::SetBarySurfaceMesh_all(const std::shared_ptr<Mesh>& _selectSurface)
{
	//Fail test to see if the method contains any actors to delete, if yes progress
	if (spawnVector.empty())
	{
		std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
		return;
	}

	for (const auto& actor : spawnVector)
	{
		actor->SetBarySurfaceMesh(_selectSurface);
		//std::cout << "[LOG]:Actor BarySurface Updated Successfully \n";
	}
	//std::cout << "\n";
}

void Spawner::UpdateBaryCentriCoordinates_all(float dt)
{
	//Fail test to see if the method contains any actors to delete, if yes progress
	if (spawnVector.empty())
	{
		std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
		return;
	}

	for (const auto& actor : spawnVector)
	{
		MathLib::DoBarycentricCoordinatesMesh<Mesh, Mesh>(actor->getMesh(), actor->mSurfaceMesh, dt);
		//std::cout << "[LOG]:Actor BarySurface Updated Successfully \n";
	}
	//std::cout << "\n";
}

void Spawner::SetBarySurfaceMesh_single(const std::shared_ptr<Actor>& actor, const std::shared_ptr<Mesh>& _selectSurface)
{
	//Fail test to see if the method contains any actors to delete, if yes progress
	if (spawnVector.empty())
	{
		std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
		return;
	}

	//using std::find to locate the specified actor in the list
	auto it = FindActor(actor);

	//Iterating over list of actors in list
	for (auto& actor : spawnVector)
	{
		//If the actor is not the last element in the list it gets deleted
		if (it != spawnVector.end())
		{
			it->get()->SetBarySurfaceMesh(_selectSurface);
			//std::cout << "[LOG]:Actor BarySurface Updated Successfully \n";
		}
		else
		{
			std::cerr << "[WARNING]:Actor not found in the list \n";
		}
	}
}

void Spawner::UpdateBaryCentriCoordinates_single(const std::shared_ptr<Actor>& actor, float dt)
{
	// Fail test to see if the method contains any actors to delete, if yes progress
    if (spawnVector.empty())
    {
    	std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
    	return;
    }

	//using std::find to locate the specified actor in the list
	auto it = FindActor(actor);

	//Iterating over list of actors in list
	for (auto& actor : spawnVector)
	{
		//If the actor is not the last element in the list it gets deleted
		if (it != spawnVector.end())
		{
			it->get()->UpdateBarycentricCoords(dt);
			//std::cout << "[LOG]:Actor BarySurface Updated Successfully \n";
		}
		else
		{
			std::cerr << "[WARNING]:Actor not found in the list \n";
		}
	}
}


//Physics
void Spawner::EnablePhysics_all(const bool& enablePhysics)
{
	//Fail test to see if the method contains any actors to delete, if yes progress
	if (spawnVector.empty())
	{
		std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
		return;
	}

	for (const auto& actor : spawnVector)
	{
		actor->EnablePhysics(enablePhysics);
		//std::cout << "[LOG]:Actor Physics Updated Successfully \n";
	}
	//std::cout << "\n";
}

void Spawner::EnablePhysics_single(const std::shared_ptr<Actor>& actor, const bool& enablePhysics)
{
	//Fail test to see if the method contains any actors to delete, if yes progress
	if (spawnVector.empty())
	{
		std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
		return;
	}

	//using std::find to locate the specified actor in the list
	auto it = FindActor(actor);

	//Iterating over list of actors in list
	for (auto& actor : spawnVector)
	{
		//If the actor is not the last element in the list it gets deleted
		if (it != spawnVector.end())
		{
			it->get()->EnablePhysics(enablePhysics);
			//std::cout << "[LOG]:Actor Physics Updated Successfully \n";
		}
		else
		{
			std::cerr << "[WARNING]:Actor not found in the list \n";
		}
	}
	//std::cout << "\n";
}

void Spawner::SetPosition_all(const glm::vec3& position)
{
	//Fail test to see if the method contains any actors to delete, if yes progress
	if (spawnVector.empty())
	{
		std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
		return;
	}

	for (auto& actor : spawnVector)
	{
		if (!actor)
		{
			std::cerr << "[WARNING]:Invalid Actor \n";
		}
		else if (actor->rigidB)
		{
			actor->rigidB->pos = actor->rigidB->pos + position;
		}
		else
		{
			actor->SetLocalPosition(position + actor->GetLocalPosition());
		}
		//std::cout << "[LOG]:Actor Position Updated Successfully \n";
	}
	//std::cout << "\n";
}

std::shared_ptr<Actor> Spawner::CreateActor(MeshShape shape, int index, float scale)
{
	mScale = scale;
	switch (shape)
	{
	case MeshShape::Cube:
		return std::make_shared<Actor>(Mesh::CreateCube(mScale), "Cube_ " + std::to_string(index), CollisionType::AABB);
	case MeshShape::Sphere:
		return std::make_shared<Actor>(Mesh::CreateSphere(16, 16, static_cast<int>(mScale)), "Sphere_ " + std::to_string(index), CollisionType::BallBall);
	case MeshShape::Pyramid:
		return std::make_shared<Actor>(Mesh::CreatePyramid(mScale), "Pyramid_ " + std::to_string(index));
	default:
		std::cerr << "[WARNING]:No shape selected or uknown type \n";
		return nullptr;
	}
}

std::shared_ptr<Actor> Spawner::CreateActor(MeshShape shape, int index)
{
	switch (shape)
	{
	case MeshShape::Cube:
		return std::make_shared<Actor>(Mesh::CreateCube(mScale), "Cube_" + std::to_string(index), CollisionType::AABB);
	case MeshShape::Sphere:
		return std::make_shared<Actor>(Mesh::CreateSphere(16, 16, static_cast<int>(mScale)), "Sphere_" + std::to_string(index), CollisionType::BallBall);
	case MeshShape::Pyramid:
		return std::make_shared<Actor>(Mesh::CreatePyramid(mScale), "Pyramid_" + std::to_string(index));
	default:
		std::cerr << "[WARNING]:No shape selected or uknown type \n";
		return nullptr;
	}
}
