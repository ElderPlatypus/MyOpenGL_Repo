#include "ParticleSimulation.h"

ParticleSimulation::ParticleSimulation(int _particleNumber, MeshShape _shape, ParticleType _pTypes)
	: particleNumber(_particleNumber), shape(_shape), pt(_pTypes)
{
	spawners.reserve(_particleNumber);
	spawners.emplace_back(std::make_shared<Spawner>(particleNumber, 1.0f, shape));
	spawners.back()->EnablePhysics_all(true);
}

ParticleSimulation::~ParticleSimulation()
{
	for (const auto& spawner : spawners)
	{
		spawner->~Spawner();
	}
	spawners.clear();
}

void ParticleSimulation::Update(float dt)
{
	//Timer
	elapsedTime += dt;

	/*if (elapsedTime >= spawnInterval)
	{
		spawners.emplace_back(CreateNewSpawn());
		elapsedTime = 0.0f;
	}*/

	for (const auto& spawner : spawners)
	{
		spawner->UpdateAll(dt);
	}

	UpdateSystem(dt);
}

void ParticleSimulation::SetShader(const std::shared_ptr<Shader>& shader)
{
	for (const auto& spawner : spawners)
	{
		spawner->SetShaderAll(shader);
	}
}

//SnowSystem
void ParticleSimulation::SnowParticle(const float& _origin, const float& _area, const std::shared_ptr<Actor>& _bound, const std::unordered_map<std::string, std::shared_ptr<Actor>>& _objects)
{
	if (pt != ParticleType::Snow)
	{
		std::cout << "[WARNING]:Snow particle system must be selected\n";
		spawners.clear();
		return;
	}
	std::cout << "[LOG]:Snow particle system initiated\n";
	for (const auto& spawner : spawners)
	{
		if (spawner->spawnVector.empty())
		{
			std::cerr << "[ERROR]: Vector empty!\n";
			return;
		}
		origin = _origin;
		area = _area;
		bound = _bound;
		objects = _objects;

		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_real_distribution<float> random(-area, area);

		for (const auto& particle : spawner->spawnVector)
		{
			//Update shader config
			particle->UseTexConfig(true, TextureType::SnowFlake);

			//Update start position
			glm::vec3 startPosition = glm::vec3(glm::vec3(random(eng), origin + random(eng), random(eng)));
			particle->rigidB->SetLocalPosition(startPosition);
			particle->rigidB->velocity = glm::vec3(0.0f);
		}
	}
}

void ParticleSimulation::UpdateSnowPartices(float dt)
{
	if (pt != ParticleType::Snow)
	{
		std::cout << "[WARNING]:Snow particle system must be selectedn\n";
		return;
	}
	for (const auto& spawner : spawners)
	{
		if (spawner->spawnVector.empty())
		{
			std::cerr << "[ERROR]: Vector empty!\n";
			return;
		}
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_real_distribution<float> random(-area, area);

		for (const auto& particle : spawner->spawnVector)
		{
			//Applying dampening 
			particle->rigidB->velocity *= 0.98f;

			//Apply fallingspeed
			float fallMagnitude = 0.008f;
			float fallSpeed = random(eng) * fallMagnitude;
			particle->rigidB->velocity.y += fallSpeed;

			//Horizontal sway
			float timeFactor = static_cast<float>(glfwGetTime());
			float swayMagnitude = 0.02f;
			glm::vec3 snowSway(glm::sin(timeFactor + random(eng)) * swayMagnitude, 0.0f, glm::cos(timeFactor + random(eng)) * swayMagnitude);

			//Computing new position based on snow effect and velocity
			glm::vec3 currentPos = particle->GetLocalPosition();
			glm::vec3 newPos = currentPos + snowSway + particle->rigidB->velocity * dt;

			// Compute the new position with clamp to stay withn bounds
			newPos.x = glm::clamp(newPos.x, -area, area);
			newPos.z = glm::clamp(newPos.z, -area, area);

			particle->rigidB->SetLocalPosition(newPos);

			if (Collision::AABBInverse<Actor, Actor>(particle, bound))
			{
				//Reseting position on collision
				glm::vec3 startPosition = glm::vec3(glm::vec3(random(eng), origin, random(eng)));
				particle->rigidB->SetLocalPosition(startPosition);

				//Changed code
				particle->rigidB->velocity.y = 0;
			}
			for (const auto& [name,object] : objects)
			{
				if (object->getCollisionType() == CollisionType::AABB)
				{
					Collision::AABB<Actor, Actor>(particle, object);
				}
				else if (object->getCollisionType() == CollisionType::BallBall)
				{
					Collision::BallBall<Actor, Actor>(particle, object);
				}
			}
		}
	}
}

