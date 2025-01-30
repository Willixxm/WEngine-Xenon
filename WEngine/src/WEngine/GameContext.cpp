#include "GameContext.h"
#include "Game.h"
#include "WMath.h"
#include "Entity.h"

#include <memory>
#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>

#include <SDL.h>
#include <SDL_events.h>
#include <SDL_joystick.h>
#include <box2d.h>
#include <glad/glad.h>
#include <glm.hpp>
#include "stb_image.h"



namespace WE
{
	struct RenderGLEntity2D
	{
		Entity* entity;
		std::string	 filePath;
		unsigned int tex;
		unsigned int vao;
	};

	struct TextureInfo
	{
		bool isValid = false;
		std::string filepath = "";
		unsigned int pixelWidth = 0;
		unsigned int pixelHeight = 0;
		unsigned int horizontalTileCount = 1;
		unsigned int verticalTileCount = 1;
		unsigned int totalTiles = 1;

		unsigned int tex = 0;
		std::vector<Entity*> users;
	};

	struct SurfaceRectParams
	{
		void SetParams(int hCount, int vCount, int tWidth, int tHeight)
		{
			if (hCount < 1)
				hCount = 1;
			if (vCount < 1)
				vCount = 1;


			hTileCount = hCount;
			vTileCount = vCount;
			texWidth = tWidth;
			texHeight = tHeight;

			tileWidth = texWidth / hTileCount;
			tileHeight = texHeight / vTileCount;

			nTiles = hTileCount * vTileCount;

			CalcRect();
		}

		void CalcRect()
		{
			while (animationProgress > nTiles)
			{
				animationProgress -= nTiles;
			}
			
			int x = (int)animationProgress % hTileCount;
			int y = (int)animationProgress / hTileCount;
			

			surfaceRect.x = x * tileWidth;
			surfaceRect.y = y * tileHeight;
			surfaceRect.w = tileWidth;
			surfaceRect.h = tileHeight;

		}

		void AddAnimationTime(float deltaTime)
		{
			animationProgress += deltaTime * animationFps;

			CalcRect();
		}

		void SetAnimationState(float state0to1)
		{
			if (state0to1 < 0)
				state0to1 = 0;
			if (state0to1 > 1)
				state0to1 = 1;

			animationProgress = state0to1 * (nTiles) -0.5f;

			CalcRect();
		}

		int texWidth = 0;
		int texHeight = 0;
		int hTileCount = 1;
		int vTileCount = 1;
		int tileWidth = 0;
		int tileHeight = 0;
		int nTiles = 1;

		float animationProgress = 0.f;
		int currentTile = 0;
		float animationFps = 10.f;

		SDL_Rect surfaceRect;

	};

	struct RenderSDLEntity2D
	{
		Entity* entity;
		std::string filepath;
		SDL_Texture* texture = nullptr;

		SurfaceRectParams params;

		int entityWidth = 100;
		int entityHeight = 100;

		bool autoAnimate = true;
		float manualAnimationState = 0.5f;
		int renderLayer = 0;
	};

	struct SurfaceInfo
	{
		bool isValid = false;
		std::string filepath;
		
		SurfaceRectParams params;

		SDL_Texture* texture = nullptr;
		std::vector<Entity*> users;
	};

	

	class GameContext::SDLWindow
	{
	public:
		SDLWindow(std::string wName, unsigned int wWidth, unsigned int wHeight) : windowName { wName }, windowWidth{ wWidth }, windowHeight{ wHeight }
		{
			// INIT SDL
			SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
			/*
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
			*/
			SDL_JoystickEventState(true);
			
			OpenJoyStick();


			window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
			assert(window, "failed to create window");

			renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


			if (renderTarget == nullptr)
				std::cout << "failed to create renderTarget" << std::endl;


			SDL_SetRenderDrawColor(renderTarget, 0, 0, 0, 0xFF);

		}

		~SDLWindow()
		{
			SDL_DestroyWindow(window);
			window = nullptr;

			SDL_DestroyRenderer(renderTarget);
			renderTarget = nullptr;

			SDL_JoystickClose(playerJoyStick);
			playerJoyStick = nullptr;
			

			SDL_Quit();
		}

