#ifndef ABBY_CLIENT_STRUCTS_H_
#define ABBY_CLIENT_STRUCTS_H_

#include <nlohmann/json.hpp>
#include <string>

using nlohmann::json;

// enum
enum RotationSensorStep {
  NoValue = -1,
  Value0 = 0,
  Value1 = 200,
  Value2 = 400,
  Value3 = 600,
  Value4 = 800
};

enum CaregiverEventType {
  CaregiverSlideShow = 1,
  CaregiverVideo = 2,
  CaregiverAudio = 3,
  CaregiverActivity = 4
};

// struct
struct ContentState {
  std::string browserUrl;
  std::vector<std::string> mediaFiles;
  int sensorValue;
  std::string blankPageUrl;
};

struct DisplayState {
  CaregiverEventType caregiverEventType;
  ContentState content;
};

// kiosk
struct Kiosk {
  int id;
  std::string name;
  int languageId;
  int identifier;
  std::string facility;
};

void from_json(const json& j, Kiosk& k) {
  j.at("id").get_to(k.id);
  j.at("name").get_to(k.name);
  j.at("languageId").get_to(k.languageId);
  j.at("identifier").get_to(k.identifier);
  j.at("facility").get_to(k.facility);
}

struct KioskKeyTag {
  std::string tagId;
};

void from_json(const json& j, KioskKeyTag& kt) {
  j.at("tagId").get_to(kt.tagId);
}

// resident
struct Resident {
  int id;
  std::string firstName;
  std::string lastName;
  std::string gender;
  int languageId;
  short gameDifficultyLevel;
  std::optional<int> bluetoothBeaconId;
  std::optional<std::string> uwbTagId;
  std::optional<std::string> profilePicture;
};

void from_json(const json& j, Resident& r) {
  j.at("id").get_to(r.id);
  j.at("firstName").get_to(r.firstName);
  j.at("lastName").get_to(r.lastName);
  j.at("gender").get_to(r.gender);
  j.at("languageId").get_to(r.languageId);
  j.at("gameDifficultyLevel").get_to(r.gameDifficultyLevel);
  j.at("bluetoothBeaconId").get_to(r.bluetoothBeaconId);
  j.at("uwbTagId").get_to(r.uwbTagId);
  j.at("profilePicture").get_to(r.profilePicture);
}

void to_json(json& j, const Resident& r) {
  j = json{{"id", r.id},
           {"firstName", r.firstName},
           {"lastName", r.lastName},
           {"gender", r.gender},
           {"languageId", r.languageId},
           {"gameDifficultyLevel", r.gameDifficultyLevel},
           {"bluetoothBeaconId", r.bluetoothBeaconId},
           {"uwbTagId", r.uwbTagId},
           {"profilePicture", r.profilePicture}};
}

// response type
struct ResponseType {
  short id;
  short responseTypeCategoryId;
  std::string description;
  int interactiveActivityTypeId;
  bool isRandom;
  bool isRotational;
  bool isUninterruptable;
  std::string url;
};

void from_json(const json& j, ResponseType& rt) {
  j.at("id").get_to(rt.id);
  j.at("responseTypeCategoryId").get_to(rt.responseTypeCategoryId);
  j.at("description").get_to(rt.description);
  j.at("interactiveActivityTypeId").get_to(rt.interactiveActivityTypeId);
  j.at("isRandom").get_to(rt.isRandom);
  j.at("isRotational").get_to(rt.isRotational);
  j.at("isUninterruptable").get_to(rt.isUninterruptable);
  j.at("url").get_to(rt.url);
}

// media path type
struct MediaPathType {
  short id;
  short mediaPathTypeCategoryId;
  std::string path;
  std::string description;
  std::string shortDescription;
  std::string longDescription;
  std::string summaryLabel;
  std::string summaryLabelPlural;
  int responseTypeId;
  bool isSystem;
  bool isSharable;
  short sortOrder;
  std::string category;
};

