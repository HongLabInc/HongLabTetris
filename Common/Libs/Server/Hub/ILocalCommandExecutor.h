#pragma once

class ILocalCommandExecutor
{
public:
	ILocalCommandExecutor() = default;
	virtual ~ILocalCommandExecutor() = 0;

protected:
};

inline ILocalCommandExecutor::~ILocalCommandExecutor() = default;
