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
#include "PokemonSV_AutoStory_Segment_04.h"

//#include <iostream>
//using std::cout;
//using std::endl;
//#include <unordered_map>
//#include <algorithm>

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSV{

using namespace Pokemon;




std::string AutoStory_Segment_04::name() const{
    return "04: Rescue Legendary";
}

std::string AutoStory_Segment_04::start_text() const{
    return "Start: Finished catch tutorial. Walked to the cliff and heard mystery cry.";
}

std::string AutoStory_Segment_04::end_text() const{
    return "End: Saved the Legendary. Escaped from the Houndoom cave.";
}

void AutoStory_Segment_04::run_segment(SingleSwitchProgramEnvironment& env, BotBaseContext& context, AutoStoryOptions options) const{
    AutoStoryStats& stats = env.current_stats<AutoStoryStats>();

    context.wait_for_all_requests();
    env.console.log("Start Segment 04: Rescue Legendary", COLOR_ORANGE);
    env.console.overlay().add_log("Start Segment 04: Rescue Legendary", COLOR_ORANGE);

    checkpoint_08(env, context, options.notif_status_update);

    context.wait_for_all_requests();
    env.console.log("End Segment 04: Rescue Legendary", COLOR_GREEN);
    env.console.overlay().add_log("End Segment 04: Rescue Legendary", COLOR_GREEN);
    stats.m_segment++;
    env.update_stats();

}

void checkpoint_08(
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

        realign_player(env.program_info(), env.console, context, PlayerRealignMode::REALIGN_NEW_MARKER, 230, 70, 100);
        env.console.log("overworld_navigation: Go to cliff.");
        overworld_navigation(env.program_info(), env.console, context, NavigationStopCondition::STOP_DIALOG, NavigationMovementMode::DIRECTIONAL_ONLY, 128, 0, true, true);

        env.console.log("clear_dialog: Look over the injured Miraidon/Koraidon on the beach. Fall down the cliff");
        clear_dialog(env.console, context, ClearDialogMode::STOP_TIMEOUT, 30, {});
        // long animation
        env.console.log("overworld_navigation: Go to Legendary pokemon laying on the beach.");
        overworld_navigation(env.program_info(), env.console, context, NavigationStopCondition::STOP_DIALOG, NavigationMovementMode::DIRECTIONAL_ONLY, 128, 0, 30, true, true);

        env.console.log("clear_dialog: Offer Miraidon/Koraidon a sandwich.");
        clear_dialog(env.console, context, ClearDialogMode::STOP_TIMEOUT, 10, {});

        // TODO: Bag menu navigation
        context.wait_for_all_requests();
        env.console.log("Feed mom's sandwich");
        env.console.overlay().add_log("Feed mom's sandwich", COLOR_WHITE);
        
        GradientArrowWatcher arrow(COLOR_RED, GradientArrowType::RIGHT, {0.104, 0.312, 0.043, 0.08});
        context.wait_for_all_requests();

        int ret = run_until(
            env.console, context,
            [](BotBaseContext& context){
                for (int i = 0; i < 10; i++){
                    pbf_press_dpad(context, DPAD_UP, 20, 250);
                }
            },
            {arrow}
        );
        if (ret < 0){
            throw OperationFailedException(
                ErrorReport::SEND_ERROR_REPORT, env.console,
                "Failed to feed mom's sandwich.",
                true
            );  
        }

        // only press A when the sandwich is selected
        pbf_mash_button(context, BUTTON_A, 100);

        env.console.log("clear_dialog: Miraidon/Koraidon gets up and walks to cave entrance.");
        clear_dialog(env.console, context, ClearDialogMode::STOP_TIMEOUT, 35, {});
        // long animation

        // First Nemona cave conversation
        context.wait_for_all_requests();
        env.console.log("Enter cave");
        env.console.overlay().add_log("Enter cave", COLOR_WHITE);
        do_action_and_monitor_for_battles(env.program_info(), env.console, context,
            [&](const ProgramInfo& info, ConsoleHandle& console, BotBaseContext& context){
                pbf_move_left_joystick(context, 128, 20, 10 * TICKS_PER_SECOND, 20);
                pbf_move_left_joystick(context, 150, 20, 1 * TICKS_PER_SECOND, 20);
                pbf_move_left_joystick(context, 128, 20, 8 * TICKS_PER_SECOND, 20);
                pbf_move_left_joystick(context, 150, 20, 2 * TICKS_PER_SECOND, 20);                
            }
        );
        
        env.console.log("overworld_navigation: Go to cave.");
        overworld_navigation(env.program_info(), env.console, context, NavigationStopCondition::STOP_DIALOG, NavigationMovementMode::DIRECTIONAL_ONLY, 128, 20, 10, true, true);

        env.console.log("clear_dialog: Talk to Nemona yelling down, while you're down in the cave.");
        clear_dialog(env.console, context, ClearDialogMode::STOP_TIMEOUT, 10, {CallbackEnum::PROMPT_DIALOG});

        do_action_and_monitor_for_battles(env.program_info(), env.console, context,
            [&](const ProgramInfo& info, ConsoleHandle& console, BotBaseContext& context){
                // Legendary rock break
                context.wait_for_all_requests();
                console.log("Rock break");
                console.overlay().add_log("Rock break", COLOR_WHITE);
                pbf_move_left_joystick(context, 128, 20, 3 * TICKS_PER_SECOND, 20);
                realign_player(env.program_info(), console, context, PlayerRealignMode::REALIGN_NO_MARKER, 230, 25, 30);
                pbf_move_left_joystick(context, 128, 0, 2 * TICKS_PER_SECOND, 5 * TICKS_PER_SECOND);

                // Houndour wave
                context.wait_for_all_requests();
                console.log("Houndour wave");
                console.overlay().add_log("Houndour wave", COLOR_WHITE);
                pbf_move_left_joystick(context, 140, 20, 4 * TICKS_PER_SECOND, 2 * TICKS_PER_SECOND);
                realign_player(env.program_info(), console, context, PlayerRealignMode::REALIGN_NO_MARKER, 220, 15, 30);
                pbf_move_left_joystick(context, 128, 20, 5 * TICKS_PER_SECOND, 2 * TICKS_PER_SECOND);
                pbf_move_left_joystick(context, 128, 20, 6 * TICKS_PER_SECOND, 2 * TICKS_PER_SECOND);
                realign_player(env.program_info(), console, context, PlayerRealignMode::REALIGN_NO_MARKER, 220, 25, 20);
                pbf_move_left_joystick(context, 128, 20, 4 * TICKS_PER_SECOND, 2 * TICKS_PER_SECOND);
                pbf_move_left_joystick(context, 128, 20, 4 * TICKS_PER_SECOND, 2 * TICKS_PER_SECOND);
                realign_player(env.program_info(), console, context, PlayerRealignMode::REALIGN_NO_MARKER, 220, 25, 25);
                pbf_move_left_joystick(context, 128, 20, 6 * TICKS_PER_SECOND, 20 * TICKS_PER_SECOND);

                // Houndoom encounter
                context.wait_for_all_requests();
                console.log("Houndoom encounter");
                console.overlay().add_log("Houndoom encounter", COLOR_WHITE);
                pbf_move_left_joystick(context, 128, 20, 4 * TICKS_PER_SECOND, 20);
                realign_player(env.program_info(), console, context, PlayerRealignMode::REALIGN_NO_MARKER, 245, 20, 20);
                pbf_move_left_joystick(context, 128, 20, 2 * TICKS_PER_SECOND, 20);
                realign_player(env.program_info(), console, context, PlayerRealignMode::REALIGN_NO_MARKER, 255, 90, 20);
                pbf_move_left_joystick(context, 128, 20, 8 * TICKS_PER_SECOND, 8 * TICKS_PER_SECOND);
                pbf_press_button(context, BUTTON_L, 20, 20);
            }
        );
        
        env.console.log("overworld_navigation: Go to Houndoom.");
        overworld_navigation(env.program_info(), env.console, context, NavigationStopCondition::STOP_DIALOG, NavigationMovementMode::DIRECTIONAL_ONLY, 128, 20, 40, true, true);
        
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
