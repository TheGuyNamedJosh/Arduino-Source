/*  Train Pokemon Name OCR
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_Pokemon_TrainPokemonOCR_H
#define PokemonAutomation_Pokemon_TrainPokemonOCR_H

#include "CommonFramework/Options/SimpleIntegerOption.h"
#include "CommonFramework/Options/StringOption.h"
#include "CommonFramework/Options/EnumDropdownOption.h"
#include "ComputerPrograms/ComputerProgram.h"

namespace PokemonAutomation{
namespace Pokemon{


class TrainPokemonOCR_Descriptor : public ComputerProgramDescriptor{
public:
    TrainPokemonOCR_Descriptor();
};



class TrainPokemonOCR : public ComputerProgramInstance{
public:
    TrainPokemonOCR();

    virtual void program(ProgramEnvironment& env, CancellableScope& scope) override;

private:
    StringOption DIRECTORY;
    EnumDropdownOption MODE;
    SimpleIntegerOption<uint32_t> THREADS;  //  Can't use "size_t" due to integer type ambiguity.

};



}
}
#endif
