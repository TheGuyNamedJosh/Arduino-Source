/*  AutoStory
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "CommonFramework/GlobalSettingsPanel.h"
#include "CommonFramework/Exceptions/FatalProgramException.h"
#include "CommonFramework/Exceptions/OperationFailedException.h"
#include "CommonFramework/InferenceInfra/InferenceRoutines.h"
#include "CommonFramework/Notifications/ProgramNotifications.h"
#include "CommonFramework/Tools/StatsTracking.h"
#include "CommonFramework/Tools/VideoResolutionCheck.h"
#include "NintendoSwitch/Commands/NintendoSwitch_Commands_PushButtons.h"
#include "Pokemon/Pokemon_Strings.h"
#include "PokemonSwSh/Inference/PokemonSwSh_IvJudgeReader.h"
#include "PokemonSV/Programs/PokemonSV_GameEntry.h"
#include "PokemonSV/Programs/PokemonSV_SaveGame.h"
#include "PokemonSV/Inference/PokemonSV_TutorialDetector.h"
#include "PokemonSV_AutoStoryTools.h"
#include "PokemonSV_AutoStory_Segment_05.h"

//#include <iostream>
//using std::cout;
//using std::endl;
//#include <unordered_map>
//#include <algorithm>

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSV{

using namespace Pokemon;




std::string AutoStory_Segment_05::name() const{
    return "05: First Arven Battle";
}

std::string AutoStory_Segment_05::start_text() const{
    return "Start: Saved the Legendary. Escaped from the Houndoom cave.";
}

std::string AutoStory_Segment_05::end_text() const{
    return "End: Battled Arven, received Legendary's Pokeball. Talked to Nemona at Lighthouse.";
}

void AutoStory_Segment_05::run_segment(SingleSwitchProgramEnvironment& env, BotBaseContext& context, AutoStoryOptions options) const{
    AutoStoryStats& stats = env.current_stats<AutoStoryStats>();

    context.wait_for_all_requests();
    env.console.log("Start Segment 05: First Arven Battle", COLOR_ORANGE);
    env.console.overlay().add_log("Start Segment 05: First Arven Battle", COLOR_ORANGE);

    checkpoint_09(env, context, options.notif_status_update);
    checkpoint_10(env, context, options.notif_status_update);

    context.wait_for_all_requests();
    env.console.log("End Segment 05: First Arven Battle", COLOR_GREEN);
    env.console.overlay().add_log("End Segment 05: First Arven Battle", COLOR_GREEN);
    stats.m_segment++;
    env.update_stats();

}


void checkpoint_09(
    SingleSwitchProgramEnvironment& env, 
    BotBaseContext& context, 
    EventNotificationOption& notif_status_update
){
    AutoStoryStats& stats = env.current_stats<AutoStoryStats>();
    bool first_attempt = true;
    while (true){
    try{        
        if (first_attempt){
            checkpoint_save(env, context, notif_status_update);
            first_attempt = false;
        }      
        context.wait_for_all_requests();

        realign_player(env.program_info(), env.console, context, PlayerRealignMode::REALIGN_NEW_MARKER, 230, 120, 100);
        env.console.log("overworld_navigation: Go to Arven at the tower.");
        overworld_navigation(env.program_info(), env.console, context, NavigationStopCondition::STOP_DIALOG, NavigationMovementMode::DIRECTIONAL_ONLY, 128, 0, true, true);
            
        context.wait_for_all_requests();
        env.console.log("Found Arven");
        env.console.overlay().add_log("Found Arven", COLOR_WHITE);
        // can lose battle, and story will still continue
        mash_button_till_overworld(env.console, context, BUTTON_A);
        context.wait_for_all_requests();
        env.console.log("Receive legendary ball");
        env.console.overlay().add_log("Receive legendary ball", COLOR_WHITE);

        break;
    }catch(...){
        context.wait_for_all_requests();
        env.console.log("Resetting from checkpoint.");
        reset_game(env.program_info(), env.console, context);
        stats.m_reset++;
        env.update_stats();
    }            
    }
}

void checkpoint_10(
    SingleSwitchProgramEnvironment& env, 
    BotBaseContext& context, 
    EventNotificationOption& notif_status_update
){
    AutoStoryStats& stats = env.current_stats<AutoStoryStats>();
    bool first_attempt = true;
    while (true){
    try{
        if (first_attempt){
            checkpoint_save(env, context, notif_status_update);
            first_attempt = false;
        }        
        context.wait_for_all_requests();
        env.console.log("Lighthouse view");
        env.console.overlay().add_log("Lighthouse view", COLOR_WHITE);
        do_action_and_monitor_for_battles(env.program_info(), env.console, context,
            [&](const ProgramInfo& info, ConsoleHandle& console, BotBaseContext& context){
                realign_player(env.program_info(), console, context, PlayerRealignMode::REALIGN_NEW_MARKER, 230, 110, 100);
                pbf_move_left_joystick(context, 128, 0, 6 * TICKS_PER_SECOND, 8 * TICKS_PER_SECOND);
                pbf_move_left_joystick(context, 128, 0, 4 * TICKS_PER_SECOND, 20);
                pbf_move_left_joystick(context, 255, 128, 15, 20);
                pbf_press_button(context, BUTTON_L, 20, 20);
                pbf_move_left_joystick(context, 128, 0, 7 * TICKS_PER_SECOND, 20);                
            }
        );

        env.console.log("overworld_navigation: Go to Nemona on the lighthouse.");
        overworld_navigation(env.program_info(), env.console, context, NavigationStopCondition::STOP_DIALOG, NavigationMovementMode::DIRECTIONAL_SPAM_A, 128, 0, 20, true, true);

        mash_button_till_overworld(env.console, context, BUTTON_A);

        break;
    }catch(...){
        context.wait_for_all_requests();
        env.console.log("Resetting from checkpoint.");
        reset_game(env.program_info(), env.console, context);
        stats.m_reset++;
        env.update_stats();
    }           
    }
}




}
}
}