void from_json(const json& j, MediaPathType& pt) {
  j.at("id").get_to(pt.id);
  j.at("mediaPathTypeCategoryId").get_to(pt.mediaPathTypeCategoryId);
  j.at("path").get_to(pt.path);
  j.at("description").get_to(pt.description);
  j.at("shortDescription").get_to(pt.shortDescription);
  j.at("longDescription").get_to(pt.longDescription);
  j.at("summaryLabel").get_to(pt.summaryLabel);
  j.at("summaryLabelPlural").get_to(pt.summaryLabelPlural);
  j.at("responseTypeId").get_to(pt.responseTypeId);
  j.at("isSystem").get_to(pt.isSystem);
  j.at("isSharable").get_to(pt.isSharable);
  j.at("sortOrder").get_to(pt.sortOrder);
  j.at("category").get_to(pt.category);
}

// gadget type
struct GadgetType {
  int id;
  std::string description;
};

void from_json(const json& j, GadgetType& gt) {
  j.at("id").get_to(gt.id);
  j.at("description").get_to(gt.description);
}

// config detail
struct ConfigDetail {
  int id;
  int configId;
  std::string description;
  std::string gadgetType;
  short gadgetId;
  short gadgetStyleId;
  bool isIncremental;
  short responseTypeId;
};

void from_json(const json& j, ConfigDetail& cd) {
  j.at("id").get_to(cd.id);
  j.at("configId").get_to(cd.configId);
  j.at("description").get_to(cd.description);
  j.at("gadgetType").get_to(cd.gadgetType);
  j.at("gadgetId").get_to(cd.gadgetId);
  j.at("gadgetStyleId").get_to(cd.gadgetStyleId);
  j.at("isIncremental").get_to(cd.isIncremental);
  j.at("responseTypeId").get_to(cd.responseTypeId);
}

void to_json(json& j, const ConfigDetail& cd) {
  j = json{{"id", cd.id},
           {"configId", cd.configId},
           {"description", cd.description},
           {"gadgetType", cd.gadgetType},
           {"gadgetId", cd.gadgetId},
           {"gadgetStyleId", cd.gadgetStyleId},
           {"isIncremental", cd.isIncremental},
           {"responseTypeId", cd.responseTypeId}};
}

// text resource
struct TextResource {
  int id;
  std::string text;
};

void from_json(const json& j, TextResource& t) {
  j.at("id").get_to(t.id);
  j.at("text").get_to(t.text);
}

// interactive activity
struct InteractiveActivityType {
  int id;
  std::string description;
  std::string url;
};

void from_json(const json& j, InteractiveActivityType& a) {
  j.at("id").get_to(a.id);
  j.at("description").get_to(a.description);
  j.at("url").get_to(a.url);
}

//--- WEBSOCKET structs ---

// key/card slot
struct Payload_KeyCard {
  std::string type;
  short id;
  std::string value;
};

struct Payload_Gadget {
  std::string type;
  short id;
  int value;
};

void from_json(const json& j, Payload_KeyCard& p) {
  j.at("type").get_to(p.type);
  j.at("id").get_to(p.id);
  j.at("value").get_to(p.value);
}

void from_json(const json& j, Payload_Gadget& p) {
  j.at("type").get_to(p.type);
  j.at("id").get_to(p.id);
  j.at("value").get_to(p.value);
}

// other gadgets
struct PayloadGadgetWrapper {
  Payload_Gadget payload_gadget;
};

struct PayloadKeyCardWrapper {
  Payload_KeyCard payload_keycard;
};

void from_json(const json& j, PayloadGadgetWrapper& w) {
  j.at("payload_gadget").get_to(w.payload_gadget);
}

void from_json(const json& j, PayloadKeyCardWrapper& w) {
  j.at("payload_keycard").get_to(w.payload_keycard);
}

//--- CAREGIVER structs (begin) ---

struct ResidentPathType {
  short mediaPathTypeId;
};

void from_json(const json& j, ResidentPathType& ptm) {
  j.at("mediaPathTypeId").get_to(ptm.mediaPathTypeId);
}

void to_json(json& j, const MediaPathType& pt) {
  j = json{{"Id", pt.id},
           {"MediaPathTypeCategoryId", pt.mediaPathTypeCategoryId},
           {"Path", pt.path},
           {"Description", pt.description},
           {"ShortDescription", pt.shortDescription},
           {"SummaryLabel", pt.summaryLabel},
           {"SummaryLabelPlural", pt.summaryLabelPlural},
           {"ResponseTypeId", pt.responseTypeId},
           {"IsSystem", pt.isSystem},
           {"IsSharable", pt.isSharable},
           {"SortOrder", pt.sortOrder},
           {"Category", pt.category}};
}

