#include "protocol.h"
using namespace std;

class Link{
	public:
		int device_id;
        int socket_desc;
        struct sockaddr_in server_addr;
        Link(int p){
            device_id = (p/1000)-2;
            socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if(socket_desc < 0){
                cout<<"Error while creating socket\n";
            }

            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(p);
            server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
        }
        int transmit(Dataframe df){
            if(sendto(socket_desc, (void*)(&df), sizeof(df), 0,
                (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
                cout<<"Unable to send message\n";
                return -1;
            }
            return 0;
        } 
};