// Copyright 2022 Dreamingpoet All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UnrealExtensionsType.generated.h"

/*文件数据结构体*/
USTRUCT(BlueprintType)
struct UNREALEXTENSIONS_API FFileData
{
	GENERATED_USTRUCT_BODY()

	/*文件或文件夹的创建时间*/
	UPROPERTY(BlueprintReadOnly)
		FDateTime CreationTime;
	/*文件或文件夹的访问时间*/
	UPROPERTY(BlueprintReadOnly)
		FDateTime AccessTime;
	/*文件或文件夹的修改时间*/
	UPROPERTY(BlueprintReadOnly)
		FDateTime ModificationTime;
	/*文件或文件夹的大小*/
	UPROPERTY(BlueprintReadOnly)
		int64 FileSize;
	/*是否为目录*/
	UPROPERTY(BlueprintReadOnly)
		bool IsDirectory;
	/*是否为只读*/
	UPROPERTY(BlueprintReadOnly)
		bool IsReadOnly;
	/*是否有效*/
	UPROPERTY(BlueprintReadOnly)
		bool bIsValid;

};