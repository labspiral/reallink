/*
 *
 *                                                            ,--,      ,--,
 *             ,-.----.                                     ,---.'|   ,---.'|
 *   .--.--.   \    /  \     ,---,,-.----.      ,---,       |   | :   |   | :      ,---,           ,---,.
 *  /  /    '. |   :    \ ,`--.' |\    /  \    '  .' \      :   : |   :   : |     '  .' \        ,'  .'  \
 * |  :  /`. / |   |  .\ :|   :  :;   :    \  /  ;    '.    |   ' :   |   ' :    /  ;    '.    ,---.' .' |
 * ;  |  |--`  .   :  |: |:   |  '|   | .\ : :  :       \   ;   ; '   ;   ; '   :  :       \   |   |  |: |
 * |  :  ;_    |   |   \ :|   :  |.   : |: | :  |   /\   \  '   | |__ '   | |__ :  |   /\   \  :   :  :  /
 *  \  \    `. |   : .   /'   '  ;|   |  \ : |  :  ' ;.   : |   | :.'||   | :.'||  :  ' ;.   : :   |    ;
 *   `----.   \;   | |`-' |   |  ||   : .  / |  |  ;/  \   \'   :    ;'   :    ;|  |  ;/  \   \|   :     \
 *   __ \  \  ||   | ;    '   :  ;;   | |  \ '  :  | \  \ ,'|   |  ./ |   |  ./ '  :  | \  \ ,'|   |   . |
 *  /  /`--'  /:   ' |    |   |  '|   | ;\  \|  |  '  '--'  ;   : ;   ;   : ;   |  |  '  '--'  '   :  '; |
 * '--'.     / :   : :    '   :  |:   ' | \.'|  :  :        |   ,/    |   ,/    |  :  :        |   |  | ;
 *   `--'---'  |   | :    ;   |.' :   : :-'  |  | ,'        '---'     '---'     |  | ,'        |   :   /
 *             `---'.|    '---'   |   |.'    `--''                              `--''          |   | ,'
 *               `---`            `---'                                                        `----'
 *
 * 2025 Copyright to (c)SpiralLAB. All rights reserved.
 * Description : RealLink cpp client
 * Author : hong chan, choi / hcchoi@spirallab.co.kr (http://spirallab.co.kr)
 *
 */

#ifndef REALLINKCPP_CLIENT_H
#define REALLINKCPP_CLIENT_H

#include "reallinkcpplibcfg.h"
#include "reallinkvalue.h"
#include <functional>
#include <vector>

#ifdef __cplusplus
extern "C"
{
#endif

namespace reallinkcpp
{

	class RealLinkClientPimpl;

	/// <summary>
	/// RealLinkClient for c++ client
	/// </summary>
	class REALLINKCPP_LIB_DLL RealLinkClient
	{
	public:
		typedef std::function<void __cdecl (const std::vector<reallinkcpp::value>&)> MethodHandler;

		/// <summary>
		/// Start(or try to connect)
		/// <para>
		/// Must be executed <c>spirallab.reallink.server.exe</c> at first. <br/>
		/// Synchronous(or blocking) operation. <br/>
		/// </para>
		/// </summary>
		/// <returns>Success to connect to server or not</returns>
		bool __stdcall Start();
		/// <summary>
		/// Stop(or disconnect)
		/// <para>
		/// Synchronous(or blocking) operation. <br/>
		/// </para>
		/// </summary>
		/// <returns>Success to disconnect to server or not</returns>
		bool __stdcall Stop();
		/// <summary>
		/// Is connected to <c>spirallab.reallink.server.exe</c> or not
		/// <para>
		/// Synchronous(or blocking) operation. <br/>
		/// </para>
		/// </summary>
		/// <returns>Is connected to server or not</returns>
		bool __stdcall IsConnected();
		/// <summary>
		/// Reconnect(try to reconnect)
		/// <para>
		/// Synchronous(or blocking) operation. <br/>
		/// </para>
		/// </summary>
		/// <returns>Success to reconnect to server or not</returns>
		bool __stdcall Reconnect();
		/// <summary>
		/// Register call-back function
		/// </summary>
		/// <param name="methodName">Server method name</param>
		/// <param name="handler">Callback handler</param>
		void __stdcall On(const char* lpMethodName, const MethodHandler& handler);

		/// <summary>
		/// Send(or notify) data to client(s)
		/// <para>
		/// Synchronous(or blocking) operation. <br/>
		/// </para>
		/// </summary>
		/// <param name="userName">User(or client) name.<br/>"all", "broadcast" or "others" are reserved.</param>
		/// <param name="message">Send message</param>
		/// <param name="v">Send object by value (string, double, array, map, ...)</param>
		/// <returns>Returned <c>False</c>, if client(or username) is not exist.<br/>Returned <c>True</c> is not guarantee success of sending data. <br/></returns>
		bool __stdcall Send(const char* lpUserName, const char* lpMessage, const value& v);

		/// <summary>
		/// Send data to client and wait for response data
		/// <para>
		/// Synchronous(or blocking) operation. <br/>
		/// </para>
		/// </summary>
		/// <param name="userName">User(or client) name.<br/>"all", "broadcast" or "others" are not allowed.</param>
		/// <param name="message">Send message</param>
		/// <param name="v">Send object by value (string, double, array, map, ...)</param>
		/// <returns>Returned(or responsed) <c>value</c>.<br/>If <c>null</c> if client(or username) is not exist.<br/></returns>
		value __stdcall Request(const char* lpUserName, const char* lpMessage, const value& v);

		///// <summary>
		///// Response data to client  
		///// </summary>
		///// <param name="userName">User(or client) name.<br/>Must be same user by from.</param>
		///// <param name="v">Send object by value (string, double, array, map, ...)</param>
		///// <param name="ticketNo">Received ticket value</param>
		//void __stdcall Response(const char* lpUserName, const value& v, double ticketNo);
	private:
		RealLinkClientPimpl* _pPimpl;

	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="lpszUrl">URI<br/>For example: http://locahost:5000</param>
		/// <param name="lpszHubName">Hub name<br/>For example: reallink1</param>
		/// <param name="lpszUserName">Client user name<br/>'all', 'broadcast' or 'others' are reserved.</param>
		RealLinkClient(const char* lpUrl, const char* lpHubName, const char* lpUserName);
		virtual ~RealLinkClient();
	};

	/// <summary>
	/// Create reallink client
	/// </summary>
	/// <param name="lpszUrl">URI<br/>For example: http://locahost:5000</param>
	/// <param name="lpszHubName">Hub name<br/>For example: reallink1</param>
	/// <param name="lpszUserName">Client user name<br/>'all', 'broadcast' or 'others' are reserved.</param>
	/// <returns><c>RealLinkClient</c></returns>
	REALLINKCPP_LIB_DLL RealLinkClient* __stdcall CreateRealLink(const char* lpUrl = "http://locahost:5000", const char* lpHubName = "reallink1", const char* lpUserName = "cpp");
	/// <summary>
	/// Destory reallink client
	/// </summary>
	/// <param name="ppClient"><c>RealLinkClient</c></param>
	REALLINKCPP_LIB_DLL void __stdcall DestroyRealLink(RealLinkClient** ppClient);

}//namespace


#ifdef __cplusplus
}
#endif

#endif