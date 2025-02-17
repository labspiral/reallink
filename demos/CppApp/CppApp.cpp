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
    // m[0]: from (string)
    // m[1]: message (string)
    // m[2]: object (std::map)
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
    else
	{
		std::cout << "Unknown message: " << message << std::endl;
	}
}

// c++ client is not supported 'OnResponse' callback function
// 
//void __cdecl OnResponse(const std::vector<reallinkcpp::value>& m)
//{
//    // m[0]: from (string)
//    // m[1]: message (string)
//	// m[2]: object (std::map)
//    // m[3]: ticket no (double)
//    assert(m.size() == 3 + 1); //with ticket no
//
//    std::string from = m[0].as_string();
//    std::string message = m[1].as_string();
//    //std::map value_map = m[2].as_map();
//    double ticketNo = m[3].as_double();
//
//    std::cout << "Response: " << message << " From " << from << std::endl;
//
//    if (message == "substrate")
//    {
//        SUBSTRATE sub;
//        sub.Name = m[2].as_string();
//        sub.Rows = 1;
//        sub.Cols = 1;
//        sub.Units.push_back(UNIT(0, 0, 100));
//        //...
//        nlohmann::json j;
//        sub.Serialize(j);
//        //std::cout << j.dump(2) << std::endl;
//        value v = JSONToRealLinkValue(j);
//        try
//        {
//            //username, object, ticket number
//            pRealLink->Response(from.c_str(), v, ticketNo);
//        }
//        catch (const std::exception& e)
//        {
//            std::cerr << e.what() << std::endl;
//        }
//    }
//    else
//    {
//        std::cout << "Unknown message: " << message << std::endl;
//    }
//}

bool SendSubstrate(std::string name)
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

    try
    {
        bool success = pRealLink->Send("consoleapp", "substrate", v);
        if (!success)
            std::cerr << "Failed to send substrate" << std::endl;
        return success;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool QuerySubstrate(std::string name)
{
    try
    {
        value v = pRealLink->Request("consoleapp", "substrate", value(name));
        if (!v.is_map())
            return false;
        nlohmann::json j = RealLinkValueToJSON(v);
        std::cout << j.dump(2) << std::endl;

        SUBSTRATE sub;
        sub.Deserialize(j);
        //...
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

void RegisterCallBacks()
{
    pRealLink->On("Receive", &OnReceive);
    // c++ client is not supported 'Response' callback function
    //pRealLink->On("Response", &OnResponse);
}


int main()
{
    pRealLink = reallinkcpp::CreateRealLink("http://localhost:5000", "reallink1", "cppapp");
    RegisterCallBacks();

    int i = 0;
    char buf[255] = { 0, };
    bool exit = false;

    do
    {
        std::cout << "Demo for RealLink cpp client - (c)SpiralLAB" << std::endl;
        std::cout << "'1' : start" << std::endl;
        std::cout << "'2' : reconnect" << std::endl;
        std::cout << "'3' : send" << std::endl;
        std::cout << "'4' : response" << std::endl;
        std::cout << "'5' : stop" << std::endl;
        std::cout << "'Q' : quit" << std::endl;
        std::cout << "Select your target : ";
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
            sprintf_s(buf, "SUBSTRATE_TESTNAME_%d", i++);
            SendSubstrate(buf);
            break;
        case '4':
            sprintf_s(buf, "SUBSTRATE_TESTNAME_%d", i);
            QuerySubstrate(buf);
            break;
        case '5':
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