		void UpdateEvents()
		{
			userInputs.mouseInputVector = WVec2(0);
			for (float value : userInputs.joyAxis)
				value = 0.f;


			while (SDL_PollEvent(&windowEvent))
			{
				switch (windowEvent.type)
				{	
					case SDL_QUIT:
						stopGame = true;
						break;

						
					case SDL_KEYUP:
					case SDL_KEYDOWN:
						switch (windowEvent.key.keysym.sym)
						{
						case SDLK_w:
							userInputs.keys[INPUT_KeyCode::Key_W] = windowEvent.key.state; 
							break;
						case SDLK_a:
							userInputs.keys[INPUT_KeyCode::Key_A] = windowEvent.key.state;
							break;
						case SDLK_s:
							userInputs.keys[INPUT_KeyCode::Key_S] = windowEvent.key.state;
							break;
						case SDLK_d:
							userInputs.keys[INPUT_KeyCode::Key_D] = windowEvent.key.state;
							break;
						}
						break;


					case SDL_MOUSEBUTTONDOWN:
					case SDL_MOUSEBUTTONUP:
						switch (windowEvent.button.button)
						{
						case 1:
							userInputs.keys[INPUT_KeyCode::Key_MouseLeft] = windowEvent.button.state;
							break;
						case 2:
							userInputs.keys[INPUT_KeyCode::Key_MouseRight] = windowEvent.button.state;
							break;
						}
						break;

					
					case SDL_MOUSEMOTION:
						userInputs.mouseInputVector.x += windowEvent.motion.xrel;
						userInputs.mouseInputVector.y += windowEvent.motion.yrel;
						break;


					case SDL_JOYAXISMOTION:
						if (windowEvent.jaxis.which < 12)
							userInputs.joyAxis[windowEvent.jaxis.axis] = (float)windowEvent.jaxis.value / maxStickValue;
						else
							std::cerr << "axis index exceeded\n";
						break;

					case SDL_JOYBUTTONDOWN:
					case SDL_JOYBUTTONUP:
						if (windowEvent.jbutton.button < 24)
							userInputs.joyButtons[windowEvent.jbutton.button] = windowEvent.jbutton.state;
						else
							std::cerr << "button index exceeded\n";
						break;

					case SDL_JOYDEVICEADDED:
						OpenJoyStick();
						break;
					case SDL_JOYDEVICEREMOVED:
						if (windowEvent.jdevice.which == 0)
							SDL_JoystickClose(playerJoyStick);
						playerJoyStick = nullptr;
						std::cout << "Controller disconnected\n";
						OpenJoyStick();
						break;

				} 
			}
		}

		void OpenJoyStick()
		{
			if (SDL_NumJoysticks() && playerJoyStick == nullptr)
			{
				playerJoyStick = SDL_JoystickOpen(0);
				std::cout << SDL_JoystickNameForIndex(0) << " connected\n";
			}
		}
		UserInputs* GetUserInputs()
		{
			return &userInputs;
		}


		void RenderFrame(float deltaTime)
		{
			/*
			glClearColor(0.f, 0.f, 0.f, 0.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (size_t i = 0; i < renderGLEntities.size(); ++i)
			{
				if (renderGLEntities[i].entity == nullptr)
				{
					RemoveFromGLRender(nullptr); //removes all invalid entities from renderObjects3D

					if (i >= renderGLEntities.size())
						break; //exit loop if current count exceeds new vector size (after invalid entities removal)
				}

				/*
				glm::mat4 model = renderEntities2D[i].entity->GetGlobalTransform();

				renderObjects3D[i].shader->use();

				renderObjects3D[i].shader->setMat4("model", model);
				renderObjects3D[i].shader->setMat4("view", view);
				renderObjects3D[i].shader->setMat4("projection", projection);
				renderObjects3D[i].shader->set3Float("lightPosition", lightPosition.x, lightPosition.y, lightPosition.z);
				*/

				//glBindVertexArray(renderGLEntities[i].vao);
				//glBindTexture(GL_TEXTURE_2D, renderObjects3D[i].tex);

				//draw elements here (create vbo for square on contructor)
				/*
				glBindVertexArray(0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			SDL_GL_SwapWindow(window);*/

			


			SDL_RenderClear(renderTarget);

			
			for (size_t i = 0; i < renderSDLEntities.size(); ++i)
			{
				if (renderSDLEntities[i].entity == nullptr)
				{
					std::cout << "NULL ENTITY \n";
					continue;
				}
				
				
				if (renderSDLEntities[i].autoAnimate)
				{
					renderSDLEntities[i].params.AddAnimationTime(deltaTime);
				}
				else
				{
					renderSDLEntities[i].params.SetAnimationState(renderSDLEntities[i].manualAnimationState);
				}
				

				WVec2 location = renderSDLEntities[i].entity->GetLocation();

				SDL_Rect EntityPosition;
				EntityPosition.w = (renderSDLEntities[i].entityWidth) * 20;
				EntityPosition.h = (renderSDLEntities[i].entityHeight) * 20;
				EntityPosition.x = (windowWidth / 2) + (location.x * 20) - EntityPosition.w/2;
				EntityPosition.y = (windowHeight / 2) - (location.y * 20) - EntityPosition.h / 2;
				

				SDL_RenderCopy(renderTarget, renderSDLEntities[i].texture, &renderSDLEntities[i].params.surfaceRect, &EntityPosition);

			}

			SDL_RenderPresent(renderTarget);
			
		}


