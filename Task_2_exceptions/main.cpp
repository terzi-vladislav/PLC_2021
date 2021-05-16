#include "exception.h"

void yes() {
    THROW( new CVZException() );
}

void no() {
    std::cout << "yes" << '\n';
    yes();
}


int main() {
    TRY {
        TRY {
            no();
        }
        CATCH( CVZException, e ) {
            std::cout << e->GetMessage() << " 1 " << '\n';
        }
        CATCH( CException, e ) {
            std::cout << e->GetMessage() << " 2 " << '\n';
        }
        ENDTRYCATCH;
    }
    CATCH( CException, e ) {
        std::cout << e->GetMessage() << " 3 " << '\n';
    }
    ENDTRYCATCH;

    std::cout << "no" << '\n';

    return 0;
}