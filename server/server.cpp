#include "server.h"
#include "Keyholder.h"
#define PORT 8080 
#define TRUE   1 

int server_fd, new_socket; 
struct sockaddr_in address;
unsigned int MAX_BUF_LENGTH = 4096;

void serverInitFunction() 
{ 
    int opt = 1; 
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
   
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0) 
    { 
        perror("The binding failed"); 
        exit(EXIT_FAILURE); 
    } 
   
    if (listen(server_fd, 4) < 0)   // The second parameter is the length of the queue for pending connections. can be changed if necessary.
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
    cout <<"Server Initialized"<<endl;
} 

int main()
{
    Keyholder k;
    
    serverInitFunction();
    int addrlen = sizeof(address); 
    
    //The server runs in a loop below. It waits for requests on the file descriptor and accepts the first pending request for the listening socket.
    while(TRUE)
    {    
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
        { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        }

	cout << "Incoming connection" << endl;
	
        //Get the input from the client
        vector<char> buffer(MAX_BUF_LENGTH);
        string input_from_client;   
        int bytesReceived = 0;
        do {
            buffer = vector<char>(MAX_BUF_LENGTH);
            bytesReceived = read(new_socket, &buffer[0], buffer.size());
            // append string from buffer.
            if ( bytesReceived == -1 ) { 
                // error 
            } else {
                input_from_client.append( buffer.cbegin(), buffer.cend() );
            }
        } while ( bytesReceived == MAX_BUF_LENGTH );

        stringstream ss(input_from_client);
        string arg, message, result;
  
        
        char delim = '|';
        std::getline(ss, arg, delim);
        std::getline(ss, message, delim);
        if(arg.compare("INIT") == 0)
        {
            cout<<"Initializing Keyholder"<<endl;
            k = Keyholder(message);
            result = "Complete";
        }
        else if(arg.compare("S1_R1") == 0)
        {
            Scheme1_Round1_send input = Scheme1_Round1_send(message);
            Scheme1_Round1_receive output = k.Scheme1_Round1(input);
            result = output.toString();
            cout<<"Scheme 1 Round 1 Complete!"<<endl;
        }
        
        else if(arg.compare("S1_R2") == 0)
        {
            Scheme1_Round2_send input = Scheme1_Round2_send(message);
            Scheme1_Round2_receive output = k.Scheme1_Round2(input);
            result = output.toString();
            cout<<"Scheme 1 Round 2 Complete!"<<endl;
        }

        else if(arg.compare("S0") == 0)
        {
            Scheme0_send input = Scheme0_send(message);
            Scheme0_receive output = k.Scheme0_Round1(input);
            result = output.toString();
            cout<<"Scheme 0 Complete!"<<endl;
        }
        
        send(new_socket , result.c_str() , result.length() , 0 );

	cout <<"Done, closing connection"<<endl;
	
        close(new_socket);
    } 

    return 0;
}
