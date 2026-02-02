//#include "media_query.h"
//#include <dbaccess.h>
//#include <algorithm>  // for std::shuffle
//#include <chrono>     // for seeding the random number generator
//#include <fstream>
//#include <nlohmann/json.hpp>
//#include <random>  // for random number generation
//#include <sstream>
//#include <vector>
//#include "constants.h"
//
//using nlohmann::json;
//
//namespace media_query {
//const std::filesystem::path EXEC_PATH =
//    std::filesystem::current_path().string();
//const std::filesystem::path MEDIA_ROOT = "/srv/_aatabbymedia";
//const std::filesystem::path CONTENT_PATH = EXEC_PATH / "content";
//const std::filesystem::path ACTIVITIES_PATH = EXEC_PATH / "activities";
//
//// randomize the lists with std::mt19937 for a Mersenne Twister engine (using hi
//// res)
//std::mt19937 rng(
//    std::chrono::high_resolution_clock::now().time_since_epoch().count());
//
//// for interactive activities (games)
//std::filesystem::path _activityUrl;
//int _kioskId;
//int _languageId;
//bool _isScopeCaregiver;
//std::string _residentName;
//short _difficultyLevel;
//int _timeout;
//
//void from_json(const json& j, PlaylistFile& plf) {
//  j.at("id").get_to(plf.id);
//  j.at("filename").get_to(plf.filename);
//  j.at("title").get_to(plf.title);
//  j.at("residentId").get_to(plf.residentId);
//  j.at("mediaPathTypeId").get_to(plf.mediaPathTypeId);
//  j.at("responseTypeId").get_to(plf.responseTypeId);
//  j.at("dateCreated").get_to(plf.dateCreated);
//  j.at("isLinked").get_to(plf.isLinked);
//  j.at("path").get_to(plf.path);
//  j.at("mediaCategory").get_to(plf.mediaCategory);
//  j.at("responseCategory").get_to(plf.responseCategory);
//  j.at("residentName").get_to(plf.residentName);
//}
//
//int GetFileCount(std::vector<PlaylistFile>& playlistFiles,
//                 int residentId,
//                 short mediaPathTypeId = -1) {
//  int numFiles = 0;
//  if (playlistFiles.size() > 0) {
//    for (const auto& item : playlistFiles) {
//      if (mediaPathTypeId > 0) {
//        if (item.mediaPathTypeId == mediaPathTypeId)
//          numFiles++;
//      } else {
//        numFiles++;
//      }
//    }
//  }
//  return numFiles;
//}
//
//std::vector<std::string> GetAssembledFileList(
//    std::vector<PlaylistFile>& playlistFiles,
//    int residentId,
//    short mediaPathTypeId) {
//  std::vector<std::string> list;
//  if (playlistFiles.size() > 0) {
//    for (const auto& item : playlistFiles) {
//      if (item.mediaPathTypeId == mediaPathTypeId || mediaPathTypeId < 0) {
//        std::filesystem::path profileFolder =
//            item.isLinked ? "SharedLibrary/"
//                          : "Profiles/" + std::to_string(residentId) + "/";
//        std::filesystem::path mediaPath = item.path + "/";
//        std::filesystem::path fullPath =
//            MEDIA_ROOT / profileFolder / mediaPath / item.filename;
//        list.push_back(fullPath.string());
//      }
//    }
//  }
//  return list;
//}
//
//std::string VectorToCommaSeparatedString(const std::vector<std::string>& vec) {
//  std::stringstream ss;
//  ss << "\"";
//
//  for (size_t i = 0; i < vec.size(); ++i) {
//    ss << vec[i];
//    if (i < vec.size() - 1) {  // Add comma for all but the last element
//      ss << "\",\"";
//    }
//  }
//  ss << "\"";
//  return ss.str();
//}
//
//std::vector<PlaylistFile> FilterPlaylistByMediaPathType(
//    std::vector<PlaylistFile>& playlistFiles,
//    short mediaPathTypeId) {
//  std::vector<PlaylistFile> filteredList;
//  for (const auto& item : playlistFiles) {
//    if (item.mediaPathTypeId == mediaPathTypeId) {
//      filteredList.push_back(item);
//    }
//  }
//  return filteredList;
//}
//
//std::vector<std::string> InitializeMediaForResponseType(ContentProps props) {
//  std::vector<std::string> mediaFiles;
//
//  int residentId = props.residentId;
//  short responseTypeId = props.responseTypeId;
//  short responseTypeCategoryId = props.responseTypeCategoryId;
//  short mediaPathTypeId = props.mediaPathTypeId;
//  short mediaPathTypeCategoryId = props.mediaPathTypeCategoryId;
//
//  std::string result = DataAccess::getPlaylistFilesForResponseType(
//      residentId, responseTypeId, mediaPathTypeId);
//  json j = json::parse(result);
//  auto playlistFiles = j.get<std::vector<PlaylistFile>>();
//
//  // get a file count
//  int numFiles = GetFileCount(playlistFiles, residentId, mediaPathTypeId);
//
//  // for residents might need to pad their media with some community media
//  if (residentId > 0) {
//    // if no files then look in community playlist
//    if (numFiles == 0) {
//      result = DataAccess::getPlaylistFilesForResponseType(
//          Constants::CommunitySource::Id, responseTypeId, mediaPathTypeId);
//      j = json::parse(result);
//      playlistFiles = j.get<std::vector<PlaylistFile>>();
//
//      // set the file count
//      numFiles = GetFileCount(playlistFiles, residentId, mediaPathTypeId);
//    }
//  }
//
//  // if (playlistFiles.size() > 0 && numFiles > 0) {
//  // assemble a list of file paths of all the required media
//  mediaFiles = GetAssembledFileList(playlistFiles, residentId, mediaPathTypeId);
//
//  std::filesystem::path contentFolder;
//  switch (responseTypeCategoryId) {
//    case Constants::ResponseTypeCategoryId::Audio:
//    case Constants::ResponseTypeCategoryId::Image:
//    case Constants::ResponseTypeCategoryId::Video: {
//      switch (responseTypeId) {
//        case Constants::ResponseTypeId::SlideShow: {
//          contentFolder = "SlideShow";
//
//          if (mediaFiles.size() > 1)
//            std::shuffle(mediaFiles.begin(), mediaFiles.end(), rng);
//
//          // write list of image full paths to data.json file
//          std::filesystem::path jsonPath =
//              CONTENT_PATH / contentFolder / "data.json";
//          std::ofstream dataFile;
//          dataFile.open(jsonPath);
//          std::string mediaList = VectorToCommaSeparatedString(mediaFiles);
//          std::string data = "data=[" + mediaList + "]";
//          dataFile << data;
//          dataFile.close();
//
//          // for the slide show also need to pick a song from a randomized list
//          // and return it in a list
//          std::string result1 = DataAccess::getPlaylistFilesForResponseType(
//              residentId, Constants::ResponseTypeId::Radio,
//              Constants::MediaPathTypeId::Music);
//          json j1 = json::parse(result1);
//          auto musicPlaylistFiles = j1.get<std::vector<PlaylistFile>>();
//
//          // if the resident has no custom music then get from shared library
//          if (musicPlaylistFiles.size() == 0) {
//            std::string result2 = DataAccess::getPlaylistFilesForResponseType(
//                Constants::CommunitySource::Id,
//                Constants::ResponseTypeId::Radio,
//                Constants::MediaPathTypeId::Music);
//            json j2 = json::parse(result2);
//            musicPlaylistFiles = j2.get<std::vector<PlaylistFile>>();
//          }
//          std::vector<std::string> musicFiles = GetAssembledFileList(
//              musicPlaylistFiles, residentId, mediaPathTypeId);
//          if (musicFiles.size() > 1)
//            std::shuffle(musicFiles.begin(), musicFiles.end(), rng);
//
//          mediaFiles.clear();
//          mediaFiles.push_back(musicFiles.front());
//
//          break;
//        }
//
//        default: {
//          contentFolder = "Media";
//
//          if (mediaFiles.size() > 1)
//            std::shuffle(mediaFiles.begin(), mediaFiles.end(), rng);
//
//          std::filesystem::path jsonPath =
//              CONTENT_PATH / contentFolder / "data.json";
//          std::ofstream dataFile;
//          dataFile.open(jsonPath);
//          std::string mediaList = VectorToCommaSeparatedString(mediaFiles);
//
//          std::string category =
//              mediaPathTypeCategoryId ==
//                      Constants::MediaPathTypeCategoryId::Audio
//                  ? "audio"
//                  : "video";
//          std::string data = "category=\"" + category + "\"";
//          dataFile << data;
//          dataFile.close();
//
//          break;
//        }
//      }
//      break;
//    }
//    case Constants::ResponseTypeCategoryId::InteractiveActivity: {
//      std::string isCaregiverString = _isScopeCaregiver ? "true" : "false";
//      std::string kioskIdString = std::to_string(_kioskId);
//      std::string languageIdString = std::to_string(_languageId);
//      std::string difficultyLevelString = std::to_string(_difficultyLevel);
//      std::string timeoutString = std::to_string(_timeout);
//      std::filesystem::path activityFolder = _activityUrl.remove_filename();
//      std::string activityFolderString = activityFolder.string();
//
//      switch (responseTypeId) {
//        case Constants::ResponseTypeId::MatchingGame: {
//          // get playlist file data for shapes and sounds
//          std::vector<PlaylistFile> shapePlaylistFiles =
//              FilterPlaylistByMediaPathType(
//                  playlistFiles,
//                  Constants::MediaPathTypeId::MatchingGameShapes);
//          std::vector<PlaylistFile> soundPlaylistFiles =
//              FilterPlaylistByMediaPathType(
//                  playlistFiles,
//                  Constants::MediaPathTypeId::MatchingGameSounds);
//
//          // assemble into a list of full paths
//          std::vector<std::string> shapeFiles = GetAssembledFileList(
//              shapePlaylistFiles, residentId,
//              Constants::MediaPathTypeId::MatchingGameShapes);
//          std::vector<std::string> soundFiles = GetAssembledFileList(
//              soundPlaylistFiles, residentId,
//              Constants::MediaPathTypeId::MatchingGameSounds);
//
//          // randomize the list
//          std::shuffle(shapeFiles.begin(), shapeFiles.end(), rng);
//
//          // write to data.json
//          std::filesystem::path jsonPath =
//              ACTIVITIES_PATH / activityFolder / "data.json";
//          std::ofstream dataFile;
//          dataFile.open(jsonPath);
//          std::string shapesList = VectorToCommaSeparatedString(shapeFiles);
//          std::string soundsList = VectorToCommaSeparatedString(soundFiles);
//
//          std::string data =
//              "kioskId=" + kioskIdString + "\n" +
//              "isCaregiver=" + isCaregiverString + "\n" + "residentName=\"" +
//              _residentName + "\"\n" + "level=" + difficultyLevelString + "\n" +
//              "language=" + languageIdString + "\n" +
//              "timeout=" + timeoutString + "\n" + "sounds=[" + soundsList +
//              "]\n" + "shapes=[" + shapesList + "]\n";
//          dataFile << data;
//          dataFile.close();
//
//          break;
//        }
//
//        default: {
//          // no playlist data is needed for other activities
//          // just need to write base level properties to data.json
//          std::filesystem::path jsonPath =
//              ACTIVITIES_PATH / activityFolder / "data.json";
//          std::ofstream dataFile;
//          dataFile.open(jsonPath);
//
//          std::string data = "kioskId=" + kioskIdString + "\n" +
//                             "isCaregiver=" + isCaregiverString + "\n" +
//                             "residentName=\"" + _residentName + "\"\n" +
//                             "level=" + difficultyLevelString + "\n" +
//                             "language=" + languageIdString + "\n" +
//                             "timeout=" + timeoutString;
//          dataFile << data;
//          dataFile.close();
//
//          break;
//        }
//      }
//      break;
//    }
//    case Constants::ResponseTypeCategoryId::System: {
//      switch (responseTypeId) {
//        case Constants::ResponseTypeId::Ambient: {
//          contentFolder = "Ambient/";
//
//          // get ambient invitations
//          std::string invitations_json = DataAccess::getAmbientInvitations();
//
//          // write to data.json
//          std::filesystem::path jsonPath =
//              CONTENT_PATH / contentFolder / "data.json";
//          std::ofstream dataFile;
//          dataFile.open(jsonPath);
//          std::string mediaList = VectorToCommaSeparatedString(mediaFiles);
//          std::string data = "videos=[" + mediaList + "]\n" +
//                             "invitations=" + invitations_json + "\n" +
//                             "videoDuration=15000\n" +
//                             "invitationDuration=15000\n";
//          dataFile << data;
//          dataFile.close();
//
//          break;
//        }
//      }
//    }
//  }
//  // }
//
//  return mediaFiles;
//}
//
//std::vector<std::string> InitializeMediaForActivityType(ActivityProps props) {
//  _activityUrl = props.url;
//  _kioskId = props.kioskId;
//  _isScopeCaregiver = props.isScopeCaregiver;
//  _residentName = props.residentName;
//  _difficultyLevel = props.difficultyLevel;
//  _languageId = props.languageId;
//  _timeout = props.timeout;
//
//  ContentProps contentProps = {
//      props.residentId, props.responseTypeId,
//      Constants::ResponseTypeCategoryId::InteractiveActivity};
//
//  return InitializeMediaForResponseType(contentProps);
//}
//
//std::vector<PlaylistFile> GetResidentPlaylistFiles(int residentId,
//                                                   short responseTypeId) {
//  std::vector<PlaylistFile> playlistFiles;
//
//  std::string result =
//      DataAccess::getPlaylistFilesForResponseType(residentId, responseTypeId);
//  json j = json::parse(result);
//  playlistFiles = j.get<std::vector<PlaylistFile>>();
//
//  return playlistFiles;
//}
//
//std::vector<int> ShuffleIdList(std::vector<int>& ids) {
//  std::shuffle(ids.begin(), ids.end(), rng);
//  return ids;
//}
//}  // namespace media_query