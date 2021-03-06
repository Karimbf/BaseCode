#include "StateParser.h"
#include "GameObjectFactory.h"
#include "TextureManager.h"
#include "Game.h"
#include "LoaderParams.h"
#include <string>

bool StateParser::parseState(const char* stateFile, string stateID,
	std::vector<GameObject*> * pObjects, std::vector<string> * pTextureIDs)
{
	
	TiXmlDocument xmlDoc; // crea el documento xml

	
	if (!xmlDoc.LoadFile(stateFile)) {  // carga el statefile
		return false;
	}

	TiXmlElement* pRoot = xmlDoc.RootElement();

	TiXmlElement* pStateRoot = 0;
	for (TiXmlElement* e = pRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == stateID)
		{
			pStateRoot = e;
			break;
		}
	}
	TiXmlElement* pTextureRoot = 0;


	
	for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == string("TEXTURES"))
		{
			pTextureRoot = e;
			break;
		}
	}

	parseTextures(pTextureRoot, pTextureIDs);

	TiXmlElement* pObjectRoot = 0;

	for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == string("OBJECTS"))
		{
			pObjectRoot = e;
			break;
		}
	}

	parseObjects(pObjectRoot, pObjects);
	return true;
}

void StateParser::parseTextures(TiXmlElement* pStateRoot, std::vector<string> *pTextureIDs)
{
	for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		const char* filenameAttribute, *idAttribute;

		filenameAttribute = e->Attribute("filename");
		idAttribute = e->Attribute("ID");

		pTextureIDs->push_back(idAttribute);
		TextureManager::Instance()->load(filenameAttribute, idAttribute, Game::Instance()->getRenderer());
	}
}

void StateParser::parseObjects(TiXmlElement *pStateRoot, std::vector<GameObject *> *pObjects)
{
	int width;
	int height;
	string textureID;
	int currentRow;
	int sprits;
	int flip;
	Vector2D position, velocity, maxVelocity, acceleration, friction;
	int px, py;
	int vx, vy;
	int mvx, mvy;
	int ax, ay;
	int fx, fy;
	int callbackId;
	int collision;


	for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		e->Attribute("posicionx", &px);
		e->Attribute("posiciony", &py);
		e->Attribute("velocidadx", &vx);
		e->Attribute("velocidady", &vy);
		e->Attribute("maximavelocidadx", &mvx);
		e->Attribute("maximavelocidady", &mvy);
		e->Attribute("acelx", &ax);
		e->Attribute("acely", &ay);
		e->Attribute("friccionx", &fx);
		e->Attribute("fricciony", &fy);
		e->Attribute("callbackID", &callbackId);
		e->Attribute("width", &width);
		e->Attribute("height", &height);
		textureID = e->Attribute("textureID");
		e->Attribute("currentRow", &currentRow);
		e->Attribute("numFrames", &sprits);
		e->Attribute("flip", &flip);

		position = *(new Vector2D(px, py));
		velocity = *(new Vector2D(vx, vy));
		maxVelocity = *(new Vector2D(mvx, mvy));
		acceleration = *(new Vector2D(ax, ay));
		friction = *(new Vector2D(fx, fy));

		GameObject* tGameObject = GameObjectFactory::Instance()->CreateGameObject(e->Attribute("type"));
		tGameObject->load(new LoaderParams(width, height, textureID, currentRow, sprits, flip, callbackId, position, velocity, maxVelocity, acceleration, friction));
		pObjects->push_back(tGameObject);
	}
}