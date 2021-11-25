#include "protocol.h"
using namespace std;

class Dataframe{
    
    char msg[2000];
    int msg_seq_num;
    int source;
    int destination;

    public:
        Dataframe(char* message, int msg_seq_num, int source, int destination){
            this->source = source;
            this->destination = destination;
            this->msg_seq_num = msg_seq_num;
            strcpy(this->msg, message);
        }
        Dataframe(){}
        int getDestination(){
            return this->destination;
        }
        int getSource(){
            return this->source;
        }
        char* getMessage(){
            return this->msg;
        }
};

class DataFrameWithThis{
    public:
        Dataframe df;
        void * object;
        DataFrameWithThis(Dataframe df, void* obj){
            this->df = df;
            this->object = obj;
        }
        DataFrameWithThis(){}
};

