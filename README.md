# REALLINK 
> REALLINK¢â is library for IPC(inter-process communication) bewteen remote clients 
![logo](https://github.com/user-attachments/assets/6600dc0d-21b5-4cef-bfc2-137149e24c67)

## 1. Features
- Realtime communication between clients by Microsoft SignalR¢â web service.
- Can be passing arguments with serialized JSON object for remote clients.
- Support asynchronous operation by async/await. 
- Support cross platform environment.
- Can be monitored and logged communication messages into file. 
 

## 2. Environment
Support various client runtime environments.
- C# with .NET Framework 4.7.2 or higher
- C# with .NET8 or higher
- C++ with v143 toolset and x64 only

      
## 3. Libraries
![server](https://github.com/user-attachments/assets/5e5d2b11-d8d2-4a3b-ad36-2a51c4c5c01f)
spirallab.reallink.server.exe
- Download latest version: http://spirallab.co.kr/download/spirallab.reallink.server.zip
- RealLink server program must be executed at first 
- Runtime: .NET8
 
spirallab.reallink.client.dll
- Library for .NET client
- Runtime: Windows .NET8 or higher, .NET Framework 4.7.2 or higher
 
spirallab.reallinkcpp.dll 
- Library for C++ client
- Runtime: Windows C++ x64
- Used Multi-byte character set and toolset as v143 

 
## 4. How to use ?
Must be execute reallink server "spirallab.reallink.server.exe" program at first.
- Optional arguments: "spirallab.reallink.server.exe" [hub name] [http port] [https port] [max buffer size]
- Default hub name: "reallink1"
- Default http, https port: 5001, 5002
- Default max. buffer size: 1048576 (= 1Mbytes)
 
For .NET clients
- Add reference "spirallab.reallink.client.dll" file into project
 
For c++ clients
- Include "include\reallinkcpplib.h" file into project
- Config search file path at project for "spirallab.reallinkcpp.lib" or "spirallab.reallinkcppD.lib"
- Copy all dll files at "bin\cpp\Debug" or "bin\cpp\Release" to your working directory
    

## 5. Examples
- Demo 'winforms app' winforms client project by .NET framework and C#
- Demo 'wpf app' wpf client project by .NET8 with WPF and C# 
- Demo 'console app' console client project by .NET framework and C# 
- Demo 'cpp app' console project by C++ 
 
 
## 6. Core Methods
```
    client        |       server       |     client
       'A'        |                    |        'B'
                  |                    |
                  |                    |
Invoke('B', __.   |                    |
    message,    \ |                    |
    object)      \|                    |
                  |----  "Send"  ----->|---> On("Receive")
                  |                    |     {
                  |                    |      ...
                  |                    |     }
                  .                    .         
                  .                    .
                  .                    .     
  Invoke('B', _   .                    . 
      message,  \ |                    |
      object)    \|                    |
                  |---- "Request" ---->|---> On("Response")
                  |                    |     { 
                  |                    |         return ...;
                .-|<-------------------|<--- }
               /  |                    |
  await...  __/   |                    |
                  |                    |
```


## 7. Server Hub APIs
```
// Server side:
public async Task<bool> Send(string userName, string message, object arg);
// Client side:
public Task Receive(string userName, string message, object arg);

// Server side:
public async Task<object> Request(string userName, string message, object arg);
// Client side:
public Task<object> Response(string userName, string message, object arg);
```


## 8. For .NET client
```
var client = new SpiralLab.RealLink.Client("http://localhost:5000", "hubname", "username");
client.On("Receive", 
    new[] { typeof(string), typeof(string), typeof(object) }, 
    (args, state) => 
    { 
        ... 
    });
client.On("Response", 
    new[] { typeof(string), typeof(string), typeof(object) }, 
    (args, state) => 
    { 
        ... 
    });
client.StartAsync();
...
var success = await client.InvokeAsync<bool>("Send", "username", "command", object);
var response = await client.InvokeAsync<TResult>("Request", "username", "command", object);    
```


## 9. For C++ client
```
reallinkcpp::RealLinkClient* client = reallinkcpp::CreateRealLink("http://localhost:5000", "hubname", "username");
void __cdecl OnReceive(const std::vector<reallinkcpp::value>& v) 
{ 
    ... 
}
client->On("Receive", &OnReceive);
//"Request" is not supported at C++ client
client->Start();
...
bool success = client->Send("username", "command", reallinkcpp::value(arg));
reallinkcpp::value response = client->Request("username", "command", reallinkcpp::value(arg));
```
 

## 10. Copyrights
- Homepage: http://spirallab.co.kr
- Email: <a href="mailto:hcchoi@spirallab.co.kr">hcchoi@spirallab.co.kr</a> 
- Evaluation copy mode would be activated during 30 mins without license for server program.
- All rights reserved. 2018-2025 Copyright to (c)SpiralLAB. 
  
 
## 11. Version history
* 2025.2.17 v1.1.80.0
   - fixed) server
      - change) server hub APIs 
      - added) checking duplicated client name has connected
      - added) compare version info when client has connected
   - fixed) c++ client 
      - removed) ${VCPKG} dependency to build c++ client
      - added) nlohmann json library for c++ client
* 2025.2.5 v1.0.0.0
   - first commit 
