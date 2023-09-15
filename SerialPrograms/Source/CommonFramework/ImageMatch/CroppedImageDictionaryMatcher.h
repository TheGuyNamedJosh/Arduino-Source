/*  Cropped Image Dictionary Matcher
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_CommonFramework_CroppedImageDictionaryMatcher_H
#define PokemonAutomation_CommonFramework_CroppedImageDictionaryMatcher_H

#include "Common/Compiler.h"
#include "CommonFramework/ImageTools/FloatPixel.h"
#include "ImageMatchResult.h"
#include "ExactImageMatcher.h"

namespace PokemonAutomation{
namespace ImageMatch{

// Similar to `ExactImageDictionaryMatcher` but will crop the image based on background pixel colors before matching.
class CroppedImageDictionaryMatcher{
public:
    CroppedImageDictionaryMatcher(
        const WeightedExactImageMatcher::InverseStddevWeight& weight = WeightedExactImageMatcher::InverseStddevWeight()
    );
    virtual ~CroppedImageDictionaryMatcher() = default;

    void add(const std::string& slug, const ImageViewRGB32& image);

    ImageMatchResult match(const ImageViewRGB32& image, double alpha_spread) const;


protected:
    // return the cropped the image based on background colors. Also return the background color.
    virtual ImageRGB32 process_image(const ImageViewRGB32& image, Color& background) const = 0;


private:
    WeightedExactImageMatcher::InverseStddevWeight m_weight;
    std::map<std::string, WeightedExactImageMatcher> m_database;
};


}
}
#endif
