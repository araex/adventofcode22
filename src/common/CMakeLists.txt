set(targetName "common")
add_library(${targetName} 
	STATIC "include/common/FileReader.h" "src/FileReader.cpp")
target_include_directories(${targetName}
	PUBLIC "include"
	PRIVATE "include/common"
)