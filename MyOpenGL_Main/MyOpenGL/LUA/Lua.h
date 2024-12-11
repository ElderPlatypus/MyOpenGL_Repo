//Glm include
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <algorithm>

// Link to lua library
#include "../Dependency/includes/LUA/lua.hpp"

//Full inclusion
#include "../Utility/FileHandler.h"
#include "../ECS/Components.h"

//c++
#include <memory>
#include <unordered_map>

class LuaLib
{
	lua_State* L;

	//Components
	std::shared_ptr<SceneGraphComponent> sceneGraphComponent;
	std::shared_ptr<TransformComponent> transformComponent;
	std::shared_ptr<ComponentArchive<SceneGraphComponent>> sceneGraphManager;
public:
	LuaLib(const std::shared_ptr<SceneGraphComponent>& _sceneGraphComponent, const std::shared_ptr<TransformComponent>& _transformComponent, const std::shared_ptr<ComponentArchive<SceneGraphComponent>>& _sceneGraphManager)
		   : sceneGraphComponent(_sceneGraphComponent), transformComponent(_transformComponent), sceneGraphManager(_sceneGraphManager)
	{
		//Initialise lua
		L = luaL_newstate();
		luaL_openlibs(L);
		BindLuaFunc(); 
	}

	~LuaLib()
	{
		//close lua
		lua_close(L);
	}

	void Read(const std::string& filePath)
	{
		if (luaL_dofile(L,filePath.c_str()))
		{
			// Flag for lua error
			std::cerr << "[ERROR]: Failed to execute Lua file '" << 
			filePath << "': " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);  // Pop error message from the stack
		}
	}
private:
	void BindLuaFunc()
	{
		//Pushing class instance as light userdata
		lua_pushlightuserdata(L, this);

		//pushing hte closer with one upvalue
		lua_pushcclosure(L, Lua_uSpawnMap, 1);
		
		//Setting closure as global value in Lua
		lua_setglobal(L, "uSpawnMap");
	}

	static int Lua_uSpawnMap(lua_State* L)
	{
		//Retrieve instance
		LuaLib* lua_lib = (LuaLib*)(lua_touserdata(L, lua_upvalueindex(1)));

		if (!lua_lib) 
		{
			luaL_error(L, "[ERROR]: LuaLib instance is null.");
			return 0;
		}

		//Read data from Lua-script
		std::string name = lua_tostring(L, 1);
		int numSpawn = (int)lua_tointeger(L, 2);
		float disrtibMin = (float)lua_tointeger(L, 3);
		float disrtibMax = (float)lua_tointeger(L, 4);
		MeshShape type = (MeshShape)lua_tointeger(L,5);

		//Create new spaner and confidure it
		auto& spawnerMap = lua_lib->sceneGraphComponent->uSpawnerMap;

		spawnerMap[name] = std::make_shared<Spawner>(numSpawn, disrtibMin, disrtibMax, type);
		spawnerMap[name]->UseLight_all(LightType::Slope);
		spawnerMap[name]->SetPosition_all(glm::vec3(10.0f, -40.0f, 0.0f)); 
		spawnerMap[name]->EnablePhysics_all(true);

		//Push return value to lua
		lua_pushstring(L, "Spawner created successfully");
		return 1; //NUmber of returned value
	}
};