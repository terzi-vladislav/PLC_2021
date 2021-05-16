#include "exception.h"

CExceptionMechanism exceptionMechanism;

std::string CException::GetMessage() const {
    return message;
}

CTry::CTry() : opened( true ), lastTry( exceptionMechanism.latestTry ), handled( true ), except( 0 ), catchRunning( false ) {
    exceptionMechanism.latestTry = this;
}

CTry::~CTry() {
    assert( !opened );
    if( opened ) {
        terminate();
    }
}

void CTry::Close() {
    assert( opened );
    if( !opened ) {
        terminate();
    }

    opened = false;
    exceptionMechanism.latestTry = lastTry;

    if( handled ) {
        if( except != 0 ) {
            if( except != exceptionMechanism.except ) {
                delete except;
            }
            except = 0;
        }
    } else {
        ThrowException( except );
    }
}

CTry* CTry::GetLastTry() {
    return lastTry;
}

void ThrowException( CException* e ) {
    if( exceptionMechanism.except != 0 ) {
        terminate();
    }

    exceptionMechanism.except = e;
    CTry* handler = exceptionMechanism.latestTry;
    while( handler != 0 ) {
        if( handler->catchRunning ) {
            handler->Close();
            handler = handler->GetLastTry();
        } else {
            longjmp( handler->env, 1 );
        }
    }
    terminate();
}