		void AddToGLRender(Entity* entity, std::string textureFilePath)
		{
			if (textureFilePath == "")
				textureFilePath = BMP_PLACEHOLDER;

			//return if entity is already in render list
			for (size_t i = 0; i < renderGLEntities.size(); ++i)
				if (renderGLEntities[i].entity == entity)
					return;

			TextureInfo texture;

			while (true)
			{
				texture = GetTextureInfo(textureFilePath);

				if (texture.isValid)
					break;

				if (textureFilePath == BMP_PLACEHOLDER)
				{
					//placeholder texture not loading
					std::cout << "ERROR: Couldn't load base placeholder texture! \n";
					return;
				}
				textureFilePath = BMP_PLACEHOLDER;
			}

			AddTextureUser(textureFilePath, entity);


			RenderGLEntity2D newObj;
			newObj.entity = entity;
			newObj.filePath = textureFilePath;
			newObj.tex = texture.tex;


			//gen and bind vao
			glGenVertexArrays(1, &newObj.vao);
			glBindVertexArray(newObj.vao);

			glBindTexture(GL_TEXTURE_2D, texture.tex);


			glBindVertexArray(0);

			renderGLEntities.push_back(newObj);
		}
		void RemoveFromGLRender(Entity* entity)
		{
			//check if entity is in render list
			for (size_t i = 0; i < renderGLEntities.size(); ++i)
			{
				if (renderGLEntities[i].entity != entity)
					continue;

				if (renderGLEntities[i].entity == nullptr)
					std::cerr << "ERROR: Invalid entity in renderObjcets3D \n";

				RemoveTextureUser(renderGLEntities[i].filePath, entity);

				glDeleteVertexArrays(1, &renderGLEntities[i].vao);

				renderGLEntities.erase(renderGLEntities.begin() + i);

				--i;
			}
		}

		void AddToSDLRender(Entity* entity, std::string filePath, int hTiles, int vTiles, float width, float height, int renderLayer)
		{
			if (filePath == "")
				filePath = BMP_PLACEHOLDER;

			//return if entity is already in render list
			for (size_t i = 0; i < renderSDLEntities.size(); ++i)
				if (renderSDLEntities[i].entity == entity)
					return;

			SurfaceInfo surface;

			while (true)
			{
				surface = GetSurfaceInfo(filePath, hTiles, vTiles);

				if (surface.isValid)
					break;

				if (filePath == BMP_PLACEHOLDER)
				{
					//placeholder texture not loading
					std::cout << "ERROR: Couldn't load base placeholder texture! \n";
					return;
				}
				filePath = BMP_PLACEHOLDER;
			}

			AddTextureUser(filePath, entity);


			RenderSDLEntity2D newEntity;
			newEntity.entity = entity;
			newEntity.filepath = filePath;
			newEntity.texture = surface.texture;
			newEntity.params = surface.params;	
			newEntity.renderLayer = renderLayer;
			newEntity.entityWidth = width;
			newEntity.entityHeight = height;
			
			
			renderSDLEntities.push_back(newEntity);
			OrderEntitiesByLayer();
		}
		void RemoveFromSDLRender(Entity* entity)
		{
			//check if entity is in render list
			for (size_t i = 0; i < renderSDLEntities.size(); ++i)
			{
				if (renderSDLEntities[i].entity != entity)
					continue;

				if (renderSDLEntities[i].entity == nullptr)
					std::cerr << "ERROR: Invalid entity in renderObjcets3D \n";

				RemoveSurfaceUser(renderSDLEntities[i].filepath, renderSDLEntities[i].entity);

				renderSDLEntities.erase(renderSDLEntities.begin() + i);

				--i;
			}

			OrderEntitiesByLayer();
		}

