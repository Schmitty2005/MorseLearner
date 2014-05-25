##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=MorseLearner
ConfigurationName      :=Debug
WorkspacePath          := "C:\CppCode\MorseLearner\MorseLearner"
ProjectPath            := "C:\CppCode\MorseLearner\MorseLearner"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Schmitt
Date                   :=01/30/14
CodeLitePath           :="C:\Program Files (x86)\CodeLite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="MorseLearner.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=windres
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch).. $(IncludeSwitch)C:/cppLibs/pa_stable_v19_20111121/include $(IncludeSwitch). $(IncludeSwitch)C:/cppLibs/pa_stable_v19_20111121/include $(IncludeSwitch)c:/cpplibs/portaudio/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)portaudio $(LibrarySwitch)portaudio 
ArLibs                 :=  "portaudio" "portaudio" 
LibPath                := $(LibraryPathSwitch).C:/cppLibs/pa_stable_v19_20111121/lib $(LibraryPathSwitch)C:/cppLibs/pa_stable_v19_20111121/lib $(LibraryPathSwitch)c:/cppLibs/portaudio/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
UNIT_TEST_PP_SRC_DIR:=C:\UnitTest++-1.3
Srcs=main.cpp PortAudio.cpp 

Objects0=$(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/PortAudio$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/*.*
	$(RM) $(OutputFile)
	$(RM) $(OutputFile).exe
	$(RM) ".build-debug/MorseLearner"


