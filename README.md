# Caustic
Caustic is my personal library that I use for building various graphics applications. 
This is a code base that I have worked on and off for the past several 
years. I am currently porting it from my internal git repository to this 
repository and in the process cleaning up a bunch of the code. 
The code is currently licensed under the MIT License. See file LICENSE for details.

## Building
In order to build the solution you need to edit Caustic.props and modify \<CausticRoot> to point to the folder that contains the source.  
```
setx CausticRoot "c:\GitHub\Caustic" /M
```
  
You will also need to set \<CausticFXCPath> to point to where the FXC shader compiler lives:  
                 e.g. c:\Program Files (x86)\Windows Kits\10\bin\10.0.18362.0\x64\fxc.exe  
```
setx CausticFXCPath "c:\Program Files (x86)\Windows Kits\10\bin\10.0.18362.0\x64\fxc.exe" /M
```
  
You will need to set \<AzureKinectSDKDir> to point to your Azure Kinect SDK folder (e.g. "C:\Program Files\Azure Kinect SDK v1.4.0")  
```
setx AzureKinectSDKDir "C:\Program Files\Azure Kinect SDK v1.4.0" /M
```

You will need to install:
=========================
ImGui into $(CausticRoot)\External:
```
cd %CausticRoot%\External
git clone https://github.com/ocornut/imgui.git
cd github
git checkout docking
```

WPFDXInterop into $(CausticRoot)\External. This library is used for integrating WPF and Caustic:
```
cd %CausticRoot%\External
git clone https://github.com/microsoft/WPFDXInterop.git
```

Git for Windows: https://gitforwindows.org
    
NaturalDocs: https://github.com/NaturalDocs/NaturalDocs
You will need to manually load and build both the CLI and Engine projects. Build both the Debug and Release versions.
```
cd %CausticRoot%\External
git clone https://github.com/NaturalDocs/NaturalDocs.git
```

Azure Kinect SDK: https://docs.microsoft.com/en-us/azure/kinect-dk/sensor-sdk-download  
Azure Kinect Body Tracking: https://docs.microsoft.com/en-us/azure/kinect-dk/body-sdk-download  
.Net SDK 4.7: https://dotnet.microsoft.com/download/dotnet-framework/thank-you/net47-developer-pack-offline-installer

## Documentation
Documentation for the library can be found at https://pat-sweeney.github.io/Caustic/