		void SetAnimationParameters(Entity* entity, bool autoAnimate, float animationFps)
		{
			//check if entity is in render list
			for (size_t i = 0; i < renderSDLEntities.size(); ++i)
			{
				if (renderSDLEntities[i].entity != entity)
					continue;

				renderSDLEntities[i].autoAnimate = autoAnimate;
				renderSDLEntities[i].params.animationFps = animationFps;

			}
		}
		void SetAnimationStateValue(Entity* entity, float state_0to1)
		{
			for (size_t i = 0; i < renderSDLEntities.size(); ++i)
			{
				if (renderSDLEntities[i].entity == entity)
					renderSDLEntities[i].manualAnimationState = state_0to1;
			}
		}

	private:
		void AddTextureUser(const std::string& filepath, Entity* entity)
		{
			for (size_t i = 0; i < LoadedTextures.size(); ++i)
				if (filepath == LoadedTextures[i].filepath) //if texture is loaded
				{
					//add entity to user list if it isn't already
					for (size_t j = 0; j < LoadedTextures[i].users.size(); ++j)
						if (LoadedTextures[i].users[j] == entity) return;

					LoadedTextures[i].users.push_back(entity);
				}
		}
		void RemoveTextureUser(const std::string& filepath, Entity* entity)
		{
			for (size_t i = 0; i < LoadedTextures.size(); ++i)
			{
				if (filepath != LoadedTextures[i].filepath) 
					continue;

				//removes entity from users or invalid entities if entity == nullptr  
				for (size_t j = 0; j < LoadedTextures[i].users.size(); ++j)
				{
					if (LoadedTextures[i].users[j] == entity) 
					{
						LoadedTextures[i].users.erase(LoadedTextures[i].users.begin() + j);
						--j;
						continue;
					}
				}

				UnloadUnusedTextures(LoadedTextures[i].filepath);
			}
		}
		TextureInfo GetTextureInfo(const std::string& filepath)
		{
			//If texture is already parsed and loaded, return respective ID;
			for (size_t i = 0; i < LoadedTextures.size(); ++i)
				if (filepath == LoadedTextures[i].filepath)
					return LoadedTextures[i];

			//Texture with this filepath not yet loaded:
			TextureInfo LoadedTexture;
			LoadedTexture.filepath = filepath;

			if (LoadFileIntoTexture(filepath, LoadedTexture))
			{
				LoadedTexture.isValid = true;
				LoadedTextures.push_back(LoadedTexture);
			}
			return LoadedTexture;
		}
		bool LoadFileIntoTexture(const std::string& filepath, TextureInfo LoadedTexture)
		{
			// load and generate the texture
			int width, height, nrChannels;
			stbi_set_flip_vertically_on_load(true);
			unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
			if (!data)
			{
				std::cout << LOG_TEXTURE_FAILED << "[" << filepath << "]\n";
				return false;
			}
			
			std::cout << LOG_TEXTURE_LOADED << "[" << filepath << "]\n";

			//unbind any vao
			glBindVertexArray(0);

			glGenTextures(1, &LoadedTexture.tex);
			glBindTexture(GL_TEXTURE_2D, LoadedTexture.tex);

			// set the texture wrapping/filtering options (on the currently bound texture object)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);

			stbi_image_free(data);

			return true;
		}
		void UnloadUnusedTextures(const std::string& filePath)
		{
			for (size_t i = 0; i < LoadedTextures.size(); ++i)
			{
				if (filePath != "" && filePath != LoadedTextures[i].filepath)
					continue;

				if (LoadedTextures[i].users.size() != 0)
					continue;

				if (LoadedTextures[i].tex)
				{
					glDeleteTextures(1, &LoadedTextures[i].tex);
					LoadedTextures[i].tex = 0;

					std::cout << LOG_TEXTURE_UNLOAD << "[" << LoadedTextures[i].filepath << "]\n";
				}

			}
		}

