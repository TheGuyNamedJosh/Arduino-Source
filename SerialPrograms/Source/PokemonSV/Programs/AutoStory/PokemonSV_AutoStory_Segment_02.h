/*  Autostory
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonSV_AutoStory_Segment_02_H
#define PokemonAutomation_PokemonSV_AutoStory_Segment_02_H

#include <functional>
#include "Common/Cpp/Options/EnumDropdownOption.h"
#include "CommonFramework/Notifications/EventNotificationsTable.h"
#include "CommonFramework/Options/LanguageOCROption.h"
#include "NintendoSwitch/Options/NintendoSwitch_GoHomeWhenDoneOption.h"
#include "Common/NintendoSwitch/NintendoSwitch_ControllerDefs.h"
#include "PokemonSV/Programs/PokemonSV_Navigation.h"
#include "PokemonSV_AutoStoryTools.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSV{

class AutoStory_Segment_02 : public AutoStory_Segment{
public:
    virtual std::string name() const override;
    virtual std::string start_text() const override;
    virtual std::string end_text() const override;
    virtual void run_segment(
        SingleSwitchProgramEnvironment& env, 
        BotBaseContext& context,
        AutoStoryOptions options) const override;
};


// start: Received starter pokemon and changed move order. Cleared autoheal tutorial.
// end: Battled Nemona on the beach.
void checkpoint_04(SingleSwitchProgramEnvironment& env, BotBaseContext& context, EventNotificationOption& notif_status_update);


}
}
}
#endif
