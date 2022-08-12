#pragma once

enum class CheatStatus
{
	UpToDate = 0,
	Outdated = 1,
	Detected = 2
};

struct Cheat
{
	int CurrentStream = 0;
	
	int ID;
	int GameID;
	std::vector<std::string> ReleaseStreams;
	std::string Name;
	int StartsAt;
	CheatStatus Status;
	std::string ExpiresOn;

	Cheat(int id, int gameID, std::vector<std::string> releaseStreams, std::string name, int startsAt, CheatStatus status, std::string expiresOn)
	{
		ID = id;
		GameID = gameID;
		ReleaseStreams = releaseStreams;
		Name = name;
		StartsAt = startsAt;
		Status = status;
		ExpiresOn = expiresOn;
	}
};