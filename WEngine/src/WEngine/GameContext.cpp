#include "GameContext.h"
#include "Game.h"
#include "WMath.h"
#include "Entity.h"
#include "CoroutineInstance.h"

#include <memory>
#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>

#include <SDL3/SDL.h>
#include <box2d.h>
#include <collision.h>
#include <glad/glad.h>
#include "stb_image.h"

#include "Shader.h"



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
			tileSpan = nTiles;

			CalcRect();
		}

		void CalcRect()
		{
			while (animationProgress > tileSpan)
			{
				animationProgress -= tileSpan;
			}
			
			int x = ((int)animationProgress + tileOffset) % hTileCount;
			int y = ((int)animationProgress + tileOffset) / hTileCount;
			

			surfaceRect.x = x * tileWidth;
			surfaceRect.y = y * tileHeight;
			surfaceRect.w = tileWidth;
			surfaceRect.h = tileHeight;
			
			tileAspectRatio = tileWidth / tileHeight;

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
			if (state0to1 >= 1)
				state0to1 -= 0.001f;

			animationProgress = state0to1 * (tileSpan);

			CalcRect();
		}

		int texWidth = 0;
		int texHeight = 0;
		int hTileCount = 1;
		int vTileCount = 1;
		int tileWidth = 0;
		int tileHeight = 0;
		int nTiles = 1;
		int tileOffset = 0;
		int tileSpan = nTiles;

		float tileAspectRatio = 1.f;

		float animationProgress = 0.f;
		int currentTile = 0;
		float animationFps = 10.f;

		SDL_FRect surfaceRect;

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
		SDLWindow(std::string wName, unsigned int wWidth, unsigned int wHeight, WRenderEngine renderEngine) :
			windowName{ wName }, windowWidth{ wWidth }, windowHeight{ wHeight }, renderEngine{ renderEngine }
		{

			// INIT SDL
			SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
			
			SDL_SetJoystickEventsEnabled(true);
			
			TryOpenFirstJoystick();


			window = SDL_CreateWindow(windowName.c_str(), windowWidth, windowHeight, SDL_WINDOW_OPENGL);
			assert(window, "failed to create window");

			if (renderEngine == WRenderEngine::SDL)
			{
				renderTarget = SDL_CreateRenderer(window, NULL);
				if (renderTarget == nullptr)
					std::cout << "failed to create renderTarget" << std::endl;

				SDL_SetRenderDrawColor(renderTarget, 0, 0, 0, 0xFF);
			}
			else
			{
				
				glGenVertexArrays(1, &baseVao);
				glBindVertexArray(baseVao);

				shader.CreateShaderProgram("../shaders/shader_vertex.glsl", "../shaders/shader_fragment.glsl");
				shader.Use();

				glGenBuffers(1, &baseVbo);
				glBindBuffer(GL_ARRAY_BUFFER, baseVbo);
				glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), squareVertices, GL_STATIC_DRAW);

				glGenBuffers(1, &baseEbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, baseEbo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), squareElements, GL_STATIC_DRAW);

				shader.setVertexAttribPointer("position", 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);



			}



		}

		~SDLWindow()
		{

			UnloadAllTextures();

			if (renderEngine == WRenderEngine::OpenGL)
			{
				glDeleteBuffers(1, &baseEbo);
				glDeleteBuffers(1, &baseVbo);
				glDeleteBuffers(1, &baseVao);

				shader.DeleteShaderProgram();
			}

			SDL_DestroyWindow(window);
			window = nullptr;

			SDL_DestroyRenderer(renderTarget);
			renderTarget = nullptr;

			SDL_CloseJoystick(playerJoyStick);
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
					case SDL_EVENT_QUIT:
						stopGame = true;
						break;

						
					case SDL_EVENT_KEY_UP:
					case SDL_EVENT_KEY_DOWN:
						switch (windowEvent.key.key)
						{
						case SDLK_W:
							userInputs.keys[INPUT_KeyCode::W] = windowEvent.key.down;
							break;
						case SDLK_A:
							userInputs.keys[INPUT_KeyCode::A] = windowEvent.key.down;
							break;
						case SDLK_S:
							userInputs.keys[INPUT_KeyCode::S] = windowEvent.key.down;
							break;
						case SDLK_D:
							userInputs.keys[INPUT_KeyCode::D] = windowEvent.key.down;
							break;
						case SDLK_SPACE:
							userInputs.keys[INPUT_KeyCode::Spacebar] = windowEvent.key.down;
							break;

						}
						break;


					case SDL_EVENT_MOUSE_BUTTON_DOWN:
					case SDL_EVENT_MOUSE_BUTTON_UP:
						switch (windowEvent.button.button)
						{
						case 1:
							userInputs.keys[INPUT_KeyCode::MouseLeft] = windowEvent.button.down;
							break;
						case 2:
							userInputs.keys[INPUT_KeyCode::MouseRight] = windowEvent.button.down;
							break;
						}
						break;

					
					case SDL_EVENT_MOUSE_MOTION:
						userInputs.mouseInputVector.x += windowEvent.motion.xrel;
						userInputs.mouseInputVector.y += windowEvent.motion.yrel;
						break;


					case SDL_EVENT_JOYSTICK_AXIS_MOTION:
						if (windowEvent.jaxis.which < 12)
							userInputs.joyAxis[windowEvent.jaxis.axis] = (float)windowEvent.jaxis.value / maxStickValue;
						else
							std::cerr << "axis index exceeded\n";
						break;

					case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
					case SDL_EVENT_JOYSTICK_BUTTON_UP:
						if (windowEvent.jbutton.button < 24)
							userInputs.joyButtons[windowEvent.jbutton.button] = windowEvent.jbutton.down;
						else
							std::cerr << "button index exceeded\n";
						break;

					case SDL_EVENT_JOYSTICK_ADDED:
						TryOpenFirstJoystick();
						break;
					case SDL_EVENT_JOYSTICK_REMOVED:
						if (windowEvent.jdevice.which == 0)
							SDL_CloseJoystick(playerJoyStick);
						playerJoyStick = nullptr;
						std::cout << "Controller disconnected\n";
						TryOpenFirstJoystick();
						break;

				} 
			}
		}

		void TryOpenFirstJoystick()
		{
			if (SDL_HasJoystick() && !playerJoyStick)
			{
				int nOfJoysticks = 0;
				SDL_JoystickID* joySticks = SDL_GetJoysticks(&nOfJoysticks);
				playerJoyStick = SDL_OpenJoystick(*joySticks);
				std::cout << SDL_GetJoystickNameForID(*joySticks) << " connected\n";
				SDL_free(joySticks);
			}
		}
		UserInputs* GetUserInputs()
		{
			return &userInputs;
		}

		void SetOrthoCameraSize(float size)
		{
			orthoCameraSize = size;
		}
		void SetOrthoCameraPosition(WVec2 newPos)
		{
			cameraPosition = newPos;
		}

	public:
		void RenderFrame(float deltaTime)
		{
			if (renderEngine == WRenderEngine::SDL)
				RenderFrameSDL(deltaTime);
			else
				RenderFrameGL(deltaTime);
		}
	
		void AddToRender(Entity* entity, std::string filePath, int hTiles, int vTiles, int tileOffset, int tileSpan, float width, float height, int renderLayer)
		{
			if (renderEngine == WRenderEngine::SDL)
				AddToSDLRender(entity, filePath, hTiles, vTiles, tileOffset, tileSpan, width, height, renderLayer);
			else
				AddToGLRender(entity, filePath);
		}
		void RemoveFromRender(Entity* entity)
		{
			if (renderEngine == WRenderEngine::SDL)
				RemoveFromSDLRender(entity);
			else
				RemoveFromGLRender(entity);
		}

		void UnloadAllTextures()
		{
			if (renderEngine == WRenderEngine::SDL)
				UnloadAllTexturesSDL();
			else
				UnloadAllTexturesGL();
		}

	private:
		void RenderFrameSDL(float deltaTime)
		{

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

				SDL_FRect EntityPosition;

				if (renderSDLEntities[i].entityHeight || renderSDLEntities[i].entityWidth)
				{
					EntityPosition.w = (renderSDLEntities[i].entityWidth) * windowHeight / orthoCameraSize;
					EntityPosition.h = (renderSDLEntities[i].entityHeight / renderSDLEntities[i].params.tileAspectRatio) * windowHeight / orthoCameraSize;
					EntityPosition.x = (windowWidth / 2) + ((location.x - cameraPosition.x) * windowHeight / orthoCameraSize) - EntityPosition.w / 2;
					EntityPosition.y = (windowHeight / 2) - ((location.y - cameraPosition.y) * windowHeight / orthoCameraSize) - EntityPosition.h / 2;

					SDL_RenderTexture(renderTarget, renderSDLEntities[i].texture, &renderSDLEntities[i].params.surfaceRect, &EntityPosition);
				}
				else
				{
					SDL_RenderTexture(renderTarget, renderSDLEntities[i].texture, &renderSDLEntities[i].params.surfaceRect, NULL);
				}

			}

			SDL_RenderPresent(renderTarget);
		}

		void AddToSDLRender(Entity* entity, std::string filePath, int hTiles, int vTiles, int tileOffset, int tileSpan, float width, float height, int renderLayer)
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
				surface = GetTextureInfoSDL(filePath, hTiles, vTiles);

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

			AddTextureUserSDL(filePath, entity);


			RenderSDLEntity2D newEntity;
			newEntity.entity = entity;
			newEntity.filepath = filePath;
			newEntity.texture = surface.texture;
			newEntity.params = surface.params;
			newEntity.renderLayer = renderLayer;
			newEntity.entityWidth = width;
			newEntity.entityHeight = height;
			newEntity.params.tileOffset = tileOffset;
			newEntity.params.tileSpan = tileSpan;


			renderSDLEntities.push_back(newEntity);
			SDL_OrderEntitiesByLayer();
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

				RemoveTextureUserSDL(renderSDLEntities[i].filepath, renderSDLEntities[i].entity);

				renderSDLEntities.erase(renderSDLEntities.begin() + i);

				--i;
			}

			SDL_OrderEntitiesByLayer();
		}

		void AddTextureUserSDL(const std::string& filepath, Entity* entity)
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
		void RemoveTextureUserSDL(const std::string& filepath, Entity* entity)
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

				if(autoUnloadAssetIfUnused)
					UnloadUnusedTexturesSDL(LoadedSurfaces[i].filepath);
			}
		}
		SurfaceInfo GetTextureInfoSDL(const std::string& filepath, int hTiles, int vTiles)
		{
			//If texture is already parsed and loaded, return respective ID;
			for (size_t i = 0; i < LoadedSurfaces.size(); ++i)
				if (filepath == LoadedSurfaces[i].filepath)
					return LoadedSurfaces[i];

			//Texture with this filepath not yet loaded:
			SurfaceInfo LoadedSurface;
			LoadedSurface.filepath = filepath;


			if (LoadTextureFileSDL(filepath, LoadedSurface))
			{
				std::cout << LOG_SURFACE_LOADED << "[" << LoadedSurface.filepath << "]\n";

				LoadedSurface.isValid = true;


				float tWidth;
				float tHeight;

				SDL_GetTextureSize(LoadedSurface.texture, &tWidth, &tHeight);

				LoadedSurface.params.SetParams(hTiles, vTiles, tWidth, tHeight);

				LoadedSurfaces.push_back(LoadedSurface);
			}
			return LoadedSurface;
		}
		bool LoadTextureFileSDL(const std::string& filepath, SurfaceInfo& LoadedSurface)
		{
			SDL_Surface* surface = SDL_LoadBMP(filepath.c_str());
			if (surface == NULL)
			{
				std::cout << LOG_SURFACE_FAILED << "[" << filepath.c_str() << "]\n";

				SDL_DestroySurface(surface);
				return false;
			}
			else
			{
				SDL_SetSurfaceColorKey(surface, true, SDL_MapSurfaceRGB(surface, 255, 0, 255));

				LoadedSurface.texture = SDL_CreateTextureFromSurface(renderTarget, surface);
				SDL_SetTextureScaleMode(LoadedSurface.texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST);



				SDL_DestroySurface(surface);

				if (LoadedSurface.texture == NULL)
					return false;

				return true;
			}
		}
		void UnloadUnusedTexturesSDL(const std::string& filePath)
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

					LoadedSurfaces.erase(LoadedSurfaces.begin() + i);
					break;
				}
			}
		}
		void UnloadAllTexturesSDL()
		{
			for (size_t i = 0; i < LoadedSurfaces.size(); ++i)
			{
				SDL_DestroyTexture(LoadedSurfaces[i].texture);

				std::cout << LOG_SURFACE_UNLOAD << "[" << LoadedSurfaces[i].filepath << "]\n";

				LoadedSurfaces.erase(LoadedSurfaces.begin() + i);

				--i;
			}
		}

		void SDL_OrderEntitiesByLayer()
		{
			std::sort(renderSDLEntities.begin(), renderSDLEntities.end(),
				[](const RenderSDLEntity2D& a, const RenderSDLEntity2D& b)
				{
					return a.renderLayer < b.renderLayer;
				});
		}

		
	private:
		void RenderFrameGL(float deltaTime)
		{

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

				
				//glm::mat4 model = renderGLEntities[i]

				

				//renderGLEntities[i].shader->setMat4("model", model);
				

				//glBindVertexArray(renderGLEntities[i].vao);
				//glBindTexture(GL_TEXTURE_2D, renderObjects3D[i].tex);

				//draw elements here (create vbo for square on contructor)
				
				glBindVertexArray(0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			SDL_GL_SwapWindow(window);




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
				texture = GetTextureInfoGL(textureFilePath);

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

			AddTextureUserGL(textureFilePath, entity);


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

				RemoveTextureUserGL(renderGLEntities[i].filePath, entity);

				glDeleteVertexArrays(1, &renderGLEntities[i].vao);

				renderGLEntities.erase(renderGLEntities.begin() + i);

				--i;
			}
		}

		void AddTextureUserGL(const std::string& filepath, Entity* entity)
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
		void RemoveTextureUserGL(const std::string& filepath, Entity* entity)
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

				if (autoUnloadAssetIfUnused)
					UnloadUnusedTexturesGL(LoadedTextures[i].filepath);
			}
		}
		TextureInfo GetTextureInfoGL(const std::string& filepath)
		{
			//If texture is already parsed and loaded, return respective ID;
			for (size_t i = 0; i < LoadedTextures.size(); ++i)
				if (filepath == LoadedTextures[i].filepath)
					return LoadedTextures[i];

			//Texture with this filepath not yet loaded:
			TextureInfo LoadedTexture;
			LoadedTexture.filepath = filepath;

			if (LoadTextureFileGL(filepath, LoadedTexture))
			{
				LoadedTexture.isValid = true;
				LoadedTextures.push_back(LoadedTexture);
			}
			return LoadedTexture;
		}
		bool LoadTextureFileGL(const std::string& filepath, TextureInfo LoadedTexture)
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
		void UnloadUnusedTexturesGL(const std::string& filePath)
		{
			for (size_t i = 0; i < LoadedTextures.size(); ++i)
			{

				if (LoadedTextures[i].tex)
				{
					glDeleteTextures(1, &LoadedTextures[i].tex);
					LoadedTextures[i].tex = 0;

					std::cout << LOG_TEXTURE_UNLOAD << "[" << LoadedTextures[i].filepath << "]\n";
				}

				LoadedTextures.erase(LoadedTextures.begin() + i);

				--i;
			}
		}
		void UnloadAllTexturesGL()
		{
			for (size_t i = 0; i < LoadedTextures.size(); ++i)
			{
				glDeleteTextures(1, &LoadedTextures[i].tex);
				LoadedTextures[i].tex = 0;

				std::cout << LOG_TEXTURE_UNLOAD << "[" << LoadedTextures[i].filepath << "]\n";

				LoadedTextures.erase(LoadedTextures.begin() + i);

				--i;
			}
		}




	public:
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
		void SetAnimationTileParameters(Entity* entity, int tileOffset, int tileSpan)
		{
			//check if entity is in render list
			for (size_t i = 0; i < renderSDLEntities.size(); ++i)
			{
				if (renderSDLEntities[i].entity != entity)
					continue;

				renderSDLEntities[i].params.tileOffset = tileOffset;
				renderSDLEntities[i].params.tileSpan = tileSpan;

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

		WRenderEngine renderEngine;

		Shader shader;
		GLuint baseVao;

		GLuint baseVbo;
		float squareVertices[8]{
			-0.5f, 0.5f,		0.5f, 0.5f,
			-0.5f, -0.5f,		0.5, -0.5f };

		GLuint baseEbo;
		float squareElements[6]{
			0, 1, 2,	2, 1, 3 };

		float orthoCameraSize = 10;
		WVec2 cameraPosition = WVec2(0);

		SDL_Event windowEvent = SDL_Event();
		UserInputs userInputs;

		SDL_Joystick* playerJoyStick;
		short int maxStickValue = 32767;

	public:
		bool stopGame = false;
		bool autoUnloadAssetIfUnused = true;

	};



	class GameContext::Box2D
	{
	public:
		Box2D()
		{
			CreateWorld();
			auto ver = b2GetVersion();
			std::cout << "Current Box2d version: " << ver.major << "." << ver.minor << "." << ver.revision << '\n';
		}

		~Box2D()
		{
			b2DestroyWorld(worldId);
		}

		void UpdatePhysicsWorld(float deltaTime)
		{

			b2World_Step(worldId, deltaTime, 4);
			
			HandleContacts();
			
		}

		void HandleContacts()
		{
			b2ContactEvents contacts = b2World_GetContactEvents(worldId);

			for (int i = 0; i < contacts.beginCount; ++i)
			{
				b2ContactBeginTouchEvent* beginTouchEvent = contacts.beginEvents + i;

				b2Vec2 contactPoint = beginTouchEvent->manifold.points->point;

				if (beginTouchEvent->manifold.pointCount > 1)
				{
					b2Vec2 contactPoint2 = (beginTouchEvent->manifold.points + 1)->point;

					contactPoint = { (contactPoint.x + contactPoint2.x) / 2 , (contactPoint.y + contactPoint2.y) / 2 };
				}

				b2BodyId bodyA = b2Shape_GetBody(beginTouchEvent->shapeIdA);
				b2BodyId bodyB = b2Shape_GetBody(beginTouchEvent->shapeIdB);

				Entity* entityA = static_cast<Entity*>(b2Body_GetUserData(bodyA));
				Entity* entityB = static_cast<Entity*>(b2Body_GetUserData(bodyB));

				if (entityA && entityB)
				{
					WVec2 point = WVec2(contactPoint.x, contactPoint.y);
					entityA->On_CollisionBegin(entityB, point);
					entityB->On_CollisionBegin(entityA, point);
				}
			}


			b2SensorEvents sensorEvents = b2World_GetSensorEvents(worldId);
			for (int i = 0; i < sensorEvents.beginCount; ++i)
			{
				b2SensorBeginTouchEvent* beginTouch = sensorEvents.beginEvents + i;

				b2BodyId sensorBody = b2Shape_GetBody(beginTouch->sensorShapeId);
				b2BodyId otherBody = b2Shape_GetBody(beginTouch->visitorShapeId);

				Entity* sensorEntity = static_cast<Entity*>(b2Body_GetUserData(sensorBody));
				Entity* otherEntity = static_cast<Entity*>(b2Body_GetUserData(otherBody));
				
				sensorEntity->On_SensorBeginOverlap(otherEntity);
				otherEntity->On_EnterOtherSensor(sensorEntity);
			}
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

		WPhysBodyId CreateObj(Entity* entity, WBodyType bodyType, WVec2 boxSize, uint32_t collisionLayer, uint32_t collidesWith, bool isSensor, float density)
		{
			b2BodyDef bodyDef = b2DefaultBodyDef();
			bodyDef.type = (b2BodyType)bodyType;
			WVec2 pos = entity->GetLocation();
			bodyDef.position = { pos.x, pos.y};
			bodyDef.fixedRotation = true;
			bodyDef.userData = entity;
			b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

			if (boxSize.x || boxSize.y)
			{
				b2Polygon bodyBox = b2MakeBox(boxSize.x / 2, boxSize.y / 2);
				b2ShapeDef boxShapeDef = b2DefaultShapeDef();
				boxShapeDef.density = density;
				if (isSensor)
					boxShapeDef.isSensor = isSensor;
				boxShapeDef.enableContactEvents = true;
				
				boxShapeDef.filter.categoryBits = collisionLayer;
				boxShapeDef.filter.maskBits = collidesWith;

				boxShapeDef.friction = 0.1f;

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


	GameContext::GameContext(Game* game) : game{ game }, 
		windowPImpl { std::make_unique<SDLWindow>(game->gameName, game->windowWidth, game->windowHeight, game->renderEngine) }, 
		physicsPImpl{ std::make_unique<Box2D>() }
	{
		
	}

	GameContext::~GameContext()
	{

	}

	void GameContext::SYSTEM_UpdatePhysics(float deltaTime)
	{
		if (deltaTime > 1.f / 40.f)
			deltaTime = 1.f / 40.f; //to prevent large simulation distances and tunneling as result. Simulation will slow down with framerates lower than 40 but "it's a sacrifice I'm willing to take" lol

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
		if (game->stopGame)
			SYSTEM_DestroyAllEntities();
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

	void GameContext::PHYS_AddPhysComponentToEntity(Entity* entity, WBodyType bodyType)
	{ 
		PHYS_AddPhysComponentToEntity(entity, bodyType, entity->GetInitialSize(), 0, 0, false, 1.0f); 
	}
	void GameContext::PHYS_AddPhysComponentToEntity(Entity* entity, WBodyType bodyType, WVec2 sizeOverride, uint32_t collisionLayer, uint32_t collidesWith, bool isSensor, float density)
	{
		if (!entity->bodyId.isValid)
		{
			entity->bodyId = physicsPImpl->CreateObj(entity, bodyType, sizeOverride, collisionLayer, collidesWith, isSensor, density);
		}
			
	}

	WVec2 GameContext::SYSTEM_GetLocationOfPhysObj(WPhysBodyId id)
	{
		if (!id.isValid)
		{
			std::cout << "ERROR: Tried accessing invalid physics obj!\n";
			return WVec2(0);
		}
		else
			return physicsPImpl->GetObjPos(id);
	}
	void GameContext::PHYS_SetLinearVelocityOnPhysObj(WPhysBodyId id, WVec2 vel)
	{
		assert(id.isValid, "Tried accessing invalid physics obj!");
		physicsPImpl->SetObjLinearVelocity(id, vel);
	}
	void GameContext::PHYS_SetLocationOnPhysObj(WPhysBodyId id, WVec2 pos)
	{
		if (id.isValid)
			physicsPImpl->SetObjPos(id, pos);
		else
			std::cout << "Tried accessing invalid physics obj! \n";
	}
	void GameContext::PHYS_SetWorldGravity(WVec2 vec)
	{
		physicsPImpl->SetWorldGravity(vec);
	}

	void GameContext::RENDER_AddRenderComponent(Entity* entity, std::string filePath, int hTiles, int vTiles, int tileOffset, int tileSpan, int layer)
	{
		RENDER_AddRenderComponent(entity, filePath, hTiles, vTiles, tileOffset, tileSpan, layer, entity->GetInitialSize());
	}
	void GameContext::RENDER_AddRenderComponent(Entity* entity, std::string filePath, int hTiles, int vTiles, int tileOffset, int tileSpan, int layer, WVec2 sizeOverride)
	{
		windowPImpl->AddToRender(entity, filePath, hTiles, vTiles, tileOffset, tileSpan, sizeOverride.x, sizeOverride.y, layer);
	}
	void GameContext::RENDER_RemoveRenderComponent(Entity* entity)
	{
		windowPImpl->RemoveFromRender(entity);
	}
	void GameContext::RENDER_SetAnimationParameters(Entity* entity, bool autoAnimate, float animationFps)
	{
		windowPImpl->SetAnimationParameters(entity, autoAnimate, animationFps);
	}
	void GameContext::RENDER_SetAnimationTileParameters(Entity* entity, int tileOffset, int tileSpan)
	{
		windowPImpl->SetAnimationTileParameters(entity, tileOffset, tileSpan);
	}
	void GameContext::RENDER_SetManualAnimationState(Entity* entity, float state_0to1)
	{
		windowPImpl->SetAnimationStateValue(entity, state_0to1);
	}
	
	void GameContext::RENDER_SetOrthoCameraSize(float size)
	{
		windowPImpl->SetOrthoCameraSize(size);
	}
	void GameContext::RENDER_SetOrthoCameraPosition(WVec2 newPos)
	{
		windowPImpl->SetOrthoCameraPosition(newPos);
	}

	void GameContext::GAME_StartCoroutine(Entity* caller, int ID, float duration)
	{
		CoroutineInstance* corInstance = new CoroutineInstance(caller, ID, duration);
		On_InstantiateEntity(corInstance, WVec2(0), WVec2(0));

		//instancedEntities.push_back(corInstance);
		//corInstance->Start();
	}
	void GameContext::On_InstantiateEntity(Entity* entity, WVec2 pos, WVec2 size)
	{
		entity->gameContext = this;
		entity->SetLocation(pos);
		entity->initialSize = size;

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
			
			windowPImpl->RemoveFromRender(instancedEntities[i]);

			delete instancedEntities[i];
			instancedEntities[i] = nullptr;
			instancedEntities.erase(instancedEntities.begin() + i);

		}
	}
	void GameContext::SYSTEM_DestroyAllEntities()
	{
		for (size_t i = 0; i < instancedEntities.size(); ++i)
		{
			if (instancedEntities[i]->bodyId.isValid)
				physicsPImpl->DestroyObj(instancedEntities[i]->bodyId);

			windowPImpl->RemoveFromRender(instancedEntities[i]);

			delete instancedEntities[i];
			instancedEntities[i] = nullptr;
			instancedEntities.erase(instancedEntities.begin() + i);
		}
	}


	void GameContext::MEMORY_SetAutoUnloadAssetIfUnused(bool enabled)
	{
		windowPImpl->autoUnloadAssetIfUnused = enabled;
	}
}