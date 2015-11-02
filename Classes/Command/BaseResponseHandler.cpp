#include "BaseResponseHandler.h"

BaseResponseHandler::BaseResponseHandler() {

}

BaseResponseHandler::~BaseResponseHandler() {

}

void BaseResponseHandler::registerResponseHandler(int command, RESPONSE_HANDLER handler) {
    if ( 0<command && NULL != handler) {
        m_handlerMap.insert(std::make_pair(command, handler));
    }
}

void BaseResponseHandler::handleResponse(int command, void* pData,WORD len) {
    if (command > 0) {
        std::map<int, RESPONSE_HANDLER >::iterator it = m_handlerMap.find(command);
        if (it != m_handlerMap.end()) {
            RESPONSE_HANDLER callback = m_handlerMap[command];
            if (NULL != callback) {
                (this->*callback)(pData,len);
            }
        }
    }
}

__String *BaseResponseHandler::handleErrorMessage() {
  
    return NULL;
}