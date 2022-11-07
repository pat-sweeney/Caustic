# Caustic
Caustic is my personal library that I use for building various graphics applications. 
This is a code base that I have worked on and off for the past several 
years. I am currently porting it from my internal git repository to this 
repository and in the process cleaning up a bunch of the code. 
The code is currently licensed under the MIT License. See file LICENSE for details.

## Building
You must use Microsoft Visual Studio Version 17.0.0 to compile this.
You can download this version of MSVS from https://docs.microsoft.com/en-us/visualstudio/releases/2022/release-history
Version 17.2.6 (most recent version as of this writing) seems to have broken module support and no longer compiles the code.

In order to build the solution you need to edit Caustic.props and modify the following variables to point to the folder that contains the source:
```
 \<GithubFolder> - should point to the drive and folder where you're Github enlistment resides
```
Also check the paths (such as $(AzureKinectSDKDir) and other paths pointing at c:\Program Files\... to make sure they are correct
for your system.
 
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
Git for Windows: https://gitforwindows.org
    
ImGui into $(CausticRoot)\External:
```
cd %CausticRoot%\External
git clone https://github.com/ocornut/imgui.git
cd imgui
git checkout docking
```

ImPlot into $(CausticRoot)\External:
```
cd %CausticRoot%\External
git clone https://github.com/epezent/implot.git
```

WPFDXInterop into $(CausticRoot)\External. This library is used for integrating WPF and Caustic:
```
cd %CausticRoot%\External
git clone https://github.com/microsoft/WPFDXInterop.git
```

NaturalDocs: https://github.com/NaturalDocs/NaturalDocs
You will need to manually load and build both the CLI and Engine projects. Build both the Debug and Release versions.
```
cd %CausticRoot%\External
git clone https://github.com/NaturalDocs/NaturalDocs.git
```

Azure Kinect SDK: https://docs.microsoft.com/en-us/azure/kinect-dk/sensor-sdk-download  
Azure Kinect Body Tracking: https://docs.microsoft.com/en-us/azure/kinect-dk/body-sdk-download  
.Net SDK 4.7: https://dotnet.microsoft.com/download/dotnet-framework/thank-you/net47-developer-pack-offline-installer

If you wish to compile WebcamToNDI you will need to have the NDI Advanced SDK installed.
The SDK can be found at:
https://www.ndi.tv/sdk/
You will also need to define an environment variabled named 'UseNDI'

## Documentation
Documentation for the library can be found at https://pat-sweeney.github.io/Caustic/
