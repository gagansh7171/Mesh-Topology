//Includes all the required header files and structure definition of frame
#include "protocol.h"
#include "dataframe.h"
#include "link.h"
using namespace std;

int NUMDEVICES = 8;
int* port;

class Node{
	public:
		int device_id;
        int msg_seq_num;
		vector<Link> links;
	
		Node(){
            msg_seq_num = 0;
            cout<<"Enter device ID : ";
            cin>>device_id;   
			cout<<"Device ID : "<<device_id<<"\nPort Number : "<<port[device_id]<<endl;

            int socket_desc = configureServer();

            pthread_t server_thread;
			pthread_create( &server_thread, NULL, server_function, &socket_desc );
			
            cout<<"Is peer ready to connect to the Network? (y/n) : ";
			string input;
			cin>>input;
			if( input[0] == 'Y' || input[0] == 'y' ){
                
                configureClient();

                while(true){
                    cout<<"Enter Message : ";
                    char message[2000];
                    int x;
                    cin.ignore();
                    cin.getline(message,2000);
                    cout<<"Enter number of recepients : ";
                    cin>>x;
                    cout<<"Enter space separated Recepients : ";

                    int reciepents[x];
                    for(int i = 0; i<x; i++)
                        cin>>reciepents[i];
                    
                    Dataframe df[x];
                    DataFrameWithThis dff[x];
                    
                    pthread_t client_thread[x];
                    for(int i = 0; i<x; i++){
                        if(reciepents[i] != device_id && reciepents[i] >=0 && reciepents[i] < NUMDEVICES ){
                            df[i] = Dataframe(message, msg_seq_num, device_id, reciepents[i]);
                            dff[i] = DataFrameWithThis(df[i], this);
                            pthread_create(&client_thread[i], NULL, &client_function_wrapper, &dff[i]);
                            msg_seq_num++;
                            (msg_seq_num) %= NUMDEVICES;
                        }
                    }
                    for(int i = 0; i<x; i++){
                        if(reciepents[i] != device_id && reciepents[i] >=0 && reciepents[i] < NUMDEVICES ){
                            pthread_join( client_thread[i], NULL );
                        }
                    }
                }
				pthread_join( server_thread, NULL );
			}	
		}

		static void* server_function(void* args){
			struct sockaddr_in client_addr;
			socklen_t client_struct_length = sizeof(client_addr);
			while(true){
				Dataframe client_message;
				if (recvfrom( *(int*)(args), (void*)(&client_message), sizeof(client_message), 0, (struct sockaddr*)&client_addr, &client_struct_length) < 0){
					printf("Couldn't receive\n");
					return NULL;
				}
				receive(client_message);
			}
			close(*(int*)(args));
			return NULL;
		}

		static void* client_function_wrapper(void *dff){
			reinterpret_cast<Node*>(((struct DataFrameWithThis*)dff)->object)->client_function(&(((struct DataFrameWithThis* )dff)->df));
			return NULL;
		}

        void* client_function(void* args){
            Dataframe df = *(Dataframe*)(args);
            links[df.getDestination()].transmit(df);
            return NULL;
        }

		static void receive(Dataframe client_message){
			cout<<"\nDevice "<<client_message.getSource()<<" : "<<client_message.getMessage()<<endl<<"Enter Message : "<<flush;
			return;
		}

        int configureServer(){
            struct sockaddr_in server_addr;
            int socket_desc;
            int port_number = port[device_id];
			
			// Create UDP socket:
			socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			
			if(socket_desc < 0){
				cout<<"Error while creating socket\n";
			}
			
			// Set port and IP:
			server_addr.sin_family = AF_INET;
			server_addr.sin_port = htons(port_number);
			server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			
			// Bind to the set port and IP:
			if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
				cout<<"Couldn't bind to the port\n";
			}

            return socket_desc;
        }

        void configureClient(){
            for(int i = 0; i<NUMDEVICES;i++){
                links.push_back(Link(port[i]));
            }
        }
};

int main(){
    port = new int[NUMDEVICES]{2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000};
    Node peer;
}