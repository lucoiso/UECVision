// Developer: Lucas Vilas-Boas
// Year: 2022

#include "VisionGameModeBase.h"
#include "Vision_Char.h"

AVisionGameModeBase::AVisionGameModeBase()
{
	DefaultPawnClass = AVision_Char::StaticClass();
}