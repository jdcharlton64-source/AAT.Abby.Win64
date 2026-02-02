#include <filesystem>

// string constants
const std::filesystem::path EXEC_ROOT =
    std::filesystem::current_path().string();
const std::filesystem::path SHARED_LIBRARY_FOLDER = "SharedLibrary";
const std::filesystem::path PROFILES_FOLDER = "Profiles";
const std::filesystem::path CONTENT_ROOT = EXEC_ROOT / "content";
const std::filesystem::path ACTIVITIES_ROOT = EXEC_ROOT / "activities";
const std::filesystem::path IMAGES_ROOT = EXEC_ROOT / "images";

// urls
constexpr char URI_BLANK[] = "_Common/blank.html";
const std::string URI_AMBIENT = "Ambient/index.html";
const std::string URI_SLIDESHOW = "SlideShow/index.html";
const std::string URI_CAREGIVER = "Caregiver/index.html";
const std::filesystem::path URL_AMBIENT = CONTENT_ROOT / URI_AMBIENT;
const std::filesystem::path MEDIA_ROOT = "/srv/_aatabbymedia/";

// timeouts
constexpr int TIMEOUT_CAREGIVER = 600000;
constexpr int TIMEOUT_ACTIVITY = 600000;

const int MAX_CAPSENSE_ID = 16;

class Constants {
 public:
  class ResponseTypeId {
   public:
    static const short SlideShow = 1;
    static const short MatchingGame = 2;
    static const short Cats = 3;
    static const short KillDisplay = 4;
    static const short Radio = 5;
    static const short Television = 6;
    static const short Caregiver = 7;
    static const short Ambient = 8;
    static const short Volume = 10;
    static const short Wheel = 15;
    static const short RandomActivity = 18;

    // prevent accidental instantiation
    ResponseTypeId() = delete;  // delete default constructor
    ResponseTypeId(const ResponseTypeId&) = delete;  // delete copy constructor
    ResponseTypeId& operator=(const ResponseTypeId&) =
        delete;  // delete copy assignment operator
  };

  class ResponseTypeCategoryId {
   public:
    static const short Image = 1;
    static const short Audio = 2;
    static const short Video = 3;
    static const short InteractiveActivity = 4;
    static const short System = 5;

    // prevent accidental instantiation
    ResponseTypeCategoryId() = delete;  // delete default constructor
    ResponseTypeCategoryId(const ResponseTypeCategoryId&) =
        delete;  // delete copy constructor
    ResponseTypeCategoryId& operator=(const ResponseTypeCategoryId&) =
        delete;  // delete copy assignment operator
  };

  class GadgetTypeId {
   public:
    static const short KeyCard = 0;
    static const short Capsense = 1;
    static const short Switch = 2;

    // prevent accidental instantiation
    GadgetTypeId() = delete;                     // delete default constructor
    GadgetTypeId(const GadgetTypeId&) = delete;  // delete copy constructor
    GadgetTypeId& operator=(const GadgetTypeId&) =
        delete;  // delete copy assignment operator
  };

  class GadgetId {
   public:
    static const short KeyCard = 0;
    static const short RadioDial = 21;
    static const short TVDial = 22;
    // prevent accidental instantiation
    GadgetId() = delete;                 // delete default constructor
    GadgetId(const GadgetId&) = delete;  // delete copy constructor
    GadgetId& operator=(const GadgetId&) =
        delete;  // delete copy assignment operator
  };

  class GadgetStyleTypeId {
   public:
    static const short Touch = 1;
    static const short MultiTurn = 2;
    static const short StopTurn = 3;
    static const short Slider = 4;
    static const short OnOff = 5;
    static const short OnOnly = 6;
    static const short NonRotational = 6;

    // prevent accidental instantiation
    GadgetStyleTypeId() = delete;  // delete default constructor
    GadgetStyleTypeId(const GadgetStyleTypeId&) =
        delete;  // delete copy constructor
    GadgetStyleTypeId& operator=(const GadgetStyleTypeId&) =
        delete;  // delete copy assignment operator
  };

  class MediaPathTypeId {
   public:
    static const short Music = 1;
    static const short RadioShows = 2;
    static const short MatchingGameShapes = 7;
    static const short MatchingGameSounds = 8;
    static const short Ambient = 9;

    // prevent accidental instantiation
    MediaPathTypeId() = delete;
    MediaPathTypeId(const MediaPathTypeId&) = delete;
    MediaPathTypeId& operator=(const MediaPathTypeId&) = delete;
  };

  class MediaPathTypeCategoryId {
   public:
    static const short Audio = 1;
    static const short Image = 2;
    static const short Video = 3;
    static const short InteractiveActivity = 4;
    static const short System = 5;

    // prevent accidental instantiation
    MediaPathTypeCategoryId() = delete;
    MediaPathTypeCategoryId(const MediaPathTypeCategoryId&) = delete;
    MediaPathTypeCategoryId& operator=(const MediaPathTypeCategoryId&) = delete;
  };

  class TextResourceTypeId {
   public:
    static const short VolumeAdjust = 1;
    static const short KioskProfileLabel = 2;
    static const short KeyCardNotFound = 3;

    // prevent accidental instantiation
    TextResourceTypeId() = delete;  // delete default constructor
    TextResourceTypeId(const TextResourceTypeId&) =
        delete;  // delete copy constructor
    TextResourceTypeId& operator=(const TextResourceTypeId&) =
        delete;  // delete copy assignment operator
  };

  class CommunitySource {
   public:
    static const int Id = 0;
    static const int GameDifficultyLevel = 1;

    // prevent accidental instantiation
    CommunitySource() = delete;
    CommunitySource(const CommunitySource&) = delete;
    CommunitySource& operator=(const CommunitySource&) = delete;
  };

  class LangugageId {
   public:
    static const int English = 1;
    static const int French = 2;

    // prevent accidental instantiation
    LangugageId() = delete;
    LangugageId(const LangugageId&) = delete;
    LangugageId& operator=(const LangugageId&) = delete;
  };
};