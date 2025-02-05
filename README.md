# REALLINK 

**1. Descriptions**

 REALLINK is library for IPC(inter-process communication) bewteen cross platform clients remotely

![logo](https://github.com/user-attachments/assets/6600dc0d-21b5-4cef-bfc2-137149e24c67)

![server](https://github.com/user-attachments/assets/5e5d2b11-d8d2-4a3b-ad36-2a51c4c5c01f)


----


**2. Major features**

 - Realtime communication between clients by Microsoft SignalR web service.
 - Can be passing arguments with serialized JSON object for remote clients.
 - Support cross platform environment.
 - Support asynchronous operation by async/await. 
 - Can be monitored and logged communication messages into file.


----


**3. Environment**
 
 - Support various client runtime environments.
    - c++ (x64)
    - Winforms and WPF (.NET Framework 4.7.2 or higher)
    - .NET8 (or higher)
  
   
----


**5. Libraries**

 - spirallab.reallink.client.dll
    - Library for .NET client
    - Runtime: Windows .NET8 or higher, .NET Framework 4.7.2 or higher
 - spirallab.reallinkcpp.dll 
    - Library for c++ client
    - Runtime: Windows c++ x64
    - Edit '$(VCPKG)\include\nlohmann' for VCPKG path
    - Used Multi-byte character set and toolset as v143 
 - spirallab.reallink.server.exe
    - Reallink server program
    - Runtime: .NET8
    - Must be run at first (require license dongle key to execute)


----


**6. How to use ?**

 - Must be execute reallink server "spirallab.reallink.server.exe" program at first.
    - download latest version: http://spirallab.co.kr/reallink/spirallab.reallink.server.zip
    - execute "spirallab.reallink.server.exe"
    - optional arguments: "spirallab.reallink.server.exe" [hub name] [http port] [https port] [max buffer size]
    - default hub name: "reallink1"
    - default http, https port: 5001, 5002
    - default max. buffer size: 1048576 (= 1Mbytes)
 - For .NET clients
    - add reference "spirallab.reallink.client.dll" file into project
 - For c++ clients
    - include "include\reallinkcpplib.h" file into project
    - config search file path for "spirallab.reallinkcpp.lib" or "spirallab.reallinkcppD.lib" at project
    - copy all dll files at "bin\cpp\Debug" or "bin\cpp\Release" to your working directory
    
    
     
 ----


**7. Examples**

 - Demo 'console app' console project for .NET framework c# client 
 - Demo 'cpp app' console project for c++ client example
 - Demo 'winforms app' project for .NET framework winforms c# client
 - Demo 'wpf app' console project for .NET framework WPF c# client
 

----

  
**8. Copyrights**
 
 - Homepage: http://spirallab.co.kr
 - Email: <a href="mailto:hcchoi@spirallab.co.kr">hcchoi@spirallab.co.kr</a> 
 - All rights reserved. 2018-2025 Copyright to (c)SpiralLAB. 
 - License dongle key must be exist to execute reallink server program.
   

----


**9. Version history**

* 2025.2.5 v1.0
   - first commit 
