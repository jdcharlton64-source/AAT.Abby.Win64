#include <iostream>
#include <nlohmann/json.hpp>
#include <SQLiteCpp/SQLiteCpp.h>
#include "dbaccess.h"
#include <optional>

// database
const std::filesystem::path DATABASE_FOLDER = "D:\\srv";
const std::filesystem::path DATABASE_NAME = "aat_abby.db3";
const std::filesystem::path DATABASE_PATH = DATABASE_FOLDER / DATABASE_NAME;

using json = nlohmann::json;

// helpers
std::string getCurrentDateString(bool longString = false)
{
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;

    if (!longString)
    {
        ss << std::put_time(std::localtime(&currentTime_t), "%m-%d-%Y");
        return ss.str();
    }
    else
    {
        auto now = std::chrono::system_clock::now();
        std::tm* currentTime_tm = std::localtime(&currentTime_t);
        ss << std::put_time(currentTime_tm, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
}

// kiosk
std::string DataAccess::getKiosk()
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT id, kioskId, name, languageId, identifier, phidgetHubSerialNumber, facility FROM kiosk");

        q.executeStep();

        int id = q.getColumn(0);
        int kioskId = q.getColumn(1);
        std::string name = q.getColumn(2);
        short languageId = q.getColumn(3);
        short identifier = q.getColumn(4);
        int phidgetHubSerialNumber = q.getColumn(5);
        std::string facility = q.getColumn(6);

        json j = {
            {"id", id},
            {"kioskId", kioskId},
            {"name", name},
            {"languageId", languageId},
            {"identifier", identifier},
            {"phidgetHubSerialNumber", phidgetHubSerialNumber},
            {"facility", facility}
        };

        result = j.dump();
        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();

    }

    return result;
}

std::string DataAccess::updateKiosk(int id, std::string name, int identifier, short languageId, int phidgetHubSerialNumber, std::string facility)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Transaction transaction(db);

        SQLite::Statement q(db, "UPDATE kiosk SET kioskId = ?, name = ?, identifier = ?, languageId = ?, phidgetHubSerialNumber = ? , facility = ? WHERE id = 1");

        q.bind(1, id);
        q.bind(2, name);
        q.bind(3, identifier);
        q.bind(4, languageId);
        q.bind(5, phidgetHubSerialNumber);
        q.bind(6, facility);
        q.exec();

        transaction.commit();

        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::getKioskKeyTagIds()
{
    std::string result;
    json tagIds = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT tagId FROM kioskKeyTagId");

        while (q.executeStep()) {
            std::string tagId = q.getColumn(0);

            json j = {
                {"tagId", tagId}
            };

            tagIds.push_back(j);
        }

        result = tagIds.dump();
        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();

    }

    return result;
}

std::string DataAccess::addKioskKeyTagId(std::string tagId)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Transaction transaction(db);

        SQLite::Statement q(db, "INSERT INTO kioskKeyTagId(tagId) VALUES (?)");

        q.bind(1, tagId);
        q.exec();

        transaction.commit();

        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::truncateKioskKeyTagIds()
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Transaction transaction(db);

        SQLite::Statement q(db, "DELETE FROM kioskKeyTagId");

        q.exec();

        transaction.commit();
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::getAccessToken()
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT accessToken, tokenExpiryDate FROM kiosk WHERE id = 1");

        q.executeStep();

        std::string token = q.getColumn(0);
        std::string expiryDate = q.getColumn(1);

        json j = {
            {"accessToken", token},
            {"expiryDate", expiryDate}
        };

        result = j.dump();
        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::updateAccessToken(std::string token, std::string expiryDate)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Transaction transaction(db);

        SQLite::Statement q(db, "UPDATE kiosk SET accessToken = ?, tokenExpiryDate = ? WHERE id = 1");

        q.bind(1, token);
        q.bind(2, expiryDate);
        q.exec();

        transaction.commit();

        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