//SimpleWater
void ParticleSimulation::RainParticle(const float& _origin, const float& _area, const std::shared_ptr<Actor>& _bound, const std::unordered_map<std::string, std::shared_ptr<Actor>>& _object)
{
	if (pt != ParticleType::Rain)
	{
		std::cout << "[WARNING]:Rain particle system must be selectedn\n";
		spawners.clear();
		return;
	}
	std::cout << "[LOG]:Rain particle system initiated\n";
	for (const auto& spawner : spawners)
	{
		if (spawner->spawnVector.empty())
		{
			std::cerr << "[ERROR]: Vector empty!\n";
			return;
		}
		origin = _origin;
		area = _area;
		bound = _bound;
		objects = _object;

		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_real_distribution<float> random(-area, area);

		for (const auto& particle : spawner->spawnVector)
		{
			particle->SetLightColor(glm::vec3(0.4f, 0.4f, 1.0f));
			particle->SetObjectColor(glm::vec3(0.8f, 0.8f, 1.f)); 
			particle->UseLightConfig(true,LightType::Default);

			//Update start position
			glm::vec3 startPosition = glm::vec3(glm::vec3(random(eng), origin + random(eng)*3.f, random(eng)));
			particle->rigidB->SetLocalPosition(startPosition);
			//particle->rigidB->velocity = glm::vec3(0.0f);

			//Update shader config
		}
	}
}

void ParticleSimulation::UpdateRainParticles(float dt)
{
	//Flag if not correct type is set
	if (pt != ParticleType::Rain)
	{
		std::cout << "[WARNING]:Rain particle system must be selected\n";
		return;
	}
	for (const auto& spawner : spawners)
	{

		if (spawner->spawnVector.empty())
		{
			std::cerr << "[ERROR]: Vector empty!\n";
			return;
		}
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_real_distribution<float> random(-area, area);

		for (const auto& particle : spawner->spawnVector)
		{
			//Apply fallingspeed
			float fallSpeed = -1.0f;
			particle->rigidB->velocity.y += fallSpeed; 

			//Computing new position based on snow effect and velocity
			glm::vec3 currentPos = particle->GetLocalPosition();
			glm::vec3 newPos = currentPos + particle->rigidB->velocity * dt;

			// Compute the new position with clamp to stay withn bounds
			newPos.x = glm::clamp(newPos.x, -area, area);
			newPos.z = glm::clamp(newPos.z, -area, area);

			particle->rigidB->SetLocalPosition(newPos);

			if (Collision::AABBInverse<Actor, Actor>(particle, bound))
			{
				//Reseting position on collision
				particle->rigidB->velocity.y = 0.0f;
				glm::vec3 startPosition = glm::vec3(glm::vec3(random(eng), origin, random(eng)));
				particle->rigidB->SetLocalPosition(startPosition);
			}
			for (const auto& [name, object] : objects)
			{
				//Utilising collision types based on actors collison type
				if (object->getCollisionType() == CollisionType::AABB)
				{
					Collision::AABB<Actor, Actor>(particle, object);
				}
				else if (object->getCollisionType() == CollisionType::BallBall)
				{
					Collision::BallBall<Actor, Actor>(particle, object);
				}
			}
		}
	}
}


void ParticleSimulation::SetPosition(const glm::vec3& pos)
{
	for (const auto& spawner : spawners)
	{
		for (const auto& particle : spawner->spawnVector)
		{
			particle->rigidB->SetLocalPosition(pos);
		}
	}
}

glm::vec3 ParticleSimulation::GetPosition()
{
	for (const auto& spawner : spawners)
	{
		for (const auto& particle : spawner->spawnVector)
		{
			return particle->rigidB->GetLocalPosition();
		}
	}
}

std::shared_ptr<Spawner> ParticleSimulation::CreateNewSpawn()
{
	std::shared_ptr<Spawner> temp = std::make_shared<Spawner>(particleNumber, 1.0f, shape);
	temp->EnablePhysics_all(true);
	return temp;
}

void ParticleSimulation::UpdateSystem(float dt)
{
	switch (pt)
	{
	  case ParticleType::None:
	  {
		spawners.clear();
		break;
	  }
	  case ParticleType::Snow:
	  {
		UpdateSnowPartices(dt);
		break;
	  }
	  case ParticleType::Rain:
	  {
		UpdateRainParticles(dt);
		break;
	  }
	  default:
		  break;
	}
}


