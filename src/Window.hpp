#ifndef Strong_Window_hpp
#define Strong_Window_hpp

#include "Client.hpp"

class Window{
public:
    
    bool running = true;
    
    Client* client;
    
    Window();
    
    ~Window(){}
    
    void update();
    
};

#endif