// residents
std::string DataAccess::getResidents()
{
    std::string result;
    json residents = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT id, firstName, lastName, gender, languageId, gameDifficultyLevel, bluetoothBeaconId, uwbTagId, profilePicture, dateCreated, dateUpdated FROM residents");

        while (q.executeStep()) {
            int id = q.getColumn(0);
            std::string firstName = q.getColumn(1);
            std::string lastName = q.getColumn(2);
            std::string gender = q.getColumn(3);
            short languageId = q.getColumn(4);
            short gameDifficultyLevel = q.getColumn(5);
            std::optional<int> bluetoothBeaconId = q.getColumn(6).isNull()
                ? std::nullopt
                : std::optional<int>(q.getColumn(6).getInt());
            std::optional<std::string> uwbTagId = q.getColumn(7).isNull()
                ? std::nullopt
                : std::optional<std::string>(q.getColumn(7).getString());
            std::optional<std::string> pictureBase64 = q.getColumn(8).isNull()
                ? std::nullopt
                : std::optional<std::string>(q.getColumn(8).getString());
            std::string profilePicture = pictureBase64.has_value()
                ? "data:image/png;base64, " + pictureBase64.value()
                : "";
            std::string dateCreated = q.getColumn(9);
            std::string dateUpdated = q.getColumn(10);

            json j = {
                {"id", id},
                {"container", ""},
                {"firstName", firstName},
                {"lastName", lastName},
                {"gender", gender},
                {"languageId", languageId},
                {"gameDifficultyLevel", gameDifficultyLevel},
                {"bluetoothBeaconId", bluetoothBeaconId},
                {"uwbTagId", uwbTagId},
                {"profilePicture", profilePicture},
                {"createDate", dateCreated},
                {"updateDate", dateUpdated}
            };

            residents.push_back(j);
        }
        result = residents.dump();
        return result;
    }
    catch (const std::runtime_error& e)
    {
        result = e.what();
    }

    return result;
}

bool DataAccess::residentExists(int id)
{
    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT COUNT(0) FROM residents WHERE id = ?");

        q.bind(1, id);
        q.executeStep();

        return q.getColumn(0).getInt() > 0;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what();
    }

    return false;
}

bool DataAccess::residentsAny()
{
    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT COUNT(*) FROM residents");

        q.executeStep();

        return q.getColumn(0).getInt() > 0;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what();
    }

    return false;
}

std::string DataAccess::getResident(int id)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT id, firstName, lastName, gender, languageId, gameDifficultyLevel, bluetoothBeaconId, uwbTagId, profilePicture, dateCreated, dateUpdated FROM residents WHERE id = ?");

        q.bind(1, id);

        q.executeStep();

        int id = q.getColumn(0);
        std::string firstName = q.getColumn(1);
        std::string lastName = q.getColumn(2);
        std::string gender = q.getColumn(3);
        short languageId = q.getColumn(4);
        short gameDifficultyLevel = q.getColumn(5);
        std::optional<int> bluetoothBeaconId = q.getColumn(6);
        std::optional<std::string> uwbTagId = q.getColumn(7).isNull()
            ? std::nullopt
            : std::optional<std::string>(q.getColumn(7).getString());
        std::optional<std::string> profilePicture = q.getColumn(8).isNull()
            ? std::nullopt
            : std::optional<std::string>(q.getColumn(8).getString());
        std::string dateCreated = q.getColumn(9);
        std::string dateUpdated = q.getColumn(10);

        json j = {
            {"id", id},
            {"container", ""},
            {"firstName", firstName},
            {"lastName", lastName},
            {"gender", gender},
            {"languageId", languageId},
            {"gameDifficultyLevel", gameDifficultyLevel},
            {"bluetoothBeaconId", bluetoothBeaconId},
            {"uwbTagId", uwbTagId},
            {"profilePicture", profilePicture},
            {"createDate", dateCreated},
            {"updateDate", dateUpdated}
        };

        result = j.dump();
        return result;
    }
    catch (const std::runtime_error& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::updateResident(int id, std::string firstName, std::string lastName, std::string gender, short languageId, short gameDifficultyLevel, int bluetoothBeaconId, std::string uwbTagId, std::string profilePicture)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "UPDATE residents SET firstName = ?, lastName = ?, gender = ?, languageId = ?, gameDifficultyLevel = ?, bluetoothBeaconId = ?, uwbTagId = ?, profilePicture = ?, dateUpdated = ? WHERE id = ?");

        SQLite::Transaction transaction(db);

        std::string dateUpdated = getCurrentDateString();

        q.bind(1, firstName);
        q.bind(2, lastName);
        q.bind(3, gender);
        q.bind(4, languageId);
        q.bind(5, gameDifficultyLevel);
        q.bind(6, bluetoothBeaconId);
        q.bind(7, uwbTagId);
        q.bind(8, profilePicture);
        q.bind(9, dateUpdated);
        q.bind(10, id);

        q.exec();

        transaction.commit();

        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::addResident(int id, std::string firstName, std::string lastName, std::string gender, short languageId, short gameDifficultyLevel, int bluetoothBeaconId, std::string uwbTagId, std::string profilePicture)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "INSERT INTO residents (id, firstName, lastName, gender, languageId, gameDifficultyLevel, bluetoothBeaconId, uwbTagId, profilePicture, dateCreated, dateUpdated) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

        SQLite::Transaction transaction(db);

        std::string dateCreated = getCurrentDateString();

        q.bind(1, id);
        q.bind(2, firstName);
        q.bind(3, lastName);
        q.bind(4, gender);
        q.bind(5, languageId);
        q.bind(6, gameDifficultyLevel);
        q.bind(7, bluetoothBeaconId);
        q.bind(8, uwbTagId);
        q.bind(9, profilePicture);
        q.bind(10, dateCreated);
        q.bind(11, dateCreated);
        q.exec();

        transaction.commit();

        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::deleteResident(int id)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "DELETE FROM residents WHERE id = ?");

        SQLite::Transaction transaction(db);

        q.bind(1, id);
        q.exec();

        transaction.commit();

        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

