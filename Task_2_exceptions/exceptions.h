#pragma once
#include <csetjmp>
#include <string>
#include <cassert>
#include <iostream>


class CException {
	public:
    	CException( const std::string& theMessage = std::string( "" ) ) : message( theMessage ) {}
    	virtual ~CException() {};
    	std::string GetMessage() const;

	private:
    	std::string message;
};

class CTry {
	public:
    	CTry();
    	~CTry();

    	void Close();
    	CTry* GetLastTry();

	    CException* except;
	    bool handled;
	    bool catchRunning;

	    jmp_buf env;
	    int val;

	private:
	    bool opened;
	    CTry* lastTry;
};

struct CExceptionMechanism {
    CExceptionMechanism() : latestTry( 0 ), except( 0 ) {}
    CTry* latestTry;
    CException* except;
};

extern CExceptionMechanism exceptionMechanism;

void ThrowException( CException* e );

#define THROW( e ) ThrowException( e );

#define TRY \
	{ \
		CTry Try; Try.val = setjmp( Try.env ); \
		if( Try.val != 0 ) { Try.catchRunning = true; Try.handled = false; Try.except = exceptionMechanism.except; exceptionMechanism.except = 0; } \
		if( Try.val == 0 )

#define ENDTRYCATCH Try.Close(); }

#define CATCH( T, e ) \
	{ \
		T* e; \
		if( ( e = dynamic_cast<T*>( Try.except ) ) != 0 ) { \
			if (Try.handled ) { delete Try.except; Try.except = 0; \
			} else { Try.handled = true; } \
		} \
	} if( T* e = dynamic_cast<T*>( Try.except ) )

class CVZException : public CException {
	public:
    	CVZException( const std::string& theMessage = std::string( "VZ" ) ) : CException( theMessage ) {}
};