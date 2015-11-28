#include "GameServiceClientManager.h"

static GameServiceClientManager *__instance = NULL;

GameServiceClientManager::GameServiceClientManager() {
    m_clientMap = __Dictionary::create();
    m_clientMap->retain();

    m_currentRoom = "";
    m_currentClientName = "";
}

GameServiceClientManager::~GameServiceClientManager() {
    CC_SAFE_RELEASE_NULL(m_clientMap);
}

GameServiceClientManager *GameServiceClientManager::sharedInstance() {
    if (NULL == __instance) {
        __instance = new GameServiceClientManager;
    }
    return __instance;
}

void GameServiceClientManager::purgeInstance() {
    CC_SAFE_DELETE(__instance);
}

void GameServiceClientManager::registerServiceClient(const char *name, GameServiceClient *client, bool needScriptResponseHandler) {
    if (NULL != name && NULL == m_clientMap->objectForKey(name) && NULL != client) {
        m_clientMap->setObject(client, name);
        client->setGameZone(name);

    
    }
}

void GameServiceClientManager::setCurrentServiceClientName(const char *name) {
    if (NULL != name) {
        m_currentClientName = name;
    }
}

std::string GameServiceClientManager::getCurrentServiceClientName() {
    return m_currentClientName;
}


void GameServiceClientManager::registerServiceClient(const char* name, bool needScriptResponseHandler,int sorketIndex) {
    if (NULL != name && NULL == m_clientMap->objectForKey(name)) {
        GameServiceClient *client = GameServiceClient::create(sorketIndex);

        registerServiceClient(name, client, needScriptResponseHandler);
    }
}

void GameServiceClientManager::removeServiceClient(const char* name) {
	if (NULL != name) {
        GameServiceClient *client = serviceClientForName(name);
        if (NULL != client) {
            client->cleanup();
        }
		m_clientMap->removeObjectForKey(name);
	}
}

void GameServiceClientManager::removeAllServiceClients() {
    CCArray *allKeys = m_clientMap->allKeys();
    if (NULL != allKeys && allKeys->count() > 0) {
        for (int i = 0; i < allKeys->count(); ++i) {
            __String *name = (__String*)allKeys->getObjectAtIndex(i);
            GameServiceClient *client = serviceClientForName(name->getCString());
            if (NULL != client) {
                client->cleanup();
            }
        }
    }
    m_clientMap->removeAllObjects();
}

GameServiceClient *GameServiceClientManager::serviceClientForName(const char* name) {
	if (NULL != name)
	{
		return (GameServiceClient*)m_clientMap->objectForKey(name);
	}
    return NULL;
}


void GameServiceClientManager::setCurrentRoom(const char *room) {
    if (NULL != room) {
        m_currentRoom = room;
    }
}

std::string GameServiceClientManager::getCurrentRoom() {
    return m_currentRoom;
}

GameServiceClient *GameServiceClientManager::getCurrentServiceClient() {
    return serviceClientForName(m_currentClientName.c_str());
}