// ambient invitations
std::string DataAccess::getAmbientInvitations()
{
    std::string result;
    json invitations = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT id, message, isExecuteRandom FROM AmbientInvitations");

        while (q.executeStep()) {
            int id = q.getColumn(0);
            std::string message = q.getColumn(1);
            short executeRandom = q.getColumn(2);
            bool isExecuteRandom = executeRandom == 1;

            json j = {
                {"id", id},
                {"message", message},
                {"isExecuteRandom", isExecuteRandom}
            };

            invitations.push_back(j);
        }

        result = invitations.dump();
        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::updateAmbientInvitation(int id, std::string message, bool isExecuteRandom)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "UPDATE AmbientInvitations SET message = ?, isExecuteRandom = ? WHERE id = ?");

        SQLite::Transaction transaction(db);

        q.bind(1, message);
        q.bind(2, isExecuteRandom ? "1" : "0");
        q.bind(3, id);
        q.exec();

        transaction.commit();

        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::deleteAmbientInvitation(int id)
{
    std::string result;
    json invitations = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "DELETE FROM ambientInvitations WHERE id = ?");

        SQLite::Transaction transaction(db);

        q.bind(1, id);
        q.exec();

        transaction.commit();

        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

// media path types
std::string DataAccess::getMediaPathTypeDescs()
{
    std::string result;
    json pathTypeDescs = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT p.id, p.mediaPathTypeCategoryId, c.description category, p.path, p.description, p.shortDescription, p.longDescription, p.summaryLabel, p.summaryLabelPlural, p.responseTypeId, p.isSystem, p.isSharable FROM mediaPathTypes p INNER JOIN MediaPathTypeCategories c ON p.mediaPathTypeCategoryId = c.id");

        while (q.executeStep()) {
            short id = q.getColumn(0);
            int categoryId = q.getColumn(1);
            std::string category = q.getColumn(2);
            std::string path = q.getColumn(3);
            std::string description = q.getColumn(4);
            std::string shortDescription = q.getColumn(5);
            std::string longDescription = q.getColumn(6);
            std::string summaryLabel = q.getColumn(7);
            std::string summaryLabelPlural = q.getColumn(8);
            int responseTypeId = q.getColumn(9);
            short system = q.getColumn(10);
            bool isSystem = system == 1;
            short sharable = q.getColumn(11);
            bool isSharable = sharable == 1;

            json j = {
                {"id", id},
                {"mediaPathTypeCategory", {
                    {"id", categoryId},
                    {"description", category}
                }},
                {"path", path},
                {"description", description},
                {"shortDescription", shortDescription},
                {"longDescription", longDescription},
                {"summaryLabel", summaryLabel},
                {"summaryLabelPlural", summaryLabelPlural},
                {"responseTypeId", responseTypeId},
                {"isSystem", isSystem},
                {"isSharable", isSharable}
            };

            pathTypeDescs.push_back(j);
        }

        result = pathTypeDescs.dump();
        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::updateMediaPathTypeDesc(int id, std::string description, std::string shortDescription, std::string longDescription, std::string summaryLabel, std::string summaryLabelPlural)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Transaction transaction(db);

        SQLite::Statement q(db, "UPDATE MediaPathTypes SET description = ?, shortDescription = ?, longDescription = ?, summaryLabel = ?, summaryLabelPlural = ? WHERE id = ?");

        q.bind(1, description);
        q.bind(2, shortDescription);
        q.bind(3, longDescription);
        q.bind(4, summaryLabel);
        q.bind(5, summaryLabelPlural);
        q.bind(6, id);
        q.exec();

        transaction.commit();

        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

// text resources
std::string DataAccess::getTextResources()
{
    std::string result;
    json textResources = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT id, text FROM textResources");

        while (q.executeStep()) {
            int id = q.getColumn(0);
            std::string text = q.getColumn(1);

            json j = {
                {"id", id},
                {"text", text}
            };

            textResources.push_back(j);
        }

        result = textResources.dump();
        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

bool DataAccess::textResourceExists(int id)
{
    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT COUNT(0) FROM textResources WHERE id = ?");

        q.bind(1, id);
        q.executeStep();

        return q.getColumn(0).getInt() > 0;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what();
    }

    return false;
}

std::string DataAccess::getTextResource(int id)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT id, text FROM textResources WHERE Id = ?");

        q.bind(1, id);

        q.executeStep();

        int id = q.getColumn(0);
        std::string text = q.getColumn(1);

        json j = {
            {"id", id},
            {"text", text}
        };

        result = j.dump();

        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::updateTextResource(int id, std::string text)
{
    std::string result;
    json invitations = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "UPDATE textResources SET text = ? WHERE id = ?");

        SQLite::Transaction transaction(db);

        q.bind(1, text);
        q.bind(2, id);
        q.exec();

        transaction.commit();

        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

// interactive activity types
std::string DataAccess::getInteractiveActivityTypes()
{
    std::string result;
    json activityTypes = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT id, description, url FROM interactiveActivityTypes");

        while (q.executeStep()) {
            int id = q.getColumn(0);
            std::string description = q.getColumn(1);
            std::string url = q.getColumn(2);

            json j = {
                {"id", id},
                {"description", description},
                {"url", url}
            };

            activityTypes.push_back(j);
        }

        result = activityTypes.dump();
        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::getInteractiveActivityDescs()
{
    std::string result;
    json activityDescs = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT id, description FROM interactiveActivityTypes");

        while (q.executeStep()) {
            int id = q.getColumn(0);
            std::string description = q.getColumn(1);

            json j = {
                {"id", id},
                {"description", description}
            };

            activityDescs.push_back(j);
        }

        result = activityDescs.dump();
        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::updateInteractiveActivityDesc(int id, std::string description)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "UPDATE interactiveActivityTypes SET description = ? WHERE id = ?");

        SQLite::Transaction transaction(db);

        q.bind(1, description);
        q.bind(2, id);
        q.exec();

        transaction.commit();

        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

// playlistFiles
std::string DataAccess::getPlaylistFiles()
{
    std::string result;
    json playlistFiles = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT id, residentId, mediaPathTypeId, responseTypeId, mediaFileId, filename, title, isLinked, dateCreated, dateUpdated FROM playlistFiles");

        while (q.executeStep()) {
            int id = q.getColumn(0);
            int residentId = q.getColumn(1);
            short mediaPathTypeId = q.getColumn(2);
            short responseTypeId = q.getColumn(3);
            int mediaFileId = q.getColumn(4);
            std::string filename = q.getColumn(5);
            std::string title = q.getColumn(6);
            short linked = q.getColumn(7);
            bool isLinked = linked == 1;
            std::string dateCreated = q.getColumn(8);
            std::string dateUpdated = q.getColumn(9);

            json j = {
                {"id", id},
                {"residentId", residentId},
                {"mediaPathTypeId", mediaPathTypeId},
                {"responseTypeId", responseTypeId},
                {"mediaFileId", mediaFileId},
                {"filename", filename},
                {"title", title},
                {"isLinked", isLinked},
                {"dateCreated", dateCreated},
                {"dateUpdated", dateUpdated}
            };

            playlistFiles.push_back(j);
        }
        result = playlistFiles.dump();
        return result;
    }
    catch (const std::runtime_error& e)
    {
        result = e.what();
    }

    return result;
}

bool DataAccess::playlistFileExists(int mediaFileId)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT COUNT(0) FROM playlistFiles WHERE mediaFileId = ?");

        q.bind(1, mediaFileId);

        q.executeStep();

        return q.getColumn(0).getInt() > 0;
    }
    catch (const std::runtime_error& e)
    {
        result = e.what();
    }

    return false;
}

