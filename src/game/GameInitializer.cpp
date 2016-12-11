#include "GameInitializer.h"

#include "main/Player.h"
#include "script/ScriptClassParser.h"

using namespace MX;

using namespace std;

void Boro::GameInitializer::Init()
{
	Script::onParsed.static_connect([&]() { AfterScriptParse(); });
	AfterScriptParse();

}


void Boro::GameInitializer::AfterScriptParse()
{
	ScriptClassParser::AddCreator(L"Boro.Player", new OutsideScriptClassCreatorContructor<Boro::Player>()); 
	ScriptClassParser::AddCreator(L"Boro.Controller.Key", new OutsideScriptClassCreatorContructor<Boro::KeyController>()); 
	ScriptClassParser::AddCreator(L"Boro.Controller.AlwaysLeft", new OutsideScriptClassCreatorContructor<Boro::AlwaysLeftController>()); 
}
