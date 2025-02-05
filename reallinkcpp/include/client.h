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
		/// </para>
		/// </summary>
		void __stdcall Start();
		/// <summary>
		/// Stop(or disconnect)
		/// </summary>
		void __stdcall Stop();
		/// <summary>
		/// Is connected to reallink server or not
		/// </summary>
		/// <returns></returns>
		bool __stdcall IsConnected();
		/// <summary>
		/// Reconnect(try to reconnect)
		/// </summary>
		/// <returns></returns>
		bool __stdcall Reconnect();
		/// <summary>
		/// Register call-back function
		/// </summary>
		/// <param name="methodName">Server method name</param>
		/// <param name="handler">Callback function</param>
		void __stdcall On(const char* methodName, const MethodHandler& handler);
		/// <summary>
		/// Send(or notify) message and value(s) to specific client(s)
		/// </summary>
		/// <param name="userName">User(or client) name.<br/>"all", "broadcast" or "others" are reserved.</param>
		/// <param name="message">Sending message</param>
		/// <param name="v">Sending object by value (string, double, array, map, ...)</param>
		/// <returns>If <c>false</c>, user(or client) is not exist.</returns>
		bool __stdcall Send(const char* userName, const char* message, const value& v);
		/// <summary>
		/// Send message and value(s) to specific client(s) and waiting for return 
		/// </summary>
		/// <param name="userName">User(or client) name.<br/>"all", "broadcast" or "others" are reserved.</param>
		/// <param name="message">Sending message</param>
		/// <param name="v">Sending object by value (string, double, array, map, ...)</param>
		/// <returns>Returned kvalue</returns>
		value __stdcall SendForReturn(const char* lpszUserName, const char* lpszMessage, const value& v);

	private:
		RealLinkClientPimpl* _pPimpl;

	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="lpszUrl">URI<br/>For example: http://locahost:5000</param>
		/// <param name="lpszHubName">Hub name<br/>For example: reallink1</param>
		/// <param name="lpszUserName">Client user name<br/>Must be contain "handler", seq", "cpp" or "c++" string for c++ client.<br/>'all', 'broadcast' or 'others' are reserved.</param>
		RealLinkClient(const char* lpszUrl, const char* lpszHubName, const char* lpszUserName);
		virtual ~RealLinkClient();
	};

	/// <summary>
	/// Create reallink client
	/// </summary>
	/// <param name="lpszUrl">URI<br/>For example: http://locahost:5000</param>
	/// <param name="lpszHubName">Hub name<br/>For example: reallink1</param>
	/// <param name="lpszUserName">Client user name<br/>Must be contain "handler", seq", "cpp" or "c++" string for c++ client.<br/>'all', 'broadcast' or 'others' are reserved.</param>
	/// <returns><c>RealLinkClient</c></returns>
	REALLINKCPP_LIB_DLL RealLinkClient* __stdcall CreateRealLink(const char* lpszUrl = "http://locahost:5000", const char* lpszHubName = "reallink1", const char* lpszUserName = "cpp");
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