		void AddSurfaceUser(const std::string& filepath, Entity* entity)
		{
			for (size_t i = 0; i < LoadedSurfaces.size(); ++i)
				if (filepath == LoadedSurfaces[i].filepath) //if surface is loaded
				{
					//add entity to user list if it isn't already
					for (size_t j = 0; j < LoadedSurfaces[i].users.size(); ++j)
						if (LoadedSurfaces[i].users[j] == entity) return;

					LoadedSurfaces[i].users.push_back(entity);
				}
		}
		void RemoveSurfaceUser(const std::string& filepath, Entity* entity)
		{
			for (size_t i = 0; i < LoadedSurfaces.size(); ++i)
			{
				if (filepath != LoadedSurfaces[i].filepath)
					continue;

				//removes entity from users or invalid entities if entity == nullptr  
				for (size_t j = 0; j < LoadedSurfaces[i].users.size(); ++j)
				{
					if (LoadedSurfaces[i].users[j] == entity)
					{
						LoadedSurfaces[i].users.erase(LoadedSurfaces[i].users.begin() + j);
						--j;
						continue;
					}
				}

				UnloadUnusedSurfaces(LoadedSurfaces[i].filepath);
			}
		}
		SurfaceInfo GetSurfaceInfo(const std::string& filepath, int hTiles, int vTiles)
		{
			//If texture is already parsed and loaded, return respective ID;
			for (size_t i = 0; i < LoadedSurfaces.size(); ++i)
				if (filepath == LoadedSurfaces[i].filepath)
					return LoadedSurfaces[i];

			//Texture with this filepath not yet loaded:
			SurfaceInfo LoadedSurface;
			LoadedSurface.filepath = filepath;
			

			if (LoadFileIntoSurface(filepath, LoadedSurface))
			{
				LoadedSurface.isValid = true;


				int tWidth;
				int tHeight;

				SDL_QueryTexture(LoadedSurface.texture, NULL, NULL, &tWidth, &tHeight);
				LoadedSurface.params.SetParams(hTiles, vTiles, tWidth, tHeight);

				LoadedSurfaces.push_back(LoadedSurface);
			}
			return LoadedSurface;
		}
		bool LoadFileIntoSurface(const std::string& filepath, SurfaceInfo& LoadedSurface)
		{
			SDL_Surface* surface = SDL_LoadBMP(filepath.c_str());
			if (surface == NULL)
			{
				std::cout << LOG_SURFACE_FAILED << "[" << filepath.c_str() << "]\n";

				SDL_FreeSurface(surface);
				return false;
			}
			else
			{
				SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));

				LoadedSurface.texture = SDL_CreateTextureFromSurface(renderTarget, surface);

				SDL_FreeSurface(surface);

				if (LoadedSurface.texture == NULL)
					return false;
				
