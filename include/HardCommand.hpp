#pragma once

class HardCommand{
    public:
    int m_ID;   
    int m_array[10];
    time_t seconds = time(NULL);
    tm* timeinfo = localtime(&seconds);
    void print(){
        std::cout<<asctime(timeinfo); 
        std::cout<<"ID "<< m_ID << std::endl;
        for (int i = 0; i<10; i++){
            std::cout<< m_array[i];
        };
        std::cout<< std::endl;
        };
        
HardCommand (const int id,const int (&b)[10]){
    this->m_ID = id;
    for (int i = 0; i<10; i++){
        this-> m_array[i] = b[i];
    };
         };
HardCommand () = default;
~HardCommand () = default;

};