#include <Windows.h>
#include <future>
#include <iostream>
#include <string>
#include <conio.h> 
#include "substrate.h"
#include "jsonhelper.h"
#include "..\..\reallinkcpp\include\reallinkcpplib.h"


reallinkcpp::RealLinkClient* pRealLink = nullptr;


void __cdecl OnReceive(const std::vector<reallinkcpp::value>& m)
{
    assert(m.size() == 3);
    std::string from = m[0].as_string();
    std::string message = m[1].as_string();
    std::cout << "Received: " << message << " From " << from << std::endl;

    if (message == "substrate")
    {
        assert(m[2].is_map());
        nlohmann::json j = RealLinkValueToJSON(m[2].as_map());
        //std::cout << j.dump(2) << std::endl;

        SUBSTRATE sub;
        bool success = sub.Deserialize(j);
        // do something
    }
}

void __cdecl OnReceiveForReturn(const std::vector<reallinkcpp::value>& m)
{
    assert(m.size() == 3 + 1); //with ticket no

    std::string from = m[0].as_string();
    std::string message = m[1].as_string();
    std::cout << "Received: " << message << " From " << from << std::endl;

    if (message == "substrate")
    {
        assert(m[2].is_map());
        nlohmann::json j = RealLinkValueToJSON(m[2].as_map());
        //std::cout << j.dump(2) << std::endl;

        SUBSTRATE sub;
        bool success = sub.Deserialize(j);

        double ticket = m[3].as_double();
        value v = JSONToRealLinkValue(j);
        std::vector<value> args{ v, ticket };
    }
}

void CreateSubstrateAndSend(std::string name)
{
    SUBSTRATE sub;
    sub.Name = name;
    sub.Rows = 1;
    sub.Cols = 2;
    sub.Units.push_back(UNIT(0, 1, 2));
    sub.Units.push_back(UNIT(3, 4, 100));

    nlohmann::json j;
    sub.Serialize(j);

    std::cout << j.dump(2) << std::endl;
    value v = JSONToRealLinkValue(j);

    nlohmann::json jj = RealLinkValueToJSON(v);
    std::cout << jj.dump(2) << std::endl;

    bool success = pRealLink->Send("winformapp", "substrate", v);
    if (!success)
        std::cerr << "Failed to send substrate" << std::endl;
}

bool QuerySubstrate(std::string name)
{
    value v = pRealLink->SendForReturn("winformapp", "substrate", value(name));
    if (!v.is_map())
        return false;

    nlohmann::json j = RealLinkValueToJSON(v);
    std::cout << j.dump(2) << std::endl;

    SUBSTRATE sub;
    sub.Deserialize(j);
    //...
    return true;
}



int main()
{
    pRealLink = reallinkcpp::CreateRealLink("http://localhost:5000", "reallink1", "cppapp");
    pRealLink->On("Receive", &OnReceive);
    pRealLink->On("ReceiveForReturn", &OnReceiveForReturn);

    int i = 0;
    char buf[255] = { 0, };
    bool exit = false;

    do
    {
        printf("Demo for RealLink cpp client - (c)SpiralLAB\r\n");
        printf("'1' : start\r\n");
        printf("'2' : reconnect\r\n");
        printf("'3' : send\r\n");
        printf("'4' : stop\r\n");
        printf("'Q' : quit\r\n");
        printf("Select your target : ");
		char ch = _getch();
        std::cout << std::endl;
        switch (ch)
        {
        case 'q':
        case 'Q':
            exit = true;
            break;
        case '1':
			std::cout << "Starting Reallink Client 'cpp app' is running and try to connect" << std::endl;
            try
            {
                pRealLink->Start();
            }
            catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
            break;
        case '2':
            std::cout << "Reconnecting Reallink ..." << std::endl;
            try
            {
                pRealLink->Reconnect();
            }
            catch (const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
            break;
        case '3':
            sprintf_s(buf, "SUBSTRATE_TESTNAME_%d", i);
            CreateSubstrateAndSend(buf);
            break;
        case '4':
            try
            {
                pRealLink->Stop();
            }
            catch (const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
            break;
        }
    } while (!exit);
   
    reallinkcpp::DestroyRealLink(&pRealLink);
    return 0;
}

