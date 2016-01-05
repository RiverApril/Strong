
#include "Network.hpp"

namespace Network{
    
    void init(){
        if (SDLNet_Init() < 0) {
    		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
    		exit(EXIT_FAILURE);
    	}
    }
    
    void cleanup(){
    	SDLNet_Quit();
    }
    
    bool initHost(int port, TCPsocket* socket, IPaddress* ip){
        if (SDLNet_ResolveHost(ip, NULL, port) < 0) {
    		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
            return false;
    	}
        if (!(*socket = SDLNet_TCP_Open(ip))) {
    		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
            return false;
    	}
        return true;
    }
    
    bool acceptGuest(TCPsocket* host, TCPsocket* client){
        *client = SDLNet_TCP_Accept(*host);
        if(*client){
            return true;
        }else{
            return false;
        }
        return true;
    }
    
    bool connectToHost(string address, int port, TCPsocket* socket, IPaddress* ip){
        if (SDLNet_ResolveHost(ip, address.c_str(), port) < 0) {
    		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
    		return false;
    	}
    	if (!(*socket = SDLNet_TCP_Open(ip))) {
    		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
    		return false;
    	}
        return true;
    }
    
    bool sendData(TCPsocket* socket, vector<unsigned char>& data){
        if (SDLNet_TCP_Send(*socket, static_cast<void*>(&data), (int)data.size()) < (int)data.size()) {
			fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
            return false;
		}
        return true;
    }
    
    
    
    void initPacket(vector<unsigned char>& data, unsigned char code){
        data.push_back(code);
        addDataSizeT(data, 0);
    }
    
    void finishPacket(vector<unsigned char>& data){
        union {
            size_t n;
            unsigned char b[sizeof(size_t)];
        } uBytes;

        uBytes.n = data.size();
        for (size_t i = 0; i < sizeof(size_t); i++) {
            data[1+i] = (unsigned char) (uBytes.b[i] & 0xFF);
        }
    }

    int reciveData(TCPsocket* socket, unsigned char* data, size_t dataSize){
        int t = 0;
        bool b;
        
        do{
            b = false;
            int result = SDLNet_TCP_Recv(*socket, data+t, dataSize-t);
            switch(result){
                case 0:{
                    return 0;
                }
                case -1:{
                    return -1;
                }
                default:{
                    if(result < dataSize){
                        b = true;
                        t += result;
                    }else{
                        return dataSize;
                    }
                    break;
                }
            }
        }while(b);
    }
    
    
    
    
    
    void addDataSizeT(vector<unsigned char>& data, size_t n){
        union {
            size_t n;
            unsigned char b[sizeof(size_t)];
        } uBytes;

        uBytes.n = n;
        for (size_t i = 0; i < sizeof(size_t); i++) {
            data->push_back((unsigned char) (uBytes.b[i] & 0xFF));
        }
    }
    
    void addDataUChar(vector<unsigned char>& data, unsigned char n){
        data.push_back(n);
    }
    
    void addDataString(vector<unsigned char>& data, string n){
        addDataSizeT(data, n.size());
        for(char c : n){
            data.push_back((unsigned char)c);
        }
    }
    
    
    
    
    void readDataSizeT(unsigned char* data, size_t& position, size_t& n){
        union {
            size_t n;
            unsigned char b[sizeof(size_t)];
        } uBytes;

        for (size_t i = 0; i<sizeof(size_t); i++) {
            uBytes.b[i] = data[*position];
            (*position)++;
        }
        n = uBytes.n;
    }
    
    void readDataUChar(unsigned char* data, size_t& position, unsigned char& n){
        n = data[*position]
        (*position)++;
    }
    
    void readDataString(unsigned char* data, size_t& position, string& n){
        n = "";
        size_t ss;
        readDataSizeT(data, position, ss);
        for(size_t i = 0; i < ss; i++){
            char c = (char)data[*position];
            (*position)++;
            n += c;
        }
    }
    
    
}