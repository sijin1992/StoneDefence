// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleScreenLoading.h"
#include "SScreenLoading.h"
#include "MoviePlayer.h"

#define LOCTEXT_NAMESPACE "FSimpleScreenLoadingModule"

void FSimpleScreenLoadingModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSimpleScreenLoadingModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FSimpleScreenLoadingModule::SetupScreenLoading()
{
	//安装Loading到GameInstance, GameInstance不会因为关卡切换而销毁
	//预加载地图代理
	FCoreUObjectDelegates::PreLoadMap.AddRaw(this, &FSimpleScreenLoadingModule::BeginLoadingScreen);
	//加载完成后通知代理
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddRaw(this, &FSimpleScreenLoadingModule::EndLoadingScreen);
}

void FSimpleScreenLoadingModule::BeginLoadingScreen(const FString& MapName)
{
	FLoadingScreenAttributes LoadingScreenArg;
	LoadingScreenArg.bAutoCompleteWhenLoadingCompletes = false;//false加载完成资源可以自动跳过开场动画 true加载好资源后还要等待动画播放完成才会进入下一个场景
	LoadingScreenArg.bMoviesAreSkippable = true;//动画是否可以跳过,上面设置为false时，该选项无意义
	LoadingScreenArg.bWaitForManualStop = false; //动画播放完会自动循环，直至资源加载完成
	LoadingScreenArg.PlaybackType = EMoviePlaybackType::MT_LoadingLoop;//枚举，播放完循环
	LoadingScreenArg.WidgetLoadingScreen = SNew(SScreenLoading, MapName);//创建实力
	//使用MoviePlayer模块
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenArg);
	GetMoviePlayer()->PlayMovie();
}

void FSimpleScreenLoadingModule::EndLoadingScreen(UWorld* World)
{

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleScreenLoadingModule, SimpleScreenLoading)