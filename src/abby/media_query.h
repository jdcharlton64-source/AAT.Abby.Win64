//#ifndef ABBY_MEDIA_QUERY_H_
//#define ABBY_MEDIA_QUERY_H_
//
//#include <optional>
//#include <string>
//#include <vector>
//
//namespace media_query {
//
//struct PlaylistFile {
//  int id;
//  int mediaFileId;
//  std::string filename;
//  std::string title;
//  int residentId;
//  short responseTypeId;
//  short mediaPathTypeId;
//  std::string dateCreated;
//  bool isLinked;
//  std::string path;
//  std::string mediaCategory;
//  std::string responseCategory;
//  std::optional<std::string> residentName;
//};
//
//struct ContentProps {
//  int residentId;
//  short responseTypeId;
//  short responseTypeCategoryId;
//  short mediaPathTypeId = -1;
//  short mediaPathTypeCategoryId = -1;
//};
//
//struct ActivityProps {
//  int residentId;
//  short responseTypeId;
//  std::string url;
//  int kioskId;
//  std::string residentName;
//  short difficultyLevel;
//  int languageId;
//  int timeout;
//  bool isScopeCaregiver;
//};
//
//std::vector<std::string> InitializeMediaForResponseType(ContentProps props);
//std::vector<std::string> InitializeMediaForActivityType(ActivityProps props);
//std::vector<PlaylistFile> GetResidentPlaylistFiles(int residentId,
//                                                   short responseTypeId = -1);
//std::vector<int> ShuffleIdList(std::vector<int>& ids);
//}  // namespace media_query
//
//#endif  // ABBY_MEDIA_QUERY_H_