				return true;
			}
		}
		void UnloadUnusedSurfaces(const std::string& filePath)
		{
			for (size_t i = 0; i < LoadedSurfaces.size(); ++i)
			{
				if (filePath != "" && filePath != LoadedSurfaces[i].filepath)
					continue;

				if (LoadedSurfaces[i].users.size() != 0)
					continue;

				if (LoadedSurfaces[i].texture != NULL)
				{
					SDL_DestroyTexture(LoadedSurfaces[i].texture);

					std::cout << LOG_SURFACE_UNLOAD << "[" << LoadedSurfaces[i].filepath << "]\n";
				}

			}
		}

		void OrderEntitiesByLayer()
		{
			std::sort(renderSDLEntities.begin(), renderSDLEntities.end(),
				[](const RenderSDLEntity2D& a, const RenderSDLEntity2D& b)
				{
					return a.renderLayer < b.renderLayer;
				});
		}

		

	private:
		std::vector<RenderGLEntity2D> renderGLEntities;
		std::vector<TextureInfo> LoadedTextures;

		std::vector<RenderSDLEntity2D> renderSDLEntities;
		std::vector<SurfaceInfo> LoadedSurfaces;


		const std::string BMP_PLACEHOLDER = "graphics/bblogo.bmp";

		const std::string LOG_TEXTURE_LOADED = "[TEXTURE] GPU++   loaded ----> ";
		const std::string LOG_TEXTURE_UNLOAD = "[TEXTURE] GPU--   unloaded --> ";
		const std::string LOG_TEXTURE_FAILED = "[TEXTURE] FAILED  load ------> ";

		const std::string LOG_SURFACE_LOADED = "[SURFACE]         loaded ----> ";
		const std::string LOG_SURFACE_UNLOAD = "[SURFACE]         unloaded --> ";
		const std::string LOG_SURFACE_FAILED = "[SURFACE] FAILED  load ------> ";


		SDL_Window* window = nullptr;
		SDL_Renderer* renderTarget = nullptr;
		std::string windowName;
		unsigned int windowWidth;
		unsigned int windowHeight;

		SDL_Event windowEvent = SDL_Event();
		UserInputs userInputs;

		SDL_Joystick* playerJoyStick;
		short int maxStickValue = 32767;

	public:
		bool stopGame = false;

	};

	class GameContext::Box2D
	{
	public:
		Box2D()
		{
			CreateWorld();
		}

		~Box2D()
		{
			b2DestroyWorld(worldId);
		}

		void UpdatePhysicsWorld(float deltaTime)
		{
			b2World_Step(worldId, deltaTime, 4);
		}

		b2BodyId GetB2IdFromWId(WPhysBodyId WId)
		{
			b2BodyId b2Id;
			b2Id.index1 = WId.physBodyId;
			b2Id.world0 = WId.physWorldId;
			b2Id.generation = WId.physGeneration;

			return b2Id;
		}

		WVec2 GetObjPos(WPhysBodyId id)
		{
			b2Vec2 position = b2Body_GetPosition(GetB2IdFromWId(id));

			return WVec2(position.x, position.y);
		}

		void SetObjPos(WPhysBodyId id, WVec2 pos)
		{
			b2BodyId b2Id = GetB2IdFromWId(id);
			b2Vec2 position;
			position.x = pos.x;
			position.y = pos.y;
			b2Rot rotation = b2Body_GetRotation(b2Id);

			b2Body_SetTransform(b2Id, position, rotation);
		}

		void SetObjLinearVelocity(WPhysBodyId id, WVec2 vel)
		{
			b2Body_SetLinearVelocity(GetB2IdFromWId(id), { vel.x, vel.y });
		}

		void CreateWorld()
		{
			b2WorldDef worldDef = b2DefaultWorldDef();
			worldDef.gravity = { 0.f, 0.f };
			worldId = b2CreateWorld(&worldDef);
		}

		void SetWorldGravity(WVec2 vec)
		{
			b2Vec2 gravity;
			gravity.x = vec.x;
			gravity.y = vec.y;

			b2World_SetGravity(worldId, gravity);
		}

		WPhysBodyId CreateObj(WVec2 pos, WBodyType bodyType, WVec2 boxSize)
		{
			b2BodyDef bodyDef = b2DefaultBodyDef();
			bodyDef.type = (b2BodyType)bodyType;
			bodyDef.position = { pos.x, pos.y};
			bodyDef.fixedRotation = true;
			b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

			if (boxSize.x || boxSize.y)
			{
				b2Polygon bodyBox = b2MakeBox(boxSize.x / 2, boxSize.y / 2);
				b2ShapeDef boxShapeDef = b2DefaultShapeDef();
				boxShapeDef.density = 1.f;
				//boxShapeDef.friction = 0.3f;

				b2CreatePolygonShape(bodyId, &boxShapeDef, &bodyBox);
			}

			return WPhysBodyId(bodyId.index1, bodyId.world0, bodyId.generation);
		}

		void DestroyObj(WPhysBodyId id)
		{
			b2DestroyBody(GetB2IdFromWId(id));
		}


	private:
		b2WorldId worldId;


	};


	GameContext::GameContext(Game* game) : game{ game }, windowPImpl { std::make_unique<SDLWindow>(game->gameName, game->windowWidth, game->windowHeight) }, physicsPImpl{ std::make_unique<Box2D>() }
	{
		
	}

	GameContext::~GameContext()
	{

	}

	void GameContext::SYSTEM_UpdatePhysics(float deltaTime)
	{
		physicsPImpl->UpdatePhysicsWorld(deltaTime);
		for (size_t i = 0; i < instancedEntities.size(); ++i)
		{
			if(instancedEntities[i]->bodyId.isValid)
				instancedEntities[i]->SetLocation(physicsPImpl->GetObjPos(instancedEntities[i]->bodyId));
		}
	}
	void GameContext::SYSTEM_UpdateEventSystem()
	{
		windowPImpl->UpdateEvents();
		if (windowPImpl->stopGame)
			game->stopGame = true;
			
	}
	void GameContext::SYSTEM_UpdateEntities(float deltaTime)
	{
		for (size_t i = 0; i < instancedEntities.size(); ++i)
		{
			instancedEntities[i]->Update(deltaTime);
		}
	}
	void GameContext::SYSTEM_Render(float deltaTime)
	{
		windowPImpl->RenderFrame(deltaTime);
	}

	bool GameContext::INPUT_GetKeyDown(INPUT_KeyCode keyCode)
	{
		return windowPImpl->GetUserInputs()->keys[keyCode];
	}
	WVec2 GameContext::INPUT_GetMouseInput()
	{
		return windowPImpl->GetUserInputs()->mouseInputVector;
	}
	float GameContext::INPUT_GetJoyAxisValue(INPUT_JoyAxisCode axisCode)
	{
		return windowPImpl->GetUserInputs()->joyAxis[axisCode];
	}
	bool GameContext::INPUT_GetJoyButtonDown(INPUT_JoyButtonCode buttonCode)
	{
		return windowPImpl->GetUserInputs()->joyButtons[buttonCode];
	}

	void GameContext::PHYS_AddPhysComponentToEntity(Entity* entity, WBodyType bodyType, WVec2 size)
	{
		if (!entity->bodyId.isValid)
			entity->bodyId = physicsPImpl->CreateObj(entity->GetLocation(), bodyType, size);
	}

	WVec2 GameContext::PHYS_GetLocationOfPhysObj(WPhysBodyId id)
	{
		return physicsPImpl->GetObjPos(id);
	}
	void GameContext::PHYS_SetLinearVelocityOnPhysObj(WPhysBodyId id, WVec2 vel)
	{
		physicsPImpl->SetObjLinearVelocity(id, vel);
	}
	void GameContext::PHYS_SetLocationOnPhysObj(WPhysBodyId id, WVec2 pos)
	{
		physicsPImpl->SetObjPos(id, pos);
	}
	void GameContext::PHYS_SetWorldGravity(WVec2 vec)
	{
		physicsPImpl->SetWorldGravity(vec);
	}


	void GameContext::RENDER_AddRenderComponent(Entity* entity, std::string filePath, int hTiles, int vTiles, float width, float height, WRenderType renderType, int layer)
	{
		if (renderType == WRenderType::Render_Surface)
			windowPImpl->AddToSDLRender(entity, filePath, hTiles, vTiles, width, height, layer);
		else
			windowPImpl->AddToGLRender(entity, filePath);
	}
	void GameContext::RENDER_RemoveRenderComponent(Entity* entity, WRenderType renderType)
	{
		if (renderType == WRenderType::Render_Surface)
			windowPImpl->RemoveFromSDLRender(entity);
		else
			windowPImpl->RemoveFromGLRender(entity);
	}
	void GameContext::RENDER_SetAnimationParameters(Entity* entity, bool autoAnimate, float animationFps)
	{
		windowPImpl->SetAnimationParameters(entity, autoAnimate, animationFps);
	}
	void GameContext::RENDER_SetManualAnimationState(Entity* entity, float state_0to1)
	{
		windowPImpl->SetAnimationStateValue(entity, state_0to1);
	}
	

	void GameContext::On_InstantiateEntity(Entity* entity, WVec2 pos)
	{
		entity->gameContext = this;
		entity->SetLocation(pos);

		instancedEntities.push_back(entity);

		entity->Start();
	}
	
	void GameContext::GAME_DestroyEntity(Entity* entity)
	{
		for (size_t i = 0; i < instancedEntities.size(); ++i)
		{
			if (instancedEntities[i] != entity)
				continue;
			
			if(instancedEntities[i]->bodyId.isValid)
				physicsPImpl->DestroyObj(instancedEntities[i]->bodyId);
			
			windowPImpl->RemoveFromSDLRender(instancedEntities[i]);
			windowPImpl->RemoveFromGLRender(instancedEntities[i]);

			delete instancedEntities[i];
			instancedEntities[i] = nullptr;
			instancedEntities.erase(instancedEntities.begin() + i);

		}
	}

}