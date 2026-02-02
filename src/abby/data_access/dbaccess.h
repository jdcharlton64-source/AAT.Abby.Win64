#ifndef __DATA_ACCESS_H__
#define __DATA_ACCESS_H__

#include <string>

class DataAccess
{
public:
    // kiosk
    static std::string getKiosk();
    static std::string updateKiosk(int id, std::string name, int identifier, short languageId, int phidgetHubSerialNumber, std::string facility);
    static std::string getKioskKeyTagIds();
    static std::string addKioskKeyTagId(std::string tagId);
    static std::string truncateKioskKeyTagIds();
    static std::string getAccessToken();
    static std::string updateAccessToken(std::string token, std::string expiryDate);

    // residents
    static std::string getResidents();
    static bool residentExists(int id);
    static bool residentsAny();
    static std::string getResident(int id);
    static std::string updateResident(int id, std::string firstName, std::string lastName, std::string gender, short languageId, short gameDifficultyLevel, int bluetoothBeaconId, std::string uwbTagId, std::string profilePicture);
    static std::string addResident(int id, std::string firstName, std::string lastName, std::string gender, short languageId, short gameDifficultyLevel, int bluetoothBeaconId, std::string uwbTagId, std::string profilePicture);
    static std::string deleteResident(int id);

    // ambient invitations
    static std::string getAmbientInvitations();
    static std::string updateAmbientInvitation(int id, std::string message, bool isExecuteRandom);
    static std::string deleteAmbientInvitation(int id);

    // media path type descriptions
    static std::string getMediaPathTypeDescs();
    static std::string updateMediaPathTypeDesc(int id, std::string description, std::string shortDescription, std::string longDescription, std::string summaryLabel, std::string summaryLabelPlural);

    // text resources
    static std::string getTextResources();
    static bool textResourceExists(int id);
    static std::string getTextResource(int id);
    static std::string updateTextResource(int id, std::string text);

    // interactive activity descriptions
    static std::string getInteractiveActivityTypes();
    static std::string getInteractiveActivityDescs();
    static std::string updateInteractiveActivityDesc(int id, std::string description);

    // playlists
    static std::string getPlaylistFiles();
    static bool playlistFileExists(int mediaFileId);
    static std::string getPlaylistFile(int id);
    static std::string getPlaylistFilesByMediaFileId(int mediaFileId);
    static std::string getPlaylistMediaPathTypes(int residentId);
    static std::string updatePlaylistFile(int id, int residentId, std::string title);
    static std::string addPlaylistFile(int residentId, short mediaPathTypeId, short responseTypeId, int mediaFileId, std::string filename, std::string title, bool isLinked);
    static std::string deletePlaylistFile(int id, int residentId);

    // event logs
    static std::string getEventLogs(short eventLogTypeId = -1);
    static std::string addEventLog(short eventLogTypeId, std::string description);

    // ui
    static std::string getPlaylistFilesForResponseType(int residentId = -1, short responseTypeId = -1, short mediaPathTypeId = -1);
    static std::string getActiveConfigDetails();
    static std::string getResponseType(short id);
    static std::string getResponseTypes(bool filterRandom = false, bool filterRotational = false);
    static std::string getMediaPathTypes();

    // gadgets
    static std::string getGadgetTypes();

    // phidgets
    static std::string getPhidgets();
};

#endif // __DATA_ACCESS_H__