struct CaregiverMediaSource {
  int id;
  std::string name;
  std::string kioskName;
  std::string beaconIdDescription;
  std::string uwbTagIdDescription;
  short gameDifficultyLevel;
  int languageId;
  std::string picture;
};

void to_json(json& j, const CaregiverMediaSource& cf) {
  j = json{{"Id", cf.id},
           {"Name", cf.name},
           {"KioskName", cf.kioskName},
           {"BeaconIdDescription", cf.beaconIdDescription},
           {"UwbTagIdDescription", cf.uwbTagIdDescription},
           {"GameDifficultyLevel", cf.gameDifficultyLevel},
           {"LanguageId", cf.languageId},
           {"Picture", cf.picture}};
}

struct CaregiverTab {
  short responseTypeId;
  std::string category;
  std::string kioskDescription;
  std::string residentDescription;
  std::string tabDescription;
  std::string summaryLabel;
  std::string summaryLabelPlural;
  bool isResident;
  short sortOrder;
};

void to_json(json& j, const CaregiverTab& cf) {
  j = json{{"ResponseTypeId", cf.responseTypeId},
           {"Category", cf.category},
           {"KioskDescription", cf.kioskDescription},
           {"ResidentDescription", cf.residentDescription},
           {"TabDescription", cf.tabDescription},
           {"SummaryLabel", cf.summaryLabel},
           {"SummaryLabelPlural", cf.summaryLabelPlural},
           {"IsResident", cf.isResident},
           {"SortOrder", cf.sortOrder}};
}

struct CaregiverFile {
  int id;
  std::string title;
  std::string thumbnailUrl;
  std::string dateCreated;
};

void to_json(json& j, const CaregiverFile& cf) {
  j = json{{"Id", cf.id},
           {"Title", cf.title},
           {"ThumbnailUrl", cf.thumbnailUrl},
           {"DateAdded", cf.dateCreated}};
}

struct CaregiverFiles {
  int responseTypeId;
  std::vector<CaregiverFile> files;
};

void to_json(json& j, const CaregiverFiles& cf) {
  j = json{{"ResponseTypeId", cf.responseTypeId}, {"Files", cf.files}};
}

// for the initial data load (community)
struct CaregiverContent {
  int residentId;
  std::vector<CaregiverFiles> responseFiles;
};

void to_json(json& j, const CaregiverContent& cc) {
  j = json{{"ResidentId", cc.residentId}, {"ResponseFiles", cc.responseFiles}};
}

// for loading resident content from the hamburger menu
struct CaregiverResidentContent {
  int residentId;
};

void from_json(const json& j, CaregiverResidentContent& cr) {
  j.at("residentId").get_to(cr.residentId);
}

struct CaregiverContentWrapper {
  CaregiverResidentContent caregiverContent;
};

void from_json(const json& j, CaregiverContentWrapper& w) {
  j.at("caregiverContent").get_to(w.caregiverContent);
}

// used for requesting an item to play
struct CaregiverRequest {
  int residentId;
  int contentId;
  CaregiverEventType eventType;
};

void from_json(const json& j, CaregiverRequest& cr) {
  j.at("residentId").get_to(cr.residentId);
  j.at("contentId").get_to(cr.contentId);
  j.at("eventType").get_to(cr.eventType);
}

struct CaregiverRequestWrapper {
  CaregiverRequest caregiverRequest;
};

void from_json(const json& j, CaregiverRequestWrapper& w) {
  j.at("caregiverRequest").get_to(w.caregiverRequest);
}

//--- CAREGIVER structs (end) ---

// to retrieve the current ambient video time
struct AmbientContentState {
  short videoIndex;
  int currentTime;
  short invitationIndex;
};

void from_json(const json& j, AmbientContentState& cs) {
  j.at("videoIndex").get_to(cs.videoIndex);
  j.at("currentTime").get_to(cs.currentTime);
  j.at("invitationIndex").get_to(cs.invitationIndex);
}

struct AmbientContentStateWrapper {
  AmbientContentState ambientContentState;
};

void from_json(const json& j, AmbientContentStateWrapper& w) {
  j.at("ambientContentState").get_to(w.ambientContentState);
}

#endif  // ABBY_CLIENT_STRUCTS_H_