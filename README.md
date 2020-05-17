# Caustic
Caustic is my personal library that I use for building various graphics applications. 
This is a code base that I have worked on and off for the past several 
years. I am currently porting it from my internal git repository to this 
repository and in the process cleaning up a bunch of the code. 
The code is currently licensed under the MIT License. See file LICENSE for details.

## Building
In order to build the solution you need to edit Caustic.props and modify \<CausticRoot> to point to the folder that contains the source.  
You will also need to set \<CausticFXPath> to point to where the FXC shader compiler lives:  
                 e.g. c:\Program Files (x86)\Windows Kits\10\bin\10.0.18362.0\x64\fxc.exe  
Lastly, you will need to set \<AzureKinectSDKDir> to point to your Azure Kinect SDK folder (e.g. "C:\Program Files\Azure Kinect SDK v1.4.0")  
You will need to install:  
NaturalDocs: https://www.naturaldocs.org/download/  
Azure Kinect SDK: https://docs.microsoft.com/en-us/azure/kinect-dk/sensor-sdk-download  

## Documentation
Documentation for the library can be found at https://pat-sweeney.github.io/Caustic/