std::string DataAccess::getPlaylistFile(int id)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT id, residentId, mediaPathTypeId, mediaTypeCategory mediaCategory, responseTypeId, responseTypeCategory responseCategory, mediaFileId, filename, title, isLinked, path, dateCreated FROM playlistFilesExpanded WHERE id = ?");

        q.bind(1, id);

        q.executeStep();

        int id = q.getColumn(0);
        int residentId = q.getColumn(1);
        short mediaPathTypeId = q.getColumn(2);
        std::string mediaCategory = q.getColumn(3);
        short responseTypeId = q.getColumn(4);
        std::string responseCategory = q.getColumn(5);
        int mediaFileId = q.getColumn(6);
        std::string filename = q.getColumn(7);
        std::string title = q.getColumn(8);
        short linked = q.getColumn(9);
        bool isLinked = linked == 1;
        std::string path = q.getColumn(10);
        std::string dateCreated = q.getColumn(11);

        json j = {
            {"id", id},
            {"residentId", residentId},
            {"mediaPathTypeId", mediaPathTypeId},
            {"mediaCategory", mediaCategory},
            {"responseTypeId", responseTypeId},
            {"responseCategory", responseCategory},
            {"mediaFileId", mediaFileId},
            {"filename", filename},
            {"title", title},
            {"isLinked", isLinked},
            {"path", path},
            {"dateCreated", dateCreated}
        };

        result = j.dump();
        return result;
    }
    catch (const std::runtime_error& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::getPlaylistFilesByMediaFileId(int mediaFileId)
{
    std::string result;
    json playlistFiles = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT id, residentId, mediaPathTypeId, responseTypeId, mediaFileId, filename, title, isLinked, dateCreated, dateUpdated FROM playlistFiles WHERE mediaFileId = ?");

        while (q.executeStep()) {
            int id = q.getColumn(0);
            int residentId = q.getColumn(1);
            short mediaPathTypeId = q.getColumn(2);
            short responseTypeId = q.getColumn(3);
            int mediaFileId = q.getColumn(4);
            std::string filename = q.getColumn(5);
            std::string title = q.getColumn(6);
            short linked = q.getColumn(7);
            bool isLinked = linked == 1;
            std::string dateCreated = q.getColumn(8);
            std::string dateUpdated = q.getColumn(9);

            json j = {
                {"id", id},
                {"residentId", residentId},
                {"mediaPathTypeId", mediaPathTypeId},
                {"responseTypeId", responseTypeId},
                {"mediaFileId", mediaFileId},
                {"filename", filename},
                {"title", title},
                {"isLinked", isLinked},
                {"dateCreated", dateCreated},
                {"dateUpdated", dateUpdated}
            };

            playlistFiles.push_back(j);
        }

        result = playlistFiles.dump();
        return result;
    }
    catch (const std::runtime_error& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::getPlaylistMediaPathTypes(int residentId)
{
    std::string result;
    json playlistPathTypes = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT distinct mediaPathTypeId FROM playlistFiles WHERE residentId = ? ORDER BY mediaPathTypeId");

        q.bind(1, residentId);

        while (q.executeStep()) {
            int mediaPathTypeId = q.getColumn(0);

            json j = {
                {"mediaPathTypeId", mediaPathTypeId}
            };

            playlistPathTypes.push_back(j);
        }
        result = playlistPathTypes.dump();
        return result;
    }
    catch (const std::runtime_error& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::updatePlaylistFile(int id, int residentId, std::string title)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "UPDATE playlistFiles SET title = ? WHERE id = ?");

        SQLite::Transaction transaction(db);

        q.bind(1, title);
        q.bind(2, id);
        q.exec();

        transaction.commit();

        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::addPlaylistFile(int residentId, short mediaPathTypeId, short responseTypeId, int mediaFileId, std::string filename, std::string title, bool isLinked)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "INSERT INTO playlistFiles (residentId, mediaPathTypeId, responseTypeId, mediaFileId, filename, title, isLinked, dateCreated, dateUpdated) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");

        SQLite::Transaction transaction(db);

        std::string dateCreated = getCurrentDateString();

        q.bind(1, residentId);
        q.bind(2, mediaPathTypeId);
        q.bind(3, responseTypeId);
        q.bind(4, mediaFileId);
        q.bind(5, filename);
        q.bind(6, title);
        q.bind(7, isLinked);
        q.bind(8, dateCreated);
        q.bind(9, dateCreated);
        q.exec();

        transaction.commit();

        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::deletePlaylistFile(int id, int residentId)
{
    std::string result;
    json invitations = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "DELETE FROM playlistFiles WHERE id = ?");

        SQLite::Transaction transaction(db);

        q.bind(1, id);
        q.exec();

        transaction.commit();

        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

// ui
std::string DataAccess::getPlaylistFilesForResponseType(int residentId, short responseTypeId, short mediaPathTypeId)
{
    std::string result;
    json playlistFiles = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT id, mediaFileId, filename, title, residentId, mediaPathTypeId, responseTypeId, dateCreated, isLinked, path, mediaTypeCategory mediaCategory, responseTypeCategory responseCategory, residentName FROM playlistFilesExpanded WHERE (residentId = ? OR ? < 0) AND (responseTypeId = ? OR ? < 0) AND (mediaPathTypeId = ? OR ? < 0) ORDER BY residentId, responseTypeId, mediaPathTypeId, title");

        SQLite::Transaction transaction(db);

        q.bind(1, residentId);
        q.bind(2, residentId);
        q.bind(3, responseTypeId);
        q.bind(4, responseTypeId);
        q.bind(5, mediaPathTypeId);
        q.bind(6, mediaPathTypeId);

        while (q.executeStep()) {
            int id = q.getColumn(0);
            int mediaFileId = q.getColumn(1);
            std::string filename = q.getColumn(2);
            std::string title = q.getColumn(3);
            int residentId = q.getColumn(4);
            short mediaPathTypeId = q.getColumn(5);
            short responseTypeId = q.getColumn(6);
            std::string dateCreated = q.getColumn(7);
            short linked = q.getColumn(8);
            bool isLinked = linked == 1;
            std::string path = q.getColumn(9);
            std::string mediaCategory = q.getColumn(10);
            std::string responseCategory = q.getColumn(11);
            std::string residentName = q.getColumn(12);

            json j = {
                {"id", id},
                {"mediaFileId", mediaFileId},
                {"filename", filename},
                {"title", title},
                {"residentId", residentId},
                {"mediaPathTypeId", mediaPathTypeId},
                {"responseTypeId", responseTypeId},
                {"dateCreated", dateCreated},
                {"isLinked", isLinked},
                {"path", path},
                {"mediaCategory", mediaCategory},
                {"responseCategory", responseCategory},
                {"residentName", residentName}
            };

            playlistFiles.push_back(j);
        }
        result = playlistFiles.dump();
        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::getActiveConfigDetails()
{
    json configDetails = json::array();
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT id, configId, isActive, config, description, gadgetType, gadgetId, phidgetTypeId, phidgetId, gadgetStyleId, gadgetStyle, isIncremental, responseTypeId, responseType FROM configDetailsExpanded WHERE isActive = 1");

        while (q.executeStep()) {

            int id = q.getColumn(0);
            int configId = q.getColumn(1);
            short active = q.getColumn(2);
            bool isActive = active == 1;
            std::string config = q.getColumn(3);
            std::string description = q.getColumn(4);
            std::string gadgetType = q.getColumn(5);
            short gadgetId = q.getColumn(6);
            short phidgetTypeId = q.getColumn(7);
            short phidgetId = q.getColumn(8);
            short gadgetStyleId = q.getColumn(9);
            std::string gadgetStyle = q.getColumn(10);
            short incremental = q.getColumn(11);
            bool isIncremental = incremental == 1;
            short responseTypeId = q.getColumn(12);
            std::string responseType = q.getColumn(13);

            json j = {
                {"id", id},
                {"configId", configId},
                {"isActive", isActive},
                {"config", config},
                {"description", description},
                {"gadgetType", gadgetType},
                {"gadgetId", gadgetId},
                {"phidgetTypeId", phidgetTypeId},
                {"phidgetId", phidgetId},
                {"gadgetStyleId", gadgetStyleId},
                {"gadgetStyle", gadgetStyle},
                {"isIncremental", isIncremental},
                {"responseTypeId", responseTypeId},
                {"responseType", responseType}
            };

            configDetails.push_back(j);
        }

        result = configDetails.dump();
        return result;
    }
    catch (const std::runtime_error& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::getResponseType(short id)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT id, responseTypeCategoryId, description, isRandom, isRotational, isUninterruptable, interactiveActivityTypeId, url FROM responseTypesExpanded WHERE id = ?");

        q.bind(1, id);

        q.executeStep();

        short id = q.getColumn(0);
        short responseTypeCategoryId = q.getColumn(1);
        std::string description = q.getColumn(2);
        short random = q.getColumn(3);
        bool isRandom = random == 1;
        short rotational = q.getColumn(4);
        bool isRotational = rotational == 1;
        short uninterruptable = q.getColumn(5);
        bool isUninterruptable = uninterruptable == 1;
        int interactiveActivityTypeId = q.getColumn(6);
        std::string url = q.getColumn(7);

        json j = {
            {"id", id},
            {"responseTypeCategoryId", responseTypeCategoryId},
            {"description", description},
            {"interactiveActivityTypeId", interactiveActivityTypeId},
            {"isRandom", isRandom},
            {"isRotational", isRotational},
            {"isUninterruptable", isUninterruptable},
            {"url", url}
        };

        result = j.dump();
        return result;
    }
    catch (const std::runtime_error& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::getResponseTypes(bool filterRandom, bool filterRotational)
{
    json responseTypes = json::array();
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        short isRandom = filterRandom ? 1 : 0;
        short isRotational = filterRotational ? 1 : 0;

        SQLite::Statement q(db, "SELECT id, responseTypeCategoryId, description, isRandom, isRotational, isUninterruptable, interactiveActivityTypeId, url FROM responseTypesExpanded WHERE (isRandom = ? OR ? = 0) OR (isRotational = ? OR ? = 0)");

        q.bind(1, isRandom);
        q.bind(2, isRandom);

        q.bind(3, isRotational);
        q.bind(4, isRotational);

        while (q.executeStep()) {

            short id = q.getColumn(0);
            short responseTypeCategoryId = q.getColumn(1);
            std::string description = q.getColumn(2);
            short random = q.getColumn(3);
            bool isRandom = random == 1;
            short rotational = q.getColumn(4);
            bool isRotational = rotational == 1;
            short uninterruptable = q.getColumn(5);
            bool isUninterruptable = uninterruptable == 1;
            int interactiveActivityTypeId = q.getColumn(6);
            std::string url = q.getColumn(7);

            json j = {
                {"id", id},
                {"responseTypeCategoryId", responseTypeCategoryId},
                {"description", description},
                {"interactiveActivityTypeId", interactiveActivityTypeId},
                {"isRandom", isRandom},
                {"isRotational", isRotational},
                {"isUninterruptable", isUninterruptable},
                {"url", url}
            };

            responseTypes.push_back(j);
        }

        result = responseTypes.dump();
        return result;
    }
    catch (const std::runtime_error& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::getMediaPathTypes()
{
    json mediaPathTypes = json::array();
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT id, mediaPathTypeCategoryId, responseTypeId, path, description, shortDescription, longDescription, summaryLabel, summaryLabelPlural, isSystem, isSharable, sortOrder, category FROM mediaPathTypesExpanded ORDER BY sortOrder");

        while (q.executeStep()) {

            short id = q.getColumn(0);
            short mediaPathTypeCategoryId = q.getColumn(1);
            short responseTypeId = q.getColumn(2);
            std::string path = q.getColumn(3);
            std::string description = q.getColumn(4);
            std::string shortDescription = q.getColumn(5);
            std::string longDescription = q.getColumn(6);
            std::string summaryLabel = q.getColumn(7);
            std::string summaryLabelPlural = q.getColumn(8);
            short system = q.getColumn(9);
            bool isSystem = system == 1;
            short sharable = q.getColumn(10);
            bool isSharable = sharable == 1;
            int sortOrder = q.getColumn(11);
            std::string category = q.getColumn(12);

            json j = {
                {"id", id},
                {"mediaPathTypeCategoryId", mediaPathTypeCategoryId},
                {"responseTypeId", responseTypeId},
                {"path", path},
                {"description", description},
                {"shortDescription", shortDescription},
                {"longDescription", longDescription},
                {"summaryLabel", summaryLabel},
                {"summaryLabelPlural", summaryLabelPlural},
                {"isSystem", isSystem},
                {"isSharable", isSharable},
                {"sortOrder", sortOrder},
                {"category", category}
            };

            mediaPathTypes.push_back(j);
        }

        result = mediaPathTypes.dump();
        return result;
    }
    catch (const std::runtime_error& e)
    {
        result = e.what();
    }

    return result;
}

// event logs
std::string DataAccess::getEventLogs(short eventLogTypeId)
{
    std::string result;
    json eventLogs = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT el.id, el.eventLogTypeId, t.description eventLogType, el.description FROM eventLogs el LEFT JOIN eventLogTypes t ON el.eventLogTypeId = t.id WHERE (el.eventLogTypeId = ? OR ? < 0)");

        q.bind(1, eventLogTypeId);
        q.bind(2, eventLogTypeId);

        while (q.executeStep()) {
            int id = q.getColumn(0);
            short eventLogTypeId = q.getColumn(1);
            std::string eventLogType = q.getColumn(2);
            std::string description = q.getColumn(3);

            json j = {
                {"id", id},
                {"eventLogTypeId", eventLogTypeId},
                {"eventLogType", eventLogType},
                {"description", description}
            };

            eventLogs.push_back(j);
        }

        result = eventLogs.dump();
        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

std::string DataAccess::addEventLog(short eventLogTypeId, std::string description)
{
    std::string result;

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "INSERT INTO eventLogs (eventLogTypeId, description, dateCreated) VALUES (?, ?, ?)");

        SQLite::Transaction transaction(db);

        std::string dateCreated = getCurrentDateString(true);

        q.bind(1, eventLogTypeId);
        q.bind(2, description);
        q.bind(3, dateCreated);
        q.exec();

        transaction.commit();

        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

// gadgets
std::string DataAccess::getGadgetTypes()
{
    std::string result;
    json gadgetTypes = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT id, description FROM gadgetTypes");

        while (q.executeStep()) {
            int id = q.getColumn(0);
            std::string description = q.getColumn(1);

            json j = {
                {"id", id},
                {"description", description}
            };

            gadgetTypes.push_back(j);
        }

        result = gadgetTypes.dump();
        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

// phidgets
std::string DataAccess::getPhidgets()
{
    std::string result;
    json phidgets = json::array();

    try
    {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);

        SQLite::Statement q(db, "SELECT p.id, p.isHubPort, p.port, p.channel, p.phidgetTypeId, p.keyMap, p.description, CASE WHEN cd.id IS NOT NULL THEN 1 ELSE 0 END isConfigured FROM phidgets p LEFT JOIN configDetails cd ON p.id = cd.phidgetId");

        while (q.executeStep()) {
            short id = q.getColumn(0);
            short hubPort = q.getColumn(1);
            bool isHubPort = hubPort == 1;
            short port = q.getColumn(2);
            short channel = q.getColumn(3);
            short phidgetTypeId = q.getColumn(4);
            std::string keyMap = q.getColumn(5);
            std::string description = q.getColumn(6);
            short configured = q.getColumn(7);
            bool isConfigured = configured == 1;

            json j = {
                {"id", id},
                {"isHubPort", isHubPort},
                {"port", port},
                {"channel", channel},
                {"phidgetTypeId", phidgetTypeId},
                {"keyMap", keyMap},
                {"description", description},
                {"isConfigured", isConfigured}
            };

            phidgets.push_back(j);
        }

        result = phidgets.dump();
        return result;
    }
    catch (std::exception& e)
    {
        result = e.what();
    }

    return result;
}

/*
int main()
{
    std::string result;

    // result = DataAccess::getAllMediaFiles();
    // result = DataAccess::getPlaylistFilesForResponseType(3);
    result = DataAccess::getEventLogs();

    std::cout << result;

    return 0;
}
*/
