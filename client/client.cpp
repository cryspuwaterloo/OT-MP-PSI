#include "client.h" 
using namespace std;
client::client(string connection_address, int __log)
{
    sock= 0;  
    MAX_BUF_LENGTH = 4096;  
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT);                 
    if(inet_pton(AF_INET, connection_address.c_str(), &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
    }

    log = __log;
    //Initialize vectors to store message sizes in the form [#messages, bytes_sent, bytes_recived]
    for(int i=0 ; i<3 ;i++)
    {
        s1r1_comm_cost.push_back(0);
        s1r2_comm_cost.push_back(0);
        s0_comm_cost.push_back(0);
    }
}
       
string client::send_to_server(string arg, string raw_message) 
{ 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error"); 
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        cout << "Connection failed" << endl;
        return "ERR";
    }
    //Send to server 
    string message = arg + "|" + raw_message;
    send(sock , message.c_str() , message.length() , 0 ); 

    //Get the response from server
    vector<char> buffer(MAX_BUF_LENGTH);
    string response;   
    int bytesReceived = 0;
    do {
        buffer = vector<char>(MAX_BUF_LENGTH);
        bytesReceived = read(sock, &buffer[0], buffer.size());
        //Append to string from the buffer
        if ( bytesReceived == -1 ) { 
            cout<< "Error: couldn't read from server"<<endl;
        } else {
            response.append( buffer.cbegin(), buffer.cend() );
        }
    } while ( bytesReceived == MAX_BUF_LENGTH );
    response.resize(strlen(response.c_str()));
    if(close(sock)<0)
    {
        cout<<"Error: connection could not be closed"<<endl;
    } 

    if(arg.compare("S1_R1") == 0)
    {
        s1r1_comm_cost[0] += 1;
        s1r1_comm_cost[1] += message.length();
        s1r1_comm_cost[2] += response.length();
    }
    
    else if(arg.compare("S1_R2") == 0)
    {
        s1r2_comm_cost[0] += 1;
        s1r2_comm_cost[1] += message.length();
        s1r2_comm_cost[2] += response.length();
    }

    else if(arg.compare("S0") == 0)
    {
        s0_comm_cost[0] += 1;
        s0_comm_cost[1] += message.length();
        s0_comm_cost[2] += response.length();
        }
    
    return response; 
} 

vector <int> client::get_message_sizes() 
{
   vector<int>  message_sizes;
   if(s1r1_comm_cost[0] != 0)
   {
     message_sizes.push_back(s1r1_comm_cost[1]/s1r1_comm_cost[0]);
     message_sizes.push_back(s1r1_comm_cost[2]/s1r1_comm_cost[0]);
   }
   if(s1r2_comm_cost[0] != 0)
   {
    message_sizes.push_back(s1r2_comm_cost[1]/s1r2_comm_cost[0]);
    message_sizes.push_back(s1r2_comm_cost[2]/s1r2_comm_cost[0]);
   }
   if(s0_comm_cost[0] != 0)
   {
    message_sizes.push_back(s0_comm_cost[1]/s0_comm_cost[0]);
    message_sizes.push_back(s0_comm_cost[2]/s0_comm_cost[0]);
   }
   return message_sizes;
}