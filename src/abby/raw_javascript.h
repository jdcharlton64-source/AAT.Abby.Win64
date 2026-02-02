#ifndef ABBY_RAW_JAVASCRIPT_H_
#define ABBY_RAW_JAVASCRIPT_H_

#include <string>

namespace raw_javascript {
enum CaregiverCloseButtonType { CaregiverMedia, CaregiverActivity };

std::string loadingMessage();
std::string volumeDialog();
std::string caregiverCloseButton(
    CaregiverCloseButtonType buttonType = CaregiverMedia);
std::string residentPopup();
std::string invalidKeyCardPopup();
std::string alert(bool autoClose = false);
std::string noSpeakerAttached();
}  // namespace raw_javascript

#endif  // ABBY_RAW_JAVASCRIPT_H_